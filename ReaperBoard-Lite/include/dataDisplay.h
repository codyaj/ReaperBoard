#ifndef DATADISPLAY_H
#define DATADISPLAY_H

#include <Arduino.h>
#include <ESP8266WiFi.h>
#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>

#include "oledController.h"
#include "sdManager.h"

#define MAX_DATA 10
#define MAX_ON_SCREEN 7

enum class CurrentPage {
    DATA_LIST_PAGE,
    DATA_SHOW_PAGE
};

class DataDisplayDisplay : public OLEDDisplay {
private:
    int index = 0;
    String dataList[MAX_DATA];
    int dataLoaded = 0;
    CurrentPage currentPage = CurrentPage::DATA_LIST_PAGE;
    String pageData = "";
public:
    bool scanInputs() override;
    void displayScreen() override;
    void onEnter() override;
};

#endif