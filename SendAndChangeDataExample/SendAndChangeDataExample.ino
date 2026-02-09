int motorValue = 0;

void setup() {
  Serial.begin(9600);
}

void loop() {

  if (Serial.available() > 0) {
    motorValue = Serial.parseInt();
    while (Serial.available()) Serial.read();  // clear buffer
  }

  Serial.print("Motor=");
  Serial.println(motorValue);

  delay(100);
}

