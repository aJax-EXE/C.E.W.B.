#ifndef CEWBFUNCTIONS_H
#define CEWBFUNCTIONS_H
#include <Arduino.h>

// Function to turn the desired voltage value to PWM
uint8_t voltToPWM(float voltage);

// Function to turn the PWM value into a voltage
float PWMToVolt(uint8_t PWM);

// Function to format the Voltage from sXXXXXX to sXX.XXXX
float makeVoltage(long value);

#endif
