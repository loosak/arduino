//#include <Wire.h>
#include <TinyWireM.h>
#include <TinyOzOLED.h>
#include <tinyBMP085.h>
/*
P0 SDA (I2C data)
P2 SCK (I2C clock)
*/

tinyBMP085 bmp;

uint32_t targetTime = 0;   // for next 1 second timeout
uint8_t  hh,mm,ss;         //containers for current time

static uint8_t conv2d(const char* p) {
    uint8_t v = 0;
    if ('0' <= *p && *p <= '9') v = *p - '0';
    return 10 * v + *++p - '0';
}

void setup() {
    OzOled.init();                      //initialze Oscar OLED display
    OzOled.setNormalDisplay();          //Set display to normal mode (i.e non-inverse mode)
    OzOled.setPageMode();               //Set addressing mode to Page Mode
    OzOled.clearDisplay();

    OzOled.printString(__TIME__, 4, 0);
    hh = conv2d(__TIME__);
    mm = conv2d(__TIME__+3);
    ss = conv2d(__TIME__+6);
    targetTime = millis() + 1000;

    bmp.begin();
}

void loop() {

    if (targetTime < millis()) {
        targetTime = millis()+1000;
        ss++;
        if (ss == 60) {
          ss = 0;
          mm++;
          if(mm > 59) {
            mm = 0;
            hh++;
            if (hh > 23) hh = 0;
          }
        }
      char pbuff[16];
      snprintf ( pbuff, 16, "T: %02d:%02d:%02d", hh, mm , ss);
      OzOled.printString(pbuff, 1, 1);

      //int pres = bmp.readPressure();
      //int temp = bmp.readTemperature10C();
      //float temp = bmp.readTemperature();
      //int alt = bmp.readAltitude();

      //OzOled.printNumber( temp, 2, 6, 4);
      //int altMM = bmp.readAltitudemm();
      //int altSTDdm = bmp.readAltitudeSTDdm();
   }

}
