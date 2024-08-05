#include <Arduino.h>
#include <Adeept_WS2812_for_ESP32.h>
#define LEDS_COUNT   7     //Define the count of WS2812,When adding WS2812 LED, you can modify this value.
#define LEDS_PIN  	32    //Define the pin number for ESP32
#define CHANNEL	    0     //Define the channels that control WS2812

int ws2812_task_mode = 0;
int ws2812_strip_time_now;
int ws2812_strip_time_next;
unsigned char ws2812_strip_leds[LEDS_COUNT][3];

//ambiant colors for the WS2812 are 0 to 6 except 1 and 2
int ambiant_leds[] = {0, 3, 4, 5, 6};
unsigned char client_indicator = 1; //client indicator
unsigned char server_indicator = 2; //server


Adeept_ESP32_WS2812 ws2812_strip = Adeept_ESP32_WS2812(LEDS_COUNT, LEDS_PIN, CHANNEL, TYPE_GRB);

//Set the display color1 for WS2812
void WS2812_Fill(unsigned char color_1, unsigned char color_2, unsigned char color_3)  //Set the display color1 for WS2812
{
  for (char i : ambiant_leds)
  {
      ws2812_strip_leds[i][0] = constrain(color_1, 0, 255);
      ws2812_strip_leds[i][1] = constrain(color_2, 0, 255);
      ws2812_strip_leds[i][2] = constrain(color_3, 0, 255);
  }
}

void WS2812_Set_Color(int number, unsigned char color_1, unsigned char color_2, unsigned char color_3)
{
  ws2812_strip_leds[number][0] = constrain(color_2, 0, 255);
  ws2812_strip_leds[number][1] = constrain(color_1, 0, 255);
  ws2812_strip_leds[number][2] = constrain(color_3, 0, 255);
}


void WS2812_Commit() {
  for (int i = 0; i < LEDS_COUNT; i++)
    ws2812_strip.setLedColorData(i, ws2812_strip_leds[i][0], ws2812_strip_leds[i][1], ws2812_strip_leds[i][2]);

  ws2812_strip.show();
}

//Set the display color1 for WS2812
void WS2812_Standard()  //Set the display color1 for WS2812
{
  //set 0 and 3 as Aqua
  WS2812_Set_Color(0, 0, 255, 255);
  WS2812_Set_Color(3, 0, 255, 255);

  //set 4, 6 as red
  WS2812_Set_Color(4, 255, 0, 0);
  WS2812_Set_Color(5, 255, 0, 0);
  WS2812_Set_Color(6, 255, 0, 0);
}

//Close the display WS2812
void ws2812_close(void)
{
  for (int i = 0; i < LEDS_COUNT; i++)
    ws2812_strip.setLedColor(i, 0, 0, 0);
  ws2812_strip.show();
}

//WS2812 initialization function
void WS2812_Setup(void)
{
  ws2812_strip.begin();
  ws2812_strip.setBrightness(50);
  ws2812_close();
  WS2812_Standard();
  ws2812_strip_time_now = millis();

}

//WS2812 set mode
void WS2812_SetMode(int mode)
{
  ws2812_task_mode = constrain(mode, 0, 5);
}
