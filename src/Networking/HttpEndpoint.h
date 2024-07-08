#ifndef HTTPSERVER_H
#define HTTPSERVER_H

#include <ESPAsyncWebServer.h>
#include <ArduinoJson.h>

// Define constants
#define ROVER_NAME "Bertinette"

// Declare the server
extern AsyncWebServer httpServer;

// Function prototypes
void HTTPServer_setup();

#endif // HTTPSERVER_H
