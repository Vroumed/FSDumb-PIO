#ifndef BATTERY_MONITOR_H
#define BATTERY_MONITOR_H

#include <Arduino.h>

extern float batteryCoefficient;
extern float batteryVoltage;

float readBatteryVoltage();

#endif // BATTERY_MONITOR_H
