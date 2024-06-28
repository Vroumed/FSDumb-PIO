#ifndef _BUZZER_H_
#define _BUZZER_H_

#include <esp32-hal-ledc.h>
#include <Arduino.h>
#define BUZZER_FREQUENCY 2000 // Define the resonant frequency of the buzzer

// Buzzer initialization
void Buzzer_Setup(void);

// Buzzer variable frequency
void Buzzer_Variable(bool enable, int frequency = BUZZER_FREQUENCY);

#endif