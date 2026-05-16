/*
------------------------------------------------------------
AMB82-mini Persistent AI Telegram Assistant
with Gemini + Camera Vision + SD Card Memory
------------------------------------------------------------

Author:
  ChungYi Fu (Kaohsiung, Taiwan)
  https://www.facebook.com/francefu

Description:
  This project implements an AI-powered Telegram assistant
  running on the AMB82-mini development board with persistent
  conversation memory stored on SD card.

  The device connects to WiFi, receives Telegram messages,
  communicates with Google's Gemini API, executes hardware
  control commands, performs camera-based visual analysis,
  and preserves conversation history across system reboot.

Main Features:
  - Telegram bot messaging interface
  - Gemini conversational AI
  - Persistent dialogue memory via SD card
  - Automatic memory restoration after reboot
  - Real-time Google Search through Gemini tools
  - Camera image capture and Telegram image upload
  - Gemini Vision image analysis
  - Remote LED control
  - FreeRTOS background message polling
  - Automatic WiFi reconnection
  - Runtime memory diagnostics

Supported Commands:
  /help     Show command list
  /on       Turn on LED
  /off      Turn off LED
  /still    Capture and send image
  /vision   Analyze captured image
  /search   Search recent online information
  /memory   Show system memory status
  /reset    Clear dialogue history and restart chat

Persistent Memory:
  Conversation history is stored in:

      /dialogue.txt

  This allows Gemini to continue previous context
  even after power cycle or device restart.

System Workflow:
  User -> Telegram Bot -> AMB82-mini
       -> Command Routing
       -> Gemini API Processing
       -> Optional Tool Execution
       -> Store Conversation to SD
       -> Return Response to Telegram

Hardware Requirements:
  - AMB82-mini development board
  - Camera module
  - SD card
  - WiFi connection

Dependencies:
  - WiFi.h
  - ArduinoJson
  - FreeRTOS
  - AmebaFatFS
  - VideoStream
  - Base64

Notes:
  Gemini prompt definitions must remain JSON-safe.
  Avoid unsupported characters inside system prompt
  and tool definition strings.

Version:
  Persistent Memory Edition
  2026-05-14 23:00

------------------------------------------------------------
*/

// WiFi credentials
char wifi_ssid[] = "xxxxxxxxxx";
char wifi_pass[] = "xxxxxxxxxx";

// Telegram bot configuration
String telegramBot_token = "xxxxxxxxxx";
String telegramBot_chatID = "xxxxxxxxxx";

// Gemini API configuration
String gemini_apikey = "xxxxxxxxxx";

// System prompt that defines assistant behavior.
// Must be JSON-safe (avoid invalid escape characters or unsupported symbols).
String gemini_role = R"(
You are a professional assistant, responding according to the user's language.
)"; 

// Tool routing rules for Gemini.
// Must be JSON-safe (avoid invalid escape characters or unsupported symbols).
String tools_definition = R"(
If the system determines that the user intends to query recent data, it will only return the identified command string "/search".
If the system determines that the user expects assistance in turning on the light, it will only return the identified command string "/on".
If the system determines that the user expects assistance in turning off the lights, it will only return the identified command string "/off".
If the system determines that the user intends to take and send back an image, it will only return the identified command string "/still".
If the system determines that the user intends to perform visual analysis on the image, it will only return the identified command string "/vision".
Otherwise, it will respond with a normal conversational reply, but its reply cannot start with "/" or "{".
)";

String gemini_model = "gemini-3-flash-preview";
int gemini_maxOutputTokens = 2000;  // If the AI ​​is unable to transmit complete data, please increase the value.
float gemini_temperature = 1.0;

// Serialized system prompt content used as the initial conversation context
String system_content = "";
  
// Stores entire chat history in Gemini API JSON format
// Used to preserve conversation memory across requests
String historical_messages = "";

// LED output pin
int pinLed = 24;    // green led (AMB82-mini: 24, HUB 8735 Ultra: 25)

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

#include "AmebaFatFS.h"

// FAT file system instance
AmebaFatFS fs;

// File object for SD card access
File file;

// Root file system path
String file_path = "";

// Historical message file name
String file_name = "dialogue.txt";

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

// Capture a still image from camera and upload it to Telegram as JPEG.
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

// Build JSON-formatted historical conversation message
String buildHistoricalData(String role, String content) {
  String jsonMessage = ", {\"role\": \"";
  jsonMessage += role;
  jsonMessage += "\", \"parts\":[{ \"text\": \"";
  jsonMessage += content;
  jsonMessage += "\" }]}";
  return jsonMessage;
}

// Load historical conversation messages from SD card
String getHistoricalMessages_sd() {
  String data = "";
  fs.begin();
  file_path = String(fs.getRootPath());
  file = fs.open(file_path+"/"+file_name);
  if (file) {
    while (file.available()) {
        char file_read_char = file.read();
        data += String(file_read_char);
    }
  }
  file.close();
  fs.end();
  
  if (data!="")
    return data;
  else
    return historical_messages;
}

