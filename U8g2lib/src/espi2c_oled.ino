/*
https://www.wemos.cc/product/oled-shield.html
D1	SCL
D2	SDA
64x48 pixels (0.66” Across)
Driver IC: SSD1306
IIC Address: 0x3C or 0x3D
*/

#include <Arduino.h>
#include <SPI.h>
#include <U8x8lib.h>
#include <Wire.h>
#include <Streaming.h>

#include "DHT.h" //Adafruit DHT-sensor-library  https://github.com/adafruit/DHT-sensor-library
#define DHTPIN  D4  	//https://www.wemos.cc/product/dht-pro-shield.html
#define DHTTYPE DHT22 //DHT11, DHT21, DHT22
DHT dht(DHTPIN, DHTTYPE, 15);

//SW I2C
//U8X8_SSD1306_64X48_ER_SW_I2C u8x8(/* clock=*/ D1, /* data=*/ D2 /* [, reset]*/);
//HW I2C
//U8X8_SSD1306_64X48_ER_HW_I2C u8x8;
//U8X8_SSD1306_128X64_NONAME_HW_I2C([reset [, clock, data]])
U8X8_SSD1306_128X64_NONAME_HW_I2C u8x8;

void setup(void) {
	Serial.begin(115200);
  dht.begin();

  u8x8.begin();
	u8x8.setFont(u8x8_font_victoriabold8_r);
  u8x8.drawGlyph(u8x8.getCols()-1, u8x8.getRows()-1, 'B');

	for (size_t i = 0; i < u8x8.getRows(); i++) {
		Serial.printf("%i\n",i);
		u8x8.drawString(0,i, String(i).c_str());
	}

}

void upTime(int sec, char* retStr, int bufferSize) {
	int min = sec / 60;
	int hr = min / 60;
	snprintf( retStr , bufferSize, "%02d:%02d:%02d", hr, min % 60, sec % 60 );
	retStr[bufferSize-1] = '\0'; // ensure a valid terminating zero!
}

void loop(void) {

	float h = dht.readHumidity();     //Luftfeuchte auslesen
	float t = dht.readTemperature();  //Temperatur auslesen

	// Print time
	char buffer[16]; // caller-defined buffers: 16 bytes allocated here on the stack.
	char str[6];
	int sec = millis() / 1000;
	upTime(sec, buffer, sizeof(buffer));
	Serial << "Buffer: " << buffer << "\t";
	u8x8.drawString(0,0,buffer);


	if (isnan(t) || isnan(h))  {
		Serial.println("DHT22 konnte nicht ausgelesen werden");
	}
	else {
		Serial << "Luftfeuchte: " << h << "%\t" << "Temperatur: " << t << "°C" << endl;
		char str[6]=""; //nebo strcpy(str, "");
		dtostrf(t, 4, 2, &str[strlen(str)]); //4 is mininum width, 2 is precision; float value is copied onto str
		//Serial.println(str);
		snprintf( buffer , sizeof(buffer), "%s*C", str );
		//Serial.println( buffer );
		u8x8.drawString(0,3,buffer);

	  memset(str, 0, 6); // mandatory reinitialize str
		dtostrf(h, 4, 2, &str[strlen(str)]); //4 is mininum width, 2 is precision; float value is copied onto str
		snprintf( buffer , sizeof(buffer), "%s\%", str );
		//Serial.println(buffer);
		u8x8.drawString(0,4,buffer);
	}

	//u8x8.drawString(0,2,"Hello___");
	//u8x8.drawString(0,3,"__World!");

  delay(2000);
}
