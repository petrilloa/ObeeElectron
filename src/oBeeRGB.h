/* oBeeRGB.h */

// This #include statement was automatically added by the Particle IDE.
#include "lib/neopixel.h"

#ifndef _oBeeRGB_h
#define _oBeeRGB_h


// Pattern types supported:
enum  pattern { RAINBOW_CYCLE, THEATER_CHASE, COLOR_WIPE, SCANNER, FADE, SOLID_COLOR, FIRE, FIREPLASMA, SPARKLE, TIMER_IDLE, TIMER_RUNNING, TIMER_FINISH };
// Patern directions supported:
enum  direction { FORWARD, REVERSE };

class oBeeRGB : public Adafruit_NeoPixel
{
    public:

    // Member Variables:
    pattern  ActivePattern;  // which pattern is running
    direction Direction;     // direction to run the pattern

    unsigned long Interval;   // milliseconds between updates
    unsigned long lastUpdate; // last update of position

    uint32_t Color1, Color2;  // What colors are in use
    uint16_t TotalSteps;  // total number of steps in the pattern
    uint16_t Index;  // current step within the pattern

    bool Repeat = false; //Indicates if must repeat animation
    bool Running = true; //Indicates if the animation has ended

    //void (*OnComplete)();  // Callback on completion of pattern

    // Constructor - calls base-class constructor to initialize strip

    //oBeeRGB(uint16_t pixels, uint8_t pin, uint8_t type, void (*callback)());
    oBeeRGB();
    oBeeRGB(int pixels, int pin, int type);

    //void Init(uint16_t pixels, uint8_t pin, uint8_t type, void (*callback)());
    //void Init(uint16_t pixels, uint8_t pin, uint8_t type);
    void Update();
    void Increment();
    void Reverse();

    void RainbowCycle(uint8_t interval, direction dir = FORWARD);
    void RainbowCycleUpdate();

    void TheaterChase(uint32_t color1, uint32_t color2, uint8_t interval, direction dir = FORWARD);
    void TheaterChaseUpdate();

    void ColorWipe(uint32_t color, uint8_t interval, direction dir = FORWARD);
    void ColorWipeUpdate();

    void Scanner(uint32_t color1, uint8_t interval);
    void ScannerUpdate();

    void Fade(uint32_t color1, uint32_t color2, uint16_t steps, uint8_t interval, direction dir = FORWARD);
    void FadeUpdate();

    // Calculate 50% dimmed version of a color (used by ScannerUpdate)
    uint32_t DimColor(uint32_t color);


    void SolidColor(uint32_t color);
    void SolidColorUpdate();

    // Set all pixels to a color (synchronously)
    void ColorSet(uint32_t color);


    // Returns the Red component of a 32-bit color
    uint8_t Red(uint32_t color);

    // Returns the Green component of a 32-bit color
    uint8_t Green(uint32_t color);

    // Returns the Blue component of a 32-bit color
    uint8_t Blue(uint32_t color);

    // Input a value 0 to 255 to get a color value.
    // The colours are a transition r - g - b - back to r.
    uint32_t Wheel(byte WheelPos);

};

#endif
