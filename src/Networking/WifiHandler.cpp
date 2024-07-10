#include <Wifi.h>
#include <Car_WS2812.h>
#include <Display/Screen.h>
#include <random>
#include <list>
#include <IR/IRReceiver.h>
#include <qrcode.h>

///////////////////WiFi drive area////////////////////////////////////////
const char* ssid_Router;    //Modify according to your router name
const char* password_Router;    //Modify according to your router password
std::string api_key;

struct WiFiCredentials {
    const char* ssid;
    const char* password;
};

// Define a constant array of WiFiCredentials
const WiFiCredentials knownNetworks[] = {
    {"SFR_D8E0", "panige10"},
    {"iPhone de Fumy", "Truc.EXE"},
    {"PANINIPC", "Truc.EXE"},
    {"BENS-OMEN", "hehehehe"},
    {"iPhone de Fumy", "Truc.EXE"},

};

const int numKnownNetworks = sizeof(knownNetworks) / sizeof(WiFiCredentials);


IPAddress local_IP(192, 168, 4, 1);
bool ledOn = false;

std::string random_string(std::size_t length)
{
    const std::string CHARACTERS = "0123456789ABCDEFGHIJKLMNOPQRSTUVWXYZ";

    std::random_device random_device;
    std::mt19937 generator(random_device());
    std::uniform_int_distribution<> distribution(0, CHARACTERS.size() - 1);

    std::string random_string;

    for (std::size_t i = 0; i < length; ++i)
    {
        random_string += CHARACTERS[distribution(generator)];
    }

    return random_string;
}

//Initialize WiFi function
void WiFi_Setup(bool WiFi_Mode)
{
    bool selectedwifi = false;

    while (!selectedwifi) {
      WS2812_Set_Color(server_indicator, 255, 0, 0);
      WS2812_Commit();
      

      Screen_Display_Text("Searching for network");
      std::list<String> foundKnownSSIDs = std::list<String>();
      int numNetworks = WiFi.scanNetworks();

      for (int i = 0; i < numNetworks; i++) {
          String ssid = WiFi.SSID(i);
          for (int j = 0; j < numKnownNetworks; j++) {
              if (ssid == knownNetworks[j].ssid) {
                  foundKnownSSIDs.push_back(ssid);
                  break;
              }
          }
      }
      uint size = foundKnownSSIDs.size();
      if (size == 0) {
          Screen_Display_Text("no known network found\npress OK to retry");
          while (!AwaitForIRInput() == 16) {
              delay(100);
          }
          ESP.restart();
      }
      if (size == 1) {
        for (int j = 0; j < numKnownNetworks; j++) {
            if (foundKnownSSIDs.front() == knownNetworks[j].ssid) {
              ssid_Router = knownNetworks[j].ssid;
              password_Router = knownNetworks[j].password;
              selectedwifi = true;
              break;
            }
          }
      }
      else {
        int sel = 0;
        String ssidArray[size];
        std::copy(foundKnownSSIDs.begin(), foundKnownSSIDs.end(), ssidArray);
        Screen_Display_Selector(ssidArray,size, sel);
        while (true) {
          int input = AwaitForIRInput();
          if (input == 13) {
            sel++;
            sel = sel % size;
            Screen_Display_Selector(ssidArray,size, sel);
          }
          else if (input == 14) {
            sel--;
            if (sel < 0) {
              sel = size - 1;
            }
            Screen_Display_Selector(ssidArray,size, sel);
          }
          else if (input == 16) {
            //loop through knownNetworks to find the password
            for (int j = 0; j < numKnownNetworks; j++) {
              if (ssidArray[sel] == knownNetworks[j].ssid) {
                ssid_Router = knownNetworks[j].ssid;
                password_Router = knownNetworks[j].password;
                selectedwifi = true;
                break;
              }
            }
            break;
          }
        }
      }
    }


    WiFi.mode(WIFI_STA);
    WiFi.begin(ssid_Router, password_Router);
    WiFi.setSleep(false);
    WiFi.setAutoConnect(true);
    WiFi.setAutoReconnect(true);
    while (WiFi.isConnected() != true) {
      if (ledOn) {
        WS2812_Set_Color(server_indicator, 0, 0, 0);
        WS2812_Commit();
        ledOn = false;
        Screen_Display_Text("Connecting... [. ]");
      } else {
        WS2812_Set_Color(server_indicator, 255, 128, 0);
        WS2812_Commit();
        ledOn = true;
        Screen_Display_Text("Connecting... [ .]");
      }
      delay(500);
    }
    IPAddress local_ip = WiFi.localIP();

    api_key = (char*)random_string(8).c_str();

    std::string echo = std::string(local_ip.toString().c_str()) + '\n';

    echo += api_key;

    std::string qrData = std::string(local_ip.toString().c_str()) + ';' + api_key;
    Clear();
    Screen_Display_Text(echo.c_str());

    QRCode qrcode;
    uint8_t qrcodeData[qrcode_getBufferSize(3)];
    qrcode_initText(&qrcode, qrcodeData, 3, 0, qrData.c_str());

    // Dimensions de l'écran
    const uint8_t screenWidth = 128;
    const uint8_t screenHeight = 32;

    // Calcul des coordonnées de base pour centrer le QR code
    uint8_t basX = (screenWidth - qrcode.size * 2) / 2;
    uint8_t basY = (screenHeight - qrcode.size) / 2;

    
    Clear();

    for (uint8_t x = 0; x < (qrcode.size*2) + 4; x++) {
      display.writePixel(basX-2 + x, basY-1, SSD1306_WHITE);
      display.writePixel(basX-2 + x, basY + qrcode.size, SSD1306_WHITE);
    }
    for (uint8_t y = 0; y < qrcode.size; y++) {
      // Each horizontal module
      
      display.writePixel(basX - 2, basY + y, SSD1306_WHITE);
      display.writePixel(basX - 1, basY + y, SSD1306_WHITE);
      for (uint8_t x = 0; x < qrcode.size; x++) {
          Serial.print(qrcode_getModule(&qrcode, x, y) ? "\u2588\u2588": "  ");
          display.writePixel(basX + x*2, basY + y, qrcode_getModule(&qrcode, x, y) ?  SSD1306_BLACK : SSD1306_WHITE);
          display.writePixel(basX + x*2 + 1, basY + y, qrcode_getModule(&qrcode, x, y) ?  SSD1306_BLACK : SSD1306_WHITE);
      }
      display.writePixel(basX + qrcode.size*2 + 2, basY + y, SSD1306_WHITE);
      display.writePixel(basX + qrcode.size*2 + 1, basY + y, SSD1306_WHITE);
    }

    Commit();

    WS2812_Set_Color(server_indicator, 255, 255, 0);
    WS2812_Commit();
}


