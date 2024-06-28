#include <esp32-hal-ledc.h>
#include <Arduino.h>

//////////////////////Buzzer drive area///////////////////////////////////
// Buzzer pin definition
#define PIN_BUZZER 2          // Define the pins for the ESP32 control buzzer
#define BUZZER_CHN 0          // Define the PWM channel for ESP32
#define BUZZER_FREQUENCY 2000 // Define the resonant frequency of the buzzer

// Buzzer initialization
void Buzzer_Setup(void)
{
  pinMode(PIN_BUZZER, OUTPUT);
  ledcSetup(BUZZER_CHN, BUZZER_FREQUENCY, 10);
  ledcAttachPin(PIN_BUZZER, BUZZER_CHN);
  ledcWriteTone(BUZZER_CHN, 0);
  delay(10);
}

// Buzzer variable frequency
void Buzzer_Variable(bool enable, int frequency = BUZZER_FREQUENCY)
{
  if (enable == 1)
  {
    frequency = constrain(frequency, 0, 10000);
    ledcWriteTone(BUZZER_CHN, frequency);
  }
  else
    ledcWriteTone(BUZZER_CHN, 0);
}
