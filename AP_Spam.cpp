#include <Arduino.h>
#include <ESP8266WiFi.h>
extern "C" {
    #include "user_interface.h"
}

String apNames[] = { // AP NAMES
    "Test",
    "Test1",
    "Test2"
};

byte apMACs[3][6]; // apMACs[x][] -> x = length of apNames
int apCount = 3; // Length of `apNames`

int currentAP = 0;
int bPointer = 52;
char prefix = ' ';

byte packet[128] = {
    /* IEEE 802.11 Beacon Frame */
    /*0*/ 0x80, 0x00, 0x00, 0x00,                          // type/subtype = beacon
    /*4*/ 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF,              // dst addr = broadcast
    /*10*/ 0x01, 0x02, 0x03, 0x04, 0x05, 0x06,             // src addr = random MAC
    /*16*/ 0x01, 0x02, 0x03, 0x04, 0x05, 0x06,             // bss id = src addr
    /*22*/ 0x90, 0x53,                                     // seq number 1337 + frag number 0
    /*24*/ 0xDE, 0xAD, 0x00, 0xBE, 0xEF, 0x00, 0xF0, 0x0D, // timestamp
    /*32*/ 0x64, 0x00,                                     // beacon interval (~100ms)
    /*34*/ 0x01, 0x04,                                     // capabilities info

    /* IEEE 802.11 wireless LAN managment frame */
    /*36*/ 0x01, 0x08, // tag = Supported Rates, size
    0x82, 0x84, 0x8b, 0x96, 0x24, 0x30, 0x48, 0x6c,
    /*46*/ 0x03, 0x01, 0xFF, // tag = Channel, size, random channel

    /*49*/ 0x00, 0xFF, 0x23 // tag = SSID, size, prefix char
};

void fillMACs() {
    for (int j = 0; j < apCount; j++) {
        apMACs[j][0] = random(256);
        apMACs[j][1] = random(256);
        apMACs[j][2] = random(256);
        apMACs[j][3] = random(256);
        apMACs[j][4] = random(256);
        apMACs[j][5] = random(256);
    }
}

void setup() {
    Serial.begin(115200);
    delay(100);
    Serial.println("\nStarting...");
    Serial.println("AP Count:  " + apCount);

    wifi_set_opmode(STATION_MODE);
    wifi_promiscuous_enable(1);
    packet[51] = prefix;
    
    pinMode(2, OUTPUT);
    fillMACs();
}

void loop() {

    static unsigned long    delayMillis     = 0;
    static int mode                         = 0;

    if (millis() >= delayMillis) {
        delayMillis = millis() + 10000;

        // Serial.println("Toggeling LED");

        digitalWrite(2, (mode % 2));
        mode++;
    }



    if (currentAP == apCount) {
        currentAP = 0;
    }

    // Set random channel (pinning this variable wasn't giving me good results)
    byte channel = random(1, 11);
    wifi_set_channel(channel);
    packet[48] = channel;

    // Copying src addr and bssid to packet buffer
    packet[10] = packet[16] = apMACs[currentAP][0];
    packet[11] = packet[17] = apMACs[currentAP][1];
    packet[12] = packet[18] = apMACs[currentAP][2];
    packet[13] = packet[19] = apMACs[currentAP][3];
    packet[14] = packet[20] = apMACs[currentAP][4];
    packet[15] = packet[21] = apMACs[currentAP][5];

    // Copying SSID to packet buffer
    byte currLenght = apNames[currentAP].length();
    packet[50] = currLenght + 1;
    for (int i = bPointer; i < bPointer + currLenght; i++)
    {
        packet[i] = apNames[currentAP][i - bPointer];
    }

    // Sending beacon. Feel free to play with the numbers here, this was the best combination I found.
    for (int i = 0; i < 8; i++)
    {
        wifi_send_pkt_freedom(packet, bPointer + currLenght, 0);
        delayMicroseconds(64);
    }
    delay(4);

    currentAP++;
}
