/* NeoPixel */
#include "Adafruit_NeoPixel.h"
#include "WebVisu.hpp"
#include "config.h"
#include "ArduinoBLE.h"

#define NUMPIXELS 87
#define PIN 3

// WebVisu object
WebVisu userInterface;
// Neopixel object
Adafruit_NeoPixel pixels = Adafruit_NeoPixel(NUMPIXELS, PIN, NEO_GRB + NEO_KHZ800);

// define strucxture for LED position
struct ledPath
{
    std::vector<uint16_t> ledIndexRow;
    std::vector<uint16_t> ledIndexCross;
};
struct ledPosition
{
    uint16_t index;
    uint16_t posX;
    uint16_t posY;
    ledPath path;
};

// variable for snake mode
std::vector<snakeSegment> snake;

/* GLOBAL CONSTANTS */
// Modes
const uint8_t MODE_RAINBOW_FADE = 1;
const uint8_t MODE_WARM_COLOR_FADE = 2;
const uint8_t MODE_COLD_COLOR_FADE = 3;
const uint8_t MODE_RAIDIAL_RAINBOW_FADE = 4;
const uint8_t MODE_RAIDIAL_WARM_COLOR_FADE = 5;
const uint8_t MODE_RAIDIAL_COLD_COLOR_FADE = 6;
const uint8_t MODE_MONOCHROME_FADE_RAINBOW = 7;
const uint8_t MODE_MONOCHROME_FADE_WARM_COLOR = 8;
const uint8_t MODE_MONOCHROME_FADE_COLD_COLOR = 9;
const uint8_t MODE_RANDOM_COLOR = 10;
const uint8_t MODE_SINGLE_COLOR = 11;
const uint8_t MODE_WARM_WHITE = 12;
const uint8_t MODE_SNAKE = 13;
// FadeTypes
const uint8_t FADE_RAINBOW = 1;
const uint8_t FADE_WARM_COLOR = 2;
const uint8_t FADE_COLD_COLOR = 3;

// define array of LED descript by there position
const ledPosition LAMP_DEF[NUMPIXELS] = {
    {0, 140, 311, {{1}, {17,18}}},
    {1, 106, 311, {{0,2}, {}}},
    {2, 73, 311, {{1}, {3}}},

    {3, 33, 288, {{4}, {2}}},
    {4, 17, 259, {{3,5}, {}}},
    {5, 0, 231, {{4}, {6}}},

    {6, 0, 184, {{7}, {5}}},
    {7, 17, 156, {{6,8}, {}}},
    {8, 33, 127, {{7}, {9}}},

    {9, 73, 104, {{10}, {8}}},
    {10, 106, 104, {{9,11}, {}}},
    {11, 140, 104, {{10}, {12}}},

    {12, 180, 127, {{13}, {11}}},
    {13, 196, 156, {{12,14}, {}}},
    {14, 213, 184, {{13}, {15,32}}},

    {15, 213, 231, {{16}, {14,32}}},
    {16, 196, 259, {{15,17}, {}}},
    {17, 180, 288, {{16}, {0,18}}},

    {18, 180, 334, {{19}, {0,17}}},
    {19, 196, 363, {{18,20}, {}}},
    {20, 213, 392, {{19}, {21}}},

    {21, 253, 415, {{22}, {20}}},
    {22, 286, 415, {{21,23}, {}}},
    {23, 319, 415, {{22}, {24}}},

    {24, 359, 392, {{25}, {23}}},
    {25, 376, 363, {{24,26}, {}}},
    {26, 393, 334, {{25}, {27,47}}},

    {27, 393, 288, {{28}, {26,47}}},
    {28, 376, 259, {{27,29}, {}}},
    {29, 359, 231, {{28}, {30,33}}},

    {30, 253, 208, {{31}, {29,33}}},
    {31, 286, 208, {{30,32}, {}}},
    {32, 319, 208, {{31}, {14,15}}},

    {33, 359, 184, {{34}, {29,30}}},
    {34, 376, 156, {{33,35}, {}}},
    {35, 393, 127, {{34}, {36}}},

    {36, 433, 104, {{37}, {35}}},
    {37, 466, 104, {{36,38}, {}}},
    {38, 499, 104, {{37}, {39,63}}},

    {39, 539, 127, {{40}, {38,63}}},
    {40, 556, 156, {{39,41}, {}}},
    {41, 572, 184, {{40}, {42,62}}},

    {42, 572, 231, {{43}, {41,62}}},
    {43, 556, 259, {{42,44}, {}}},
    {44, 539, 288, {{43}, {45,48}}},

    {45, 433, 311, {{46}, {44,48}}},
    {46, 466, 311, {{45,47}, {}}},
    {47, 499, 311, {{46}, {26,27}}},

    {48, 539, 334, {{49}, {44,45}}},
    {49, 556, 363, {{48,50}, {}}},
    {50, 572, 392, {{49}, {51}}},

    {51, 612, 415, {{52}, {50}}},
    {52, 646, 415, {{51,53}, {}}},
    {53, 679, 415, {{52}, {54}}},

    {54, 719, 392, {{55}, {53}}},
    {55, 735, 363, {{54,56}, {}}},
    {56, 752, 334, {{55}, {57,86}}},

    {57, 752, 288, {{58}, {56,86}}},
    {58, 735, 259, {{57,59}, {}}},
    {59, 719, 231, {{58}, {60,74}}},

    {60, 679, 208, {{61}, {59,74}}},
    {61, 646, 208, {{62,60}, {}}},
    {62, 612, 208, {{61}, {41,42}}},

    {63, 539, 81, {{64}, {38,39}}},
    {64, 556, 52, {{63,65}, {}}},
    {65, 572, 23, {{64}, {66}}},

    {66, 612, 0, {{67}, {65}}},
    {67, 646, 0, {{66,68}, {}}},
    {68, 679, 0, {{67}, {69}}},

    {69, 719, 23, {{70}, {68}}},
    {70, 735, 52, {{69,71}, {}}},
    {71, 752, 81, {{70}, {72,75}}},

    {72, 719, 184, {{73}, {71,75}}},
    {73, 735, 156, {{72,74}, {}}},
    {74, 752, 127, {{73}, {59,60}}},

    {75, 792, 104, {{76}, {71,72}}},
    {76, 825, 104, {{75,77}, {}}},
    {77, 859, 104, {{76}, {78}}},

    {78, 899, 127, {{79}, {77}}},
    {79, 915, 156, {{78,80}, {}}},
    {80, 932, 184, {{79}, {81}}},

    {81, 932, 231, {{82}, {80}}},
    {82, 915, 259, {{81,83}, {}}},
    {83, 899, 288, {{82}, {84}}},

    {84, 859, 311, {{85}, {83}}},
    {85, 825, 311, {{84,86}, {}}},
    {86, 792, 311, {{85}, {56,57}}}
};

