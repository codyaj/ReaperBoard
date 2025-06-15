#ifndef DEAUTH_H
#define DEAUTH_H

#include <Arduino.h>
#include <ESP8266WiFi.h>
#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>

extern "C" {
  #include "user_interface.h"
}

#include "oledController.h"

#define MAX_CLIENTS 100

enum class AttackStage {
    NETWORK_SCANNING,
    CLIENT_SNIFFING,
    DEAUTHING
};

enum class PacketType { 
    UNKNOWN, 
    PROBE_REQ, 
    DATA, 
    JUNK 
};

enum class Direction { 
    UNKNOWN, 
    TO_AP, 
    FROM_AP 
};

struct ClientInfo {
    uint8_t mac[6];
    uint32_t packetCount;
    uint32_t lastSeenMillis;
    int beaconCount;
    int probeReqCount;
    int probeResCount;
    int dataCount;
    Direction direction;
    bool isAP;
    uint8_t linkedAP[6];
    bool isLinked;
};

class DeauthDisplay : public OLEDDisplay {
private:
    ClientInfo* findClientByMac(uint8_t* addr);
    static void snifferCallback(uint8_t *buf, uint16_t len);

    void handleBeacon(uint8_t* APMac);
    void handleProbeReq(uint8_t* clientMac);
    void handleProbeRes(uint8_t* clientMac, uint8_t* APMac);
    void handleData(uint8_t* receiverMac, uint8_t* transmitterMac, uint8_t* APMac, uint8_t toDS, uint8_t fromDs);

    static DeauthDisplay* instance; // Work around for findClientMac

    int deauthPacketsSent = 0;
    bool attackToggle = false;
    AttackStage currStage;
    ClientInfo* clients;
    int clientIndex = 0;
    uint8_t targetClientMAC[6];
    uint8_t targetAPMAC[6];
    int selectedNetworkIndex;
    int networksFound = 0;
    int index = 0;
protected:
    void onExit() override;
public:
    DeauthDisplay();

    bool scanInputs() override;
    void displayScreen() override;
    void onEnter() override;
    void attack();
};

#endif