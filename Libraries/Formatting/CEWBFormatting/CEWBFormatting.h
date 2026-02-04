#ifndef CEWBFORMATTING_H
#define CEWBFORMATTING_H

#include <Arduino.h>

// Variables used by both the parsing and formatting functions
  // Struct of the different type of variables that will be given the information from the char array
struct parsedData {
  // The first value should indicate if the info is the Master/Setting form '$' or the Feedback form '#'
  char infoType;

  // Voltage should have the form XX.XXXX
  float voltage;

  // The angle type either be a 'D' for degrees or 'R' for radians
  char angleType;

  // Angle, regardless of the type, should have the form XXX.XXXX
  float angle;

  // The Kp, Ki, and Kd values should have the form XXX.XXXX
  float Kp;
  float Ki;
  float Kd;

  // Array that will hold an error message to tell the user what is wrong
  // Will deal with the error code messages
  char errMsg[100];
};

// Function that will run as the instant fail state for any error to keep the output of the variables going
static void failState(char* info, char& infoType, char& angleType, float& voltage, 
                 float& angle, float& Kp, float& Ki, float& Kd, char* errMsg);

void parseFormat(char* info, char& infoType, char& angleType, float& voltage, 
                 float& angle, float& Kp, float& Ki, float& Kd, char* errMsg);


void formatInfo(char* info, char infoType, char angleType, float voltage, 
                float angle, float Kp, float Ki, float Kd, char* errMsg);

#endif