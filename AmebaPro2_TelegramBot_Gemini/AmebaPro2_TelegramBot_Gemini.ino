/*
------------------------------------------------------------
fuClaw AI Telegram Assistant with Gemini Integration
------------------------------------------------------------

Author:
  ChungYi Fu (Kaohsiung, Taiwan)
  https://www.facebook.com/francefu

Description:
  This project implements an AI-powered Telegram assistant
  running on the AMB82-mini (HUB 8735 Ultra / Realtek Ameba Pro2)
  development board.

  The system connects to WiFi, receives Telegram messages via
  long-polling (getUpdates), communicates with Google Gemini API,
  and executes tool-based commands for hardware control,
  vision analysis, web search, and system utilities.

Core Architecture:
  Telegram User
      ↓
  ESP32 / AMB82-mini (FreeRTOS task)
      ↓
  Gemini API (generateContent + optional Google Search tool)
      ↓
  JSON-based Tool Router
      ↓
  Hardware / Camera / Memory / Telegram Response

------------------------------------------------------------
Main Features
------------------------------------------------------------

- Telegram Bot interaction via HTTPS polling (getUpdates)
- Gemini conversational AI integration (generateContent API)
- JSON-based tool calling system (prompt-controlled routing)
- Google Search grounding via Gemini tool
- Camera capture and Telegram image upload (sendPhoto)
- Gemini Vision multimodal image analysis (image + text input)
- Digital output control (GPIO ON/OFF)
- PWM output control (0–255 brightness)
- System memory monitoring (heap + conversation size)
- Conversation memory stored in RAM (historical_messages)
- FreeRTOS background task for continuous Telegram polling
- Automatic WiFi reconnection handling

------------------------------------------------------------
Supported Telegram Commands
------------------------------------------------------------

/help     Show command list
/on       Turn ON digital output device
/off      Turn OFF digital output device
/pwm      Set PWM brightness (0–255)
/still    Capture and send image to Telegram
/vision   Analyze image using Gemini Vision
/search   Web-grounded search using Gemini
/memory   Show system memory usage
/reset    Reset conversation memory

------------------------------------------------------------
Tool Routing Mechanism (Gemini Prompt-Based)
------------------------------------------------------------

Gemini is instructed via system prompt rules to return ONLY JSON
tool instructions when an action is required.

------------------------------------------------------------
Tool Format Definition
------------------------------------------------------------

1) Digital Output Control
{
  "type": "tool_call",
  "method": "/on" | "/off",
  "params": {
    "pin": "<GPIO pin>",
    "pinmode": "digitalwrite",
    "value": 1 | 0
  }
}

2) PWM Output Control
{
  "type": "tool_call",
  "method": "/pwm",
  "params": {
    "pin": "<GPIO pin>",
    "pinmode": "analogwrite",
    "value": 0-255
  }
}

3) Image Capture
{
  "type": "tool_call",
  "method": "/still",
  "params": {}
}

4) Vision Analysis (camera + Gemini)
{
  "type": "tool_call",
  "method": "/vision",
  "params": {
    "query": "<user request>"
  }
}

5) Web Search (Gemini grounding tool)
{
  "type": "tool_call",
  "method": "/search",
  "params": {
    "query": "<search query>"
  }
}

6) System Utilities
{
  "type": "tool_call",
  "method": "/memory" | "/reset",
  "params": {}
}

7) Default Chat Response
If no tool is required:

{
  "type": "tool_call",
  "method": "/chat",
  "params": {
    "reply": "<natural language response>"
  }
}

------------------------------------------------------------
Execution Flow (Actual Implementation Behavior)
------------------------------------------------------------

1. Telegram message received via getUpdates (polling task)
2. Message is forwarded to Gemini API OR parsed as command
3. Gemini returns either:
     - tool_call JSON
     - or natural chat reply wrapped in /chat tool
4. ESP32 parses JSON using ArduinoJson
5. useTools() dispatches execution:
     - GPIO control
     - Camera capture
     - Vision analysis
     - Search via Gemini grounding
     - Memory handling
     - Telegram response
6. Result is sent back to Telegram via HTTPS API

------------------------------------------------------------
Hardware Support
------------------------------------------------------------

- AMB82-mini (HUB 8735 Ultra platform)
- Built-in WiFi (ESP32-like architecture)
- Camera module (VideoStream API)
- GPIO output (digital + PWM)

------------------------------------------------------------
Software Dependencies
------------------------------------------------------------

- WiFi.h
- WiFiSSLClient (HTTPS communication)
- ArduinoJson (JSON parsing)
- FreeRTOS (background Telegram task)
- VideoStream (camera capture)
- Base64 encoding library

------------------------------------------------------------
Implementation Notes
------------------------------------------------------------

- Conversation history is stored in RAM (historical_messages)
- System prompt is dynamically injected at runtime
- All Gemini communication uses HTTPS POST
- Telegram uses long polling (not webhook)
- Tool execution is deterministic after JSON parsing
- Camera image is Base64 encoded for Gemini Vision API

------------------------------------------------------------
Limitations
------------------------------------------------------------

- RAM usage increases with long conversations
- No persistent storage (reboot resets memory)
- Vision performance depends on image size + network speed
- Telegram polling is continuous (not event-driven webhook)
- JSON parsing relies on strict Gemini output formatting

------------------------------------------------------------
Version
------------------------------------------------------------

Persistent Memory Edition (Refactored Documentation)
2026-05-16 19:30
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
You are a professional assistant with a lively, natural, and friendly personality, responding according to the user's language.
)"; 

// Tool routing rules for Gemini.
String tools_definition = R"(
tools definition:
If the system determines that the user intends to query recent data, it must return only this valid JSON object:

{"type": "tool_call",  
  "method": "/search",
  "params": {
    "query": "<user query content>"
  }
}

If the system determines that the user expects assistance in turning on turning on a digital output device, it must return only this valid JSON object:

{
  "type": "tool_call",
  "method": "/on",
  "params": {
    "pin": "<Device pin number. If the user explicitly specifies a pin number, use that number. If the user does not specify which device pin to control, first ask the user in normal conversational reply and wait for the user's answer before filling this value. If multiple pins are mentioned, choose only the first one in order of appearance.>",
	"pinmode": "digitalwrite",
    "value": 1
  }
}

If the system determines that the user expects assistance in turning off a digital output device, it must return only this valid JSON object:

{
  "type": "tool_call",
  "method":"/off",
  "params": {
    "pin": "<Device pin number. If the user explicitly specifies a pin number, use that number. If the user does not specify which device pin to control, first ask the user in normal conversational reply and wait for the user's answer before filling this value. If multiple pins are mentioned, choose only the first one in order of appearance.>",
	"pinmode": "digitalwrite",	
    "value": 0
  }
}

If the system determines that the user expects assistance in setting a analog output device, it must return only this valid JSON object:

{
  "type": "tool_call",
  "method": "/pwm",
  "params": {
    "pin": "<Device pin number. If the user explicitly specifies a pin number, use that number. If the user does not specify which device pin to control, first ask the user in normal conversational reply and wait for the user's answer before filling this value. If multiple pins are mentioned, choose only the first one in order of appearance.>",
    "pinmode": "analogwrite",
    "value": "<brightness value from 0 to 255>"
  }
}

If the system determines that the user intends to capture and send an image, it must return only this valid JSON object:

{
  "type": "tool_call",
  "method":"/still",
  "params":{}
}

If the system determines that the user requests visual understanding, image inspection, or analysis using the device camera, and the device is equipped with a camera capable of capturing a current image, it must first capture a still image from the camera and then perform visual analysis on that image. It must return only this valid JSON object:

{
  "type": "tool_call",
  "method":"/vision",
  "params":{
    "query":"<user analysis request. The reply must be treated as a single-line JSON string. Do not format it for human readability.>"
  }
}

If the system determines that the user requests memory status, it must return only this valid JSON object:

{
  "type": "tool_call",
  "method":"/memory",
  "params":{}
}

If the system determines that the user requests resetting conversation memory, it must return only this valid JSON object:

{
  "type": "tool_call",
  "method":"/reset",
  "params":{}
}

Otherwise, it must return:

{
  "type": "tool_call",
  "method":"/chat",
  "params":{
    "reply":"<normal conversational reply to the user. The reply must be treated as a single-line JSON string. Do not format it for human readability.>"
  }
}

Rules:

- Return only valid JSON when a tool is required.
- Do not wrap JSON in markdown.
- Do not include explanation text before or after JSON.
- JSON must be syntactically valid.
- Use only the exact keys and structure defined above.
- Do not add extra fields.
- Do not omit required fields.
- Do not guess missing parameter values.
- If required information is missing, respond with a normal conversational reply asking the user for the missing information, and wait for the user's answer before returning JSON.
- Never mix conversational text and JSON in the same response.
- When returning conversational replies, do not output JSON-like content.
- Always respond in the user's language.
- Normal conversational replies must not start with "/" or "{".
- If unsure about formatting, prefer simple plain text instead of special formatting characters.
- For /pwm, value must be an integer between 0 and 255.
- For /on and /off, value must be exactly 1 or 0.
- Use "digitalwrite" only for /on and /off.
- Use "analogwrite" only for /pwm.
- Otherwise, it will respond with a normal conversational reply
)";

String gemini_model = "gemini-3-flash-preview";
int gemini_maxOutputTokens = 2000;  // If the AI ​​is unable to transmit complete data, please increase the value.
float gemini_temperature = 1.0;

// Serialized system prompt content used as the initial conversation context
String system_content = "";
String system_content_notools = "";
  
// Stores entire chat history in Gemini API JSON format
// Used to preserve conversation memory across requests
String historical_messages = "";

// Indicator LED output pin
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

// Convert role/content pair into Gemini-compatible JSON message object
String buildHistoricalData(String role, String content) {
  String jsonMessage = ", {\"role\": \"";
  jsonMessage += role;
  jsonMessage += "\", \"parts\":[{ \"text\": \"";
  jsonMessage += content;
  jsonMessage += "\" }]}";
  return jsonMessage;
}

// Reset conversation memory to initial system prompt state
void Gemini_chat_reset() {
  historical_messages = "";
  system_content = "{\"role\": \"user\", \"parts\":[{ \"text\":\"" + gemini_role + tools_definition + "\"}]}" + buildHistoricalData("model", "OK");
  system_content_notools = "{\"role\": \"user\", \"parts\":[{ \"text\":\"" + gemini_role + "\"}]}" + buildHistoricalData("model", "OK");
  
}

// Send request to Gemini and return response text
String Gemini_chat_request(String message, bool tools) {
  message.replace("\"", "\\\"");

  String user_content =
    "{\"role\": \"user\", \"parts\":[{ \"text\": \""+ message+"\" }]}";

  historical_messages += ", "+user_content;

  String contents = "";
  if (tools)
    contents = system_content + historical_messages;
  else
    contents = system_content_notools + historical_messages;  

  String request =
    "{\"contents\": [" + contents +
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

        historical_messages += ", "+assistant_content;

        return getResponse;
      }
    }

    client.stop();
    Serial.println(Feedback);

    return "Please try again with more detailed information or check if the API key is valid.";
  }
  else
    return "Connection failed";
}

// Send Gemini request with Google Search tool enabled
String Gemini_chat_search_request(String message, bool tools) {

  // Escape quotation marks for JSON formatting
  message.replace("\"", "\\\"");

  // Build user message JSON content
  String user_content = "{\"role\": \"user\", \"parts\":[{ \"text\": \""+ message+"\" }]}";

  // Append user message to conversation history
  historical_messages += ", "+user_content;

  String contents = "";
  if (tools)
    contents = system_content + historical_messages;
  else
    contents = system_content_notools + historical_messages;

  // Build Gemini API request with Google Search tool
  String request = "{\"contents\": [" + contents + "],\"tools\": [{google_search: {}}],\"generationConfig\": {\"maxOutputTokens\": " + gemini_maxOutputTokens + ", \"temperature\": " + gemini_temperature + "}}";
  
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
    
        historical_messages += ", "+assistant_content;

        return getResponse;
      }
    }

    // Close connection on timeout
    client.stop();

    Serial.println(Feedback);

    return "Please try again with more detailed information or check if the API key is valid.";
  }
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
    DynamicJsonDocument doc(8192);
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

// Control device output using digital or PWM (analog) mode.
// This function supports general-purpose actuators such as LED, relay, and other GPIO-controlled devices.
String tool_pinOutput(int pin, String mode, int value) {
	pinMode(pin, OUTPUT);
	mode.toLowerCase();
	
	if (mode=="digitalwrite") {
        if (value != 0 && value != 1)
            return "[tool_pinOutput] Error: Invalid digital value";
		
		digitalWrite(pin, value);
        if (value == 1)
            return "Device(pin="+String(pin)+") turned on";

        return "Device(pin="+String(pin)+") turned off";
	}	
    else if (mode == "analogwrite") {

        value = constrain(value, 0, 255);

        analogWrite(pin, value);

        return "Device(pin="+String(pin)+") brightness set to " + String(value);
    }

    return "[tool_pinOutput] Error: Invalid output mode";
}

// Execute tool commands returned by Gemini
void useTools(String command, JsonObject params) {

    if (command == "/on"||command == "/off"||command == "/pwm") {
      int pin = params["pin"].as<int>();
      String pinmode = params["pinmode"].as<String>();
      int value = params["value"].as<int>();
      String status = tool_pinOutput(pin, pinmode, value);
	  
      historical_messages += buildHistoricalData("user", command);
      historical_messages += buildHistoricalData("model", status); 

      status = Gemini_chat_request("Respond only with a natural language description of the device's current operating state in the user's language. Never output tool_call, JSON, or any structured format. If there are any unfinished or pending tasks, clearly inform the user and ask whether to continue processing them.", 0);
      sendMessageToTelegram(telegramBot_token, telegramBot_chatID, status,"");                
	  
    } else if (command == "/still") {
      sendCapturedImageToTelegram(telegramBot_token, telegramBot_chatID, 1);

      historical_messages += buildHistoricalData("user", command);
      historical_messages += buildHistoricalData("model", "Get still to Telegram Bot");      
      
    } else if (command == "/reset") {
      Gemini_chat_reset();
      sendMessageToTelegram(telegramBot_token, telegramBot_chatID, "OK","");

      historical_messages += buildHistoricalData("user", command);
      historical_messages += buildHistoricalData("model", "Start a new chat");      

    } else if (command == "/memory") {
      String msg = getMemoryInfo();
      sendMessageToTelegram(telegramBot_token, telegramBot_chatID, msg, "");

      historical_messages += buildHistoricalData("user", command);
      historical_messages += buildHistoricalData("model", msg);      

    } else if (command == "/search") {
      String query = params["query"].as<String>();
      String response = Gemini_chat_search_request(query, 0);
      sendMessageToTelegram(telegramBot_token, telegramBot_chatID, response, "");

      historical_messages += buildHistoricalData("user", query);
      historical_messages += buildHistoricalData("model", response);      

    } else if (command == "/vision") {
      String prompt = params["query"].as<String>();
      String response = SendStillToGeminiVision(prompt);
      sendMessageToTelegram(telegramBot_token, telegramBot_chatID, response, "");

      historical_messages += buildHistoricalData("user", prompt);
      historical_messages += buildHistoricalData("model", response);      
            
    } else if (command == "/chat") {
      String reply = params["reply"].as<String>();
      sendMessageToTelegram(telegramBot_token, telegramBot_chatID, reply, "");
    }
}

// Initialize WiFi
void initWiFi() {
  for (int i=0;i<2;i++) {

    if (String(wifi_ssid)=="")
      break;

    WiFi.begin(wifi_ssid, wifi_pass);
    delay(1000);

    Serial.println();
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
  DynamicJsonDocument doc(8192);

  String message;
  long message_id;

  if (messageLastId == 0)
    Serial.println("Connect to " + String(myDomain));

  if (botClient.connect(myDomain, 443)) {

    if (messageLastId == 0) {
      Serial.println("Connection successful");
	  
      for (int i = 0; i < 3; i++) {
        digitalWrite(pinLed, HIGH);
        delay(500);
        digitalWrite(pinLed, LOW);
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
              "/help command list\n"
              "/still get still\n"
              "/memory remaining memory\n"
              "/reset new chat\n\n"
              "You can chat with Gemini using natural language.\n"
              "You can also use Google Search for real-time information.\n\n"
              "Hardware control supported:\n"
              "- Digital output (on/off)\n"
              "- PWM output (0–255 brightness)\n\n"
              "The system will automatically choose the correct tool based on your request.";
          
            String keyboard =
              "{\"keyboard\":[[{\"text\":\"/help\"},{\"text\":\"/still\"},{\"text\":\"/memory\"},{\"text\":\"/reset\"}]],\"one_time_keyboard\":false}";
        
            sendMessageToTelegram(telegramBot_token, telegramBot_chatID, command, keyboard);

            historical_messages += buildHistoricalData("user", "Command list");
            historical_messages += buildHistoricalData("model", command);               
        
          } else if (message=="null") {
        
            botClient.stop();
            getTelegramMessage();
        
          } else {
        
            if (message.startsWith("/")) {
              useTools(message, JsonObject());
              
            } else {
              
              message = Gemini_chat_request(message, 1);

              message.replace("\\\"", "\"");            
              message.replace("\\n", "");
              message.replace("\n", "");
              message.replace("\r", "");
              message.replace("\t", "");
              message.replace(String((char)9), "");
              message.replace("\0", "");
                            
              int start = message.indexOf('{');
              int end = message.lastIndexOf('}');
              
              if (start == -1 || end == -1 || end <= start) {
                  sendMessageToTelegram(telegramBot_token, telegramBot_chatID, message, "");
                  return;
              }
              
              JsonObject obj;
              DynamicJsonDocument doc(2048);
              DeserializationError error = deserializeJson(doc, message);
              if (error) {
                  Serial.println("JSON parse failed\n\n");
                  Serial.println(message);
                  return;
              }
                  
              obj = doc.as<JsonObject>();
              String method =  obj["method"].as<String>();
              JsonObject params = obj["params"];
              useTools(method, params);
              
            }	
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
    vTaskDelay(1000 / portTICK_PERIOD_MS);
  }
}

// Arduino setup
void setup() {
  Serial.begin(115200);
  
  // Indicator LED  
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
  system_content = "{\"role\": \"user\", \"parts\":[{ \"text\":\"" + gemini_role + tools_definition + "\"}]}" + buildHistoricalData("model", "OK");
  system_content_notools = "{\"role\": \"user\", \"parts\":[{ \"text\":\"" + gemini_role + "\"}]}" + buildHistoricalData("model", "OK");
}

// Main loop
void loop() {
}
