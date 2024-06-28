#include <ESPAsyncWebServer.h>
#include <AsyncWebSocket.h>
#include <ArduinoJson.h>
#include <Adeept_Motor_For_ESP32.h>
#include <Buzzer/Buzzer.h>
#include <cmath>
#include <Car_Emotion.h>

AsyncWebServer server(80);
AsyncWebSocket ws("/ws"); // Changez le nom de ce point d'accès pour "sécuriser" l'accès à votre voiture

float mapFloat(float x, float in_min, float in_max, float out_min, float out_max) {
    return (x - in_min) * (out_max - out_min) / (in_max - in_min) + out_min;
}


void handleWebSocketMessage(void *arg, uint8_t *data, size_t len)
{
    AwsFrameInfo *info = (AwsFrameInfo *)arg;

    // Serial.println((char *)data);

    if (info->final && info->index == 0 && info->len == len && info->opcode == WS_TEXT)
    {
        data[len] = 0;

        JsonDocument doc;

        // JSON input string.
        const char* json = (char *)data;

        // Deserialize the JSON document
        DeserializationError error = deserializeJson(doc, json);

        if (error)
        {
            Serial.print("deserializeJson() failed: ");
            Serial.println(error.c_str());
            return;
        }

        int cmd = doc["cmd"];

        switch (cmd)
        {
        case 1:
            {
                float direction = doc["data"]["direction"];
                float speed = doc["data"]["speed"];
                int thrust = doc["data"]["thrust"];

                // Convert direction from -1 to 1 to 45 to 135
                int adaptedDirection = mapFloat(direction, -1, 1, 45, 135);

                // Calcul des vitesses des roues
                float absDirection = std::abs(direction);

                // Convertir les vitesses en pourcentage (0 à 100)
                int adaptedSpeedInner = mapFloat(speed - (absDirection/2), 0, 1, 0, 100);
                int adaptedSpeedOuter = mapFloat(speed, 0, 1, 0, 100);

                ServoDirection(adaptedDirection);
                if (direction < 0 ) {
                    Motor(1, thrust, adaptedSpeedOuter);
                    Motor(2, thrust, adaptedSpeedInner);
                }
                else {
                    Motor(1, thrust, adaptedSpeedInner);
                    Motor(2, thrust, adaptedSpeedOuter);
                }
                
            }
            break;
        
        case 2:
            {
                float headX = doc["data"]["headX"];
                float headY = doc["data"]["headY"];

                headX = mapFloat(headX, -1, 1, 0, 180);
                headY = mapFloat(headY, -1, 1, 65, 130);

                ServoHeadX(headX);
                ServoHeadY(headY);
            }
            break;

        case 3:
            {
                bool horn = doc["data"]["horn"] == 1;
                int frequency = doc["data"]["frequency"];
                Buzzer_Variable(horn, frequency);
            }
        case 4:
            {
                int mode = doc["data"]["mode"];
                Emotion_Show(mode % 7);
            }
            break;
        case 5:
            {
                JsonArray matrixLeft = doc["data"]["matrixLeft"];
                JsonArray matrixRight = doc["data"]["matrixRight"];

                // Créez des tableaux pour stocker les valeurs
                byte matrixLeftArray[8];
                byte matrixRightArray[8];

                // Remplissez les tableaux avec les valeurs des matrices
                for (int i = 0; i < 8; i++) {
                    matrixLeftArray[i] = matrixLeft[i];
                    matrixRightArray[i] = matrixRight[i];
                }
                Emotion_Show(-1); 
                
                matrix.showStaticArray(matrixLeftArray, matrixRightArray);
            }
            break;

        default:
            break;
        }
    }
}


void onEvent(AsyncWebSocket *server, AsyncWebSocketClient *client, AwsEventType type, void *arg, uint8_t *data, size_t len)
{
    switch (type)
    {
    case WS_EVT_CONNECT:
        Serial.printf("WebSocket client #%u connected from %s\n", client->id(), client->remoteIP().toString().c_str());
        break;
    case WS_EVT_DISCONNECT:
        Serial.printf("WebSocket client #%u disconnected\n", client->id());
        break;
    case WS_EVT_DATA:
        handleWebSocketMessage(arg, data, len);
        break;
    case WS_EVT_PONG:
    case WS_EVT_ERROR:
        break;
    }
}

void initWebSocket()
{
    ws.onEvent(onEvent);
    server.addHandler(&ws);
}

