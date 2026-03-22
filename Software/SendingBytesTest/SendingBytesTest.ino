float volt = 12.7654;

void setup() {
  Serial.begin(115200);
  // Serial.println("Beginning");
}

void loop() {
  Serial.write(12);
}
