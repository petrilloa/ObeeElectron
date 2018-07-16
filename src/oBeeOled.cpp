#include "oBeeOled.h"



oBeeOled::oBeeOled():Adafruit_SSD1306(0)
{
    }

oBeeOled::oBeeOled(int OLED_RESET)
    :Adafruit_SSD1306(OLED_RESET){

    }

void oBeeOled::Update()
{
  //display.clearDisplay();
  //display.drawBitmap(0, 0,  logo_Obee, 128, 32, 1);
  //display.display();
}
