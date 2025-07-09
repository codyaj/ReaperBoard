#ifndef SCANWIFI_H
#define SCANWIFI_H

#include <Arduino.h>
#include <ESP8266WiFi.h>
#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>

#include "oledController.h"


class WiFiDisplay : public OLEDDisplay {
private:
    int networksFound = 0;
    int index = 0;
protected:
    void onExit() override;
public:
    bool scanInputs() override;
    void displayScreen() override;
    void onEnter() override;
    bool timeoutEnabled() override;
};

#endif