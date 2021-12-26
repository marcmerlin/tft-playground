/*******************************************************************************
 * Start of Arduino_GFX setting
 ******************************************************************************/
#define NO_TFT_SPI_PIN_DEFAULTS
#include <Arduino_GFX_Library.h>

/* all display share same SPI Data Bus with individual CS and RST pins */

#if 0
#define TFT_DC  25

#define TFT_MOSI 23
#define TFT_SCK  18
#define TFT_MISO -1

//#define TFT_CS -1 // for display without CS pin
#define TFT_CS  27 // can be writed to ground
#define TFT_RST 26
#endif

#include "tft_pins.h"

Arduino_DataBus *bus = new Arduino_HWSPI(TFT_DC, TFT_CS);
Arduino_DataBus *bus2 = new Arduino_HWSPI(TFT_DC, TFT_CS2);

// SSD1331 OLED 96x64
// do not add 4th IPS argument, even FALSE
// Do not reset first device when setting 2nd device
Arduino_SSD1331 *gfx1 = new Arduino_SSD1331(bus, TFT_RST, 2 /* rotation */);

// Do not reset 2nd device, it would unsetup the first
Arduino_ILI9341 *gfx2 = new Arduino_ILI9341(bus2, -1, 3 /* rotation */);


Arduino_GFX *gfx = gfx1;

uint32_t w, h, n, n1, cx, cy, cx1, cy1, cn, cn1;
uint8_t tsa, tsb, tsc, ds;

void setup()
{
#ifdef TFT_BL
  pinMode(TFT_BL, OUTPUT);
  digitalWrite(TFT_BL, HIGH);
#endif

  Serial.begin(115200);
  while (!Serial)
  {
    // wait and do nothing
  }

  Serial.print("Arduino_GFX library dual display Test speed ");
  Serial.println(tft_spi_speed);

  // For reasons unknown if gfx2 is initialized first, it breaks gfx1
  Serial.println();
  Serial.println("ILI on breadboard at 24Mhz (40Mhz can work, 80Mhz fine on LCA board)");
  gfx2->begin(40000000);
  gfx2->fillScreen(RED);
  delay(2000);

  Serial.println();
  Serial.println("SSD1331 on breadboard requires 80Mhz while ILI3941 requires 24Mhz");
  gfx1->begin(40000000);
  gfx1->fillScreen(YELLOW);
  delay(2000);


  Serial.println("Demo Start");
}

void loop()
{
  test();

  if (gfx == gfx2)
  {
    gfx = gfx1;
    // talking to GFX2 seems to upset GFX1 and it stops working
    // resetting it each time seems to help.
    // Just a bus reset is not enough
    //bus->begin(80000000);
    // needs the init sequence
    gfx1->begin(40000000);
    Serial.println("Back to GFX1");
  }
  else
  {
    gfx = gfx2;
    //bus2->begin(40000000);
    // for reasons unknown 40Mhz seems faster than 80Mhz
    // ILI needs a different SPI bus config to work
    gfx2->begin(40000000);
    Serial.println("Switch to GFX2/ILI");
  }
  delay(200);
}

