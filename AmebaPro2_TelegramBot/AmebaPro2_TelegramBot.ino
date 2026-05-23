/*
 * Author  : ChungYi Fu (Kaohsiung, Taiwan)  2026-5-23 13:00
 * Website : https://www.facebook.com/francefu
 *
 * Description:
 *   Telegram bot running on AMB82-mini that supports:
 *   - Text commands to control LED, capture still image, and check memory
 *   - Persistent HTTPS long-polling via a keep-alive SSL connection
 *
 * Hardware: AMB82-mini (Realtek RTL8735B)
 * Dependencies: WiFi, ArduinoJson, FreeRTOS, VideoStream
 */

// ============================================================
//  Configuration — fill in your own credentials
// ============================================================

// WiFi credentials
char wifi_ssid[] = "xxxxxxxxxx";
char wifi_pass[] = "xxxxxxxxxx";

// Telegram bot configuration
String telegramBot_token = "xxxxxxxxxx";
String telegramBot_chatID = "xxxxxxxxxx";

// ============================================================
//  Constants & Global Variables
// ============================================================

// LED output pin (AMB82-mini: 24 / HUB 8735 Ultra: 25)
int pinLed = 24;

// Tracks the last processed Telegram message ID to avoid duplicates
long messageLastId = 0;

// Set to true to send /help automatically after first connection
bool sendHelp = false;

// ============================================================
//  Library Includes
// ============================================================

#include <WiFi.h>

// Persistent SSL client used for long-polling Telegram updates
WiFiSSLClient botClient;

#include <ArduinoJson.h>
#include "FreeRTOS.h"
#include "task.h"
#include "VideoStream.h"

// ============================================================
//  Camera Configuration
// ============================================================

// QVGA resolution, JPEG format, single channel
VideoSetting config(320, 240, CAM_FPS, VIDEO_JPEG, 1);
// Alternatively use VGA resolution:
// VideoSetting config(VIDEO_VGA, CAM_FPS, VIDEO_JPEG, 1);

// WiFi AP channel (unused in station mode, kept for reference)
char channel_ap[] = "2";

// Pointers to the last captured image buffer
uint32_t img_addr = 0;
uint32_t img_len  = 0;

#define CONFIG_INIC_IPC_HIGH_TP

// ============================================================
//  Telegram: Send Text Message
// ============================================================

/**
 * @brief Send a text message (and optional reply keyboard) to a Telegram chat.
 *
 * @param token    Bot token
 * @param chatid   Target chat ID
 * @param text     Message body (HTML parse mode supported)
 * @param keyboard JSON string for a custom reply keyboard; pass "" to omit
 */
void sendMessageToTelegram(String token, String chatid, String text, String keyboard) {

  // Replace literal "\n" with URL-encoded newline for Telegram's API
  text.replace("\\n", "%0A");

  const char* myDomain = "api.telegram.org";
  String getAll = "", getBody = "";
  String request = "parse_mode=HTML&chat_id=" + chatid + "&text=" + text;

  if (keyboard != "")
    request += "&reply_markup=" + keyboard;

  WiFiSSLClient client;

  if (client.connect(myDomain, 443)) {

    // Build and send HTTP POST request
    client.println("POST /bot" + token + "/sendMessage HTTP/1.1");
    client.println("Host: " + String(myDomain));
    client.println("Content-Length: " + String(request.length()));
    client.println("Content-Type: application/x-www-form-urlencoded");
    client.println("Connection: close");
    client.println();
    client.print(request);

    // Wait up to 5 s for a response (we only need to confirm delivery)
    int     waitTime  = 5000;
    long    startTime = millis();
    boolean state     = false;

    while ((startTime + waitTime) > millis()) {
      delay(100);

      while (client.available()) {
        char c = client.read();

        if (state == true)  getBody += String(c);

        if (c == '\n') {
          if (getAll.length() == 0) state = true;
          getAll = "";
        } else if (c != '\r') {
          getAll += String(c);
        }

        startTime = millis();
      }

      if (getBody.length() > 0) break;
    }

    client.stop();
  }
}

// ============================================================
//  Telegram: Capture & Send Still Image
// ============================================================

/**
 * @brief Capture a JPEG frame from the camera and upload it to Telegram.
 *
 * @param token    Bot token
 * @param chat_id  Target chat ID
 * @param capture  true = grab a fresh frame; false = reuse the last frame buffer
 * @return         Raw HTTP response body from Telegram
 */
