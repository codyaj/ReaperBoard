#include "sysInfo.h"

extern Adafruit_SSD1306 display;
extern bool awaitingExit;
int screenTimeout; // Seconds

int screenTimeoutTimes[SCREEN_TIMEOUT_TIMES_TOTAL] = {30, 60, 120, 300, 600, 999999};

bool SysInfoDisplay::scanInputs() {
    bool buttonPressed = false;

    if (digitalRead(FIRST_BUTTON) == LOW) {
        awaitingExit = true;
        onExit();
        buttonPressed = true;
    }
    if (digitalRead(SECOND_BUTTON) == LOW) {
        if (index == 0) {
            screenTimeoutIndex = (screenTimeoutIndex + 1) % (SCREEN_TIMEOUT_TIMES_TOTAL + 1);
            if (screenTimeoutIndex == SCREEN_TIMEOUT_TIMES_TOTAL) {
                setScreenTimeout = originalScreenTimeout;
            } else {
                setScreenTimeout = screenTimeoutTimes[screenTimeoutIndex];
            }
            SDManager::setScreenTimeout(setScreenTimeout);
            screenTimeout = setScreenTimeout;
        } else if (index == 1) {
            if (resetDeviceConfirmation) {
                SDManager::removeAllData();
            }

            resetDeviceConfirmation = !resetDeviceConfirmation;
        }
        buttonPressed = true;
    }
    if (digitalRead(THIRD_BUTTON) == LOW) {
        if (index == 1) {
            resetDeviceConfirmation = false;
        }

        index = (index + 1) % MENU_OPTIONS;
        buttonPressed = true;
    }

    return buttonPressed;
}

void SysInfoDisplay::displayScreen() {
    display.clearDisplay();

    display.setTextSize(1);

    display.fillRect(0, (index * 10), 118, 10, SSD1306_WHITE);

    display.setCursor(1,1);
    display.setTextColor(index == 0 ? SSD1306_BLACK : SSD1306_WHITE);
    display.print(setScreenTimeout);

    display.setCursor(1,11);
    display.setTextColor(index == 1 ? SSD1306_BLACK : SSD1306_WHITE);
    display.print(resetDeviceConfirmation ? "Are you sure?" : "Erase Device");

    display.setTextColor(SSD1306_WHITE);

    display.setCursor(1, 21);
    display.print("Uptime: ");

    unsigned long uptimeSec = millis() / 1000;
    if (uptimeSec < 60) {
        display.print(uptimeSec);
        display.print("s");
    } else if (uptimeSec < 3600) {
        unsigned long minutes = uptimeSec / 60;
        unsigned long seconds = uptimeSec % 60;
        display.print(minutes);
        display.print("m ");
        display.print(seconds);
        display.print("s");
    } else if (uptimeSec < 86400) {
        unsigned long hours = uptimeSec / 3600;
        unsigned long minutes = (uptimeSec % 3600) / 60;
        display.print(hours);
        display.print("h ");
        display.print(minutes);
        display.print("m");
    } else {
        unsigned long days = uptimeSec / 86400;
        unsigned long hours = (uptimeSec % 86400) / 3600;
        display.print(days);
        display.print("d ");
        display.print(hours);
        display.print("h");
    }

    display.setCursor(1, 31);
    float ramKB = ESP.getFreeHeap() / 1024.0;
    display.print("RAM: ");
    display.print(ramKB, 1);
    display.print("/~50 KB");

    renderSidebar(Icon::WIFI, Icon::RIGHT_ARROW, Icon::TARGET, Icon::DOWN_ARROW, -99);

    display.display();
}

void SysInfoDisplay::setOriginalScreenTimeout(int timeout) {
    if (timeout >= 15) {
        originalScreenTimeout = timeout;
    }
}

void SysInfoDisplay::onEnter() {
    String passcode;
    SDManager::loadSettings(passcode, setScreenTimeout);
}

bool SysInfoDisplay::timeoutEnabled() {
    return true;
}