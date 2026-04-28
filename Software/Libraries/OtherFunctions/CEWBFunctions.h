#ifndef CEWBFUNCTIONS_H
#define CEWBFUNCTIONS_H
#include <Arduino.h>

uint8_t voltToPWM(float voltage);
float PWMToVolt(uint8_t PWM);
float makeVoltage(long value);

#endif
