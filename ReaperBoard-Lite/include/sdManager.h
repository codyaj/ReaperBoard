#ifndef SDMANAGER_H
#define SDMANAGER_H

#include <Arduino.h>
#include <ArduinoJson.h>
#include <SPI.h>
#include <SD.h>

#define CSPIN D8
#define SETTINGS_FILE "/settings.json"
#define LOG_FILE "/logs.txt"
#define SSID_FILE "/ssids.txt"
#define MAC_FOLDER "/macs"
#define AP_FOLDER "/aps"
#define DATA_FOLDER "/data"
#define MAX_LOG_SIZE 32768 // in bytes

#define DEFAULT_PASSCODE "333333"
#define DEFAULT_SCREEN_TIMEOUT 120

#define TAMPER_THRESHOLD 900

namespace SDManager {
    void begin();

    // Settings
    void loadSettings(String &passcode, int &screenTimeout);
    void setScreenTimeout(int screenTimeout);

    // Logs
    void logEvent(const String &type, const String &message);

    // MACs
    int listMACs(String *names, int maxCount);
    bool loadMAC(const String &name, uint8_t *macAddress);

    // APs
    int listAPs(String *names, int maxCount);
    bool loadAP(const String &name, String &ssid, String &mac, int &channel, String &webpage);
    void logData(const String &apName, const String *messages, const int amount);

    // Saved Data
    int listData(String *names, int maxCount);
    bool loadData(const String &name, String &data);

    // Tamper
    bool checkTamper();

    // Erasing
    void removeAllData();
}

#endif