// Store historical conversation messages to SD card
void storeHistoricalMessages_sd(String data) {
  historical_messages += data;
  
  fs.begin();
  file_path = String(fs.getRootPath());
  
  if (fs.exists(file_path+"/"+file_name))
      fs.remove(file_path+"/"+file_name); 
  file = fs.open(file_path+"/"+file_name); 
  if (file)
    file.println(historical_messages.c_str());
  file.close();
  fs.end();
}

// Reset conversation memory to initial system prompt state
void Gemini_chat_reset() {
  historical_messages = system_content + buildHistoricalData("model", "OK");;
  storeHistoricalMessages_sd("");

}

// Send request to Gemini and return response text
String Gemini_chat_request(String message) {
  message.replace("\"", "\\\"");

  String user_content =
    "{\"role\": \"user\", \"parts\":[{ \"text\": \""+ message+"\" }]}";

  storeHistoricalMessages_sd(", "+user_content);

  String request =
    "{\"contents\": [" + historical_messages +
    "],\"generationConfig\": {\"maxOutputTokens\": " +
    gemini_maxOutputTokens +
    ", \"temperature\": " +
    gemini_temperature +
    "}}";

  WiFiSSLClient client;

  if (client.connect("generativelanguage.googleapis.com", 443)) {

    client.println("POST /v1beta/models/"+gemini_model+":generateContent?key="+gemini_apikey+" HTTP/1.1");
    client.println("Connection: close");
    client.println("Host: generativelanguage.googleapis.com");
    client.println("Content-Type: application/json; charset=utf-8");
    client.println("Content-Length: " + String(request.length()));
    client.println();

    for (int i = 0; i < request.length(); i += 1024)
      client.print(request.substring(i, i+1024));

    String getResponse="",Feedback="";
    boolean state = false;

    int waitTime = 10000;
    long startTime = millis();

    while ((startTime + waitTime) > millis()) {
      delay(100);

      while (client.available()) {
        char c = client.read();

        if (state==true)
          getResponse += String(c);

        if (c == '\n')
          Feedback = "";
        else if (c != '\r')
          Feedback += String(c);

        if (Feedback.indexOf("\",\"text\":\"")!=-1)
          state=true;

        if (Feedback.indexOf("\"},")!=-1)
          state=false;

        startTime = millis();

        if (Feedback.indexOf("\",\"text\":\"")!=-1||Feedback.indexOf("\"text\": \"")!=-1)
          state=true;

        if (getResponse.indexOf("\"\n\r")!=-1&&state==true) {
          state=false;
          getResponse = getResponse.substring(0, getResponse.lastIndexOf("\""));
          break;
        }
        else if (getResponse.indexOf("\"")!=-1&&c == '\n'&&state==true) {
          state=false;          
          getResponse = getResponse.substring(0, getResponse.lastIndexOf("\""));
          break;
        }

        delay(1);
      }

      if (getResponse.length()>0) {
        client.stop();

        String assistant_content =
          "{\"role\": \"model\", \"parts\":[{ \"text\": \""+ getResponse+"\" }]}";

        storeHistoricalMessages_sd(", "+assistant_content);

        return getResponse;
      }
    }

    client.stop();
    Serial.println(Feedback);

    return "Please try again or check if the API key is valid.";
  }
  else
    return "Connection failed";
}

