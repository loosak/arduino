/* Blink without Delay

  Turns on and off a light emitting diode(LED) connected to a digital
  pin, without using the delay() function.  This means that other code
  can run at the same time without being interrupted by the LED code.

  In MSP430 Launchpad, there is already an LED on the board that's
  attached to pin 14, so no hardware is needed for this example.

  created 2005
  by David A. Mellis
  modified 8 Feb 2010
  by Paul Stoffregen
  modified 27 Apr 2012
  by Robert Wessels
*/

// Pin 14 has an LED connected on MSP430 boards, has a name 'GREEN_LED' in the code.

// constants won't change. Used here to
// set pin numbers:
const int ledPin =  GREEN_LED;      // the number of the LED pin
//const int ledPin =  RED_LED;      // the number of the LED pin

// Variables will change:
int ledState = LOW;             // ledState used to set the LED
long previousMillis = 0;        // will store last time LED was updated

// the follow variables is a long because the time, measured in miliseconds, will quickly become a bigger
// number than can be stored in an int.
long interval = 1000;           // interval at which to blink (milliseconds)

void setup() {
  // set the digital pin as output:
  pinMode(ledPin, OUTPUT);
  Serial.begin(9600);
  Serial.print("A");
}

void loop()
{
  // here is where you'd put code that needs to be running all the time.
 // check to see if it's time to blink the LED; that is, if the
  // difference between the current time and last time you blinked
  // the LED is bigger than the interval at which you want to
  // blink the LED.
  unsigned long currentMillis = millis();

  if(currentMillis - previousMillis > interval) {
    // save the last time you blinked the LED
    Serial.print(currentMillis);
    previousMillis = currentMillis;
    // if the LED is off turn it on and vice-versa:
    if (ledState == LOW)
      ledState = HIGH;
    else
      ledState = LOW;

    // set the LED with the ledState of the variable:
    digitalWrite(ledPin, ledState);
  }
}
