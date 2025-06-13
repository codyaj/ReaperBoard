#include "main.h"

WiFiDisplay wifiDisplay;
MenuDisplay menuDisplay;

extern bool awaitingExit;

void setup() {
  Serial.begin(115200);

  initOLED();
  initButtons();
}

unsigned long lastDisplayUpdate = 0;
const unsigned long displayUpdateInterval = 250;
unsigned long lastButtonUpdate = 0;
const unsigned long debounceDelay = 250;

void showWifiPage() {
  wifiDisplay.onEnter();

  while (!awaitingExit) {
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
}

void loop() {
  awaitingExit = false;

  if (millis() - lastDisplayUpdate >= displayUpdateInterval) {
    menuDisplay.displayScreen();
    lastDisplayUpdate = millis();
  }

  if (millis() - lastButtonUpdate >= debounceDelay) {
    if (menuDisplay.scanInputs()) {
      lastButtonUpdate = millis();
    }
  }

  if (menuDisplay.selectedItem != "") {
    if (menuDisplay.selectedItem == "WiFiScan") {
      showWifiPage();
    }

    menuDisplay.selectedItem = "";
  }
}