#include "main.h"

WiFiDisplay wifiDisplay;
MenuDisplay menuDisplay;

void setup() {
  Serial.begin(115200);

  initOLED();
  initButtons();

  //wifiDisplay.onEnter();
}

unsigned long lastDisplayUpdate = 0;
const unsigned long displayUpdateInterval = 250;
unsigned long lastButtonUpdate = 0;
const unsigned long debounceDelay = 250;

void loop() {
  if (millis() - lastDisplayUpdate >= displayUpdateInterval) {
    menuDisplay.displayScreen();
    lastDisplayUpdate = millis();
  }

  if (millis() - lastButtonUpdate >= debounceDelay) {
    if (menuDisplay.scanInputs()) {
      lastButtonUpdate = millis();
    }
  }
}