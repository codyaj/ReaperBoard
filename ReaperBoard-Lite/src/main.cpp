#include "main.h"

WiFiDisplay wifiDisplay;
MenuDisplay menuDisplay;
LoginDisplay loginDisplay;
MacDisplay macDisplay;
DeauthDisplay deauthDisplay;
ScannerDisplay scannerDisplay;
BadAPDisplay badAPDisplay;

extern bool awaitingExit;
extern bool loggedIn;


void setup() {
  Serial.begin(115200);

  WiFi.mode(WIFI_OFF);
  WiFi.forceSleepBegin();
  delay(1);

  SDManager::begin();

  String passcode;
  int screenTimeout;
  
  SDManager::loadSettings(passcode, screenTimeout);

  loginDisplay.setPasscode(passcode);

  initOLED();
  initButtons();
}

unsigned long lastDisplayUpdate = 0;
const unsigned long displayUpdateInterval = 250;
unsigned long lastButtonUpdate = 0;
const unsigned long debounceDelay = 250;

void runScreen(OLEDDisplay* currentScreen) {
  if (!currentScreen) return; // safety check

  awaitingExit = false;
  currentScreen->onEnter();

  while (!awaitingExit) {
    if (millis() - lastDisplayUpdate >= displayUpdateInterval) {
      currentScreen->displayScreen();
      lastDisplayUpdate = millis();
    }

    if (millis() - lastButtonUpdate >= debounceDelay) {
      if (currentScreen->scanInputs()) {
        lastButtonUpdate = millis();
      }
    }

    currentScreen->tick();

    SDManager::checkTamper();

    delay(1);
  }
}

void loop() {
  if (loggedIn) {
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
        runScreen(&wifiDisplay);
      } else if (menuDisplay.selectedItem == " Logout ") {
        loggedIn = false;

        // Enter light sleep
        WiFi.mode(WIFI_OFF);
        WiFi.forceSleepBegin();
        delay(1);
      } else if (menuDisplay.selectedItem == "MACSpoof") {
        runScreen(&macDisplay);
      } else if (menuDisplay.selectedItem == "DeAuther") {
        runScreen(&deauthDisplay);
      } else if (menuDisplay.selectedItem == "RFID/NFC") {
        runScreen(&scannerDisplay);
      } else if (menuDisplay.selectedItem == " Bad-AP ") {
        runScreen(&badAPDisplay);
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

  SDManager::checkTamper();
}