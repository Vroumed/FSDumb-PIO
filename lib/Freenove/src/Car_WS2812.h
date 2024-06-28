#ifndef WS2812_H
#define WS2812_H

#include <Arduino.h>
#include <Freenove_WS2812_Lib_for_ESP32.h>
#define WS2812_PIN 32

extern int ws2812_task_mode;
extern int ws2812_strip_time_now;
extern int ws2812_strip_time_next;
extern unsigned char ws2812_strip_leds[7][3];
extern int ambiant_leds[];
extern unsigned char client_indicator;
extern unsigned char server_indicator;

extern Freenove_ESP32_WS2812 ws2812_strip;

void WS2812_Fill(unsigned char color_1, unsigned char color_2, unsigned char color_3);
void WS2812_Set_Color(int number, unsigned char color_1, unsigned char color_2, unsigned char color_3);
void WS2812_Commit();
void WS2812_Standard();
void ws2812_close(void);
void WS2812_Setup(void);
void WS2812_SetMode(int mode);

#endif // WS2812_H
