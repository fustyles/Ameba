/*
 * AmebaPro2 AP mode - Web page manager
------------------------------------------------------------
Author
------------------------------------------------------------
Author:
  ChungYi Fu (Kaohsiung, Taiwan)
  https://www.facebook.com/francefu

Repository:
  https://github.com/fustyles/fuClaw

Build Date: 2026-06-01 07:30
------------------------------------------------------------
Hardware
------------------------------------------------------------
AMB82-mini
- Green LED : GPIO 24
- Blue LED  : GPIO 23

HUB 8735 Ultra
- Green LED : GPIO 25
- Blue LED  : GPIO 26
- Fill LED  : GPIO 13
- Button    : GPIO 12 (input only, active-low)
------------------------------------------------------------
*/

// WiFi credentials
String wifiSsid = "teacher";
String wifiPassword = "12345678";

// AP credentials http://192.168.1.1:81
String apSsid = "fuclaw";
String apPassword = "12345678";

// Copy page content to a local buffer.
// mainPageHTML is cleared immediately so the next
// request can generate new content independently.
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

/**
 * @brief Decode URL encoded string.
 *
 * Converts:
 *   %20 -> space
 *   %2F -> '/'
 *   +   -> space
 *
 * Example:
 *   Hello%20World
 *     ->
 *   Hello World
 *
 * @param input URL encoded string
 * @return Decoded string
 */
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

/**
 * @brief HTTP server processing task.
 *
 * This FreeRTOS task continuously monitors incoming
 * HTTP client connections and processes requests.
 *
 * Supported routes:
 *
 *   GET /
 *      Home page
 *
 *   GET /chat
 *      Chat interface page
 *
 *   GET /message?<query>
 *      Receive URL encoded message
 *
 * Response generation flow:
 *
 *   Browser
 *      ↓
 *   HTTP Request
 *      ↓
 *   Parse URL
 *      ↓
 *   Generate HTML
 *      ↓
 *   Send Response
 *
 * @param param Unused task parameter
 */
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
          
		  // ----------------------------------------------------
		  // Route: GET /
		  // Display  welcome page.
		  // ----------------------------------------------------
          if ((currentLine.indexOf("GET / ") != -1) && (currentLine.indexOf(" HTTP") != -1)) {
            
            mainPageHTML = "Welcome to <a href=\"https://github.com/fustyles/fuClaw\">fuClaw</a> home!";

            currentLine = "";    
                    
          } 
		  // ----------------------------------------------------
		  // Route: GET /chat
		  //
		  // Returns the embedded chat interface stored in
		  // INDEX_CHAT_HTML.
		  // ----------------------------------------------------		  
          else if ((currentLine.indexOf("GET /chat") != -1) && (currentLine.indexOf(" HTTP") != -1)) {

            mainPageHTML = String(INDEX_CHAT_HTML);

            currentLine = "";

          }    
		  // ----------------------------------------------------
		  // Route: GET /message?<query>
		  //
		  // Example:
		  //   /message?text=Hello%20World
		  //
		  // URL parameters are automatically decoded before
		  // being processed.
		  // ----------------------------------------------------		  
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

/**
 * @brief Initialize WiFi subsystem.
 *
 * Startup sequence:
 *
 * 1. Enable AP + STA concurrent mode
 * 2. Start Access Point
 * 3. Attempt STA connection
 * 4. Timeout after 5 seconds
 *
 * AP remains active regardless of STA connection
 * success or failure.
 */
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

/**
 * @brief Arduino initialization entry point.
 *
 * Initialization order:
 *
 *   Serial
 *      ↓
 *   LED GPIO
 *      ↓
 *   WiFi
 *      ↓
 *   HTTP Server
 *      ↓
 *   FreeRTOS Task
 *      ↓
 *   Status Output
 *
 */
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
