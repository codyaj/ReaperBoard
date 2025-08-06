#include "scanWifi.h"

extern Adafruit_SSD1306 display;
extern bool awaitingExit;

bool WiFiDisplay::scanInputs() {
    if (digitalRead(FIRST_BUTTON) == LOW) {
        index = (index - 1 + networksFound) % networksFound;
        return true;
    }
    if (digitalRead(SECOND_BUTTON) == LOW) {
        awaitingExit = true;
        onExit();
        return true;
    }
    if (digitalRead(THIRD_BUTTON) == LOW) {
        index = (index + 1) % networksFound;
        return true;
    }
    return false;
}

void WiFiDisplay::displayScreen() {
    display.clearDisplay();

    display.setTextSize(1);
    display.setTextColor(SSD1306_WHITE);
    display.setCursor(0,0);

    networksFound = WiFi.scanComplete();

    if (networksFound >= 0) {
        String ssid;
        uint8_t encryptionType;
        int32_t RSSI;
        uint8_t* BSSID = nullptr;
        int32_t channel;
        bool isHidden;

        WiFi.getNetworkInfo(index, ssid, encryptionType, RSSI, BSSID, channel, isHidden);

        display.print(index + 1);
        display.print(") ");
        
        String displaySSID = isHidden ? "<Hidden>" : ssid;
        if (displaySSID.length() <= maxLineLength) {
            display.println(displaySSID);
        } else {
            display.println(displaySSID.substring(0, maxLineLength));
            display.print("   ");
            display.println(displaySSID.substring(maxLineLength));
        }

        display.println("===================");

        
        if (BSSID != nullptr) {
            char macStr1[9];
            char macStr2[9];
            sprintf(macStr1, "%02X:%02X:%02X", BSSID[0], BSSID[1], BSSID[2]);
            sprintf(macStr2, "%02X:%02X:%02X", BSSID[3], BSSID[4], BSSID[5]);
            display.print("BSSID: ");
            display.println(macStr1);
            display.print("       ");
            display.println(macStr2);
        } else {
            display.println("BSSID: N/A");
        }


        const char* encTypes[] = {
            "Open", "WEP", "WPA/TKIP", "WPA2/CCMP", "Auto", "Unknown"
        };
        const char* encStr = (encryptionType >= 0 && encryptionType <= 4) ? encTypes[encryptionType] : encTypes[5];
        display.print("Enc: ");
        display.println(encStr);

        display.print("Ch: ");
        display.print(channel);
        display.print(" (");
        display.print(RSSI);
        display.println("dBm)");
    } else {
        display.println("Loading...");
    }

    renderSidebar(Icon::WIFI, Icon::UP_ARROW, Icon::RIGHT_ARROW, Icon::DOWN_ARROW, networksFound);

    display.display();
}

void WiFiDisplay::onEnter() {
    WiFi.mode(WIFI_STA);
    WiFi.scanNetworks(true, true);
}

void WiFiDisplay::onExit() {
    if (WiFi.scanComplete() >= 0) {
        WiFi.scanDelete();
    }
}

bool WiFiDisplay::timeoutEnabled() {
    return true;
}