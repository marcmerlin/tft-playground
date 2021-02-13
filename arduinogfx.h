#include "tft_pins.h"

/*
 * Step 1: Initize one databus for your display
*/


// General software SPI
// works with ILI, 800K.  // Does not work with SSD1331
//Arduino_DataBus *bus = new Arduino_SWSPI(TFT_DC, TFT_CS, TFT_SCK , TFT_MOSI , TFT_MISO );

// General hardware SPI
// does not work for ILI, screen fill: 234K, SSD1331 6.6K
// SSD1331: works 80Mhz, not 40Mhz, or 20Mhz
//Arduino_DataBus *bus = new Arduino_ESP32SPI_DMA(TFT_DC, TFT_CS, TFT_SCK, TFT_MOSI, TFT_MISO, VSPI);

// works for ILI, screen fill: 324K  vs 349 adafruit, SSD1331 7.5K 80Mhz
// SSD1331: works 80Mhz, not 40Mhz, 20Mhz or 10Mhz
//Arduino_DataBus *bus = new Arduino_ESP32SPI(TFT_DC, TFT_CS, TFT_SCK, TFT_MOSI, TFT_MISO, VSPI);

// works for ILI scren fill 347K, SSD1331 9K at 80Mhz
// SSD1331: works 80Mhz, not 40Mhz, or 20Mhz
Arduino_DataBus *bus = new Arduino_HWSPI(TFT_DC, TFT_CS);
Arduino_DataBus *bus2 = new Arduino_HWSPI(TFT_DC, TFT_CS2);


// ESP32 parallel 8-bit
// Arduino_DataBus *bus = new Arduino_ESP32PAR8(TFT_DC, TFT_CS, 26 /* WR */, -1 /* RD */, 16 /* D0 */, 17 /* D1 */, 18 /* D2 */, 19 /* D3 */, 21 /* D4 */, 12 /* D5 */, 23 /* D6 */, 25 /* D7 */);

// SSD1331 OLED 96x64
// do not add 4th IPS argument, even FALSE
//Arduino_SSD1331 *gfx = new Arduino_SSD1331(bus, TFT_RST, 0 /* rotation */);
// ILI9341 LCD 240x320
Arduino_ILI9341 *gfx = new Arduino_ILI9341(bus2, TFT_RST, 0 /* rotation */);
