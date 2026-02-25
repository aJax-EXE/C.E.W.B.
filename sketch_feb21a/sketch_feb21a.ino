#include <NewCEWBEncoder.h>

NewCEWBEncoder mainEncoder(2, 3);
const int IN1 = 5, IN2 = 6;

int maxCount = 10000, val = 60;
bool forward = false;

void setup() {
  // put your setup code here, to run once:
  Serial.begin(115200);

  pinMode(IN1, OUTPUT);
  pinMode(IN2, OUTPUT);

  // Spin motor forward
  analogWrite(IN1, val);
  digitalWrite(IN2, LOW);

}

void loop() {
  // put your main code here, to run repeatedly:
  long count1 = mainEncoder.getCount();
  Serial.print("Count: ");
  Serial.println(count1);

  if(abs(count1) >= maxCount) {
    forward = !forward;
    // mainEncoder.reset();
    count1 = 0;
  }
  
    // --- Motor Drive ---
  if (forward) {
    digitalWrite(IN1, HIGH);
    digitalWrite(IN2, LOW);
    analogWrite(IN1, val);
  } else {
    digitalWrite(IN1, LOW);
    digitalWrite(IN2, HIGH);
    analogWrite(IN2, val);
  }


}
