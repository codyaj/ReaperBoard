#include "scanWifi.h"

extern Adafruit_SSD1306 display;
extern bool awaitingExit;

bool WiFiDisplay::scanInputs() {
    bool buttonPressed = false;

    if (digitalRead(FIRST_BUTTON) == LOW) {
        index = (index - 1 + networksFound) % networksFound;
        buttonPressed = true;
    }
    if (digitalRead(SECOND_BUTTON) == LOW) {
        awaitingExit = true;
        onExit();
        buttonPressed = true;
    }
    if (digitalRead(THIRD_BUTTON) == LOW) {
        index = (index + 1) % networksFound;
        buttonPressed = true;
    }

    return buttonPressed;
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

        display.println(String(index + 1) + ") " + ssid);
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

        String encStr;
        switch (encryptionType) {
            case ENC_TYPE_NONE:   encStr = "Open"; break;
            case ENC_TYPE_WEP:    encStr = "WEP"; break;
            case ENC_TYPE_TKIP:   encStr = "WPA/TKIP"; break;
            case ENC_TYPE_CCMP:   encStr = "WPA2/CCMP"; break;
            case ENC_TYPE_AUTO:   encStr = "Auto"; break;
            default:              encStr = "Unknown"; break;
        }
        display.println("Enc: " + encStr);

        display.println("Ch: " + String(channel) + " (" + String(RSSI) + "dBm" + ")");
        display.println(isHidden ? "Hidden" : "Visible");
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