// Send Gemini request with Google Search tool enabled
String Gemini_chat_search_request(String message) {

  // Escape quotation marks for JSON formatting
  message.replace("\"", "\\\"");

  // Build user message JSON content
  String user_content = "{\"role\": \"user\", \"parts\":[{ \"text\": \""+ message+"\" }]}";

  // Append user message to conversation history
  storeHistoricalMessages_sd(", "+user_content);

  // Build Gemini API request with Google Search tool
  String request = "{\"contents\": [" + historical_messages + "],\"tools\": [{google_search: {}}],\"generationConfig\": {\"maxOutputTokens\": " + gemini_maxOutputTokens + ", \"temperature\": " + gemini_temperature + "}}";

  WiFiSSLClient client;
    
  // Connect to Gemini API server
  if (client.connect("generativelanguage.googleapis.com", 443)) {

    // Send HTTP POST request headers
    client.println("POST /v1beta/models/"+gemini_model+":generateContent?key="+gemini_apikey+" HTTP/1.1");
    client.println("Connection: close");
    client.println("Host: generativelanguage.googleapis.com");
    client.println("Content-Type: application/json; charset=utf-8");
    client.println("Content-Length: " + String(request.length()));
    client.println();
    
    // Send request body in chunks
    for (int i = 0; i < request.length(); i += 1024) {
      client.print(request.substring(i, i+1024));
    }

    // Response parsing variables
    String getResponse="",Feedback="";
    boolean state = false;

    // Maximum response wait time
    int waitTime = 20000;
    long startTime = millis();

    while ((startTime + waitTime) > millis()) {

      //Serial.print(".");
      delay(100);

      while (client.available()) {

        char c = client.read();

        //Serial.print(c);

        // Capture response text while active
        if (state==true)
          getResponse += String(c);

        // Track line-based feedback buffer
        if (c == '\n')
          Feedback = "";
        else if (c != '\r')
          Feedback += String(c);

        // Detect response text start
        if (Feedback.indexOf("\",\"text\":\"")!=-1)
          state=true;

        // Detect response text end
        if (Feedback.indexOf("\"},")!=-1)
          state=false;

        startTime = millis();

        // Alternative response text detection
        if (Feedback.indexOf("\",\"text\":\"")!=-1||Feedback.indexOf("\"text\": \"")!=-1)
          state=true;

        // Trim response when complete
        if (getResponse.indexOf("\"\n\r")!=-1&&state==true) {
          state=false;
          getResponse = getResponse.substring(0, getResponse.lastIndexOf("\""));
          break;

        } else if (getResponse.indexOf("\"")!=-1&&c == '\n'&&state==true) {
          state=false;
          getResponse = getResponse.substring(0, getResponse.lastIndexOf("\""));
          break;
        }
      }

      // Return valid response if received
      if (getResponse.length()>0) {

        client.stop();

        // Save assistant response to history
        String assistant_content = "{\"role\": \"model\", \"parts\":[{ \"text\": \""+ getResponse+"\" }]}";
        storeHistoricalMessages_sd(", "+assistant_content);

        return getResponse;
      }
    }

    // Close connection on timeout
    client.stop();

    //Serial.println(Feedback);

    return "Gemini search error";
  }

  // Connection failed
  else
    return "Connection failed";
}

// Capture camera frame and send it to Gemini Vision for multimodal analysis
String SendStillToGeminiVision(String message) {
  message.replace("\"", "\\\"");
  const char* myDomain = "generativelanguage.googleapis.com";
  String getResponse="",Feedback="";
  //Serial.println("Connect to " + String(myDomain));
  WiFiSSLClient client;
  if (client.connect(myDomain, 443)) {
    //Serial.println("Connection successful");
    Camera.getImage(0, &img_addr, &img_len);
    
    uint8_t *fbBuf = (uint8_t*)img_addr;
    size_t fbLen = img_len;

    char *input = (char *)fbBuf;
    char output[base64_enc_len(3)];
    String imageFile = "";
    for (int i=0;i<fbLen;i++) {
      base64_encode(output, (input++), 3);
      if (i%3==0) imageFile += String(output);
    }
    String Data = "{\"contents\": [{\"parts\": [{\"text\": \""+message+"\"}, {\"inline_data\": {\"mime_type\":\"image/jpeg\",\"data\":\""+imageFile+"\"}}]}]}";

    client.println("POST /v1beta/models/"+gemini_model+":generateContent?key="+gemini_apikey+" HTTP/1.1");
    client.println("Host: "+String(myDomain));
    client.println("Content-Type: application/json; charset=utf-8");
    client.println("Content-Length: " + String(Data.length()));
    client.println("Connection: close");
    client.println();

    int Index;
    for (Index = 0; Index < Data.length(); Index = Index+1024) {
      client.print(Data.substring(Index, Index+1024));
    }

    int waitTime = 10000;
    long startTime = millis();
    boolean state = false;
    boolean markState = false;
    while ((startTime + waitTime) > millis()) {
      Serial.print(".");
      delay(100);
      while (client.available())  {
          char c = client.read();
          if (String(c)=="{") markState=true;
          if (state==true&&markState==true) Feedback += String(c);
          if (c == '\n') {
            if (getResponse.length()==0) state=true;
            getResponse = "";
         }
          else if (c != '\r')
            getResponse += String(c);
          startTime = millis();
       }
       if (Feedback.length()>0) break;
    }
    client.stop();
    Serial.println("");
    //Serial.println(Feedback);

    JsonObject obj;
    DynamicJsonDocument doc(2048);
    deserializeJson(doc, Feedback);
    obj = doc.as<JsonObject>();
    getResponse = obj["candidates"][0]["content"]["parts"][0]["text"].as<String>();
    if (getResponse == "null")
      getResponse = obj["error"]["message"].as<String>();
    }
    else {
      getResponse = "Connected to " + String(myDomain) + " failed.";
      Serial.println("Connected to " + String(myDomain) + " failed.");
    }

    return getResponse;
}

