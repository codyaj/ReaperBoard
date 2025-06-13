#include "main.h"

WiFiDisplay wifiDisplay;

void setup() {
  Serial.begin(115200);

  initOLED();
  initButtons();

  wifiDisplay.onEnter();
}

unsigned long lastDisplayUpdate = 0;
const unsigned long displayUpdateInterval = 250;
unsigned long lastButtonUpdate = 0;
const unsigned long debounceDelay = 250;

void loop() {
  if (millis() - lastDisplayUpdate >= displayUpdateInterval) {
    wifiDisplay.displayScreen();
    lastDisplayUpdate = millis();
  }

  if (millis() - lastButtonUpdate >= debounceDelay) {
    if (wifiDisplay.scanInputs()) {
      lastButtonUpdate = millis();
    }
  }
}