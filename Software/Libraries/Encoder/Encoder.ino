#include "CEWBEncoder.h"

CEWBEncoder mainEncoder(2, 3);
CEWBEncoder otherEncoder(4, 5);

void setup() {
  Serial.begin(9600);
}

void loop() {
  long count1 = mainEncoder.getCount();
  long count2 = otherEncoder.getCount();
  int direction1 = mainEncoder.getDirection();
  int direction2 = otherEncoder.getDirection();

  // Convert count to angle (degrees)
  float angle1 = (count1 * 360.0) / 2048.0;
  float angle2 = (count2 * 360.0) / 2048.0;
  // Convert count to radians
  float rads1 = (count1 * 2 * PI) / 2048.0;
  float rads2 = (count2 * 2 * PI) / 2048.0;

  Serial.print("1. Count: ");
  Serial.print(count1);
  Serial.print(" | Angle: ");
  Serial.print(angle1, 4);
  Serial.print(" deg | Direction: ");
  Serial.println(direction1 == 1 ? "CW" : "CCW");

  Serial.print("2. Count: ");
  Serial.print(count2);
  Serial.print(" | Angle: ");
  Serial.print(angle2, 4);
  Serial.print(" deg | Direction: ");
  Serial.println(direction2 == 1 ? "CW" : "CCW");
  Serial.println("===========================================");

  // delay(100);
}