String sendCapturedImageToTelegram(String token, String chat_id, bool capture) {

  const char* myDomain = "api.telegram.org";
  String getAll = "", getBody = "";
  WiFiSSLClient client;

  if (client.connect(myDomain, 443)) {

    // Optionally grab a new camera frame
    if (capture) {
      Camera.getImage(0, &img_addr, &img_len);
    }

    uint8_t* fbBuf = (uint8_t*)img_addr;
    size_t   fbLen = img_len;

    // Multipart form-data boundaries and headers
    String head =
      "--Taiwan\r\nContent-Disposition: form-data; name=\"chat_id\"; \r\n\r\n"
      + chat_id +
      "\r\n--Taiwan\r\nContent-Disposition: form-data; name=\"photo\";"
      " filename=\"esp32-cam.jpg\"\r\nContent-Type: image/jpeg\r\n\r\n";

    String tail = "\r\n--Taiwan--\r\n";

    // Use size_t to avoid overflow on images larger than 65535 bytes
    size_t extraLen = head.length() + tail.length();
    size_t totalLen = img_len + extraLen;

    // Send multipart POST
    client.println("POST /bot" + token + "/sendPhoto HTTP/1.1");
    client.println("Host: " + String(myDomain));
    client.println("Content-Length: " + String(totalLen));
    client.println("Content-Type: multipart/form-data; boundary=Taiwan");
    client.println();
    client.print(head);

    // Stream JPEG data in 1 KB chunks to avoid large single writes
    for (size_t n = 0; n < fbLen; n += 1024) {
      if (n + 1024 < fbLen) {
        client.write(fbBuf, 1024);
        fbBuf += 1024;
      } else if (fbLen % 1024 > 0) {
        client.write(fbBuf, fbLen % 1024);
      }
    }

    client.print(tail);

    // Wait up to 10 s for Telegram to acknowledge the upload
    int     waitTime  = 10000;
    long    startTime = millis();
    boolean state     = false;

    while ((startTime + waitTime) > millis()) {
      delay(100);

      while (client.available()) {
        char c = client.read();

        if (state == true)  getBody += String(c);

        if (c == '\n') {
          if (getAll.length() == 0) state = true;
          getAll = "";
        } else if (c != '\r') {
          getAll += String(c);
        }

        startTime = millis();
      }

      if (getBody.length() > 0) break;
    }

    client.stop();
    Serial.println();

  } else {
    getBody = "Connected to api.telegram.org failed.";
    Serial.println("Connected to api.telegram.org failed.");
  }

  return getBody;
}

// ============================================================
//  System: Memory Info
// ============================================================

/**
 * @brief Build a string showing current and minimum-ever free heap size.
 *
 * @return Multi-line string with heap statistics
 */
String getMemoryInfo() {
  String msg = "";
  msg += "Free heap: " + String(xPortGetFreeHeapSize());
  msg += "\nMin heap: "  + String(xPortGetMinimumEverFreeHeapSize());
  return msg;
}

// ============================================================
//  Command Dispatcher
// ============================================================

/**
 * @brief Parse a bot message and execute the corresponding action.
 *
 * Supported commands:
 *   /help   — list all commands and show a reply keyboard
 *   /on     — turn the LED on
 *   /off    — turn the LED off
 *   /still  — capture and send a camera snapshot
 *   /memory — report heap usage
 *   null    — force-close and reopen the persistent bot connection
 *
 * @param botMessage  Command string received from Telegram
 */
void executeCommand(String botMessage) {

  if (botMessage == "") return;

  // ---- Help / start ----
  if (botMessage == "help" || botMessage == "/help" || botMessage == "/start") {

    String command =
      "/help command list\n"
      "/on turn on led\n"
      "/off turn off led\n"
      "/still get still\n"
      "/memory remaining memory";

    String keyboard =
      "{\"keyboard\":["
        "[{\"text\":\"/help\"},{\"text\":\"/on\"},{\"text\":\"/off\"}],"
        "[{\"text\":\"/still\"},{\"text\":\"/memory\"}]"
      "],\"one_time_keyboard\":false}";

    sendMessageToTelegram(telegramBot_token, telegramBot_chatID, command, keyboard);

  // ---- Force reconnect ----
  } else if (botMessage == "null") {

    // Drop the current connection; getTelegramMessage() will reconnect
    botClient.stop();
    getTelegramMessage();

  // ---- LED on ----
  } else if (botMessage == "/on") {

    digitalWrite(pinLed, 1);
    sendMessageToTelegram(telegramBot_token, telegramBot_chatID, "Turn on", "");

  // ---- LED off ----
  } else if (botMessage == "/off") {

    digitalWrite(pinLed, 0);
    sendMessageToTelegram(telegramBot_token, telegramBot_chatID, "Turn off", "");

  // ---- Camera snapshot ----
  } else if (botMessage == "/still") {

    sendCapturedImageToTelegram(telegramBot_token, telegramBot_chatID, 1);

  // ---- Memory report ----
  } else if (botMessage == "/memory") {

    sendMessageToTelegram(telegramBot_token, telegramBot_chatID, getMemoryInfo(), "");

  // ---- Unknown command ----
  } else {

    sendMessageToTelegram(
      telegramBot_token, telegramBot_chatID,
      "Command is not defined.", "");
  }
}

// ============================================================
//  Hardware Initialisation
// ============================================================

