#ifndef SCREENSETUP_H
#define SCREENSETUP_H

#include <SPI.h>
#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>

#define SCREEN_WIDTH 128 // OLED display width, in pixels
#define SCREEN_HEIGHT 32 // OLED display height, in pixels
#define OLED_RESET -1 // Reset pin # (or -1 if sharing Arduino reset pin)
#define SCREEN_ADDRESS 0x3C ///< See datasheet for Address; 0x3D for 128x64, 0x3C for 128x32

extern Adafruit_SSD1306 display;

void testdrawline();
void testdrawrect();
void testfillrect();
void testdrawcircle();
void testfillcircle();
void Screen_Display_Text(const char* text, bool clearAndCommit = true);
void Screen_Display_Selector(const String text[], int arraySize, int selector);
void Commit();
void Clear();
void testdrawroundrect();
void testfillroundrect();
void testdrawtriangle();
void testfilltriangle();
void testdrawchar();
void testdrawstyles();
void testscrolltext();
void testdrawbitmap();
void testanimate(const uint8_t *bitmap, uint8_t w, uint8_t h);
void ScreenSetup();

#endif // SCREENSETUP_H
