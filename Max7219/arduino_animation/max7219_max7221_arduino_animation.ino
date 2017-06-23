#include <SPI.h>

uint8_t animation [22] = {
  0b01000000,
  0b01100000,
  0b01100001,
  0b01100101,
  0b01101101,
  0b01111101,
  0b01111101,
  0b01111111,
  0b01111111,
  0b01111111,
  0b01111111,
  0b00111111,
  0b00011111,
  0b00011111,
  0b00011011,
  0b00010011,
  0b00000011,
  0b00000010,
  0b00000000,
  0b00000000,
  0b00000000,
  0b00000000
};

void setup() {

  // We will need to control the Load/CS pin so
  // make a pin an output, for this example I'll
  // use pin 7.
  pinMode(7, OUTPUT);

  // The Maxim chip need the most siginifcant
  // bit first.
  SPI.setBitOrder(MSBFIRST);

  // Make sure that the chip is turned on
  maxTransfer(0x0C, 0x01);

  // Make sure decode mode is off
  maxTransfer(0x09, 0x00);

}

void loop() {

  for (int i = 0; i < 22; ++i) {
    maxTransfer(0x01, animation[i]);

    delay(75);
  }

}

void maxTransfer(uint8_t reg, uint8_t data)
{

  // Make sure that the Load/CS pin is low
  digitalWrite(7, LOW);

  // Open SPI
  SPI.begin();

  // The Maxim chip wants the register's
  // address first
  SPI.transfer(reg);

  // Next send the data
  SPI.transfer(data);

  // End SPI
  SPI.end();

  // The Maxim chip loads the data when the
  // Load/CS pin switches from low to high
  digitalWrite(7, HIGH);

}
