#include <Wire.h>
#include <LiquidCrystal_I2C.h>  // https://bitbucket.org/fmalpartida/new-liquidcrystal/wiki/schematics#!dfrobot

//this is for dfrobot i2c LCD board
//                    addr, en,rw,rs,d4,d5,d6,d7,bl,blpol
LiquidCrystal_I2C lcd(0x27, 2, 1, 0, 4, 5, 6, 7, 3, POSITIVE);  // Set the LCD I2C address

// Creat a set of new characters
const uint8_t charBitmap[][8] = {
   { 0xc, 0x12, 0x12, 0xc, 0, 0, 0, 0 },
   { 0x6, 0x9, 0x9, 0x6, 0, 0, 0, 0 },
   { 0x0, 0x6, 0x9, 0x9, 0x6, 0, 0, 0x0 },
   { 0x0, 0xc, 0x12, 0x12, 0xc, 0, 0, 0x0 },
   { 0x0, 0x0, 0xc, 0x12, 0x12, 0xc, 0, 0x0 },
   { 0x0, 0x0, 0x6, 0x9, 0x9, 0x6, 0, 0x0 },
   { 0x0, 0x0, 0x0, 0x6, 0x9, 0x9, 0x6, 0x0 },
   { 0x0, 0x0, 0x0, 0xc, 0x12, 0x12, 0xc, 0x0 }

};

void setup() {
  int charBitmapSize = (sizeof(charBitmap ) / sizeof (charBitmap[0]));
  for ( int i = 0; i < charBitmapSize; i++ ) {
    lcd.createChar ( i, (uint8_t *)charBitmap[i] );
  }

  lcd.begin(16,2);               // initialize the lcd

  lcd.home ();                   // go home
  lcd.print("Hello, ARDUINO ");
  lcd.setCursor ( 0, 1 );        // go to the next line
  for ( int i = 0; i < 7; i++ )
    lcd.print (char(i));
  //delay ( 8000 );
}

void loop() {
  int sec = millis() / 1000;
  int min = sec / 60;
  int hr = min / 60;
  char pbuff[20];
  snprintf ( pbuff, 20, "%02d:%02d:%02d", hr, min % 60, sec % 60 );

  lcd.setCursor ( 8, 1 );
  lcd.print(pbuff);
  delay(1000);
}
