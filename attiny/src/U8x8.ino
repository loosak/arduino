#include <Arduino.h>
#include <U8g2lib.h>
//#include <Wire.h>
#include <TinyWireM.h>

/*
All pins can be used as Digital I/O
Pin 0 → I2C SDA, PWM (LED on Model B)
Pin 1 → PWM (LED on Model A)
Pin 2 → I2C SCK, Analog In
Pin 3 → Analog In (also used for USB+ when USB is in use)
Pin 4 → PWM, Analog (also used for USB- when USB is in use)
Pin 5 → Analog In
*/

//D1(GPIO5)->SDA
//D2(GPIO4)->SCL

//oled with SSD1306 controller
//U8X8_SSD1306_128X64_NONAME_SW_I2C(clock, data [, reset])
U8X8_SSD1306_128X64_NONAME_SW_I2C u8x8(2, 0);

void setup(void) {
	//Wire.begin(/*SDA*/ 0 , /*SCL*/ 2);
	TinyWireM.begin();                    // initialize I2C lib
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
}
