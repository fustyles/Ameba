/*
Author : ChungYi Fu (Kaohsiung, Taiwan)  2026-5-13 12:30
https://www.facebook.com/francefu
*/

// WiFi credentials
char wifi_ssid[] = "xxxxxxxxxx";
char wifi_pass[] = "xxxxxxxxxx";

// Telegram bot configuration
String telegramBot_token = "xxxxxxxxxx";
String telegramBot_chatID = "xxxxxxxxxx";

// LED output pin
int pinLed = 24;    // green led (AMB82-mini: 24, HUB 8735 Ultra: 25)

// Last Telegram message ID
long messageLastId = 0;

// Whether to send help message on startup
bool sendHelp = false;

#include <WiFi.h>

// SSL client for secure Telegram polling
WiFiSSLClient botClient;

#include <ArduinoJson.h>
#include "FreeRTOS.h"
#include "task.h"
#include "VideoStream.h"

// Camera video configuration
//VideoSetting config(320, 240, CAM_FPS, VIDEO_JPEG, 1);
VideoSetting config(VIDEO_VGA, CAM_FPS, VIDEO_JPEG, 1);

// WiFi AP channel
char channel_ap[] = "2";

// Captured image buffer address and length
uint32_t img_addr = 0;
uint32_t img_len = 0;

#define CONFIG_INIC_IPC_HIGH_TP

// Send text message to Telegram bot
void sendMessageToTelegram(String token, String chatid, String text, String keyboard) {
  text.replace("\\n", "%0A");
  const char* myDomain = "api.telegram.org";
  String getAll="", getBody = "";
  String request = "parse_mode=HTML&chat_id="+chatid+"&text="+text;

  if (keyboard!="")
    request += "&reply_markup="+keyboard;

  WiFiSSLClient client;
  
  if (client.connect(myDomain, 443)) {
    client.println("POST /bot"+token+"/sendMessage HTTP/1.1");
    client.println("Host: " + String(myDomain));
    client.println("Content-Length: " + String(request.length()));
    client.println("Content-Type: application/x-www-form-urlencoded");
    client.println("Connection: close");
    client.println();
    client.print(request);

    int waitTime = 5000;
    long startTime = millis();
    boolean state = false;

    while ((startTime + waitTime) > millis()) {
      delay(100);
      while (client.available())  {
        char c = client.read();

        if (state==true)
          getBody += String(c);

        if (c == '\n')  {
          if (getAll.length()==0)
            state=true;
          getAll = "";
        }
        else if (c != '\r')
          getAll += String(c);

        startTime = millis();
      }

      if (getBody.length()>0)
        break;
    }
    client.stop();
  }
}

// Capture image and send to Telegram
String sendCapturedImageToTelegram(String token, String chat_id, bool capture) {
  const char* myDomain = "api.telegram.org";
  String getAll="", getBody = "";
  WiFiSSLClient client;

  if (client.connect(myDomain, 443)) {

    if (capture) {
      Camera.getImage(0, &img_addr, &img_len);
    }

    uint8_t *fbBuf = (uint8_t*)img_addr;
    size_t fbLen = img_len;

    String head =
      "--Taiwan\r\nContent-Disposition: form-data; name=\"chat_id\"; \r\n\r\n"
      + chat_id +
      "\r\n--Taiwan\r\nContent-Disposition: form-data; name=\"photo\"; filename=\"esp32-cam.jpg\"\r\nContent-Type: image/jpeg\r\n\r\n";

    String tail = "\r\n--Taiwan--\r\n";

    uint16_t imageLen = img_len;
    uint16_t extraLen = head.length() + tail.length();
    uint16_t totalLen = imageLen + extraLen;

    client.println("POST /bot"+token+"/sendPhoto HTTP/1.1");
    client.println("Host: " + String(myDomain));
    client.println("Content-Length: " + String(totalLen));
    client.println("Content-Type: multipart/form-data; boundary=Taiwan");
    client.println();

    client.print(head);

    // Send JPEG data in chunks
    for (size_t n=0;n<fbLen;n=n+1024) {
      if (n+1024<fbLen) {
        client.write(fbBuf, 1024);
        fbBuf += 1024;
      }
      else if (fbLen%1024>0) {
        size_t remainder = fbLen%1024;
        client.write(fbBuf, remainder);
      }
    }

    client.print(tail);

    int waitTime = 10000;
    long startTime = millis();
    boolean state = false;

    while ((startTime + waitTime) > millis()) {
      delay(100);

      while (client.available()) {
        char c = client.read();

        if (state==true)
          getBody += String(c);

        if (c == '\n') {
          if (getAll.length()==0)
            state=true;
          getAll = "";
        }
        else if (c != '\r')
          getAll += String(c);

        startTime = millis();
      }

      if (getBody.length()>0)
        break;
    }

    client.stop();
    Serial.println();

  } else {
    getBody="Connected to api.telegram.org failed.";
    Serial.println("Connected to api.telegram.org failed.");
  }

  return getBody;
}

// Get current memory usage information
String getMemoryInfo() {
  String msg = "";

  msg += "Free heap: ";
  msg += String(xPortGetFreeHeapSize());

  msg += "\nMin heap: ";
  msg += String(xPortGetMinimumEverFreeHeapSize());

  return msg;
}

