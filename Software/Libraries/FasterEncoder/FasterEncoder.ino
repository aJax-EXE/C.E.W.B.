#include "NewCEWBEncoder.h"

NewCEWBEncoder enc1(2, 3);      // PORTD
NewCEWBEncoder enc2(10, 12);    // PORTB

void setup() {
  Serial.begin(115200);
  enc1.begin();
  enc2.begin();
}

void loop() {

  static unsigned long t = 0;

  if (millis() - t > 100) {

    Serial.print("E1: ");
    Serial.print(enc1.getCount());

    Serial.print("  E2: ");
    Serial.println(enc2.getCount());

    t = millis();
  }
}