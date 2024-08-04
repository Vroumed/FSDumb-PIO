#ifndef WEBSOCKETHANDLER_H
#define WEBSOCKETHANDLER_H

#include <ESPAsyncWebServer.h>
#include <AsyncWebSocket.h>
#include <ArduinoJson.h>
#include <Adeept_Motor_For_ESP32.h>

// Déclaration des objets serveur et WebSocket
extern AsyncWebServer server;
extern AsyncWebSocket ws;
extern std::string hardwareID;
extern IPAddress clientIP;
extern bool clientConnected;
extern IPAddress gatewayIP;
extern bool gatewayConnected;

// Déclarations des fonctions
void handleClientSocketMessage(void *arg, uint8_t *data, size_t len);
void LogErrorToTerminals(const char* message, const char* header = "Error");
void onEvent(AsyncWebSocket *server, AsyncWebSocketClient *client, AwsEventType type, void *arg, uint8_t *data, size_t len);
void initWebSocket();

#endif // WEBSOCKETHANDLER_H
