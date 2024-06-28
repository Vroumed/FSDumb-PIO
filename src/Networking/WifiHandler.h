#ifndef WIFISTUP_H
#define WIFISTUP_H

#include <WiFi.h>
#include <Car_WS2812.h>

extern const char* ssid_Router;
extern const char* password_Router;
extern char* api_key;
extern bool WiFi_MODE;

extern IPAddress local_IP;
extern bool ledOn;
extern int wtdFlag;

void WiFi_Setup(bool WiFi_Mode);
void loopTask_WTD(void *pvParameters);

#endif // WIFISTUP_H
