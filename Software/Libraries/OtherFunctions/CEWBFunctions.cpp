#include "CEWBFunctions.h"

// The max voltage value that the motor can go to
const int maxVolt = 24;

// Function to turn the desired voltage value to PWM
uint8_t voltToPWM(float voltage) {
  volt = floor(((voltage)/maxVolt) * 255);
  
  if(volt >= maxVolt) { return 255; } 
  else { return volt; }
}

// Function to turn the PWM value into a voltage
float PWMToVolt(uint8_t PWM) {
  return ((PWM * maxVolt)/255);
}