// Get current memory usage information
String getMemoryInfo() {
  String msg = "";

  msg += "Free heap: ";
  msg += String(xPortGetFreeHeapSize());

  msg += "\nMin heap: ";
  msg += String(xPortGetMinimumEverFreeHeapSize());

  msg += "\nHistorical messages len: ";
  msg += String(historical_messages.length());

  return msg;
}

// Execute tool commands returned by Gemini
void useTools(String command, String message) {

    if (command == "/on") {
      digitalWrite(pinLed, 255);
      sendMessageToTelegram(telegramBot_token, telegramBot_chatID, "Turn on","");
      storeHistoricalMessages_sd(buildHistoricalData("user", command));
      storeHistoricalMessages_sd(buildHistoricalData("model", "Turn on the led"));

    } else if ((command) == "/off") {
      digitalWrite(pinLed, 0);
      sendMessageToTelegram(telegramBot_token, telegramBot_chatID, "Turn off","");
      storeHistoricalMessages_sd(buildHistoricalData("user", command));
      storeHistoricalMessages_sd(buildHistoricalData("model", "Turn off the led"));

    } else if (command == "/still") {
      sendCapturedImageToTelegram(telegramBot_token, telegramBot_chatID, 1);
      storeHistoricalMessages_sd(buildHistoricalData("user", command));
      storeHistoricalMessages_sd(buildHistoricalData("model", "Get still to Telegram Bot"));
      
    } else if (command == "/reset") {
      Gemini_chat_reset();
      sendMessageToTelegram(telegramBot_token, telegramBot_chatID, "Start a new chat","");
      storeHistoricalMessages_sd(buildHistoricalData("user", command));
      storeHistoricalMessages_sd(buildHistoricalData("model", "Start a new chat"));

    } else if (command == "/memory") {
      String msg = getMemoryInfo();
      sendMessageToTelegram(telegramBot_token, telegramBot_chatID, msg, "");
      storeHistoricalMessages_sd(buildHistoricalData("user", command));
      storeHistoricalMessages_sd(buildHistoricalData("model", msg));

    } else if (command == "/search") {
      String response = Gemini_chat_search_request(message);
      sendMessageToTelegram(telegramBot_token, telegramBot_chatID, response, "");

    } else if (command == "/vision") {
      String response = SendStillToGeminiVision(message);
      sendMessageToTelegram(telegramBot_token, telegramBot_chatID, response, "");
            
    } else {
      String response = Gemini_chat_request(command);
      if (response.indexOf("/") == 0) {
        useTools(response, command);
      } else 
        sendMessageToTelegram(telegramBot_token, telegramBot_chatID, response, "");
    }
}

// Initialize WiFi
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

}

// Poll Telegram Bot API for latest user message
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

      message_id = obj["result"][0]["message"]["message_id"].as<String>().toInt();
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
        
          if (message=="help"||message=="/help"||message=="/start") {
        
            String command =
              "/help command list\n/on turn on led\n/off turn off led\n/still get still\n/memory remaining memory\n/reset new chat";
        
            String keyboard =
              "{\"keyboard\":[[{\"text\":\"/help\"},{\"text\":\"/on\"},{\"text\":\"/off\"}],[{\"text\":\"/still\"},{\"text\":\"/memory\"},{\"text\":\"/reset\"}]],\"one_time_keyboard\":false}";
        
            sendMessageToTelegram(telegramBot_token, telegramBot_chatID, command, keyboard);
        
            storeHistoricalMessages_sd(buildHistoricalData("user", "Command list"));
            storeHistoricalMessages_sd(buildHistoricalData("model", command));   
        
          } else if (message=="null") {
        
            botClient.stop();
            getTelegramMessage();
        
          } else {
        
            useTools(message, "");
            
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

// Background task for continuous Telegram polling
void getTelegramMessage_task(void *param) {
  (void)param;
  while(1) {
    getTelegramMessage();
  }
}

// Arduino setup
void setup() {
  Serial.begin(115200);
  
  pinMode(pinLed, OUTPUT);

  initWiFi();
  
  config.setRotation(0);
  Camera.configVideoChannel(0, config);
  Camera.videoInit();
  Camera.channelBegin(0);  

  if (xTaskCreate(
        getTelegramMessage_task,
        (const char *)"getTelegramMessage_task",
        4096,
        NULL,
        tskIDLE_PRIORITY + 1,
        NULL
      )!= pdPASS) {

    Serial.println("Create getTelegramMessage task failed");
  } 

  gemini_role.replace("\"", "\\\"");
  tools_definition.replace("\"", "\\\"");
  system_content = "{\"role\": \"user\", \"parts\":[{ \"text\":\""+ gemini_role + tools_definition+"\"}]}";

  historical_messages = getHistoricalMessages_sd();
  if (historical_messages == "")
    historical_messages = system_content + buildHistoricalData("model", "OK");;

}

// Main loop
void loop() {
}