/* GLOBAL VARIABLS*/
// Minimal value for the x coordinate
static uint16_t xMin;
// Maximal value for the x coordinate
static uint16_t xMax;
// Minimal value for the y coordinate
static uint16_t yMin;
// Maximal value for the y coordinate
static uint16_t yMax;
// power status
static bool powered;
// current lamp settings
static lampSettings settings;
// step size for fade mode
static uint8_t stepSize;
// delay time for animation call
static uint8_t delayTime;
// start time
static uint32_t startTime;

/* SETUP */
void setup()
{
    delay(5000);
    // Configure PWM frequency
    analogWriteResolution(12);

    uint8_t i;
    ledPosition led;
    bool wifiConnected;

    // init pixels
    pixels.begin();
    pixels.clear();
    pixels.setPixelColor(1, 0, 0, 255);
    pixels.show();

    // Get max values for coordinates
    for (i = 0; i < NUMPIXELS; i++)
    {
        led = GetLedPosition(i);
        xMax = max(xMax, led.posX);
        yMax = max(yMax, led.posY);
    }

    // Get min values for coordinates
    /*
    xMin = xMax;
    yMin = yMax;
    for (i = 0; i < NUMPIXELS; i++)
    {
      led = GetLedPosition(i);
      xMin = min(xMax, led.posX);
      yMin = min(yMax, led.posY);
    }
    */
    xMin = 0;
    yMin = 0;

    // connect to wifi
    wifiConnected = userInterface.connectToWiFi(SECRET_SSID, SECRET_PASS, IP);

    pixels.clear();
    if (wifiConnected) // connection successful
    {
        pixels.setPixelColor(1, 0, 255, 0);
        pixels.show();
        delay(1000);
        pixels.clear();
        pixels.show();
    }
    else // connection failed
    {
        pixels.setPixelColor(1, 255, 0, 0);
        pixels.show();
        while (true)
        {
            // Do nothing
        }
    }

    userInterface.init();
    startTime = millis();
}

