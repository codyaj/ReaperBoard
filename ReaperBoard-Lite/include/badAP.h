#ifndef BADAP_H
#define BADAP_H

#include <Arduino.h>
#include <ESP8266WiFi.h>
#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>
#include <ESPAsyncWebServer.h>
#include <DNSServer.h>

#include "oledController.h"
#include "sdManager.h"

#define MAX_APS 10

enum class ScreenStage {
    menu,
    servingPage
};

class BadAPDisplay : public OLEDDisplay {
private:
    int index = 0;
    String aps[MAX_APS];
    int apsLoaded = 0;

    ScreenStage screenStage;
protected:
    void onExit() override;
public:
    bool scanInputs() override;
    void displayScreen() override;
    void onEnter() override;
    void tick() override;
};

#endif