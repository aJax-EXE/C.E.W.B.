#include "CEWBFunctions.h"

float startVolt = 15.4;
// Using this value, the PWM value should be 143
// And the voltage value given back should be 13.4588

void setup() {
  Serial.begin(9600);
  Serial.println("Start Conversions Now!");
}

void loop() {
  float voltage, value;
  uint8_t PWMVal;
  long num = 123456;

  PWMVal = voltToPWM(startVolt);
  Serial.print("PWM Val: ");
  Serial.println(PWMVal);

  voltage = PWMToVolt(PWMVal);
  Serial.print("Voltage: ");
  Serial.println(voltage, 4);

  value = makeVoltage(num);
  Serial.print("Voltage: ");
  Serial.println(value, 4);
}
