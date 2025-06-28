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

        logEvent("SDManager", "Failed to open settings for reading: " + String(SETTINGS_FILE));

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

        logEvent("SDManager", "JSON parse error: " + String(error.c_str()));

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

bool isValidMac(const String &mac) {
    if (mac.length() != 12) {
        return false;
    }

    for (int i = 0; i < 12; i++) {
        if (!isHexadecimalDigit(mac.charAt(i))) {
            return false;
        }
    }

    return true;
}

int SDManager::listMACs(String *names, int maxCount) {
    if (!SDCardPresent) {
        return 0;
    }

    File dir = SD.open(MAC_FOLDER);
    if (!dir) {
        logEvent("SDManager", "Failed to open folder containing mac addresses: " + String(MAC_FOLDER));
        return 0;
    }
    if (!dir.isDirectory()) {
        logEvent("SDManager", "Folder containing mac addresses is NOT a folder: " + String(MAC_FOLDER));
        return 0;
    }

    File file = dir.openNextFile();
    int i = 0;
    while (file && i < maxCount) {
        if (!file.isDirectory()) {
            String mac = "";
            while (file.available()) {
                char c = file.read();
                if (c == '\n' || c == '\r') break;
                mac += c;
            }

            if (isValidMac(mac)) {
                names[i] = file.name();
                i++;
            }
        }

        file.close();
        file = dir.openNextFile();
    }
    dir.close();

    return i;
}

bool SDManager::loadMAC(const String &name, uint8_t *macAddress) {
    if (!SDCardPresent) {
        return false;
    }

    File file = SD.open(MAC_FOLDER + String("/") + name);
    if (!file) {
        logEvent("SDManager", "Failed to open file containing mac address: " + String(MAC_FOLDER + String("/") + name));
        return false;
    }

    String mac = "";
    while (file.available()) {
        char c = file.read();
        if (c == '\n' || c == '\r') break;
        mac += c;
    }
    file.close();

    for (int i = 0; i < 6; i++) {
        macAddress[i] = strtoul(mac.substring(i * 2, i * 2 + 2).c_str(), NULL, 16);
    }

    return true;
}

namespace {
    void deleteRecursive(File dir) {
        if (!dir) return;
        if (!dir.isDirectory()) return;

        File file = dir.openNextFile();
        while (file) {
            String path = String(dir.name()) + "/" + String(file.name());
            if (file.isDirectory()) {
                File subdir = SD.open(path);
                deleteRecursive(subdir);
                subdir.close();
                SD.rmdir(path.c_str());
            } else {
                SD.remove(path.c_str());
            }
            file.close();
            file = dir.openNextFile();
        }
    }
}

bool SDManager::checkTamper() {
    if (!SDCardPresent || !wipeOnTamper) {
        return false;
    }

    int val = analogRead(A0);
    if (val <= TAMPER_THRESHOLD) {
        File root = SD.open("/");
        deleteRecursive(root);
        root.close();
        logEvent("WIPE", "Device has been tampered with and wiped.");
        return true;
    }
    return false;
}