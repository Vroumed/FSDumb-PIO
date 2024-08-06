#ifndef PHOTOSENSITIVE_SENSOR_H
#define PHOTOSENSITIVE_SENSOR_H

#include <Arduino.h>

#define PHOTOSENSITIVE_PIN 33

extern int light_init_value;

// Initializes the photosensitive sensor
void Light_Setup(void);

// Gets the photosensitive resistance value
int Get_Photosensitive(void);

#endif // PHOTOSENSITIVE_SENSOR_H
