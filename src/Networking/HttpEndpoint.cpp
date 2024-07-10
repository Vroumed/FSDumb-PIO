#include <ESPAsyncWebServer.h>
#include <ArduinoJson.h>
#include "WebsocketHandler.h"
#include <esp_camera.h>

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

                        // uint8_t slen[4];
                        // slen[0] = fb->len >> 0;
                        // slen[1] = fb->len >> 8;
                        // slen[2] = fb->len >> 16;
                        // slen[3] = fb->len >> 24;
                        // wf_client.write(slen, 4);
                        // wf_client.write(fb->buf, fb->len);
                        // Serial.println("Camera send");
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

    server.on("/camera", HTTP_GET, [] (AsyncWebServerRequest *request) {
        
        camera_fb_t *pic = esp_camera_fb_get();
        if (!pic) {
            request->send(500, "text/plain", "Failed to capture image");
            return;
        }
        AsyncResponseStream *response = request->beginResponseStream("image/jpeg");
        response->write(pic->buf, pic->len);
        esp_camera_fb_return(pic);
        request->send(response);

        
    });

    xTaskCreateUniversal(loopTask_Camera, "loopTask_Camera", 8192, NULL, 0, NULL, 0);
}


