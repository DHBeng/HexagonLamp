/*
    WebVisu.h - Creating a webserver running on Arduino Nano 33 IoT.
    If this webserver is connected to local WiFi, it is poosible to controlle a Lamp via a webbrowser wit hany devicer connect to the same WiFi
    This programm is handling the createn of an html website and the input send by a connected client
*/

#ifndef WebVisu_h
#define WebVisu_h

#include <Arduino.h>
#include <SPI.h>
#include <WiFiNINA.h>

struct rgbColor
{
  uint8_t red;
  uint8_t green;
  uint8_t blue;
};

struct lampSettings
{
    uint8_t mode;
    uint8_t speed;
    uint8_t brightness;
    rgbColor color;
};

class WebVisu
{
    public:
    WebVisu();
    void init();
    bool connectToWiFi(const char ssid[80],const  char password[80], IPAddress ip, uint32_t timeoutMs);
    bool handleClientRequest();
    bool getPowerState();
    lampSettings getLampSettings();

    private:
    void extractSettings(char data[80]);
    WiFiServer server = WiFiServer(80);
    static char lastClientRequest[80];
    bool powerState;
    uint8_t mode;
    uint8_t brightness;
    uint8_t speed;
    rgbColor color;
};
#endif
