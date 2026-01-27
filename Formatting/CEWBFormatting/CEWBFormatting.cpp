#include "CEWBFormatting.h"

// Function to break the info up into its smaller components
void parseFormat(String infoForm, char& infoLabel, char& angleType, float& voltage, float& angle, float& Kp, float& Ki, float& Kd); {

}

// Function to put all of the info together to be sent off
String formatInfo(char infoLabel, char angleType, float voltage, float angle, float Kp, float Ki, float Kd);