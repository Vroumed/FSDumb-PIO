#include <Arduino.h>
float batteryCoefficient = 3.7;

float batteryVoltage =0;

float readBatteryVoltage() {
    // Save the current pin mode
    pinMode(32, INPUT);

    // Read the battery voltage
    int _batteryVoltage = analogRead(32);

    // Switch the pin mode back to output
    pinMode(32, OUTPUT);
    batteryVoltage = (_batteryVoltage / 4096.0 * 3.9) * batteryCoefficient;
    return batteryVoltage;
}