#ifndef MAC_H
#define MAC_H

#include <Arduino.h>
#include <ESP8266WiFi.h>
#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>

#include "oledController.h"

class MacDisplay : public OLEDDisplay {
private:
    int index = 0;
    int itemCount = 1;
protected:
    void onExit() override;
public:
    bool scanInputs() override;
    void displayScreen() override;
    void onEnter() override;
};

#endif