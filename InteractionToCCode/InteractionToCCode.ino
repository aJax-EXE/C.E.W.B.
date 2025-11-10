void setup() {
  // put your setup code here, to run once:
  Serial.begin(9600);
  while (!Serial);
  Serial.println("Arduino Ready");

}

void loop() {
  // put your main code here, to run repeatedly:
  Serial.println("Hello from Arduino!|");
  delay(1000);

}
