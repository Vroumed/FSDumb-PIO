#include <ESPAsyncWebServer.h>
#include <ArduinoJson.h>
#include "WebsocketHandler.h"

#define ROVER_NAME "Bertinette"


void HTTPServer_setup() {
    server.on("/wrover", HTTP_GET, [] (AsyncWebServerRequest *request) {
        // Create a JSON object
        JsonDocument jsonDoc;
        jsonDoc["name"] = ROVER_NAME;
        jsonDoc["ready"] = true;
        
        // Serialize JSON object to a string
        String response;
        serializeJson(jsonDoc, response);

        // Send the response
        request->send(200, "application/json", response);
    });
}
