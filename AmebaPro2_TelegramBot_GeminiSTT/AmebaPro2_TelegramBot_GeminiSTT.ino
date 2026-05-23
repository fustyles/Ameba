/*
Author : ChungYi Fu (Kaohsiung, Taiwan)  2026-5-23 13:00
https://www.facebook.com/francefu
*/

// WiFi credentials
char wifi_ssid[] = "xxxxxxxxxx";
char wifi_pass[] = "xxxxxxxxxx";

// Telegram bot configuration
String telegramBot_token = "xxxxxxxxxx";
String telegramBot_chatID = "xxxxxxxxxx";

// Gemini API configuration
String geminiApiKey = "xxxxxxxxxx";

#define MAX_FILE_SIZE 262144
size_t downloadedFileSize = 0;

// LED output pin
int ledPin = 24;    // green led (AMB82-mini: 24, HUB 8735 Ultra: 25)

// Last Telegram message ID
long messageLastId = 0;

// Whether to send help message on startup
bool sendHelp = false;


#include <WiFi.h>

// SSL client for secure Telegram polling
WiFiSSLClient botClient;

#include "Base64.h"
#include <ArduinoJson.h>
#include "FreeRTOS.h"
#include "task.h"
#include "VideoStream.h"

// Camera video configuration
VideoSetting config(320, 240, CAM_FPS, VIDEO_JPEG, 1);
//VideoSetting config(VIDEO_VGA, CAM_FPS, VIDEO_JPEG, 1);

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

    size_t imageLen = img_len;
    size_t extraLen = head.length() + tail.length();
    size_t totalLen = imageLen + extraLen;

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

  } else {

    if ((botMessage) == "/on") {
      digitalWrite(ledPin, 1);
      sendMessageToTelegram(telegramBot_token, telegramBot_chatID, "Turn on","");

    } else if ((botMessage) == "/off") {
      digitalWrite(ledPin, 0);
      sendMessageToTelegram(telegramBot_token, telegramBot_chatID, "Turn off","");

    } else if ((botMessage) == "/still") {
      sendCapturedImageToTelegram(telegramBot_token, telegramBot_chatID, 1);

    } else if (botMessage=="/memory") {
      String msg = getMemoryInfo();
      sendMessageToTelegram(telegramBot_token, telegramBot_chatID, msg, "");

    } else {
      sendMessageToTelegram(telegramBot_token, telegramBot_chatID, botMessage + "\n\nCommand is not defined.", "" );
	  
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


String sendAudioFileToGeminiSTT(uint8_t *fileinput, size_t fileSize, String mimeType, String prompt) {
  int encodedLen = base64_enc_len(fileSize);
  char *encodedData = (char *)malloc(encodedLen);
  if (!encodedData) return "Malloc failed for Base64 encoding.";
  
  base64_encode(encodedData, (char *)fileinput, fileSize);
  
  WiFiSSLClient client;
  
  if (client.connect("generativelanguage.googleapis.com", 443)) {
    prompt.replace("\n", "");
    
    String filePart = "{\"inline_data\": {\"data\": \""+String(encodedData)+"\", \"mime_type\": \""+mimeType+"\"}}";
    String textPart = "{\"text\": \""+prompt+"\"}";
    String request = "{\"contents\": [{\"role\": \"user\", \"parts\": ["+filePart+", "+textPart+"]}]}";
    
    free(encodedData);

    client.println("POST /v1beta/models/gemini-2.5-flash:generateContent?key=" + geminiApiKey + " HTTP/1.1");
    client.println("Connection: close");
    client.println("Host: generativelanguage.googleapis.com");
    client.println("Content-Type: application/json; charset=utf-8");
    client.println("Content-Length: " + String(request.length()));
    client.println();
    
    for (int i = 0; i < request.length(); i += 1024) {
      client.print(request.substring(i, i + 1024));
    }
    
    String getResponse="", Feedback="";
    int waitTime = 20000;
    long startTime = millis();
    boolean state = false;
    boolean headState = false;
    
    while ((startTime + waitTime) > millis()) {
      delay(100);
      while (client.available()) {
        char c = client.read();
        if (state==true) Feedback += String(c);
        if (c == '\n') {
          if (getResponse.length()==0) {
            if (!headState) {
              client.readStringUntil('\n');
              headState = true;
            }
            state=true;
          }
          getResponse = "";
        }
        else if (c != '\r')
          getResponse += String(c);
        startTime = millis();
      }
      if (Feedback.length()>0) break;
    }
    client.stop();
    
    DynamicJsonDocument doc(4096);
    DeserializationError err = deserializeJson(doc, Feedback);
    if (err) return "JSON Parsing Error: " + String(err.c_str());
    
    if (doc.containsKey("error")) {
      return "Gemini Error: " + doc["error"]["message"].as<String>();
    }
    
    if (doc["candidates"][0]["content"]["parts"][0].containsKey("text")) {
      String getText = doc["candidates"][0]["content"]["parts"][0]["text"].as<String>();
      getText.replace("\n", "");
      return getText;
    }
    
    return "No text returned from Gemini.";
  }
  else {
    free(encodedData);
    return "Connected to Gemini failed.";
  }
}

uint8_t* downloadTelegramFile(String filePath) {
    uint8_t *voiceFile = (uint8_t*)malloc(MAX_FILE_SIZE);
    if (!voiceFile) return NULL;
    
    downloadedFileSize = 0;
    WiFiSSLClient client;

    if (client.connect("api.telegram.org", 443)) {
        client.println("GET /file/bot" + telegramBot_token + "/" + filePath + " HTTP/1.0");
        client.println("Host: api.telegram.org");
        client.println("Connection: close");
        client.println();

        String header = "";
        long startTime = millis();
        while (client.connected() || client.available()) {
            if (millis() - startTime > 10000) break;
            if (client.available()) {
                char c = client.read();
                header += c;
                if (header.endsWith("\r\n\r\n"))
                    break;
            }
        }

        startTime = millis();
        while ((client.connected() || client.available()) && downloadedFileSize < MAX_FILE_SIZE) {
            if (millis() - startTime > 10000) break;
            if (client.available()) {
                voiceFile[downloadedFileSize++] = client.read();
                startTime = millis();
            }
        }
        client.stop();
    }
    
    return voiceFile;
}

String getTelegramFilePath(String fileId) {

    WiFiSSLClient client;

    String filePath = "";
    String getAll="", getBody = "";    

    if (client.connect("api.telegram.org", 443)) {

        client.println("GET /bot" + telegramBot_token +
                       "/getFile?file_id=" + fileId +
                       " HTTP/1.1");

        client.println("Host: api.telegram.org");
        client.println("Connection: close");
        client.println();

        int waitTime = 5000;
        long startTime = millis();
        boolean state = false;        

        while ((startTime + waitTime) > millis()){
          delay(100);
  
          while (client.available()){
            char c = client.read();
 
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

        DynamicJsonDocument doc(2048);
        deserializeJson(doc, getBody);

        filePath = doc["result"]["file_path"].as<String>();
    }

    return filePath;
}

// Poll Telegram updates continuously
void getTelegramMessage() {

  const char* myDomain = "api.telegram.org";
  String getAll="", getBody = "";

  JsonObject obj;
  DynamicJsonDocument doc(2048);

  String text = "";
  String voiceFileId = "";
  size_t voiceFileSize;
  long message_id;

  if (messageLastId == 0)
    Serial.println("Connect to " + String(myDomain));

  if (botClient.connect(myDomain, 443)) {

    if (messageLastId == 0) {
      Serial.println("Connection successful");
      
      for (int i = 0; i < 3; i++) {
        digitalWrite(ledPin, HIGH);
        delay(500);
        digitalWrite(ledPin, LOW);
        delay(500);
      }
    }

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
      if (message_id!=messageLastId&&message_id) {

        int id_last = messageLastId;
        messageLastId = message_id;

        if (id_last==0) {
          message_id = 0;

          if (sendHelp == true) {
            executeCommand("/help");
            return;
          }            
        }
        else {
          if (obj["result"][0]["message"].containsKey("text")) {
              text = obj["result"][0]["message"]["text"].as<String>();
              executeCommand(text); 
          }
          
          if (doc["result"][0]["message"].containsKey("voice")) {
              voiceFileId = doc["result"][0]["message"]["voice"]["file_id"].as<String>();
  
              messageLastVoiceId = voiceFileId;
              
              String FilePath = getTelegramFilePath(voiceFileId);
              uint8_t *voiceFile = downloadTelegramFile(FilePath);
              
              if (voiceFile && downloadedFileSize > 0) {
                  text = sendAudioFileToGeminiSTT(voiceFile, downloadedFileSize, "audio/ogg; codecs=opus", "Transcribe this audio to text exactly as spoken.");
                  executeCommand(text);
              }
              if (voiceFile) free(voiceFile);
          }  
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

  pinMode(ledPin, OUTPUT);

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
