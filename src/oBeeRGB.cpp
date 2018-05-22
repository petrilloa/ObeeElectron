#include "oBeeRGB.h"

oBeeRGB::oBeeRGB():Adafruit_NeoPixel(3, D1, WS2812)
{
    }

oBeeRGB::oBeeRGB(int pixels, int pin, int type)
    :Adafruit_NeoPixel(pixels, pin, type){

    }

void oBeeRGB::Update()
{
    if((millis() - lastUpdate) > Interval) // time to update
    {
        lastUpdate = millis();
        if (Running)
        {
          switch(ActivePattern)
          {
              case RAINBOW_CYCLE:
                  RainbowCycleUpdate();
                  break;
              case THEATER_CHASE:
                  TheaterChaseUpdate();
                  break;
              case COLOR_WIPE:
                  ColorWipeUpdate();
                  break;
              case SCANNER:
                  ScannerUpdate();
                  break;
              case FADE:
                  FadeUpdate();
                  break;
               case SOLID_COLOR:
                  SolidColorUpdate();
                  break;
              default:
                  break;
          }
        }
        else
        {
          //Turn OFF
          ColorSet(Color(0,0,0));
        }
    }
}

void oBeeRGB::Increment()
{
        if (Direction == FORWARD)
        {
           Index++;
           if (Index >= TotalSteps)
            {
                Index = 0;
                //if (OnComplete != NULL)
                //{

                    //OnComplete(); // call the comlpetion callback
                    //Serial.println("RGBComplete");
                    //SolidColor(Color(0,0,0));
                    Running = false;

                //}
            }
        }
        else // Direction == REVERSE
        {
            --Index;
            if (Index <= 0)
            {
                Index = TotalSteps-1;
                //if (OnComplete != NULL)
                //{
                    Running = false;
                    //OnComplete(); // call the comlpetion callback
                //}
            }
        }
    }

void oBeeRGB::Reverse()
{
    if (Direction == FORWARD)
        {
            Direction = REVERSE;
            Index = TotalSteps-1;
        }
        else
        {
            Direction = FORWARD;
            Index = 0;
        }
}

void oBeeRGB::RainbowCycle(uint8_t interval, direction dir)
{
    ActivePattern = RAINBOW_CYCLE;
    Interval = interval;
    TotalSteps = 255;
    Index = 0;
    Direction = dir;

    Running = true;
}

void oBeeRGB::RainbowCycleUpdate()
{
  if (Repeat || (Repeat == false && Running == true))
  {
    for(int i=0; i< numPixels(); i++)
    {
        setPixelColor(i, Wheel(((i * 256 / numPixels()) + Index) & 255));
    }
    show();
    Increment();
  }
}

void oBeeRGB::TheaterChase(uint32_t color1, uint32_t color2, uint8_t interval, direction dir)
{
    ActivePattern = THEATER_CHASE;
    Interval = interval;
    TotalSteps = 150;
    Color1 = color1;
    Color2 = color2;
    Index = 0;
    Direction = dir;

    Running = true;
}

void oBeeRGB::TheaterChaseUpdate()
{
  if (Repeat || (Repeat == false && Running == true))
  {
        for(int i=0; i< numPixels(); i++)
        {
            if ((i + Index) % 3 == 0)
            {
                setPixelColor(i, Color1);
            }
            else
            {
                setPixelColor(i, Color2);
            }
        }
        show();
        Increment();
    }
}

void oBeeRGB::ColorWipe(uint32_t color, uint8_t interval, direction dir)
{
    ActivePattern = COLOR_WIPE;
    Interval = interval;
    TotalSteps = numPixels();
    Color1 = color;
    Index = 0;
    Direction = dir;

    Running = true;
}

void oBeeRGB::ColorWipeUpdate()
{
    if (Repeat || (Repeat == false && Running == true))
    {
        setPixelColor(Index, Color1);
        show();
        Increment();
    }
}

void oBeeRGB::Scanner(uint32_t color1, uint8_t interval)
{
    ActivePattern = SCANNER;
    Interval = interval;
    TotalSteps = (numPixels() - 1) * 2;
    Color1 = color1;
    Index = 0;

    Running = true;
}

void oBeeRGB::ScannerUpdate()
{
    if (Repeat || (Repeat == false && Running == true))
        {
        for (int i = 0; i < numPixels(); i++)
        {
            if (i == Index)  // Scan Pixel to the right
            {
                 setPixelColor(i, Color1);
            }
            else if (i == TotalSteps - Index) // Scan Pixel to the left
            {
                 setPixelColor(i, Color1);
            }
            else // Fading tail
            {
                 setPixelColor(i, DimColor(getPixelColor(i)));
            }
        }
        show();
        Increment();
    }
}

void oBeeRGB::Fade(uint32_t color1, uint32_t color2, uint16_t steps, uint8_t interval, direction dir)
{
    ActivePattern = FADE;
    Interval = interval;
    TotalSteps = steps;
    Color1 = color1;
    Color2 = color2;
    Index = 0;
    Direction = dir;

    Running = true;
}

void oBeeRGB::FadeUpdate()
{
    if (Repeat || (Repeat == false && Running == true))
    {
    // Calculate linear interpolation between Color1 and Color2
    // Optimise order of operations to minimize truncation error
    uint8_t red = ((Red(Color1) * (TotalSteps - Index)) + (Red(Color2) * Index)) / TotalSteps;
    uint8_t green = ((Green(Color1) * (TotalSteps - Index)) + (Green(Color2) * Index)) / TotalSteps;
    uint8_t blue = ((Blue(Color1) * (TotalSteps - Index)) + (Blue(Color2) * Index)) / TotalSteps;

    ColorSet(Color(red, green, blue));
    show();
    Increment();
    }
}

uint32_t oBeeRGB::DimColor(uint32_t color)
{
    // Shift R, G and B components one bit to the right
    uint32_t dimColor = Color(Red(color) >> 1, Green(color) >> 1, Blue(color) >> 1);
    return dimColor;
}

void oBeeRGB::SolidColor(uint32_t color)
{
    ActivePattern = SOLID_COLOR;
    Interval = 0;
    TotalSteps = 0;
    Color1 = color;
    Index = 0;

    Running = true;
}

void oBeeRGB::SolidColorUpdate()
{
    if (Running)
    {
        ColorSet(Color1);
        show();
    }
}

void oBeeRGB::ColorSet(uint32_t color)
    {
        for (int i = 0; i < numPixels(); i++)
        {
            setPixelColor(i, color);
        }
        show();
    }

// Returns the Red component of a 32-bit color
    uint8_t oBeeRGB::Red(uint32_t color)
    {
        return (color >> 16) & 0xFF;
    }

    // Returns the Green component of a 32-bit color
    uint8_t oBeeRGB::Green(uint32_t color)
    {
        return (color >> 8) & 0xFF;
    }

    // Returns the Blue component of a 32-bit color
    uint8_t oBeeRGB::Blue(uint32_t color)
    {
        return color & 0xFF;
    }

    // Input a value 0 to 255 to get a color value.
    // The colours are a transition r - g - b - back to r.
    uint32_t oBeeRGB::Wheel(byte WheelPos)
    {
        WheelPos = 255 - WheelPos;
        if(WheelPos < 85)
        {
            return Color(255 - WheelPos * 3, 0, WheelPos * 3);
        }
        else if(WheelPos < 170)
        {
            WheelPos -= 85;
            return Color(0, WheelPos * 3, 255 - WheelPos * 3);
        }
        else
        {
            WheelPos -= 170;
            return Color(WheelPos * 3, 255 - WheelPos * 3, 0);
        }
    }
