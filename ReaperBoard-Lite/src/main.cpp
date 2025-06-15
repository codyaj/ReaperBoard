#include "main.h"

WiFiDisplay wifiDisplay;
MenuDisplay menuDisplay;
LoginDisplay loginDisplay;
MacDisplay macDisplay;
DeauthDisplay deauthDisplay;

extern bool awaitingExit;
extern bool loggedIn;

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

void showMacPage() {
  macDisplay.onEnter();

  while (!awaitingExit) {
    if (millis() - lastDisplayUpdate >= displayUpdateInterval) {
      macDisplay.displayScreen();
      lastDisplayUpdate = millis();
    }

    if (millis() - lastButtonUpdate >= debounceDelay) {
      if (macDisplay.scanInputs()) {
        lastButtonUpdate = millis();
      }
    }
  }
}

unsigned long lastAttack = 0;
const unsigned long attackDelay = 10;

void showDeauthPage() {
  deauthDisplay.onEnter();

  while (!awaitingExit) {
    if (millis() - lastDisplayUpdate >= displayUpdateInterval) {
      deauthDisplay.displayScreen();
      lastDisplayUpdate = millis();
    }

    if (millis() - lastButtonUpdate >= debounceDelay) {
      if (deauthDisplay.scanInputs()) {
        lastButtonUpdate = millis();
      }
    }

    if (millis() - lastAttack >= attackDelay) {
      deauthDisplay.attack();
      lastAttack = millis();
    }

    delay(1); // avoid watchdog reset
  }
}



void loop() {
  if (loggedIn) {
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
      } else if (menuDisplay.selectedItem == " Logout ") {
        loggedIn = false;
      } else if (menuDisplay.selectedItem == "MACSpoof") {
        showMacPage();
      } else if (menuDisplay.selectedItem == "DeAuther") {
        showDeauthPage();
      }

      menuDisplay.selectedItem = "";
    }
  } else {
    if (millis() - lastDisplayUpdate >= displayUpdateInterval) {
      loginDisplay.displayScreen();
      lastDisplayUpdate = millis();
    }

    if (millis() - lastButtonUpdate >= debounceDelay) {
      if (loginDisplay.scanInputs()) {
        lastButtonUpdate = millis();
      }
    }
  }
}