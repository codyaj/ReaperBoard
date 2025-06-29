#ifndef CARDSCANNER_H
#define CARDSCANNER_H

#include <Arduino.h>
#include <Wire.h>
#include <PN532_I2C.h>
#include <PN532.h>

#include "oledController.h"
#include "sdManager.h"

#define SDA_PIN D2
#define SCL_PIN D1

void initPN532();

class ScannerDisplay : public OLEDDisplay {
private:
    uint8_t atqa[2];
    uint8_t sak;
    uint8_t uid[7];
    uint8_t uidLength;
    int index = 0;

    bool scan();
public:
    bool scanInputs() override;
    void displayScreen() override;
    void onEnter() override;
    void tick() override;
};

#endif