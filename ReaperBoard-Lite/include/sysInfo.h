#ifndef SYSINFO_H
#define SYSINFO_H

#include <Arduino.h>
#include <ESP8266WiFi.h>
#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>

#include "oledController.h"
#include "sdManager.h"

#define MENU_OPTIONS 2
#define SCREEN_TIMEOUT_TIMES_TOTAL 6

class SysInfoDisplay : public OLEDDisplay {
private:
    int index = 0;
    int screenTimeoutIndex = SCREEN_TIMEOUT_TIMES_TOTAL;
    int setScreenTimeout = 0;
    int originalScreenTimeout; // Preserves a custom timeout set in SD card
    bool resetDeviceConfirmation = false;
public:
    void setOriginalScreenTimeout(int timeout);
    bool scanInputs() override;
    void displayScreen() override;
    void onEnter() override;
    bool timeoutEnabled() override;
};

#endif