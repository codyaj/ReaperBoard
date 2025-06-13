#ifndef OLEDCONTROLLER_H
#define OLEDCONTROLLER_H

#include <Arduino.h>
#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>

#define SCREEN_WIDTH 128
#define SCREEN_HEIGHT 64

#define FIRST_BUTTON D7
#define SECOND_BUTTON D6
#define THIRD_BUTTON D5


const uint8_t skullBitmap[] PROGMEM = {
    0b01111100,
    0b11111110,
    0b10111010,
    0b11111110,
    0b11101110,
    0b01111100,
    0b00111000,
    0b00101000
};

const uint8_t upArrowBitmap[] PROGMEM = {
    0b00010000,
    0b00111000,
    0b00111000,
    0b01111100,
    0b01111100,
    0b11111110,
    0b11000110
};

const uint8_t downArrowBitmap[] PROGMEM = {
    0b11000110,
    0b11111110,
    0b01111100,
    0b01111100,
    0b00111000,
    0b00111000,
    0b00010000
};

const uint8_t targetBitmap[] PROGMEM = {
    0b00111000,
    0b01000100,
    0b10000010,
    0b10010010,
    0b10000010,
    0b01000100,
    0b00111000
};

enum class Icon {
    LOGO,
    WIFI,
    UP_ARROW,
    DOWN_ARROW,
    RIGHT_ARROW,
    LEFT_ARROW,
    TARGET
};

void initOLED();

void initButtons();

class OLEDDisplay {
protected:
    void handleBackBtn();
    void renderSidebar(Icon firstIcon, Icon secondIcon, Icon thirdIcon, Icon fourthIcon, int value);
    virtual void onExit() {}
public:
    virtual bool scanInputs() = 0;
    virtual void displayScreen() = 0;
    virtual void onEnter() {}
};

#endif