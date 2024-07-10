#ifndef CAMERA_HANDLER_H
#define CAMERA_HANDLER_H

#include <sensor.h>
#include <esp_camera.h>
#define PWDN_GPIO_NUM    -1
#define RESET_GPIO_NUM   -1
#define XCLK_GPIO_NUM    21
#define SIOD_GPIO_NUM    26
#define SIOC_GPIO_NUM    27
#define Y9_GPIO_NUM      35
#define Y8_GPIO_NUM      34
#define Y7_GPIO_NUM      39
#define Y6_GPIO_NUM      36
#define Y5_GPIO_NUM      19
#define Y4_GPIO_NUM      18
#define Y3_GPIO_NUM       5
#define Y2_GPIO_NUM       4
#define VSYNC_GPIO_NUM   25
#define HREF_GPIO_NUM    23
#define PCLK_GPIO_NUM    22

//Camera initialization
bool cameraSetup(void);

//Set the camera to flip up and down
void camera_vflip(bool enable);
//Set the camera to flip left and right
void camera_hmirror(bool enable);

#endif