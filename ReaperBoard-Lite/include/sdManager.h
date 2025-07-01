#ifndef SDMANAGER_H
#define SDMANAGER_H

#include <Arduino.h>
#include <ArduinoJson.h>
#include <SPI.h>
#include <SD.h>

#define CSPIN D8
#define SETTINGS_FILE "/settings.json"
#define LOG_FILE "/logs.txt"
#define MAC_FOLDER "/macs"
#define AP_FOLDER "/aps"
#define MAX_LOG_SIZE 32768 // in bytes

#define DEFAULT_PASSCODE "333333"
#define DEFAULT_SCREEN_TIMEOUT 120

#define TAMPER_THRESHOLD 900

namespace SDManager {
    void begin();

    // Settings
    void loadSettings(String &passcode, int &screenTimeout);

    // Logs
    void logEvent(const String &type, const String &message);

    // MACs
    int listMACs(String *names, int maxCount);
    bool loadMAC(const String &name, uint8_t *macAddress);

    // APs
    int listAPs(String *names, int maxCount);
    bool loadAP(const String &name, String &ssid, String &mac, int &channel, String &webpage);

    // Tamper
    bool checkTamper();
}

#endif