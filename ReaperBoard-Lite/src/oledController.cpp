#include "oledController.h"

Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, -1);

void initOLED() {
    if(!display.begin(SSD1306_SWITCHCAPVCC, 0x3C)) {
        Serial.println(F("SSD1306 allocation failed"));
        for(;;); // Loop Forever
    }
};

void initButtons() {
    pinMode(FIRST_BUTTON, INPUT_PULLUP);
    pinMode(SECOND_BUTTON, INPUT_PULLUP);
    pinMode(THIRD_BUTTON, INPUT_PULLUP);
}

void OLEDDisplay::handleBackBtn() {
    onExit();
}

void OLEDDisplay::renderSidebar(Icon firstIcon, Icon secondIcon, Icon thirdIcon, Icon fourthIcon, int value) {
    // Get bitmaps first

    display.drawLine(117, 0, 117, 63, WHITE);
    display.drawBitmap(120, 1, skullBitmap, 7, 7, WHITE);
    display.drawBitmap(120, 16, upArrowBitmap, 7, 7, WHITE);
    display.drawBitmap(120, 30, targetBitmap, 7, 7, WHITE);
    display.drawBitmap(120, 43, downArrowBitmap, 7, 7, WHITE);
}