void test()
{
  w = gfx->width();
  h = gfx->height();
  n = min(w, h);
  n1 = min(w, h) - 1;
  cx = w / 2;
  cy = h / 2;
  cx1 = cx - 1;
  cy1 = cy - 1;
  cn = min(cx1, cy1);
  cn1 = min(cx1, cy1) - 1;
  tsa = ((w <= 160) || (h <= 160)) ? 1 : (((w <= 240) || (h <= 240)) ? 2 : 3); // text size A
  tsb = ((w <= 240) || (h <= 220)) ? 1 : 2;                                    // text size B
  tsc = ((w <= 220) || (h <= 220)) ? 1 : 2;                                    // text size C
  ds = (w <= 160) ? 9 : 12;                                                    // digit size

  Serial.println(F("Benchmark                Time (microseconds)"));

  uint32_t usecFillScreen = testFillScreen();
  gfx->flush();
  Serial.print(F("Screen fill              "));
  Serial.println(usecFillScreen);
  delay(100);

  gfx->fillScreen(BLACK);
  uint32_t usecText = testText();
  gfx->flush();
  Serial.print(F("Text                     "));
  Serial.println(usecText);
  delay(3000); // delay for verifing the text

  gfx->fillScreen(BLACK);
  uint32_t usecPixels = testPixels();
  gfx->flush();
  Serial.print(F("Pixels                   "));
  Serial.println(usecPixels);
  delay(100);

  gfx->fillScreen(BLACK);
  uint32_t usecLines = testLines(BLUE);
  gfx->flush();
  Serial.print(F("Lines                    "));
  Serial.println(usecLines);
  delay(100);

  gfx->fillScreen(BLACK);
  uint32_t usecFastLines = testFastLines(RED, BLUE);
  gfx->flush();
  Serial.print(F("Horiz/Vert Lines         "));
  Serial.println(usecFastLines);
  delay(100);

  gfx->fillScreen(BLACK);
  uint32_t usecFilledRects = testFilledRects(YELLOW, MAGENTA);
  gfx->flush();
  Serial.print(F("Rectangles (filled)      "));
  Serial.println(usecFilledRects);
  delay(100);

  uint32_t usecRects = testRects(GREEN);
  gfx->flush();
  Serial.print(F("Rectangles (outline)     "));
  Serial.println(usecRects);
  delay(100);

  gfx->fillScreen(BLACK);
  uint32_t usecFilledCircles = testFilledCircles(10, MAGENTA);
  gfx->flush();
  Serial.print(F("Circles (filled)         "));
  Serial.println(usecFilledCircles);
  delay(100);

  uint32_t usecCircles = testCircles(10, WHITE);
  gfx->flush();
  Serial.print(F("Circles (outline)        "));
  Serial.println(usecCircles);
  delay(100);

  gfx->fillScreen(BLACK);
  uint32_t usecFilledArcs = testFillArcs();
  gfx->flush();
  Serial.print(F("Fill Arcs                "));
  Serial.println(usecFilledArcs);
  delay(100);

  uint32_t usecArcs = testArcs();
  gfx->flush();
  Serial.print(F("Draw Arcs                "));
  Serial.println(usecArcs);
  delay(100);

  gfx->fillScreen(BLACK);
  uint32_t usecFilledTrangles = testFilledTriangles();
  gfx->flush();
  Serial.print(F("Triangles (filled)       "));
  Serial.println(usecFilledTrangles);
  delay(100);

  uint32_t usecTriangles = testTriangles();
  gfx->flush();
  Serial.print(F("Triangles (outline)      "));
  Serial.println(usecTriangles);
  delay(100);

  gfx->fillScreen(BLACK);
  uint32_t usecFilledRoundRects = testFilledRoundRects();
  gfx->flush();
  Serial.print(F("Rounded rects (filled)   "));
  Serial.println(usecFilledRoundRects);
  delay(100);

  uint32_t usecRoundRects = testRoundRects();
  gfx->flush();
  Serial.print(F("Rounded rects (outline)  "));
  Serial.println(usecRoundRects);
  delay(100);

  Serial.println(F("Done!"));

  uint16_t c = 4;
  int8_t d = 1;
  for (int32_t i = 0; i < h; i++)
  {
    gfx->drawFastHLine(0, i, w, c);
    c += d;
    if (c <= 4 || c >= 11)
      d = -d;
  }

  gfx->setCursor(0, 0);
  gfx->setTextColor(MAGENTA);
  gfx->setTextSize(tsa);
  gfx->println(F("Arduino GFX"));
  gfx->setTextSize(1);
  gfx->println(F(""));

  if (h > w)
  {
    gfx->setTextColor(GREEN);
    gfx->setTextSize(tsb);
    gfx->print(F("Benchmark "));
    gfx->setTextSize(tsc);
    if (ds == 12)
    {
      gfx->print(F("   "));
    }
    gfx->println(F("micro-secs"));
    gfx->setTextSize(1);
    gfx->println(F(""));
    gfx->setTextColor(YELLOW);
  }

  printnice(F("Screen fill "), usecFillScreen);
  printnice(F("Text        "), usecText);
  printnice(F("Pixels      "), usecPixels);
  printnice(F("Lines       "), usecLines);
  printnice(F("H/V Lines   "), usecFastLines);
  printnice(F("Rectangles F"), usecFilledRects);
  printnice(F("Rectangles  "), usecRects);
  printnice(F("Circles F   "), usecFilledCircles);
  printnice(F("Circles     "), usecCircles);
  printnice(F("Arcs F      "), usecFilledArcs);
  printnice(F("Arcs        "), usecArcs);
  printnice(F("Triangles F "), usecFilledTrangles);
  printnice(F("Triangles   "), usecTriangles);
  printnice(F("RoundRects F"), usecFilledRoundRects);
  printnice(F("RoundRects  "), usecRoundRects);

  if (h > w)
  {
    gfx->setTextSize(1);
    gfx->println(F(""));
    gfx->setTextColor(GREEN);
    gfx->setTextSize(tsc);
    gfx->print(F("Benchmark Complete!"));
  }

  gfx->flush();
}

