/*******************************************************************************
 * Start of Arduino_GFX setting
 ******************************************************************************/
#include <Arduino_GFX_Library.h>


#if 0
#define TFT_SCK 18
#define TFT_MOSI 23
#define TFT_RST 26
#define TFT_DC 25
//#define TFT_BL 22
#define TFT_CS -1 // for display without CS pin
#define TFT_MISO -1
#endif
#include "tft_pins.h"

/*
 * Step 1: Initize one databus for your display
*/

// General software SPI => does not work
//Arduino_DataBus *bus = new Arduino_SWSPI(TFT_DC, TFT_CS, TFT_SCK , TFT_MOSI , TFT_MISO );


// General hardware SPI
Arduino_DataBus *bus = new Arduino_HWSPI(TFT_DC, TFT_CS);

// ESP32 hardware SPI, more customizable parameters
//Arduino_DataBus *bus = new Arduino_ESP32SPI(TFT_DC, TFT_CS, TFT_SCK, TFT_MOSI, TFT_MISO, VSPI);


/*
 * Step 2: Initize one driver for your display
*/


// SSD1331 OLED 96x64
Arduino_SSD1331 *gfx = new Arduino_SSD1331(bus, TFT_RST, 0 /* rotation */);

/*******************************************************************************
 * End of Arduino_GFX setting
 ******************************************************************************/

void setup(void)
{
    // Seems to require 80Mhz to work
    gfx->begin(80000000);
    gfx->fillScreen(BLACK);

#ifdef TFT_BL
    pinMode(TFT_BL, OUTPUT);
    digitalWrite(TFT_BL, HIGH);
#endif

    gfx->setCursor(10, 10);
    gfx->setTextColor(RED);
    gfx->println("Hello World!");

    delay(5000); // 5 seconds
}

void loop()
{
    gfx->setCursor(random(gfx->width()), random(gfx->height()));
    gfx->setTextColor(random(0xffff));
    gfx->setTextSize(random(9) /* x scale */, random(9) /* y scale */, random(3) /* pixel_margin */);
    gfx->println("Hello World!");

    delay(1000); // 1 second
}
