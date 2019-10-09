/*
 * Original source: https://community.blynk.cc/t/c-blynk-code-examples-for-basic-tasks-work-in-progress/22596/20
 * 
 */
#define BLYNK_PRINT Serial // This prints to Serial Monitor

#include "settings.h"           
//#include "secret.h"               // <<--- UNCOMMENT this before you use and change values on config.h tab
#include "my_secret.h"              // <<--- COMMENT-OUT or REMOVE this line before you use. This is my personal settings.

#include <ESP8266WiFi.h>
#include <BlynkSimpleEsp8266.h>

#include <ESP8266mDNS.h>  // For OTA
#include <WiFiUdp.h>      // For OTA
#include <ArduinoOTA.h>   // For OTA

int ReCnctFlag;       // Reconnection Flag
int ReCnctCount = 0;  // Reconnection counter

BlynkTimer timer;

void setup() {
  Serial.begin(115200);
  
#if defined(USE_LOCAL_SERVER)
  WiFi.begin(WIFI_SSID, WIFI_PASS);         // Non-blocking if no WiFi available
  Blynk.config(AUTH, SERVER, PORT);
  Blynk.connect();
#else
  WiFi.begin(WIFI_SSID, WIFI_PASS);         // Non-blocking if no WiFi available
  Blynk.config(AUTH);
  Blynk.connect();
#endif   

  timer.setInterval(UPTIME_TIME, UpTime);

  ArduinoOTA.setHostname(OTA_HOSTNAME);  // For OTA
  ArduinoOTA.begin();                    // For OTA
}

BLYNK_CONNECTED() {
  Serial.println("Cconnected");
  ReCnctCount = 0;
}

void UpTime() {
  Blynk.virtualWrite(vPIN_UPTIME_COUNTER, millis() / 1000);  // Send UpTime seconds to App
  Serial.print("UpTime: ");
  Serial.println(millis() / 1000);          // Send UpTime seconds to Serial
  }

void loop() {
  timer.run();
  ArduinoOTA.handle();  // For OTA

  if (Blynk.connected()) {                                                    // If connected run as normal
    Blynk.run();
  } 
  else if (ReCnctFlag == 0) {                                                 // If NOT connected and not already trying to reconnect, set timer to try to reconnect in 30 seconds
      ReCnctFlag = 1;                                                         // Set reconnection Flag
      Serial.println("Starting reconnection timer in 30 seconds...");
      timer.setTimeout(30000L, []() {                                         // Lambda Reconnection Timer Function
      ReCnctFlag = 0;                                                         // Reset reconnection Flag
      ReCnctCount++;                                                          // Increment reconnection Counter
      Serial.print("Attempting reconnection #");
      Serial.println(ReCnctCount);
      Blynk.connect();                                                        // Try to reconnect to the server
    });                                                                       // END Timer Function
    }
}
