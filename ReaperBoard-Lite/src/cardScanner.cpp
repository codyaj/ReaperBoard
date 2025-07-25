#include "cardScanner.h"

extern Adafruit_SSD1306 display;
extern bool awaitingExit;

PN532_I2C pn532_i2c(Wire);
PN532 nfc(pn532_i2c);

void initPN532() {
    nfc.begin();

    uint32_t versiondata = nfc.getFirmwareVersion();
    if (!versiondata) {
        SDManager::logEvent("Card Scanner", "Didn't find PN53x board");
        while (1) delay(10);
    }

    nfc.SAMConfig();
}

bool ScannerDisplay::scanInputs() {
    bool buttonPressed = false;

    if (digitalRead(FIRST_BUTTON) == LOW) {
        buttonPressed = true;
    }
    if (digitalRead(SECOND_BUTTON) == LOW) {
        awaitingExit = true;
        onExit();
        buttonPressed = true;
    }
    if (digitalRead(THIRD_BUTTON) == LOW) {
        buttonPressed = true;
    }

    return buttonPressed;
}

void ScannerDisplay::displayScreen() {
    display.clearDisplay();

    display.setTextSize(1);
    display.setTextColor(SSD1306_WHITE);
    display.setCursor(0,0);

    display.println("UID:");
    for (uint8_t i : uid) {
        display.print(i, HEX);
        display.print(" ");
    };
    display.println();

    display.println("atqa:");
    for (uint8_t i : atqa) {
        display.print(i, HEX);
        display.print(" ");
    };
    display.println();

    display.println("SAK:");
    display.print(sak, HEX);
    display.println();

    renderSidebar(Icon::RFID, Icon::UP_ARROW, Icon::RIGHT_ARROW, Icon::DOWN_ARROW, -99);

    display.display();
}

bool ScannerDisplay::scan() {
    return nfc.inListPassiveTarget(atqa, &sak, uid, &uidLength);
}

void ScannerDisplay::onEnter() {
    initPN532();
}

unsigned long lastPosScan = 0;
const unsigned long scanDelay = 500;

void ScannerDisplay::tick() {
    if (millis() - lastPosScan >= scanDelay) {
        if (scan()) {
            lastPosScan = millis();
        }
    }
}

bool ScannerDisplay::timeoutEnabled() {
    return false;
}