#include "scanWifi.h"

void printScanResult()
{
    int networksFound = WiFi.scanNetworks(false, true);
    Serial.printf("%d network(s) found\n", networksFound);
    for (int i = 0; i < networksFound; i++)
    {
        String ssid;
        uint8_t encryptionType;
        int32_t RSSI;
        uint8_t* BSSID;
        int32_t channel;
        bool isHidden;

        WiFi.getNetworkInfo(i, ssid, encryptionType, RSSI, BSSID, channel, isHidden);

        Serial.println("=======================");
        Serial.printf("SSID: %s\n", ssid.c_str());

        // Print (MAC address)
        char macStr[18];
        sprintf(macStr, "%02X:%02X:%02X:%02X:%02X:%02X",
                BSSID[0], BSSID[1], BSSID[2], BSSID[3], BSSID[4], BSSID[5]);
        Serial.printf("BSSID: %s\n", macStr);

        Serial.printf("Channel: %d\n", channel);
        Serial.printf("RSSI: %d dBm\n", RSSI);

        // Convert encryption type to string
        const char* encStr;
        switch (encryptionType)
        {
        case ENC_TYPE_NONE:   encStr = "Open"; break;
        case ENC_TYPE_WEP:    encStr = "WEP"; break;
        case ENC_TYPE_TKIP:   encStr = "WPA/TKIP"; break;
        case ENC_TYPE_CCMP:   encStr = "WPA2/CCMP"; break;
        case ENC_TYPE_AUTO:   encStr = "Auto"; break;
        default:              encStr = "Unknown"; break;
        }
        Serial.printf("Encryption: %s\n", encStr);

        Serial.printf("Hidden: %s\n", isHidden ? "True" : "False");
    }

    WiFi.scanDelete();
}