void loop()
{
    // check if still connected
    if (userInterface.isConnected() == false)
    {
        bool wifiConnected = false;
        wifiConnected = userInterface.connectToWiFi(SECRET_SSID, SECRET_PASS, IP);
    }
    // check for user input
    userInterface.handleClientRequest();
    powered = userInterface.getPowerState();
    if (powered)
    {
        // get settings
        settings = userInterface.getLampSettings();
        stepSize = settings.speed;
        delayTime = 50 - (settings.speed - 10);
    }

    // detect mode change
    bool modeChanged;
    static uint8_t modeOld;
    if (settings.mode != modeOld)
    {
        modeChanged = true;
        modeOld = settings.mode;
    }
    else
    {
        modeChanged = false;
    }

    // delay for animation
    uint32_t currentTime = millis();
    uint32_t lastUpdate = currentTime - startTime;
    bool callAnimation;
    if (lastUpdate > delayTime)
    {
        callAnimation = true;
        startTime = currentTime;
    }
    else
    {
        callAnimation = false;
    }

    // lamp modes
    if (powered)
    {
        switch (settings.mode)
        {
        case MODE_RAINBOW_FADE:
            LinearFade(stepSize, FADE_RAINBOW);
            break;

        case MODE_WARM_COLOR_FADE:
            LinearFade(stepSize, FADE_WARM_COLOR);
            break;

        case MODE_COLD_COLOR_FADE:
            LinearFade(stepSize, FADE_COLD_COLOR);
            break;

        case MODE_RAIDIAL_RAINBOW_FADE:
            RadialFade(stepSize, FADE_RAINBOW);
            break;

        case MODE_RAIDIAL_WARM_COLOR_FADE:
            RadialFade(stepSize, FADE_WARM_COLOR);
            break;

        case MODE_RAIDIAL_COLD_COLOR_FADE:
            RadialFade(stepSize, FADE_COLD_COLOR);
            break;

        case MODE_MONOCHROME_FADE_RAINBOW:
            if (callAnimation)
            {
                MonochromeFade(FADE_RAINBOW);
            }
            break;

        case MODE_MONOCHROME_FADE_WARM_COLOR:
            if (callAnimation)
            {
                MonochromeFade(FADE_WARM_COLOR);
            }
            break;

        case MODE_MONOCHROME_FADE_COLD_COLOR:
            if (callAnimation)
            {
                MonochromeFade(FADE_COLD_COLOR);
            }
            break;

        case MODE_RANDOM_COLOR:
            if (callAnimation)
            {
                RandomColor();
            }
            break;

        case MODE_SINGLE_COLOR:
            if (callAnimation)
            {
                SingelColor(settings.color);
            }
            break;

        case MODE_WARM_WHITE:
            if (callAnimation)
            {
                WarmWhite();
            }
            break;

        case MODE_SNAKE:
            if (callAnimation)
            {
                if (modeChanged)
                {
                    for (int i = 0; i < NUMPIXELS; i++)
                    {
                        pixels.setPixelColor(i, 0, 0, 0);
                    }
                }
                Snake();
            }
            break;
        }
    }
    else
    {
        pixels.clear();
    }

    pixels.setBrightness(settings.brightness);
    pixels.show();
}

void LinearFade(uint16_t stepSize, uint8_t fadeType)
{
    uint16_t pos, wheelPos;
    uint8_t i;
    static uint8_t j;
    ledPosition led;

    for (pos = 0; pos <= xMax + stepSize; pos = pos + stepSize)
    {
        wheelPos = uint16_t(float(255) / float(xMax) * float(pos));
        for (i = 0; i < NUMPIXELS; i++)
        {
            led = GetLedPosition(i);
            if (pos - stepSize <= led.posX & led.posX <= pos)
            {
                switch (fadeType)
                {
                case FADE_RAINBOW:
                    pixels.setPixelColor(led.index, ColorWheel((wheelPos - j) & 255));
                    break;
                case FADE_WARM_COLOR:
                    pixels.setPixelColor(led.index, WarmColorWheel((wheelPos - j) & 255));
                    break;
                case FADE_COLD_COLOR:
                    pixels.setPixelColor(led.index, ColdColorWheel((wheelPos - j) & 255));
                    break;
                }
            }
        }
    }
    j++;
}

