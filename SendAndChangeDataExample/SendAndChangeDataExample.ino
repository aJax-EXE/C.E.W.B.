int value = 0;

void setup() {
  Serial.begin(9600);
}

void loop() {

  // Receive value from PC
  if (Serial.available()) {
    value = Serial.parseInt();
  }

  // Send telemetry back
  Serial.print("Value=");
  Serial.println(value);

  delay(100);
}
