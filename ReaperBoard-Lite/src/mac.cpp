#include "mac.h"

extern Adafruit_SSD1306 display;
extern bool awaitingExit;

bool MacDisplay::scanInputs() {
    bool buttonPressed = false;

    if (digitalRead(FIRST_BUTTON) == LOW) {
        awaitingExit = true;
        onExit();
        buttonPressed = true;
    }
    if (digitalRead(SECOND_BUTTON) == LOW) {
        if (index == 0) {
            uint8_t mac[6];
            
            uint32_t r1 = os_random();
            uint32_t r2 = os_random();

            mac[0] = ((r1 >> 0) & 0xFC) | 0x02;  // LAA + unicast
            mac[1] = (r1 >> 8) & 0xFF;
            mac[2] = (r1 >> 16) & 0xFF;
            mac[3] = (r1 >> 24) & 0xFF;
            mac[4] = r2 & 0xFF;
            mac[5] = (r2 >> 8) & 0xFF;

            wifi_set_macaddr(STATION_IF, &mac[0]);
        }
        buttonPressed = true;
    }
    if (digitalRead(THIRD_BUTTON) == LOW) {
        index = (index + 1) % itemCount;
        buttonPressed = true;
    }

    return buttonPressed;
}

void MacDisplay::displayScreen() {
    display.clearDisplay();

    display.setTextSize(1);
    display.setTextColor(SSD1306_WHITE);
    display.setCursor(8,0);

    display.print(WiFi.macAddress());

    if (index == 0) {
        int x = 17;
        int y = 10;

        display.setTextColor(SSD1306_BLACK);
        display.fillRect(x - 1, y - 1, 86, 10, SSD1306_WHITE);
        display.setCursor(x, y);
        display.print("Get Random MAC");
    }

    renderSidebar(Icon::MASK, Icon::RIGHT_ARROW, Icon::TARGET, Icon::DOWN_ARROW, -99);

    display.display();
}

void MacDisplay::onEnter() {
    WiFi.mode(WIFI_STA);

    // Load saved MAC addresses
}

void MacDisplay::onExit() {
    // Unload saved MAC addresses
}