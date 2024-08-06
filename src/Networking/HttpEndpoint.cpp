#include <ESPAsyncWebServer.h>
#include <ArduinoJson.h>
#include "WebsocketHandler.h"
#include <esp_camera.h>
#include <Sensors/TrackSensor.h>
#include <Sensors/PhotosensitiveSensor.H>
#include <Sensors/BatteryIndicator.h>
#include <Sensors/UltrasonicSensor.h>

#define ROVER_NAME "Bertinette"
#define STREAM_CONTENT_BOUNDARY "123456789000000000000987654321"

WiFiServer server_Camera(7000);
void loopTask_Camera(void *pvParameters)
{
    server_Camera.begin();
    while (1)
    {
        char size_buf[12];
        WiFiClient wf_client = server_Camera.available(); // listen for incoming clients
        if (wf_client)
        { // if you get a client
            Serial.println("Camera_Server connected to a client.");
            if (wf_client.connected())
            {
                camera_fb_t *fb = NULL;
                wf_client.write("HTTP/1.1 200 OK\r\nAccess-Control-Allow-Origin: *\r\nContent-Type: multipart/x-mixed-replace; boundary=" STREAM_CONTENT_BOUNDARY "\r\n");
                while (wf_client.connected())
                { // loop while the client's connected
                    
                    fb = esp_camera_fb_get();
                    if (fb != NULL)
                    {
                        wf_client.write("\r\n--" STREAM_CONTENT_BOUNDARY "\r\n");
                        wf_client.write("Content-Type: image/jpeg\r\nContent-Length: ");
                        sprintf(size_buf, "%d\r\n\r\n", fb->len);
                        wf_client.write(size_buf);
                        wf_client.write(fb->buf, fb->len);
                        esp_camera_fb_return(fb);
                    }
                    
                }
                // close the connection:
                wf_client.stop();
                Serial.println("Camera Client Disconnected.");
                // ESP.restart();
            }
        }
    }
}

char * strtoupper( char * dest, const char * src ) {
    char * result = dest;
    while( *dest++ = toupper( *src++ ) );
    return result;
}

void HTTPServer_setup() {
    server.on("/wrover", HTTP_GET, [] (AsyncWebServerRequest *request) {
        // Create a JSON object
        JsonDocument jsonDoc;
        jsonDoc["name"] = ROVER_NAME;
        String macAddress = "ESP32-" + String(ESP.getEfuseMac(), HEX);
        char hardwareID[macAddress.length() + 1];
        macAddress.toCharArray(hardwareID, macAddress.length() + 1);
        jsonDoc["id"] = strtoupper(hardwareID, hardwareID);
        jsonDoc["ready"] = !clientConnected && !gatewayConnected;
        jsonDoc["server_ready"] = !gatewayConnected && clientConnected;
        jsonDoc["client_ready"] = !clientConnected;
        
        // Serialize JSON object to a string
        String response;
        serializeJson(jsonDoc, response);

        // Send the response
        AsyncWebServerResponse *responseReq = request->beginResponse(200, "application/json", response);
        responseReq->addHeader("Access-Control-Allow-Origin", "*");
        request->send(responseReq);
    });

    server.on("/camera", HTTP_GET, [] (AsyncWebServerRequest *request) {
        //take a fixed screenshot
        camera_fb_t *pic = esp_camera_fb_get();
        if (!pic) {
            request->send(500, "text/plain", "Failed to capture image");
            LogErrorToTerminals("Failed to capture image");
            return;
        }
        AsyncResponseStream *response = request->beginResponseStream("image/jpeg");
        response->addHeader("Content-Disposition", "inline; filename=capture.jpg"); 
        response->addHeader("Access-Control-Allow-Origin", "*");
        response->write(pic->buf, pic->len);
        esp_camera_fb_return(pic);
        request->send(response);
    });

    Light_Setup();
    Track_Setup();
    Ultrasonic_Setup();

    server.on("/sensor/all", HTTP_GET, [](AsyncWebServerRequest *request) {
        // Créer un objet JSON
        JsonDocument jsonDoc;

        // Lire les données de tous les capteurs
        jsonDoc["battery_voltage"] = readBatteryVoltage();
        jsonDoc["photosensitive"] = Get_Photosensitive();
        Track_Read();
        jsonDoc["track_left"] = sensorValue[0];
        jsonDoc["track_middle"] = sensorValue[1];
        jsonDoc["track_right"] = sensorValue[2];
        jsonDoc["ultrasonic_distance"] = Get_Sonar();

        // Envoyer la réponse JSON
        String response;
        serializeJson(jsonDoc, response);
            AsyncWebServerResponse *responseReq = request->beginResponse(200, "application/json", response);
        responseReq->addHeader("Access-Control-Allow-Origin", "*");
        request->send(responseReq);
    });

    server.on("/sensor/battery", HTTP_GET, [](AsyncWebServerRequest *request) {
        JsonDocument jsonDoc;
        jsonDoc["battery_voltage"] = readBatteryVoltage();

        String response;
        serializeJson(jsonDoc, response);
        AsyncWebServerResponse *responseReq = request->beginResponse(200, "application/json", response);
        responseReq->addHeader("Access-Control-Allow-Origin", "*");
        request->send(responseReq);
    });

    server.on("/sensor/light", HTTP_GET, [](AsyncWebServerRequest *request) {
        JsonDocument jsonDoc;
        jsonDoc["photosensitive"] = Get_Photosensitive();

        String response;
        serializeJson(jsonDoc, response);
        AsyncWebServerResponse *responseReq = request->beginResponse(200, "application/json", response);
        responseReq->addHeader("Access-Control-Allow-Origin", "*");
        request->send(responseReq);
    });

    server.on("/sensor/track", HTTP_GET, [](AsyncWebServerRequest *request) {
        JsonDocument jsonDoc;
        Track_Read();
        jsonDoc["track_left"] = sensorValue[0];
        jsonDoc["track_middle"] = sensorValue[1];
        jsonDoc["track_right"] = sensorValue[2];

        String response;
        serializeJson(jsonDoc, response);
        AsyncWebServerResponse *responseReq = request->beginResponse(200, "application/json", response);
        responseReq->addHeader("Access-Control-Allow-Origin", "*");
        request->send(responseReq);
    });

    server.on("/sensor/sonar", HTTP_GET, [](AsyncWebServerRequest *request) {
        JsonDocument jsonDoc;
        jsonDoc["distance"] = Get_Sonar();

        String response;
        serializeJson(jsonDoc, response);
        AsyncWebServerResponse *responseReq = request->beginResponse(200, "application/json", response);
        responseReq->addHeader("Access-Control-Allow-Origin", "*");
        request->send(responseReq);
    });

    xTaskCreateUniversal(loopTask_Camera, "loopTask_Camera", 8192, NULL, 0, NULL, 0);
}


