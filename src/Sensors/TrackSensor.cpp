#include <PCF8574.h>

/////////////////////PCF8574 drive area//////////////////////////////
// #define PCF8574_ADDRESS 0x27     //Tracking module IIC address
#define PCF8574_ADDRESS 0x23     //Tracking module IIC address
#define PCF8574_SDA     13       //Define the SDA pin number
#define PCF8574_SCL     14       //Define the SCL pin number

unsigned char sensorValue[4] = {0};
PCF8574 TRACK_SENSOR(PCF8574_ADDRESS);

//Trace module initialization
void Track_Setup(void)
{
  TRACK_SENSOR.begin(PCF8574_SDA, PCF8574_SCL);
}

//Tracking module reading
void Track_Read(void)
{
  sensorValue[3] = (TRACK_SENSOR.read8() & 0x07);//composite value
  sensorValue[2] = (sensorValue[3] & 0x01) >> 0; //On the left - 1
  sensorValue[1] = (sensorValue[3] & 0x02) >> 1; //In the middle - 2
  sensorValue[0] = (sensorValue[3] & 0x04) >> 2; //On the right - 4
}
