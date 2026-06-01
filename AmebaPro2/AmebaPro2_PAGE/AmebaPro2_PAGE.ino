/*
 * ============================================================
 * fuClaw - AmebaPro2 AP Mode Web Page Manager
 * ============================================================
 *
 * Purpose
 * ------------------------------------------------------------
 * This module provides a lightweight HTTP server running on
 * AmebaPro2 devices.
 *
 * Features:
 * - AP (Access Point) mode support
 * - Concurrent STA + AP WiFi operation
 * - Web page hosting
 * - Chat UI page delivery
 * - HTTP GET message receiving
 * - FreeRTOS based request handling task
 *
 * Architecture
 * ------------------------------------------------------------
 *
 * Browser
 *    |
 *    | HTTP GET
 *    v
 * +----------------+
 * | WiFiServer(80) |
 * +----------------+
 *          |
 *          v
 * +---------------------+
 * | task_getRequest()   |
 * +---------------------+
 *          |
 *          +---- "/" --------> Home page
 *          |
 *          +---- "/chat" ----> Chat UI
 *          |
 *          +---- "/message" -> Receive messages
 *
 * HTTP Endpoints
 * ------------------------------------------------------------
 *
 * GET /
 *     Home page
 *
 * GET /chat
 *     Return chat interface HTML
 *
 * GET /message?<query>
 *     Receive URL encoded message
 *
 * Example:
 *     http://192.168.1.1/message?text=Hello
 *
 * Build Information
 * ------------------------------------------------------------
 * Author:
 *   ChungYi Fu
 *   Kaohsiung, Taiwan
 *
 * Facebook:
 *   https://www.facebook.com/francefu
 *
 * Repository:
 *   https://github.com/fustyles/fuClaw
 *
 * Build Date:
 *   2026-06-01 07:30
 *
 * Hardware Support
 * ------------------------------------------------------------
 *
 * AMB82-mini
 *   Green LED : GPIO24
 *   Blue LED  : GPIO23
 *
 * HUB 8735 Ultra
 *   Green LED : GPIO25
 *   Blue LED  : GPIO26
 *   Fill LED  : GPIO13
 *   Button    : GPIO12 (Input Only, Active Low)
 *
 * WiFi Configuration
 * ------------------------------------------------------------
 *
 * AP Mode:
 *   SSID     : fuclaw
 *   Password : 12345678
 *   Default IP:
 *       http://192.168.1.1
 *
 * Station Mode:
 *   Configurable through source code
 *
 * FreeRTOS Tasks
 * ------------------------------------------------------------
 *
 * task_getRequest
 *   Priority : tskIDLE_PRIORITY + 1
 *   Stack    : 16384 bytes
 *
 * Responsibilities:
 *   - Accept client connections
 *   - Parse HTTP requests
 *   - Generate response pages
 *   - Send web content
 *
 * Notes
 * ------------------------------------------------------------
 * - Uses simple line-based HTTP parsing.
 * - Only HTTP GET requests are processed.
 * - Responses are sent in 512-byte chunks to reduce
 *   memory pressure.
 * - URL query strings are automatically URL-decoded.
 *
 * ============================================================
 */

// WiFi credentials
String wifiSsid = "xxxxxxxxxx";
String wifiPassword = "xxxxxxxxxx";

// AP credentials http://192.168.1.1:81
String apSsid = "fuclaw";
String apPassword = "12345678";

#include "index_chat_html.h"
String mainPageHTML = "";

// Indicator LED output pin
int ledPin = 24;    // green led (AMB82-mini: 24, HUB 8735 Ultra: 25)

#include <WiFi.h>

char channel_ap[] = "2";
WiFiServer server(80);

#include "FreeRTOS.h"
#include "task.h"

#define CONFIG_INIC_IPC_HIGH_TP

String urldecode(const String& input) {
    String result = "";
    result.reserve(input.length());
    for (int i = 0; i < (int)input.length(); i++) {
        if (input[i] == '%' && i + 2 < (int)input.length()) {
            char hex[3] = { input[i+1], input[i+2], '\0' };
            uint8_t val = (uint8_t)strtol(hex, nullptr, 16);
            result.concat((char)val);
            i += 2;
        } else if (input[i] == '+') {
            result += ' ';
        } else {
            result += input[i];
        }
    }
    return result;
}

