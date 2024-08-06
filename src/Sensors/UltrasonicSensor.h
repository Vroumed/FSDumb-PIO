#ifndef ULTRASONIC_SENSOR_H
#define ULTRASONIC_SENSOR_H

#include <Arduino.h>

// Pin definitions for the ultrasonic sensor
#define PIN_SONIC_TRIG 12   // Define Trig pin
#define PIN_SONIC_ECHO 15   // Define Echo pin

// Ultrasonic sensor configuration
#define MAX_DISTANCE 300    // Maximum distance in cm
#define SONIC_TIMEOUT (MAX_DISTANCE * 60) // Timeout calculation based on maximum distance
#define SOUND_VELOCITY 340  // Sound velocity in m/s

// Ultrasonic sensor initialization
void Ultrasonic_Setup(void);

// Obtain ultrasonic distance data
float Get_Sonar(void);

#endif // ULTRASONIC_SENSOR_H
