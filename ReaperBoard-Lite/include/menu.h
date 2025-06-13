#ifndef MENU_H
#define MENU_H

#include <Arduino.h>
#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>

#include "oledController.h"

#define MENU_ROWS 4
#define MENU_COLS 2

const char menuLayout[MENU_ROWS][MENU_COLS][9] PROGMEM = {
    {"WiFiScan", " Bad-AP "},
    {"RFID/NFC", "DeAuther"},
    {"MACSpoof", "Profiles"}, 
    {" Logout ", ""}
};

class MenuDisplay : public OLEDDisplay {
private:
    int index[2] = {0, 0};
public:
    bool scanInputs() override;
    void displayScreen() override;
};

#endif