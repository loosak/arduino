void setup() {
  pinMode(13, OUTPUT);
	Serial.begin(115200);
}

void loop() {
  digitalWrite(13, HIGH);
	Serial.println(millis());
  delay(10000);
  digitalWrite(13, LOW);
	Serial.println(millis());
  delay(10000);
}
