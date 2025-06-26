#include "sdManager.h"

namespace SDManager {
    bool SDCardPresent = true;
    bool enableLogs = true;
    bool wipeOnTamper = false;
}

void SDManager::begin() {
    SPI.begin();
    if (!SD.begin(CSPIN)) {
        Serial.println("Card failed, or not present.");
        SDCardPresent = false;
    }
    Serial.println("Card was read!");
}

void SDManager::loadSettings(String &passcode, int &screenTimeout) {
    if (!SDCardPresent) {
        passcode = DEFAULT_PASSCODE;
        screenTimeout = DEFAULT_SCREEN_TIMEOUT;
        return;
    }


    File file = SD.open(SETTINGS_FILE, "r");
    if (!file) {
        Serial.println("Failed to open settings.json for reading");
        enableLogs = true;
        wipeOnTamper = false;
        passcode = DEFAULT_PASSCODE;
        screenTimeout = DEFAULT_SCREEN_TIMEOUT;

        logEvent("SD Reader", "Failed to open settings.json for reading");

        return;
    }

    JsonDocument doc;
    DeserializationError error = deserializeJson(doc, file);
    file.close();

    if (error) {
        Serial.println("JSON parse error: ");
        Serial.println(error.c_str());
        enableLogs = true;
        wipeOnTamper = false;
        passcode = DEFAULT_PASSCODE;
        screenTimeout = DEFAULT_SCREEN_TIMEOUT;

        logEvent("SD Reader", "JSON parse error: " + String(error.c_str()));

        return;
    }

    enableLogs = doc["enableLogs"] | true;
    wipeOnTamper = doc["wipeOnTamper"] | false;
    passcode = String(doc["passcode"] | DEFAULT_PASSCODE);
    screenTimeout = doc["screenTimeout"] | DEFAULT_SCREEN_TIMEOUT;
}

void SDManager::logEvent(const String &type, const String &message) {
    if (!SDCardPresent || !enableLogs) {
        return;
    }

    File log = SD.open(LOG_FILE, FILE_WRITE);
    if (!log) {
        Serial.println("Failed to open log file");
        return;
    }

    if (log.size() > MAX_LOG_SIZE) {
        log.close();
        SD.remove(LOG_FILE);

        log = SD.open(LOG_FILE, FILE_WRITE);
        if (!log) {
            Serial.println("Failed to reopen log file after wipe");
            return;
        }

        log.print("[WIPE] ");
        log.print(millis());
        log.print(": Logs have been wiped for reaching a size of ");
        log.print(MAX_LOG_SIZE);
        log.print(" bytes");
    }

    log.print("[");
    log.print(type);
    log.print("] ");
    log.print(millis());
    log.print(": ");
    log.println(message);
    log.close();
}
