#ifndef CEWBFORMATTING_H
#define CEWBFORMATTING_H

#include <Arduino.h>

void parseFormat(String infoForm, char& infoLabel, char& angleType, float& voltage, float& angle, float& Kp, float& Ki, float& Kd);
String formatInfo(char infoLabel, char angleType, float voltage, float angle, float Kp, float Ki, float Kd);

#endif