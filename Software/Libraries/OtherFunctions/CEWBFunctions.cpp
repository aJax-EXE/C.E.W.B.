#include "CEWBFunctions.h"

// The max voltage value that the motor can go to
const int maxVolt = 24;

// Function to turn the desired voltage value to PWM
uint8_t voltToPWM(float voltage) {
  if (voltage >= maxVolt) return 255;
  if (voltage <= 0) return 0;
  return (uint8_t)floor((voltage / maxVolt) * 255);
}

// Function to turn the PWM value into a voltage
float PWMToVolt(uint8_t PWM) {
  return ((PWM * maxVolt)/255);
}

// Function to format the Voltage from sXXXXXX to sXX.XXXX
float makeVoltage(long value) {
  return float(value / 10000.0);
}