// Execute Telegram command
void executeCommand(String botMessage) {
  if (botMessage=="")
    return;

  if (botMessage=="help"||botMessage=="/help"||botMessage=="/start") {

    String command =
      "/help command list\n/on turn on led\n/off turn off led\n/still get still\n/memory remaining memory";

    String keyboard =
      "{\"keyboard\":[[{\"text\":\"/help\"},{\"text\":\"/on\"},{\"text\":\"/off\"}],[{\"text\":\"/still\"},{\"text\":\"/memory\"}]],\"one_time_keyboard\":false}";

    sendMessageToTelegram(telegramBot_token, telegramBot_chatID, command, keyboard);   

  } else if (botMessage=="null") {

    botClient.stop();
    getTelegramMessage();

  } else {

    if ((botMessage) == "/on") {
      digitalWrite(pinLed, 1);
      sendMessageToTelegram(telegramBot_token, telegramBot_chatID, "Turn on","");

    } else if ((botMessage) == "/off") {
      digitalWrite(pinLed, 0);
      sendMessageToTelegram(telegramBot_token, telegramBot_chatID, "Turn off","");

    } else if ((botMessage) == "/still") {
      sendCapturedImageToTelegram(telegramBot_token, telegramBot_chatID, 1);

    } else if (botMessage=="/memory") {
      String msg = getMemoryInfo();
      sendMessageToTelegram(telegramBot_token, telegramBot_chatID, msg, "");

    } else {
      sendMessageToTelegram(telegramBot_token, telegramBot_chatID, "Command is not defined.", "" );
	  
    }
  }
}

// Initialize WiFi and camera hardware
void initWiFi() {
  for (int i=0;i<2;i++) {

    if (String(wifi_ssid)=="")
      break;

    WiFi.begin(wifi_ssid, wifi_pass);
    delay(1000);

    Serial.println("");
    Serial.print("Connecting to ");
    Serial.println(wifi_ssid);

    long int StartTime=millis();

    while (WiFi.status() != WL_CONNECTED) {
      delay(500);

      if ((StartTime+5000) < millis())
        break;
    }
  }

  config.setRotation(0);
  Camera.configVideoChannel(0, config);
  Camera.videoInit();
  Camera.channelBegin(0);
}

// Poll Telegram updates continuously
void getTelegramMessage() {

  const char* myDomain = "api.telegram.org";
  String getAll="", getBody = "";

  JsonObject obj;
  DynamicJsonDocument doc(2048);

  String message;
  long message_id;

  if (messageLastId == 0)
    Serial.println("Connect to " + String(myDomain));

  if (botClient.connect(myDomain, 443)) {

    if (messageLastId == 0)
      Serial.println("Connection successful");

    while (botClient.connected()) {

      getAll = "";
      getBody = "";

      String request = "limit=1&offset=-1&allowed_updates=message";

      botClient.println("POST /bot"+telegramBot_token+"/getUpdates HTTP/1.1");
      botClient.println("Host: " + String(myDomain));
      botClient.println("Content-Length: " + String(request.length()));
      botClient.println("Content-Type: application/x-www-form-urlencoded");
      botClient.println("Connection: keep-alive");
      botClient.println();
      botClient.print(request);

      int waitTime = 5000;
      long startTime = millis();
      boolean state = false;

      while ((startTime + waitTime) > millis()){
        delay(100);

        while (botClient.available()){
          char c = botClient.read();

          if (c == '\n') {
            if (getAll.length()==0)
              state=true;
            getAll = "";
          }
          else if (c != '\r')
            getAll += String(c);

          if (state==true)
            getBody += String(c);

          startTime = millis();
        }

        if (getBody.length()>0)
          break;
      }

      deserializeJson(doc, getBody);
      obj = doc.as<JsonObject>();

      message_id = obj["result"][0]["message"]["message_id"].as<long>();
      message = obj["result"][0]["message"]["text"].as<String>();

      if (message_id!=messageLastId&&message_id) {

        int id_last = messageLastId;
        messageLastId = message_id;

        if (id_last==0) {
          message_id = 0;

          if (sendHelp == true)
            message = "/help";
          else
            message = "";
        }

        if (message!="") {
          executeCommand(message);
		}
      }
    }
  }

  // Reconnect WiFi if disconnected
  while (WiFi.status() != WL_CONNECTED) {

    WiFi.disconnect();
    WiFi.begin(wifi_ssid, wifi_pass);

    unsigned long start = millis();

    while (WiFi.status() != WL_CONNECTED &&
      millis() - start < 10000) {
      delay(500);
    }

  }
}

// FreeRTOS task wrapper for Telegram polling
void getTelegramMessage_task(void *param) {
  (void)param;
  while(1) {
    getTelegramMessage();
  }
}

// Arduino setup
void setup() {
  Serial.begin(115200);
  delay(10);

  pinMode(pinLed, OUTPUT);

  initWiFi();

  if (xTaskCreate(
        getTelegramMessage_task,
        (const char *)"getTelegramMessage_task",
        32768,
        NULL,
        tskIDLE_PRIORITY + 1,
        NULL
      )!= pdPASS) {

    Serial.println("Create getTelegramMessage task failed");
  }
}

// Main loop (unused)
void loop() {
}
