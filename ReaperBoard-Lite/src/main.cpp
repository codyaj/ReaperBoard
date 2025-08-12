#include "main.h"

WiFiDisplay wifiDisplay;
MenuDisplay menuDisplay;
LoginDisplay loginDisplay;
MacDisplay macDisplay;
DeauthDisplay deauthDisplay;
ScannerDisplay scannerDisplay;
BadAPDisplay badAPDisplay;
SysInfoDisplay sysInfoDisplay;
DataDisplayDisplay dataDisplayDisplay;

extern bool awaitingExit;
extern bool loggedIn;
extern int screenTimeout; // Seconds
extern Adafruit_SSD1306 display;

bool screenOff = false;

bool checkOffBtn() {
  return digitalRead(OFF_SWITCH) == LOW;
}

void logout() {
  loggedIn = false;

  // Enter light sleep
  WiFi.mode(WIFI_OFF);
  WiFi.forceSleepBegin();
  delay(1);
}


void setup() {
  Serial.end();

  pinMode(OFF_SWITCH, INPUT_PULLUP);
  
  WiFi.mode(WIFI_OFF);
  WiFi.forceSleepBegin();
  delay(1);

  SDManager::begin();

  String passcode;
  
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

    if (currentScreen->timeoutEnabled()) {
      if (((millis() - lastButtonUpdate) >= ((long unsigned int)screenTimeout * 1000) || checkOffBtn()) && (long unsigned int)screenTimeout * 1000 > TIMEOUT_MINIMUM) {
        logout();
        awaitingExit = true;
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

    if ((millis() - lastButtonUpdate) >= ((long unsigned int)screenTimeout * 1000)) {
      logout();
      awaitingExit = true;
    }

    if (menuDisplay.selectedItem != "") {
      if (menuDisplay.selectedItem == "WiFiScan") {
        runScreen(&wifiDisplay);
      } else if (menuDisplay.selectedItem == " Logout ") {
        logout();
      } else if (menuDisplay.selectedItem == "MACSpoof") {
        runScreen(&macDisplay);
      } else if (menuDisplay.selectedItem == "DeAuther") {
        runScreen(&deauthDisplay);
      } else if (menuDisplay.selectedItem == "RFID/NFC") {
        runScreen(&scannerDisplay);
      } else if (menuDisplay.selectedItem == " Bad-AP ") {
        runScreen(&badAPDisplay);
      } else if (menuDisplay.selectedItem == "Sys-Info") {
        runScreen(&sysInfoDisplay);
      } else if (menuDisplay.selectedItem == "  Data  ") {
        runScreen(&dataDisplayDisplay);
      }

      menuDisplay.selectedItem = "";
    }
  } else {
    bool offBtnPressed = checkOffBtn();

    if (offBtnPressed && !screenOff) {
      display.ssd1306_command(SSD1306_DISPLAYOFF);
      screenOff = true;
    } else if (!offBtnPressed && screenOff) {
      display.ssd1306_command(SSD1306_DISPLAYON);
      screenOff = false;
    }

    if (!screenOff && (millis() - lastDisplayUpdate >= displayUpdateInterval)) {
      loginDisplay.displayScreen();
      lastDisplayUpdate = millis();
    }

    if (!screenOff && (millis() - lastButtonUpdate >= debounceDelay)) {
      if (loginDisplay.scanInputs()) {
        lastButtonUpdate = millis();
      }
    }
  }

  SDManager::checkTamper();
}