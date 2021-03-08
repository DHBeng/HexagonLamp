/* NeoPixel */
#include "Adafruit_NeoPixel.h"
#include "WebVisu.h"
#include "config.h"

#define NUMPIXELS 87
#define PIN 3

WebVisu userInterface;
Adafruit_NeoPixel pixels = Adafruit_NeoPixel(NUMPIXELS, PIN, NEO_GRB + NEO_KHZ800);

struct ledPosition
{
  uint16_t index;

  uint16_t posX;
  uint16_t posY;
};

const ledPosition LAMP_DEF[NUMPIXELS] PROGMEM = {
    {0, 140, 311},
    {1, 106, 311},
    {2, 73, 311},
    {3, 33, 288},
    {4, 17, 259},
    {5, 0, 231},
    {6, 0, 184},
    {7, 17, 156},
    {8, 33, 127},
    {9, 73, 104},
    {10, 106, 104},
    {11, 140, 104},
    {12, 180, 127},
    {13, 196, 156},
    {14, 213, 184},
    {15, 213, 231},
    {16, 196, 259},
    {17, 180, 288},
    {18, 180, 334},
    {19, 196, 363},
    {20, 213, 392},
    {21, 253, 415},
    {22, 286, 415},
    {23, 319, 415},
    {24, 359, 392},
    {25, 376, 363},
    {26, 393, 334},
    {27, 393, 288},
    {28, 376, 259},
    {29, 359, 231},
    {30, 253, 208},
    {31, 286, 208},
    {32, 319, 208},
    {33, 359, 184},
    {34, 376, 156},
    {35, 393, 127},
    {36, 433, 104},
    {37, 466, 104},
    {38, 499, 104},
    {39, 539, 127},
    {40, 556, 156},
    {41, 572, 184},
    {42, 572, 231},
    {43, 556, 259},
    {44, 539, 288},
    {45, 433, 311},
    {46, 466, 311},
    {47, 499, 311},
    {48, 539, 334},
    {49, 556, 363},
    {50, 572, 392},
    {51, 612, 415},
    {52, 646, 415},
    {53, 679, 415},
    {54, 719, 392},
    {55, 735, 363},
    {56, 752, 334},
    {57, 752, 288},
    {58, 735, 259},
    {59, 719, 231},
    {60, 679, 208},
    {61, 646, 208},
    {62, 612, 208},
    {63, 539, 81},
    {64, 556, 52},
    {65, 572, 23},
    {66, 612, 0},
    {67, 646, 0},
    {68, 679, 0},
    {69, 719, 23},
    {70, 735, 52},
    {71, 752, 81},
    {72, 719, 184},
    {73, 735, 156},
    {74, 752, 127},
    {75, 792, 104},
    {76, 825, 104},
    {77, 859, 104},
    {78, 899, 127},
    {79, 915, 156},
    {80, 932, 184},
    {81, 932, 231},
    {82, 915, 259},
    {83, 899, 288},
    {84, 859, 311},
    {85, 825, 311},
    {86, 792, 311}};
static uint16_t xMax;
static bool powered;
static lampSettings settings;
static uint8_t stepSize;
static uint8_t delayTime;
static uint32_t startTime;

void setup()
{
  uint8_t i;
  ledPosition led;
  bool connect;

  // init pixels
  pixels.begin();
  pixels.clear();
  for (i = 0; i < NUMPIXELS; i++)
  {
    led = GetLedPosition(i);
    xMax = max(xMax, led.posX);
  }

  // connect to wifi
  i = 0;
  pixels.setPixelColor(i, 0, 0, 255);
  i++;
  pixels.show();
  do
  {
    connect = userInterface.connectToWiFi(SECRET_SSID, SECRET_PASS, IP, 1000000);
    pixels.setPixelColor(i, 0, 0, 255);
    i++;
    pixels.show();
  } while (not connect);
  // connection successful
  pixels.clear();
  pixels.setPixelColor(0, 0, 255, 0);
  pixels.show();
  delay(1000);
  pixels.clear();
  pixels.show();

  userInterface.init();
  startTime = millis();
}

void loop()
{
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
    case 1: // rainbow fade
      RainbowFade(stepSize);
      break;
    case 2: // monochrome fade
      if (callAnimation) 
      {
        MonochromeFade();
      }
      break;
    case 3: // singel color
      if (callAnimation) 
      {
        SingelColor(settings.color);
      }
      break;
    case 4: // warm white
      if (callAnimation) 
      {
        WarmWhite();
      }
      break;
    case 5: // random color
      if (callAnimation) 
      {
        RandomColor();
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

void RainbowFade(uint16_t stepSize)
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
        pixels.setPixelColor(led.index, ColorWheel((wheelPos - j) & 255));
      }
    }
  }
  j++;
}

void MonochromeFade()
{
  int8_t i;
  static uint8_t j;
  for (i = 0; i < NUMPIXELS; i++)
  {
    pixels.setPixelColor(i, ColorWheel(j));
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

ledPosition GetLedPosition(int i)
{
  ledPosition led;
  led.index = pgm_read_word(&LAMP_DEF[i].index);
  led.posX = pgm_read_word(&LAMP_DEF[i].posX);
  led.posY = pgm_read_word(&LAMP_DEF[i].posY);
  return led;
}
