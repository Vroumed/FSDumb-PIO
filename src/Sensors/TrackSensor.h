#ifndef TRACKING_MODULE_H
#define TRACKING_MODULE_H

#include <PCF8574.h>

// Define constants for the tracking module
#define PCF8574_ADDRESS 0x20 // Tracking module IIC address
#define PCF8574_SDA 13       // Define the SDA pin number
#define PCF8574_SCL 14       // Define the SCL pin number

// Speed levels
#define SPEED_LV4 4000
#define SPEED_LV3 3000
#define SPEED_LV2 2500
#define SPEED_LV1 1500

// Sensor value array
extern unsigned char sensorValue[4];

// Tracking sensor object
extern PCF8574 TRACK_SENSOR;

// Initialization of the trace module
void Track_Setup(void);

// Reading the tracking module
void Track_Read(void);

#endif // TRACKING_MODULE_H
