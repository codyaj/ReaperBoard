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

const uint8_t* getIconBitmap(Icon icon) {
    return (const uint8_t*)pgm_read_ptr(&iconBitmaps[static_cast<int>(icon)]);
}

void OLEDDisplay::renderSidebar(Icon firstIcon, Icon secondIcon, Icon thirdIcon, Icon fourthIcon, int value) {
    const uint8_t* firstBitmap = getIconBitmap(firstIcon);
    const uint8_t* secondBitmap = getIconBitmap(secondIcon);
    const uint8_t* thirdBitmap = getIconBitmap(thirdIcon);
    const uint8_t* fourthBitmap = getIconBitmap(fourthIcon);
    
    display.drawLine(117, 0, 117, 63, WHITE);
    display.drawBitmap(120, 1, firstBitmap, 7, 7, WHITE);
    display.drawBitmap(120, 16, secondBitmap, 7, 7, WHITE);
    display.drawBitmap(120, 30, thirdBitmap, 7, 7, WHITE);
    display.drawBitmap(120, 43, fourthBitmap, 7, 7, WHITE);
    display.setCursor(120, 55);
    display.print(value == -1 ? 0 : value);
}