/**
 * @brief Connect to WiFi and initialise the camera.
 *        Retries WiFi up to 2 times with a 5 s timeout each attempt.
 */
void initWiFi() {

  for (int i = 0; i < 2; i++) {

    if (String(wifi_ssid) == "") break;

    WiFi.begin(wifi_ssid, wifi_pass);
    delay(1000);

    Serial.println("");
    Serial.print("Connecting to ");
    Serial.println(wifi_ssid);

    long int StartTime = millis();

    while (WiFi.status() != WL_CONNECTED) {
      delay(500);
      if ((StartTime + 5000) < millis()) break;
    }
  }

  // Camera setup: no rotation, channel 0, JPEG stream
  config.setRotation(0);
  Camera.configVideoChannel(0, config);
  Camera.videoInit();
  Camera.channelBegin(0);
}

// ============================================================
//  Telegram: Long-Poll for Updates
// ============================================================

/**
 * @brief Open a persistent HTTPS connection to Telegram and process incoming
 *        text messages in a loop.
 *
 * Requests only the most recent message (limit=1, offset=-1) on each poll.
 * Skips the first message seen after (re)boot to avoid replaying old commands.
 * The function returns only when the connection is lost, allowing the FreeRTOS
 * task to call it again and trigger WiFi reconnection if needed.
 */
void getTelegramMessage() {

  const char* myDomain = "api.telegram.org";
  String getAll = "", getBody = "";

  JsonObject          obj;
  DynamicJsonDocument doc(2048);

  String message   = "";
  long   message_id = 0;

  if (messageLastId == 0)
    Serial.println("Connect to " + String(myDomain));

  if (botClient.connect(myDomain, 443)) {

    if (messageLastId == 0)
      Serial.println("Connection successful");

    // ---- Main polling loop ----
    while (botClient.connected()) {

      getAll  = "";
      getBody = "";

      // Request the single most recent message
      String request = "limit=1&offset=-1&allowed_updates=message";

      botClient.println("POST /bot" + telegramBot_token + "/getUpdates HTTP/1.1");
      botClient.println("Host: " + String(myDomain));
      botClient.println("Content-Length: " + String(request.length()));
      botClient.println("Content-Type: application/x-www-form-urlencoded");
      botClient.println("Connection: keep-alive");
      botClient.println();
      botClient.print(request);

      int     waitTime  = 5000;
      long    startTime = millis();
      boolean state     = false;

      while ((startTime + waitTime) > millis()) {
        delay(100);

        while (botClient.available()) {
          char c = botClient.read();

          if (c == '\n') {
            if (getAll.length() == 0) state = true;
            getAll = "";
          } else if (c != '\r') {
            getAll += String(c);
          }

          if (state == true) getBody += String(c);

          startTime = millis();
        }

        if (getBody.length() > 0) break;
      }

      deserializeJson(doc, getBody);
      obj = doc.as<JsonObject>();

      // Parse message_id as String then convert to int for reliability
      message_id = obj["result"][0]["message"]["message_id"].as<long>();
      message    = obj["result"][0]["message"]["text"].as<String>();

      // Process only if this is a new, valid message
      if (message_id != messageLastId && message_id) {

        int id_last   = messageLastId;
        messageLastId = message_id;

        if (id_last == 0) {
          // Skip the first message seen after (re)boot to avoid replaying old commands
          message_id = 0;
          message    = sendHelp ? "/help" : "";
        }

        if (message != "") {
          executeCommand(message);
        }
      }
    }   // end while(botClient.connected())
  }

  // ---- WiFi reconnection ----
  while (WiFi.status() != WL_CONNECTED) {

    WiFi.disconnect();
    WiFi.begin(wifi_ssid, wifi_pass);

    unsigned long start = millis();
    while (WiFi.status() != WL_CONNECTED && millis() - start < 10000) {
      delay(500);
    }
  }
}

// ============================================================
//  FreeRTOS Task Wrapper
// ============================================================

/**
 * @brief FreeRTOS task that calls getTelegramMessage() in an infinite loop.
 *
 * Stack size: 4 KB (sufficient for text-only bot without audio/Base64 work).
 */
void getTelegramMessage_task(void* param) {
  (void)param;
  while (1) {
    getTelegramMessage();
  }
}

// ============================================================
//  Arduino Entry Points
// ============================================================

void setup() {
  Serial.begin(115200);
  delay(10);

  pinMode(pinLed, OUTPUT);

  initWiFi();

  // Spawn the Telegram polling task
  if (xTaskCreate(
        getTelegramMessage_task,
        "getTelegramMessage_task",
        4096,           // Stack: 4 KB (no audio processing needed)
        NULL,
        tskIDLE_PRIORITY + 1,
        NULL) != pdPASS) {

    Serial.println("Create getTelegramMessage task failed");
  }
}

// All work is done in the FreeRTOS task; loop() is intentionally empty.
void loop() {}