void printnice(const __FlashStringHelper *item, long unsigned int v)
{
  gfx->setTextColor(CYAN);
  gfx->setTextSize(tsb);
  gfx->print(item);
  gfx->setTextColor(YELLOW);
  gfx->setTextSize(tsc);

  char str[32] = {0};
  sprintf(str, "%lu", v);
  for (char *p = (str + strlen(str)) - 3; p > str; p -= 3)
  {
    memmove(p + 1, p, strlen(p) + 1);
    *p = ',';
  }
  while (strlen(str) < ds)
  {
    memmove(str + 1, str, strlen(str) + 1);
    *str = ' ';
  }
  gfx->println(str);
}

static inline uint32_t micros_start() __attribute__((always_inline));
static inline uint32_t micros_start()
{
  uint8_t oms = millis();
  while ((uint8_t)millis() == oms)
    ;
  return micros();
}

uint32_t testFillScreen()
{
  uint32_t start = micros_start();
  // Shortened this tedious test!
  gfx->fillScreen(WHITE);
  gfx->fillScreen(RED);
  gfx->fillScreen(GREEN);
  gfx->fillScreen(BLUE);
  gfx->fillScreen(BLACK);

  return (micros() - start) / 5;
}

uint32_t testText()
{
  uint32_t start = micros_start();
  gfx->setCursor(0, 0);
  gfx->setTextSize(1);
  gfx->setTextColor(WHITE, BLACK);
  gfx->println(F("Hello World!"));
  gfx->setTextSize(2);
  gfx->setTextColor(gfx->color565(0xff, 0x00, 0x00));
  gfx->print(F("RED "));
  gfx->setTextColor(gfx->color565(0x00, 0xff, 0x00));
  gfx->print(F("GREEN "));
  gfx->setTextColor(gfx->color565(0x00, 0x00, 0xff));
  gfx->println(F("BLUE"));
  gfx->setTextSize(tsa);
  gfx->setTextSize(3);
  gfx->setTextColor(YELLOW);
  gfx->println(1234.56);
  gfx->setTextColor(WHITE);
  gfx->println((w > 128) ? 0xDEADBEEF : 0xDEADBEE, HEX);
  gfx->setTextColor(CYAN, WHITE);
  gfx->println(F("Groop,"));
  gfx->setTextSize(tsc);
  gfx->setTextColor(MAGENTA, WHITE);
  gfx->println(F("I implore thee,"));
  gfx->setTextSize(1);
  gfx->setTextColor(NAVY, WHITE);
  gfx->println(F("my foonting turlingdromes."));
  gfx->setTextColor(DARKGREEN, WHITE);
  gfx->println(F("And hooptiously drangle me"));
  gfx->setTextColor(DARKCYAN, WHITE);
  gfx->println(F("with crinkly bindlewurdles,"));
  gfx->setTextColor(MAROON, WHITE);
  gfx->println(F("Or I will rend thee"));
  gfx->setTextColor(PURPLE, WHITE);
  gfx->println(F("in the gobberwartsb"));
  gfx->setTextColor(OLIVE, WHITE);
  gfx->println(F("with my blurglecruncheon,"));
  gfx->setTextColor(DARKGREY, WHITE);
  gfx->println(F("see if I don't!"));
  gfx->setTextColor(RED);
  gfx->setTextSize(2);
  gfx->println(F("Size 2"));
  gfx->setTextColor(ORANGE);
  gfx->setTextSize(3);
  gfx->println(F("Size 3"));
  gfx->setTextColor(YELLOW);
  gfx->setTextSize(4);
  gfx->println(F("Size 4"));
  gfx->setTextColor(GREENYELLOW);
  gfx->setTextSize(5);
  gfx->println(F("Size 5"));
  gfx->setTextColor(GREEN);
  gfx->setTextSize(6);
  gfx->println(F("Size 6"));
  gfx->setTextColor(BLUE);
  gfx->setTextSize(7);
  gfx->println(F("Size 7"));
  gfx->setTextColor(PURPLE);
  gfx->setTextSize(8);
  gfx->println(F("Size 8"));
  gfx->setTextColor(PINK);
  gfx->setTextSize(9);
  gfx->println(F("Size 9"));
  uint32_t t = micros() - start;
  return t;
}

