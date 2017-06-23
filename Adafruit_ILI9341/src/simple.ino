#include "SPI.h"
#include "Adafruit_GFX.h"
#include "Adafruit_ILI9341.h"
#include "ArialRoundedMTBold_36.h"
#include "ArialRoundedMTBold_14.h"

/*
TFT LCD Pin CS  to GPIO_5   D1
TFT LCD Pin RST to RST
TFT LCD Pin DC to GPIO_2    D4
TFT LCD Pin MOSI to GPIO_13 D7
TFT LCD Pin CLK to GPIO_14  D5
TFT LCD Pin LED to +3.3 V.
*/

// Pins for the ILI9341
#define TFT_DC D4
#define TFT_CS D1
#define LED_PIN D8
#define TFT_MOSI D7
#define TFT_CLK D5

Adafruit_ILI9341 tft = Adafruit_ILI9341(TFT_CS, TFT_DC, LED_PIN);

void setup() {
    Serial.begin(115200);
    tft.begin();
    tft.setRotation(2);
    tft.fillScreen(ILI9341_BLACK);
    tft.setFont(&ArialRoundedMTBold_14);
}


void loop(void) {
    int sec = millis() / 1000;
    int min = sec / 60;
    int hr = min / 60;
    char pbuff[20];

    tft.setTextColor(ILI9341_YELLOW); tft.setTextSize(2);
    tft.setCursor(0, 24);
    tft.printf("%02d:%02d:%02d", hr, min % 60, sec % 60);
    delay(1000);
  /*
    tft.setCursor(0, 24);
    tft.printf("%8s");
  */
    tft.fillScreen(ILI9341_BLACK);
}

unsigned long diagnostics() {
    // read diagnostics (optional but can help debug problems)
    uint8_t x = tft.readcommand8(ILI9341_RDMODE);
    Serial.print("Display Power Mode: 0x"); Serial.println(x, HEX);
    x = tft.readcommand8(ILI9341_RDMADCTL);
    Serial.print("MADCTL Mode: 0x"); Serial.println(x, HEX);
    x = tft.readcommand8(ILI9341_RDPIXFMT);
    Serial.print("Pixel Format: 0x"); Serial.println(x, HEX);
    x = tft.readcommand8(ILI9341_RDIMGFMT);
    Serial.print("Image Format: 0x"); Serial.println(x, HEX);
    x = tft.readcommand8(ILI9341_RDSELFDIAG);
    Serial.print("Self Diagnostic: 0x"); Serial.println(x, HEX);
    Serial.println(F("Done!"));
}

unsigned long testText() {

  unsigned long start = micros();

  tft.fillScreen(ILI9341_BLACK);
  tft.setCursor(0, 12);
  tft.setTextColor(ILI9341_WHITE);  tft.setTextSize(1);
  tft.println("Hello World!");
  tft.setCursor(0, 12 + 24);
  tft.setTextColor(ILI9341_YELLOW); tft.setTextSize(2);
  tft.println(start);
  tft.setTextColor(ILI9341_RED);    tft.setTextSize(3);
  tft.println(0xDEADBEEF, HEX);
  tft.setTextColor(ILI9341_GREEN); tft.setTextSize(1);
  tft.println();
  tft.println(F("PWM stands for Pulse Width Modulation. A method for generating analog signal using a digital source. Two main components define it’s behavior: frequency and duty cycle.  The duty cycle determines how long the pin will output HIGH in a cycle and the frequency determines the speed of repeating that cycle, and therefore how fast it switches between high and low states. By cycling a digital signal off and on at a fast enough rate, and with a certain duty cycle, the output will appear to behave like a constant voltage analog signal when providing power to devices"));

  return micros() - start;
}
