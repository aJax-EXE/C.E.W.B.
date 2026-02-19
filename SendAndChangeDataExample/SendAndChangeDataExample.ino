int value = 0;

void setup() {
  Serial.begin(9600);
}

void loop() {

  if (Serial.available() > 0) {
    value = Serial.parseInt();
    while (Serial.available()) Serial.read();  // clear buffer
  }

  Serial.print("Value = ");
  Serial.println(value);

  delay(100);
}

