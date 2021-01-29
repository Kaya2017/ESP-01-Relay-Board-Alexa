#include <Arduino.h>
#include <ESP8266WiFi.h>
#include "fauxmoESP.h"
#define RELAY_PIN_1 0
#define SERIAL_BAUDRATE 115000
#define WIFI_SSID "Dein Wlan"
#define WIFI_PASS "Dein Password"
#define SOCKET_2 "ESP-Socket"

fauxmoESP fauxmo;

void wifiSetup() {
  
  WiFi.mode(WIFI_STA);
  Serial.printf("[WIFI] Connecting to %s ", WIFI_SSID);
  WiFi.begin(WIFI_SSID, WIFI_PASS);
 
  while (WiFi.status() != WL_CONNECTED) {
    Serial.print(".");
    delay(100);
  }
  Serial.println();  
  Serial.printf("[WIFI] STATION Mode, SSID: %s, IP address: %s\n", WiFi.SSID().c_str(), WiFi.localIP().toString().c_str());
}

void setup() {
  
  Serial.begin(SERIAL_BAUDRATE);
  Serial.println();
  
  wifiSetup();
  
  pinMode(RELAY_PIN_1, OUTPUT);
  digitalWrite(RELAY_PIN_1, HIGH);  
  
  fauxmo.createServer(true); // not needed, this is the default value
  fauxmo.setPort(80); // This is required for gen3 devices 
  fauxmo.enable(true);
  fauxmo.addDevice(SOCKET_2);  
  fauxmo.onSetState([](unsigned char device_id, const char * device_name, bool state, unsigned char value) {
           
    Serial.printf("[MAIN] Device #%d (%s) state: %s value: %d\n", device_id, device_name, state ? "AN" : "AUS", value);
    if ( (strcmp(device_name, SOCKET_2) == 0) ) {      
      digitalWrite(RELAY_PIN_1, !digitalRead(RELAY_PIN_1));
      if (state) {
        digitalWrite(RELAY_PIN_1, LOW);
      } else {
        digitalWrite(RELAY_PIN_1, HIGH);
      }
    }
    
  });

}

void loop() {
  
  fauxmo.handle();

 
}  
  