void RadialFade(uint16_t stepSize, uint8_t fadeType)
{
    static uint8_t j = 0;
    uint16_t centerX = (xMax - xMin) / 2;
    uint16_t centerY = (yMax - yMin) / 2;
    uint16_t maxDistance = max((xMax - centerX), (yMax - centerY));

    for (uint16_t pos = 0; pos <= maxDistance + stepSize; pos = pos + stepSize)
    {
        uint16_t wheelPos = uint16_t(float(255) / float(maxDistance / 2) * float(pos));
        for (uint8_t i = 0; i < NUMPIXELS; i++)
        {
            ledPosition led = GetLedPosition(i);

            // Calculate distance from the center LED using Pythagorean theorem
            int16_t distanceX = centerX - led.posX;
            int16_t distanceY = centerY - led.posY;
            uint16_t distance = sqrt(distanceX * distanceX + distanceY * distanceY);

            if (pos - stepSize <= distance & distance <= pos)
            {
                switch (fadeType)
                {
                case FADE_RAINBOW:
                    pixels.setPixelColor(led.index, ColorWheel((wheelPos - j) & 255));
                    break;
                case FADE_WARM_COLOR:
                    pixels.setPixelColor(led.index, WarmColorWheel((wheelPos - j) & 255));
                    break;
                case FADE_COLD_COLOR:
                    pixels.setPixelColor(led.index, ColdColorWheel((wheelPos - j) & 255));
                    break;
                }
            }
        }
    }
    j++;
}

void MonochromeFade(uint8_t fadeType)
{
    int8_t i;
    static uint8_t j;
    for (i = 0; i < NUMPIXELS; i++)
    {
        switch (fadeType)
        {
        case FADE_RAINBOW:
            pixels.setPixelColor(i, ColorWheel(j & 255));
            break;
        case FADE_WARM_COLOR:
            pixels.setPixelColor(i, WarmColorWheel(j & 255));
            break;
        case FADE_COLD_COLOR:
            pixels.setPixelColor(i, ColdColorWheel(j & 255));
            break;
        }
    }
    j++;
}

void SingelColor(rgbColor color)
{
    int8_t i;
    for (i = 0; i < NUMPIXELS; i++)
    {
        pixels.setPixelColor(i, color.red, color.green, color.blue);
    }
}

void WarmWhite()
{
    int8_t i;
    for (i = 0; i < NUMPIXELS; i++)
    {
        pixels.setPixelColor(i, 253, 244, 220);
    }
}

void RandomColor()
{
    uint8_t changeColor;
    uint8_t selectedColor;
    static rgbColor color;
    uint32_t pixelColorPacked;
    static rgbColor pixelColor[NUMPIXELS];
    static rgbColor stepSize[NUMPIXELS];
    static bool fade;
    static uint8_t fadeStep;

    changeColor = random(0, 250);
    if (!fade)
    {
        if (changeColor == 0)
        {
            fade = true;
            fadeStep = 0;
            selectedColor = random(1, 13);
            switch (selectedColor)
            {
            case 1: // red
                color.red = 255;
                color.green = 0;
                color.blue = 0;
                break;
            case 2: // rose
                color.red = 255;
                color.green = 0;
                color.blue = 128;
                break;
            case 3: // magenta
                color.red = 255;
                color.green = 0;
                color.blue = 255;
                break;
            case 4: // violet
                color.red = 128;
                color.green = 0;
                color.blue = 255;
                break;
            case 5: // blue
                color.red = 0;
                color.green = 0;
                color.blue = 255;
                break;
            case 6: // azure
                color.red = 0;
                color.green = 128;
                color.blue = 255;
                break;
            case 7: // cyan
                color.red = 0;
                color.green = 255;
                color.blue = 255;
                break;
            case 8: // spring green
                color.red = 0;
                color.green = 255;
                color.blue = 128;
                break;
            case 9: // spring green
                color.red = 0;
                color.green = 255;
                color.blue = 0;
                break;
            case 10: // chartreuse
                color.red = 128;
                color.green = 255;
                color.blue = 0;
                break;
            case 11: // yellow
                color.red = 255;
                color.green = 255;
                color.blue = 0;
                break;
            case 12: // orange
                color.red = 255;
                color.green = 128;
                color.blue = 0;
                break;
            }
            for (int i = 0; i < NUMPIXELS; i++)
            {
                pixelColorPacked = pixels.getPixelColor(i);
                pixelColor[i].red = (pixelColorPacked >> (8 * 2)) & 0xff;
                pixelColor[i].green = (pixelColorPacked >> (8 * 1)) & 0xff;
                pixelColor[i].blue = (pixelColorPacked >> (8 * 0)) & 0xff;
                stepSize[i].red = (color.red - pixelColor[i].red) / 49;
                stepSize[i].green = (color.green - pixelColor[i].green) / 49;
                stepSize[i].blue = (color.blue - pixelColor[i].blue) / 49;
            }
        }
    }
    else
    {
        for (int i = 0; i < NUMPIXELS; i++)
        {
            if (fadeStep < 50)
            {
                pixelColor[i].red = pixelColor[i].red + stepSize[i].red;
                pixelColor[i].green = pixelColor[i].green + stepSize[i].green;
                pixelColor[i].blue = pixelColor[i].blue + stepSize[i].blue;
                pixels.setPixelColor(i, pixelColor[i].red, pixelColor[i].green, pixelColor[i].blue);
            }
            else
            {
                pixels.setPixelColor(i, color.red, color.green, color.blue);
                fade = false;
            }
        }
        delay(100);
        fadeStep++;
    }
}

