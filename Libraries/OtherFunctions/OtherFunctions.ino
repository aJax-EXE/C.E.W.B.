#include "CEWBFunctions.h"

float startVolt = 13.4;
// Using this value, the PWM value should be 143
// And the voltage value given back should be 13.4588

void setup() {
  Serial.begin(9600);
}

void loop() {
  float voltage;
  uint8_t PWMVal;

  PWMVal = voltToPWM(startVolt);
  Serial.print("PWM Val: ");
  Serial.println(PWMVal);

  voltage = PWMToVolt(PWMVal);
  Serial.print("Voltage: ");
  Serial.println(voltage, 4);
}