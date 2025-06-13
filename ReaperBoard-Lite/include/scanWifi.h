#ifndef SCANWIFI_H
#define SCANWIFI_H

#include <Arduino.h>
#include <ESP8266WiFi.h>

/** 
 *
 * @brief Starts a Wi-Fi scanner and outputs nearby networks via serial
 * 
 * @note Requires WiFi.mod(WIFI_STA) 
 */
void printScanResult();

#endif