void Snake()
{
    for (int i = 0; i < snake.size(); i++)
    {
        snakeSegment segment = snake[i];
        uint16_t newPositionIndex;
        uint16_t head = segment.positionIndex[0];
        ledPath path = GetLedPath(head);
        bool positionFound = false;
        for (int j = 0; j < path.ledIndexRow.size(); j++)
        {
            uint16_t index = path.ledIndexRow[j];
            if (index != segment.positionIndex[1])
            {
                newPositionIndex = index;
                positionFound = true;
                break;
            }
        }
        if (!positionFound)
        {
            std::vector<uint16_t> posibleNewCrossPosition;
            for (int j = 0; j < path.ledIndexCross.size(); j++)
            {
                uint16_t index = path.ledIndexCross[j];
                if (index != segment.positionIndex[1])
                {
                    posibleNewCrossPosition.push_back(index);
                }
            }
            newPositionIndex = posibleNewCrossPosition[random(0, posibleNewCrossPosition.size())];
        }
        segment.positionIndex.insert(segment.positionIndex.begin(), newPositionIndex);
        if (segment.positionIndex.size() > segment.maxLength)
        {
            for (int j = segment.positionIndex.size() - 1; j > segment.maxLength - 1; j--)
            {
                pixels.setPixelColor(segment.positionIndex[j], 0, 0, 0);
            }
            segment.positionIndex.resize(segment.maxLength);
        }
        snake[i] = segment;
        for (int j = 0; j < segment.positionIndex.size(); j++)
        {
            uint8_t offsetFactor = segment.fadeStep * j;
            uint8_t red = max(0, (int)segment.color.red - offsetFactor);
            uint8_t green = max(0, (int)segment.color.green - offsetFactor);
            uint8_t blue = max(0, (int)segment.color.blue - offsetFactor);
            pixels.setPixelColor(segment.positionIndex[j], red, green, blue);
        }
    }
}

uint32_t ColorWheel(byte wheelPos)
{
    const int factor = 3;
    if (wheelPos < 85)
    {
        return pixels.Color(wheelPos * factor, 255 - wheelPos * factor, 0);
    }
    else if (wheelPos < 170)
    {
        wheelPos -= 85;
        return pixels.Color(255 - wheelPos * factor, 0, wheelPos * factor);
    }
    else
    {
        wheelPos -= 170;
        return pixels.Color(0, wheelPos * factor, 255 - wheelPos * factor);
    }
}

uint32_t WarmColorWheel(byte wheelPos)
{
    if (wheelPos < 128)
    {
        return pixels.Color(255, (wheelPos * 2), 0);
    }
    else
    {
        wheelPos -= 128;
        return pixels.Color(255, 255 - (wheelPos * 2), 0);
    }
}

uint32_t ColdColorWheel(byte wheelPos)
{
    if (wheelPos < 128)
    {
        return pixels.Color(0, (wheelPos * 2), 255);
    }
    else
    {
        wheelPos -= 128;
        return pixels.Color(0, 255 - (wheelPos * 2), 255);
    }
}

ledPosition GetLedPosition(int i)
{
    ledPosition led;
    led.index = LAMP_DEF[i].index;
    led.posX = LAMP_DEF[i].posX;
    led.posY = LAMP_DEF[i].posY;
    return led;
}
ledPath GetLedPath(int i)
{
    ledPath path;
    uint16_t rowSize = LAMP_DEF[i].path.ledIndexRow.size();
    uint16_t crossSize = LAMP_DEF[i].path.ledIndexCross.size();
    for (int j = 0; j < rowSize; j++)
    {
        path.ledIndexRow.push_back(LAMP_DEF[i].path.ledIndexRow[j]);
    }
    for (int j = 0; j < crossSize; j++)
    {
        path.ledIndexCross.push_back(LAMP_DEF[i].path.ledIndexCross[j]);
    }
    return path;
}

rgbColor getRgbColor(uint32_t color)
{
    rgbColor resultColor;
    resultColor.blue = (color) & 0xFF;
    resultColor.green = (color >> 8) & 0xFF;
    resultColor.red = (color >> 16) & 0xFF;
    return resultColor;
}
