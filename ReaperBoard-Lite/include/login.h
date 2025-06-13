#ifndef LOGIN_H
#define LOGIN_H

#include <Arduino.h>
#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>

#include "oledController.h"

#define MENU_ROWS 4
#define MENU_COLS 3

const char loginLayout[MENU_ROWS][MENU_COLS][2] PROGMEM = {
    {"7", "8", "9"},
    {"4", "5", "6"},
    {"1", "2", "3"},
    {"<", "0", "*"}
};

class LoginDisplay : public OLEDDisplay {
private:
    int index[2] = {0, 0};
    int charIndex = 0;
    char passcode[6];

    // Temp
    char hardCodedPassword[6] = {'3', '3', '3', '3', '3', '3'};
public:
    bool scanInputs() override;
    void displayScreen() override;
};

#endif