#include "cardScanner.h"

extern Adafruit_SSD1306 display;
extern bool awaitingExit;

PN532_I2C pn532_i2c(Wire);
PN532 nfc(pn532_i2c);

void initPN532() {
    // Wire.begin(SDA_PIN, SCL_PIN); // Already called for the oled
    nfc.begin();

    uint32_t versiondata = nfc.getFirmwareVersion();
    if (!versiondata) {
        Serial.println("Didn't find PN53x board");
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
        display.print(i);
    };

    renderSidebar(Icon::WIFI, Icon::UP_ARROW, Icon::RIGHT_ARROW, Icon::DOWN_ARROW, -99);

    display.display();
}

bool ScannerDisplay::scan() {
    return nfc.inListPassiveTarget(atqa, &sak, uid, &uidLength);
}

void ScannerDisplay::onEnter() {
    initPN532();
}

void ScannerDisplay::onExit() {
    
}