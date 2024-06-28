#ifndef WEBSOCKETHANDLER_H
#define WEBSOCKETHANDLER_H

#include <ESPAsyncWebServer.h>
#include <AsyncWebSocket.h>
#include <ArduinoJson.h>
#include <Adeept_Motor_For_ESP32.h>

// Déclaration des objets serveur et WebSocket
extern AsyncWebServer server;
extern AsyncWebSocket ws;

// Déclarations des fonctions
void handleWebSocketMessage(void *arg, uint8_t *data, size_t len);
void onEvent(AsyncWebSocket *server, AsyncWebSocketClient *client, AwsEventType type, void *arg, uint8_t *data, size_t len);
void initWebSocket();

#endif // WEBSOCKETHANDLER_H
