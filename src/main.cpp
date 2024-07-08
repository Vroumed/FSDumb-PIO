#include <Car_Emotion.h>
#include <Car_WS2812.h>
#include <Adeept_Motor_For_ESP32.h>
#include "Networking/WifiHandler.h"
#include "Display/Screen.h"
#include <thread>
#include "Networking/WebsocketHandler.h"
#include "Buzzer/Buzzer.h"
#include "IR/IRReceiver.h"
#include "Networking/HttpEndpoint.h"

void setup()
{
	Serial.begin(115200);
	PCA9685_Setup();
	ServoHeadX(90);
	ServoHeadY(90);
	ServoDirection(90);

	Buzzer_Setup();
	Buzzer_Variable(true, 400);
	delay(100);
	Buzzer_Variable(true, 1200);
	delay(100);
	Buzzer_Variable(false);


	Emotion_Setup();
	WS2812_Setup();
	ScreenSetup();
	Screen_Display_Text("Starting up...");

	IRSetup();

	WiFi_Setup(0);
	initWebSocket();
	HTTPServer_setup();
	server.begin();
}




void loop()
{
	Emotion_Show(emotion_mode);
	delay(200);
	WS2812_Commit();
}