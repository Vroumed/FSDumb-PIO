#include <ESPAsyncWebServer.h>
#include <AsyncWebSocket.h>
#include <ArduinoJson.h>
#include <Adeept_Motor_For_ESP32.h>
#include <Buzzer/Buzzer.h>
#include <cmath>
#include <Car_Emotion.h>
#include <Esp.h>
#include "WifiHandler.h"
#include <Display/Screen.h>

AsyncWebServer server(80);
AsyncWebSocket ws("/ws"); // Changez le nom de ce point d'accès pour "sécuriser" l'accès à votre voiture

std::string hardwareID = "";
bool clientConnected = false;
AsyncWebSocketClient *clientCon;
bool gatewayConnected = false;
AsyncWebSocketClient *gatewayCon;

bool autoMode = false;

float mapFloat(float x, float in_min, float in_max, float out_min, float out_max) {
    return (x - in_min) * (out_max - out_min) / (in_max - in_min) + out_min;
}

void LogErrorToTerminals(const char* message, const char* header = "Error") {
    Serial.println(header + ':');
    Serial.println(message);
    Buzzer_Variable(true);
    if (ws.availableForWriteAll()) {
        JsonDocument jsonDoc;
        jsonDoc["type"] = 0;
        jsonDoc["message"] = message;
        jsonDoc["header"] = header;
        String response;
        serializeJson(jsonDoc, response);
        ws.textAll(message);
    }
    delay(100);
    Buzzer_Variable(false);
}

void handleUnauthenticatedSocketMessage(void *arg, uint8_t *data, size_t len, AsyncWebSocketClient *client) {
    AwsFrameInfo *info = (AwsFrameInfo *)arg;

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
            LogErrorToTerminals(error.c_str(), "deserializeJson() failed");
            return;
        }

        int type = doc["type"];
        std::string apiKey = doc["api_key"];
        bool validated = strcmp(apiKey.c_str(), api_key.c_str());

        if (!validated) {
            client->text("no:unauthorized");
            return;
        }

        if (type == 0) { // client
            if (clientConnected) {
                client->text("no:occupied");
                return;
            }
            clientCon = client;
            client->text("ok");
            WS2812_Set_Color(client_indicator, 0, 255, 0);
            WS2812_Commit();
            clientConnected = true;
            Screen_Display_Text("Client connected");
            return;
        }
        else if (type == 1) { // gateway
            if (!clientConnected) {
                client->text("no:client"); // Client must be connected first, then indicate the gateway to connect
                return;
            }

            if (gatewayConnected) {
                client->text("no:occupied");
                return;
            }
            gatewayCon = client;
            client->text("ok");
            WS2812_Set_Color(server_indicator, 0, 255, 0);
            WS2812_Commit();
            gatewayConnected = true;
            Screen_Display_Text("Client & Gateway connected");
        
            return;
        }
    }
}

void handleClientSocketMessage(void *arg, uint8_t *data, size_t len)
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
            LogErrorToTerminals(error.c_str(), "deserializeJson() failed");
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
        case 6:
            {
                JsonArray ledMatrix = doc["data"]["LedMatrix"];

                if (ledMatrix.size() == 5) {
                    WS2812_Set_Color(0, ledMatrix[0][0], ledMatrix[0][1], ledMatrix[0][2]);
                    WS2812_Set_Color(3, ledMatrix[1][0], ledMatrix[1][1], ledMatrix[1][2]);
                    WS2812_Set_Color(4, ledMatrix[2][0], ledMatrix[2][1], ledMatrix[2][2]);
                    WS2812_Set_Color(5, ledMatrix[3][0], ledMatrix[3][1], ledMatrix[3][2]);
                    WS2812_Set_Color(6, ledMatrix[4][0], ledMatrix[4][1], ledMatrix[4][2]);
                    WS2812_Commit();
                }
            }
        default:
            break;
        }
    }
}

void handleGatewaySocketMessage(void *arg, uint8_t *data, size_t len) {
     AwsFrameInfo *info = (AwsFrameInfo *)arg;

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
            LogErrorToTerminals(error.c_str(), "deserializeJson() failed");
            return;
        }

        bool autoCommand = doc["impersonate_client"] == 1;
        if (autoCommand) {
            if (autoMode) {
                handleClientSocketMessage(arg, data, len);
            }
            else {
                LogErrorToTerminals("Gateway is impersonating client, but auto mode is disabled");
            }
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
        if (client == clientCon && clientConnected) {
            clientConnected = false;
            WS2812_Set_Color(client_indicator, 255, 128, 0);
            WS2812_Commit();
            server->closeAll(); // Kick all connected clients, client must be initiator   
            generateQRCode();  //Since the client is disconnected, we can generate a new QR code      
        }
        else if (client == gatewayCon && gatewayConnected) {
            gatewayConnected = false;
            WS2812_Set_Color(server_indicator, 255, 128, 0);
            WS2812_Commit();
        }
        break;
    case WS_EVT_DATA:
        if (client == clientCon && clientConnected)
            {
                handleClientSocketMessage(arg, data, len);
            }
        else if (client == gatewayCon && gatewayConnected)
            {
                handleGatewaySocketMessage(arg, data, len);
            }
        else 
            {
                handleUnauthenticatedSocketMessage(arg, data, len, client);
            }
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

