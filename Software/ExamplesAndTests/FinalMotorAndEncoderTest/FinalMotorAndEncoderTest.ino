// Created Libraries for this project
#include <CEWBEncoder.h>
#include <CEWBFormatting.h>

// Variables for the motor
uint8_t PWMVal;
const int maxVolt = 24;

// Creating the Encoder and the variables for the formatting
CEWBEncoder mainEncoder(2, 3);
parsedData motorVals;

// Functions for the voltage to PWM and back stuff
uint8_t voltToPWM(float);
float PWMToVolt(uint8_t);

void setup() {
  // put your setup code here, to run once:

}

void loop() {
  // put your main code here, to run repeatedly:

}

// Function to turn the desired voltage value to PWM
uint8_t voltToPWM(float voltage) {
  return floor(((voltage)/maxVolt) * 255);
}

// Function to turn the PWM value into a voltage
float PWMToVolt(uint8_t PWM) {
  return ((PWM * maxVolt)/255);
}