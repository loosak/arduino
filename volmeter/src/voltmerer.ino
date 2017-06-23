#include <Arduino.h>
#include <SPI.h>
#include <Wire.h>
#include <U8g2lib.h>    // https://github.com/olikraus/u8g2
#include <SFE_BMP180.h> // https://github.com/LowPowerLab/SFE_BMP180


// SPI HW
// U8X8_SH1106_128X64_NONAME_4W_HW_SPI(uint8_t cs, uint8_t dc, uint8_t reset = U8X8_PIN_NONE)
U8X8_SH1106_128X64_NONAME_4W_HW_SPI u8x8(10, 9, 8);

SFE_BMP180 bmp;
#define ALTITUDE 230.0 // Altitude of Erbenova, Brno in meters

// https://provideyourown.com/2012/secret-arduino-voltmeter-measure-battery-voltage/
long readVcc() {
  // Read 1.1V reference against AVcc
  // set the reference to Vcc and the measurement to the internal 1.1V reference
  #if defined(__AVR_ATmega32U4__) || defined(__AVR_ATmega1280__) || defined(__AVR_ATmega2560__)
    ADMUX = _BV(REFS0) | _BV(MUX4) | _BV(MUX3) | _BV(MUX2) | _BV(MUX1);
  #elif defined (__AVR_ATtiny24__) || defined(__AVR_ATtiny44__) || defined(__AVR_ATtiny84__)
     ADMUX = _BV(MUX5) | _BV(MUX0) ;
  #else
    ADMUX = _BV(REFS0) | _BV(MUX3) | _BV(MUX2) | _BV(MUX1);
  #endif

  delay(2); // Wait for Vref to settle
  ADCSRA |= _BV(ADSC); // Start conversion
  while (bit_is_set(ADCSRA,ADSC)); // measuring

  uint8_t low  = ADCL; // must read ADCL first - it then locks ADCH
  uint8_t high = ADCH; // unlocks both

  long result = (high<<8) | low;

  result = 1125300L / result; // Calculate Vcc (in mV); 1125300 = 1.1*1023*1000
  return result; // Vcc in millivolts
}

void setup() {
	Serial.begin(9600);
  bmp.begin();
	u8x8.begin();
	u8x8.setFont(u8x8_font_pcsenior_f);
}

void loop() {
	//Serial.println( readVcc(), DEC );
	u8x8.drawString(0, 0, "Time:");
	char pbuff[20], str[6];
	snprintf ( pbuff, 20, "AVcc:   %4ld mV", readVcc() );
	//ltoa(readVcc(), pbuff, 10);
	u8x8.drawString(0, 1, pbuff);

  delay(1000);

  double T,P;
  char status = bmp.startTemperature();

  if (status != 0) {
    delay(status);
    status = bmp.getTemperature(T);
    if (status != 0)  {                    // Print out the measurement:
      memset(str, 0, 6);                   // mandatory reinitialize str
      dtostrf(T, 4, 2, &str[strlen(str)]); //4 is mininum width, 2 is precision; float value is copied onto str
      snprintf ( pbuff, 20, "Temp: %6s *C", str );
      u8x8.drawString(0, 2, pbuff);
    }
  }

  status = bmp.startPressure(3);
  if (status != 0)  {
    delay(status);
    status = bmp.getPressure(P, T);
    if (status != 0)  {                    // Print out the measurement:
      memset(str, 0, 6);                   // mandatory reinitialize str
      dtostrf(P, 5, 2, &str[strlen(str)]); //4 is mininum width, 2 is precision; float value is copied onto str
      snprintf ( pbuff, 20, "Pres: %6s mB", str );
      u8x8.drawString(0, 3, pbuff);
    }
  }
}
