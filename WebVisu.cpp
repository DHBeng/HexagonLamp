#include <Arduino.h>

#include "WebVisu.h"

WebVisu::WebVisu(){};

void WebVisu::init()
{
    WebVisu::powerState = false;
    WebVisu::mode = 1;
    WebVisu::brightness = 255;
    WebVisu::speed = 30;
    WebVisu::color.red = 255;
    WebVisu::color.green = 255;
    WebVisu::color.blue = 255;
    WebVisu::colorHex = 0xFFFFFF;
}

bool WebVisu::connectToWiFi(const char ssid[80], const char password[80], IPAddress ip, uint32_t timeoutMs)
{
    /*
    This function is conneting the Arduino to the WiFi provoded by the inputs.
    The function returns true if the connection was successful.
    If an ip address is set the arduino is connected with an static ip.
    If the ip is 0.0.0.0 the router will assign an ip address.
    */

    int16_t status = WL_IDLE_STATUS;
    int16_t startTime;
    int16_t currentTime;

    startTime = millis();
    WiFi.setHostname("Lamp");

    if (timeoutMs < 5000)
    {
        timeoutMs = 5000;
    }

    if (ip != IPAddress(0, 0, 0, 0))
    {
        WiFi.config(ip);
    }

    if (WiFi.status() == WL_NO_MODULE)
    {
        return false;
    }

    while (status != WL_CONNECTED)
    {
        currentTime = millis();
        if (currentTime - startTime > timeoutMs)
        {
            return false;
        }
        status = WiFi.begin(ssid, password);
        delay(10000);
    }

    WebVisu::server.begin();
    return true;
}

bool WebVisu::handleClientRequest()
{
    /*
    This function is listining for incoming clients hand handling their requests.
    The function is returning false if no client was connected durning the call and true if client was connected.
    */

    // listen for incoming clients
    WiFiClient client = server.available();
    if (client)
    {
        // an http request ends with a blank line
        bool currentLineIsBlank = true;
        bool firstLineRecived = false;
        char clientRequest[80] = "";
        uint8_t requestLength = 0;
        char data[80] = "";
        uint8_t i = 0;
        uint8_t j = 0;

        while (client.connected())
        {
            if (client.available())
            {
                char c = client.read();
                // if you've gotten to the end of the line (received a newline
                // character) and the line is blank, the http request has ended,
                // so you can send a reply
                if (c == '\n' && currentLineIsBlank)
                {
                    // send a standard http response header
                    client.println("HTTP/1.1 200 OK");
                    client.println("Content-Type: text/html");
                    client.println("Connection: close"); // the connection will be closed after completion of the response
                    client.println();

                    client.println("<!DOCTYPE html><html lang=\"en\"><meta charset=\"utf-8\"><meta name=\"viewport\" content=\"width=device-width\">");
                    client.println("<head><title>HMI Lamp Office</title><style>body {color: honeydew;background-color: rgb(20, 20, 20);font-family: Verdana;font-size: 25px;}");
                    client.println("button {max-width: 100%;min-width: 20%;width: 100%;font-family: monospace;font-size: 25px;font-weight: bolder;}");
                    client.println("input[type='range'] {max-width: 100%;min-width: 20%;width: 100%;font-family: monospace;font-size: 25px;cursor: ew-resize;}");
                    client.println("footer {font-size: 10px;}</style></head><body><h1 style=\"text-align: center;\">");
                    client.println("⬡ Lamp</h1><p><form><button type=\"submit\" name=\"P\">Power</button></form></p>");
                    client.println("<form><p><label for=\"mode\">Select Mode</label>");
                    client.println("<select id=\"mode\" style=\"width: 100%; font-size: 25px; font-weight: bolder;\" name=\"M\">");
                    switch (WebVisu::mode)
                    {
                    case 1:
                        client.println("<option selected value=\"1\">Rainbow Fade</option>");
                        client.println("<option value=\"2\">Monochrome Fade</option>");
                        client.println("<option value=\"3\">Single Color</option>");
                        client.println("<option value=\"4\">Warm White</option>");
                        client.println("<option value=\"5\">Random Color Fade</option></select>");
                        break;
                    case 2:
                        client.println("<option value=\"1\">Rainbow Fade</option>");
                        client.println("<option selected value=\"2\">Monochrome Fade</option>");
                        client.println("<option value=\"3\">Single Color</option>");
                        client.println("<option value=\"4\">Warm White</option>");
                        client.println("<option value=\"5\">Random Color Fade</option></select>");
                        break;
                    case 3:
                        client.println("<option value=\"1\">Rainbow Fade</option>");
                        client.println("<option value=\"2\">Monochrome Fade</option>");
                        client.println("<option selected value=\"3\">Single Color</option>");
                        client.println("<option value=\"4\">Warm White</option>");
                        client.println("<option value=\"5\">Random Color Fade</option></select>");
                        break;
                    case 4:
                        client.println("<option value=\"1\">Rainbow Fade</option>");
                        client.println("<option value=\"2\">Monochrome Fade</option>");
                        client.println("<option value=\"3\">Single Color</option>");
                        client.println("<option selected value=\"4\">Warm White</option>");
                        client.println("<option value=\"5\">Random Color Fade</option></select>");
                        break;
                    case 5:
                        client.println("<option value=\"1\">Rainbow Fade</option>");
                        client.println("<option value=\"2\">Monochrome Fade</option>");
                        client.println("<option value=\"3\">Single Color</option>");
                        client.println("<option value=\"4\">Warm White</option>");
                        client.println("<option selected value=\"5\">Random Color Fade</option></select>");
                        break;
                    }
                    client.print("</p><h2>Settings</h2><label for=\"brightness\">Brightness</label><input type=\"range\" id=\"brightness\" name=\"B\" min=\"0\" max=\"255\" value=\"");
                    client.print(WebVisu::brightness);
                    client.println("\"/>");
                    client.print("<label for=\"speed\">Speed</label><input type=\"range\" id=\"speed\" name=\"S\" min=\"10\" max=\"50\" value=\"");
                    client.print(WebVisu::speed);
                    client.println("\"/>");
                    client.print("<label for=\"color\">Color</label><input type=\"color\" id=\"color\" style=\"width: 100%;\" name=\"C\" value=\"#");
                    client.print(WebVisu::colorHex, HEX);
                    client.println("\"/>");
                    client.println("<button type=\"submit\">Apply</button></form></body><hr><footer>Created by D_Herbert. Visit <a href=\"https://www.thingiverse.com/thing:4759920\">");
                    client.println("Thingiverse</a> for more information.</footer></html>");
                    break;
                }
                if (c == '\n')
                {
                    // Check if it was the first line
                    if (not firstLineRecived)
                    {
                        // Remove not needed charrs from the string:
                        // Start of string: "GET /?"
                        // End of string: " HTTP/1.1"
                        requestLength = strlen(clientRequest);
                        for (i = 6; i < requestLength - 9; i++)
                        {
                            data[j] = clientRequest[i];
                            j++;
                        }

                        // extract settings from request
                        if (data[0] == 'P' and data[1] == '=')
                        {
                            WebVisu::powerState = not WebVisu::powerState;
                        }
                        else if (data[0] == 'M' and data[1] == '=')
                        {
                            WebVisu::extractSettings(data);
                        }
                    }
                    // you're starting a new line
                    currentLineIsBlank = true;
                    firstLineRecived = true;
                }
                else if (c != '\r')
                {
                    // you've gotten a character on the current line
                    currentLineIsBlank = false;
                }
                if (not firstLineRecived)
                {
                    // The first line of the request contains the data send by the client this is saved for later
                    clientRequest[i] = c;
                    i++;
                }
            }
        }
        // give the web browser time to receive the data
        delay(1);

        // close the connection:
        client.stop();
        return true;
    }
    else
    {
        return false;
    }
}

