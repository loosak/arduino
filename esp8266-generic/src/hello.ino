const long interval = 1000;
unsigned long previousMillis = 0;

void setup() {
	Serial.begin(115200);
  Serial.println("Hello World!");
}

void printTime() {

		char buff[10];
	  uint16_t s = millis() / 1000;
	  uint16_t m = s / 60;
	  uint8_t h = m / 60;

		snprintf(buff, 10, "%02d:%02d:%02d", h, m % 60, s % 60);
		printf("%s", buff);
}

void loop() {
  unsigned long currentMillis = millis();

  // if enough millis have elapsed
  if (currentMillis - previousMillis >= interval) {
    previousMillis = currentMillis;
		printTime();
		Serial.printf(" FlashChipSize %u \n", currentMillis, ESP.getFlashChipSize() / 1024);
  }
}