uint32_t testPixels()
{
  uint32_t start = micros_start();

  for (uint16_t y = 0; y < h; y++)
  {
    for (uint16_t x = 0; x < w; x++)
    {
      gfx->drawPixel(x, y, gfx->color565(x << 3, y << 3, x * y));
    }
    yield(); // avoid long run triggered ESP8266 WDT restart
  }

  return micros() - start;
}

uint32_t testLines(uint16_t color)
{
  uint32_t start, t;
  int32_t x1, y1, x2, y2;

  x1 = y1 = 0;
  y2 = h - 1;

  start = micros_start();

  for (x2 = 0; x2 < w; x2 += 6)
  {
    gfx->drawLine(x1, y1, x2, y2, color);
  }

  x2 = w - 1;

  for (y2 = 0; y2 < h; y2 += 6)
  {
    gfx->drawLine(x1, y1, x2, y2, color);
  }

  t = micros() - start; // fillScreen doesn't count against timing

  x1 = w - 1;
  y1 = 0;
  y2 = h - 1;

  start = micros_start();

  for (x2 = 0; x2 < w; x2 += 6)
  {
    gfx->drawLine(x1, y1, x2, y2, color);
  }

  x2 = 0;
  for (y2 = 0; y2 < h; y2 += 6)
  {
    gfx->drawLine(x1, y1, x2, y2, color);
  }

  t += micros() - start;

  x1 = 0;
  y1 = h - 1;
  y2 = 0;

  start = micros_start();

  for (x2 = 0; x2 < w; x2 += 6)
  {
    gfx->drawLine(x1, y1, x2, y2, color);
  }
  x2 = w - 1;
  for (y2 = 0; y2 < h; y2 += 6)
  {
    gfx->drawLine(x1, y1, x2, y2, color);
  }
  t += micros() - start;

  x1 = w - 1;
  y1 = h - 1;
  y2 = 0;

  start = micros_start();

  for (x2 = 0; x2 < w; x2 += 6)
  {
    gfx->drawLine(x1, y1, x2, y2, color);
  }

  x2 = 0;
  for (y2 = 0; y2 < h; y2 += 6)
  {
    gfx->drawLine(x1, y1, x2, y2, color);
  }

  t += micros() - start;

  return t;
}

uint32_t testFastLines(uint16_t color1, uint16_t color2)
{
  uint32_t start;
  int32_t x, y;

  start = micros_start();

  for (y = 0; y < h; y += 5)
    gfx->drawFastHLine(0, y, w, color1);
  for (x = 0; x < w; x += 5)
    gfx->drawFastVLine(x, 0, h, color2);

  return micros() - start;
}

uint32_t testFilledRects(uint16_t color1, uint16_t color2)
{
  uint32_t start, t = 0;
  int32_t i, i2;

  for (i = n; i > 0; i -= 6)
  {
    i2 = i / 2;

    start = micros_start();

    gfx->fillRect(cx1 - i2, cy1 - i2, i, i, color1);

    t += micros() - start;

    // Outlines are not included in timing results
    gfx->drawRect(cx - i2, cy - i2, i, i, color2);
  }

  return t;
}