bool WebVisu::getPowerState()
{
    return WebVisu::powerState;
}

lampSettings WebVisu::getLampSettings()
{
    lampSettings settings;

    settings.mode = WebVisu::mode;
    settings.brightness = WebVisu::brightness;
    settings.speed = WebVisu::speed;
    settings.color = WebVisu::color;

    return settings;
}

void WebVisu::extractSettings(char data[80])
{
    uint8_t i;
    uint8_t j;
    uint8_t k;
    uint8_t separatorPos[3];
    #define VAL_LENGHT 6
    char value[VAL_LENGHT];

    // Get the index of the separation charactar ('&')
    j = 0;
    for (i = 0; i < 80; i++)
    {
        if (data[i] == '&')
        {
            separatorPos[j] = i;
            j++;
            if (j > 3)
            {
                break;
            }
        }
    }

    // Get value for mode
    j = 0;
    memset(value, 0, VAL_LENGHT);
    for (i = 2; i <= separatorPos[0] - 1; i++)
    {
        value[j] = data[i];
        j++;
    }
    WebVisu::mode = atoi(value);

    // Get value for brightness
    j = 0;    
    memset(value, 0, VAL_LENGHT);
    for (i = separatorPos[0] + 3; i <= separatorPos[1] - 1; i++)
    {
        value[j] = data[i];
        j++;
    }
    WebVisu::brightness = atoi(value);

    // Get value for brightness
    j = 0;
    k = 0;
    memset(value, 0, VAL_LENGHT);
    for (i = separatorPos[1] + 3; i <= separatorPos[2] - 1; i++)
    {
        value[j] = data[i];
        j++;
    }
    WebVisu::speed = atoi(value);

    // Get value for color
    j = 0;    
    memset(value, 0, VAL_LENGHT);
    for (i = separatorPos[2] + 6; i <= separatorPos[2] + 12; i++)
    {
        value[j] = data[i];
        j++;
    }
    WebVisu::colorHex = strtol(value, NULL, 16);
    memset(value, 0, VAL_LENGHT);
    i = separatorPos[2] + 6;
    value[0] = data[i];
    i++;
    value[1] = data[i];
    i++;
    WebVisu::color.red = strtol(value, NULL, 16);
    value[0] = data[i];
    i++;
    value[1] = data[i];
    i++;
    WebVisu::color.green = strtol(value, NULL, 16);
    value[0] = data[i];
    i++;
    value[1] = data[i];
    i++;
    WebVisu::color.blue = strtol(value, NULL, 16);
}
