#include <Arduino.h>
#include <U8g2lib.h>
#include <SPI.h>
#include <Wire.h>

/*
The constructor has the following template:
U8G2_SH1106_128X64_NONAME_1_4W_SW_SPI(rotation, clock, data, cs, dc [, reset])
U8G2_SH1106_128X64_NONAME_1_4W_SW_SPI u8g2(U8G2_R0, 13, 11, 10, 9, 8);  //D0=13, D1=11, CS=10, DC=9, Reset=8
*/

// U8x8 character mode
// SPI SW
// U8X8_SH1106_128X64_NONAME_4W_SW_SPI(clock, data, cs, dc [, reset])
// U8X8_SH1106_128X64_NONAME_4W_SW_SPI u8x8(13, 11, 10, 9, 8);  //D0=13, D1=11, CS=10, DC=9, Reset=8

// SPI HW
// U8X8_SH1106_128X64_NONAME_4W_HW_SPI(uint8_t cs, uint8_t dc, uint8_t reset = U8X8_PIN_NONE)
U8X8_SH1106_128X64_NONAME_4W_HW_SPI u8x8(10, 9, 8);


void setup(void) {
  u8x8.begin();
}

void loop(void) {
  int sec = millis() / 1000;
  int min = sec / 60;
  int hr = min / 60;
  char pbuff[20];
  snprintf ( pbuff, 20, "%02d:%02d:%02d", hr, min % 60, sec % 60 );

  //u8x8.setFont(u8x8_font_amstrad_cpc_extended_r);
  u8x8.setFont(u8x8_font_pcsenior_f);
  u8x8.drawString(0,1,pbuff);

  u8x8.setInverseFont(1);
  u8x8.drawString(2, 2, "Inverse");
  u8x8.setInverseFont(0);
  u8x8.drawString(2, 3, "Normal");
}

/*
void setup(void) {
  u8g2.begin();
}

void loop(void) {
  u8g2.firstPage();
  do {
    u8g2.setFont(u8g2_font_ncenB10_tr);
    u8g2.drawStr(0,24,"Hello World!");
    u8g2.drawStr(0,48,"Hello World!");
  } while ( u8g2.nextPage() );
}
*/
