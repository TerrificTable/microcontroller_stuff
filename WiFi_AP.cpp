#include <Arduino.h>
#include <Wire.h>
#include <SPI.h>


#include <ESP8266WiFi.h>

// for wifi to Connect to
// ===
#ifndef STASSID
#define STASSID         "WIFI NAME"
#define STAPSK          "WIFI PASSWORD"
#endif

const char *ssid            = STASSID;
const char *password        = STAPSK;
// ===

typedef struct {
    boolean         WLANConnected               = false;  
    boolean         WLANConnecting              = false;  
    unsigned long   WLANConnectingTimeOut       = 0;  
    unsigned long   WLANConnectingWaitTimeOut   = 0;  
} StatesData;
StatesData States;


WiFiClient          espClient;






void WIFI_Connect() {
    // Statemachine so it can run withouht freezing the esp
    
    // Connect to Wifi
    if (!States.WLANConnected &&  WiFi.status() == WL_CONNECTED) {
        States.WLANConnecting = false;
        States.WLANConnectingWaitTimeOut = 0;
        Serial.print("WiFi Connected: ");
        String IP = WiFi.localIP().toString();

        Serial.println(IP);
    }

    States.WLANConnected = WiFi.status() == WL_CONNECTED || WiFi.status()  == WL_IDLE_STATUS;
    
    if (States.WLANConnected) return;  
    
    
    if (States.WLANConnectingWaitTimeOut > 0 && millis() < States.WLANConnectingWaitTimeOut ) return;
    if (States.WLANConnectingWaitTimeOut > 0 && millis() > States.WLANConnectingWaitTimeOut ) States.WLANConnectingWaitTimeOut = 0;


    if (!States.WLANConnecting) {
        States.WLANConnecting = true;
        States.WLANConnectingTimeOut = millis() + 10000;  // 10 Sekunfen
        Serial.println("WiFi connecting....");
        
        WiFi.persistent(false); 
        WiFi.mode( WIFI_OFF );
        delay( 1 );
        WiFi.disconnect();
        WiFi.begin(ssid, password);
        return;
    }

    if (States.WLANConnecting &&  millis() > States.WLANConnectingTimeOut) {
        States.WLANConnecting  = false;
        Serial.println("WiFi connecting failed....");
        States.WLANConnectingWaitTimeOut = millis() + 10000;
    }
}




void setup() {
    Serial.begin(115200); // Set baud rate
    delay(100);  // Dont use delay, it freezes the entire device for whatever amount of time
  
    // Create Access Point  
    WiFi.softAP("ESP-8266 WiFi", "Password1234!");  // DO NOT USE SUCH A PASSWORD
    
    // Print AP IP
    Serial.println("WiFi AP IP: " + String(WiFi.softAPIP().toString()));
}


void loop() {
    // Connect to WIFI so if you connect to the esp wifi you will acutally get a internet connection
    WIFI_Connect();
}