uint32_t testRects(uint16_t color)
{
  uint32_t start;
  int32_t i, i2;

  start = micros_start();
  for (i = 2; i < n; i += 6)
  {
    i2 = i / 2;
    gfx->drawRect(cx - i2, cy - i2, i, i, color);
  }

  return micros() - start;
}

uint32_t testFilledCircles(uint8_t radius, uint16_t color)
{
  uint32_t start;
  int32_t x, y, r2 = radius * 2;

  start = micros_start();

  for (x = radius; x < w; x += r2)
  {
    for (y = radius; y < h; y += r2)
    {
      gfx->fillCircle(x, y, radius, color);
    }
  }

  return micros() - start;
}

uint32_t testCircles(uint8_t radius, uint16_t color)
{
  uint32_t start;
  int32_t x, y, r2 = radius * 2;
  int32_t w1 = w + radius;
  int32_t h1 = h + radius;

  // Screen is not cleared for this one -- this is
  // intentional and does not affect the reported time.
  start = micros_start();

  for (x = 0; x < w1; x += r2)
  {
    for (y = 0; y < h1; y += r2)
    {
      gfx->drawCircle(x, y, radius, color);
    }
  }

  return micros() - start;
}

uint32_t testFillArcs()
{
  int16_t i, r = 360 / cn;
  uint32_t start = micros_start();

  for (i = 6; i < cn; i += 6)
  {
    gfx->fillArc(cx1, cy1, i, i - 3, 0, i * r, RED);
  }

  return (micros() - start) / 5;
}

uint32_t testArcs()
{
  int16_t i, r = 360 / cn;
  uint32_t start = micros_start();

  for (i = 6; i < cn; i += 6)
  {
    gfx->drawArc(cx1, cy1, i, i - 3, 0, i * r, WHITE);
  }

  return (micros() - start) / 5;
}

uint32_t testFilledTriangles()
{
  uint32_t start, t = 0;
  int32_t i;

  start = micros_start();

  for (i = cn1; i > 10; i -= 5)
  {
    start = micros_start();
    gfx->fillTriangle(cx1, cy1 - i, cx1 - i, cy1 + i, cx1 + i, cy1 + i,
                      gfx->color565(0, i, i));
    t += micros() - start;
    gfx->drawTriangle(cx1, cy1 - i, cx1 - i, cy1 + i, cx1 + i, cy1 + i,
                      gfx->color565(i, i, 0));
  }

  return t;
}

uint32_t testTriangles()
{
  uint32_t start;
  int32_t i;

  start = micros_start();

  for (i = 0; i < cn; i += 5)
  {
    gfx->drawTriangle(
        cx1, cy1 - i,     // peak
        cx1 - i, cy1 + i, // bottom left
        cx1 + i, cy1 + i, // bottom right
        gfx->color565(0, 0, i));
  }

  return micros() - start;
}

uint32_t testFilledRoundRects()
{
  uint32_t start;
  int32_t i, i2;

  start = micros_start();

  for (i = n1; i > 20; i -= 6)
  {
    i2 = i / 2;
    gfx->fillRoundRect(cx1 - i2, cy1 - i2, i, i, i / 8, gfx->color565(0, i, 0));
  }

  return micros() - start;
}

uint32_t testRoundRects()
{
  uint32_t start;
  int32_t i, i2;

  start = micros_start();

  for (i = 0; i < n1; i += 6)
  {
    i2 = i / 2;
    gfx->drawRoundRect(cx1 - i2, cy1 - i2, i, i, i / 8, gfx->color565(i, 0, 0));
  }

  return micros() - start;
}

/***************************************************
  Original sketch text:

  This is an example sketch for the Adafruit 2.2" SPI display.
  This library works with the Adafruit 2.2" TFT Breakout w/SD card
  ----> http://www.adafruit.com/products/1480

  Check out the links above for our tutorials and wiring diagrams
  These displays use SPI to communicate, 4 or 5 pins are required to
  interface (RST is optional)
  Adafruit invests time and resources providing this open source code,
  please support Adafruit and open-source hardware by purchasing
  products from Adafruit!

  Written by Limor Fried/Ladyada for Adafruit Industries.
  MIT license, all text above must be included in any redistribution
 ****************************************************/