// web page
void task_getRequest(void *param) {
  (void)param;
  while (1) {
	  
    WiFiClient client = server.available();

    if (client) {
      String currentLine = "";  // Buffer to accumulate one line of the HTTP request
      
      while (client.connected()) {
        if (client.available()) {
          char c = client.read();

          if (c == '\n') {
            if (currentLine.length() == 0) {
            
              String pageToSend = mainPageHTML;
              mainPageHTML = "";
            
              client.println("HTTP/1.1 200 OK");
              client.println("Content-Type: text/html; charset=utf-8");
              client.println("Content-Length: " + String(pageToSend.length()));
              client.println("Access-Control-Allow-Origin: *");
              client.println("Cache-Control: no-cache");
              client.println("Connection: close");
              client.println();
            
              const char* ptr = pageToSend.c_str();
              int total  = pageToSend.length();
              int sent   = 0;
              while (sent < total) {
                int chunk   = (total - sent) > 512 ? 512 : (total - sent);
                int written = client.write((const uint8_t*)(ptr + sent), chunk);
                if (written > 0) sent += written;
                else delay(5);
              }
              client.flush();
            
              break;

            } else {
              currentLine = "";
            }
          }
          else if (c != '\r') {
            currentLine += c;
          }
          
          // Debug: print any URL query string (e.g. GET /?ssid=xxx HTTP/1.1) to Serial
          if ((currentLine.indexOf("GET / ") != -1) && (currentLine.indexOf(" HTTP") != -1)) {
            
            mainPageHTML = "Welcome to <a href=\"https://github.com/fustyles/fuClaw\">fuClaw</a> home!";

            currentLine = "";    
                    
          } 
          else if ((currentLine.indexOf("GET /chat") != -1) && (currentLine.indexOf(" HTTP") != -1)) {

            mainPageHTML = String(INDEX_CHAT_HTML);

            currentLine = "";

          }                      
          else if ((currentLine.indexOf("GET /message?") != -1) && (currentLine.indexOf(" HTTP") != -1)) {

            currentLine.replace("GET /message?", "");
            currentLine.replace(" HTTP", "");
            currentLine = urldecode(currentLine); 
                        
            mainPageHTML = "Receive: " + currentLine;
            
            currentLine = "";

          }      
        }
      }

      client.stop();
    }
	
  }
}

// Initialize WiFi
void initWiFi() {
	
  WiFi.enableConcurrent();
  WiFi.apbegin((char*)apSsid.c_str(), (char*)apPassword.c_str(), channel_ap, 0);
    
  for (int i=0;i<2;i++) {

    if (wifiSsid=="")
      break;

    WiFi.begin((char*)wifiSsid.c_str(), (char*)wifiPassword.c_str());
    delay(1000);

    Serial.println();
    Serial.print("Connecting to ");
    Serial.println(wifiSsid);

    unsigned long StartTime=millis();

    while (WiFi.status() != WL_CONNECTED) {
      delay(500);

      if ((StartTime+5000) < millis())
        break;
    }
  }
  
}

String Ip2String(IPAddress ip) {
  return String(ip[0])+String(".")+String(ip[1])+String(".")+String(ip[2])+String(".")+String(ip[3]);
}

// Arduino setup
void setup() {
  Serial.begin(115200);

  // Indicator LED  
  pinMode(ledPin, OUTPUT);

  initWiFi();

  server.begin();   

  if (xTaskCreate(
        task_getRequest,
        (const char *)"task_getRequest",
        16384,
        NULL,
        tskIDLE_PRIORITY + 1,
        NULL
      )!= pdPASS) {

    Serial.println("Create task_task_getRequest failed");
  }        

  Serial.println("\n"); 
  Serial.println("http://192.168.1.1");
  Serial.println("AP ssid : " + apSsid);
  Serial.println("AP password : " + apPassword);
  Serial.println("\n");  

  if (WiFi.status() == WL_CONNECTED) {
    for (int i=0 ; i<3 ; i++) {
      digitalWrite(ledPin, 1);
      delay(300);
      digitalWrite(ledPin, 0);
      delay(300);      
    }
    
    Serial.println("Home\nhttp://" + Ip2String(WiFi.localIP()));
    Serial.println("Chat\nhttp://" + Ip2String(WiFi.localIP()) + "/chat");    
    Serial.println("\n");   
  }   
  
}

// Main loop
void loop() {
}
