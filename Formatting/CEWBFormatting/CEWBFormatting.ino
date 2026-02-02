#include "CEWBFormatting.h"

// creating the struct of the individual variables that make up the info char array
parsedData results;

bool done = false;

// The size of the char array buffer for which will tell how big the formatted info will be
const int bufferSize = 50;

// The complete information format that needs to be dissected (and the temp info variable)
char info[bufferSize] = "$V12.3220AD044.8745;P009.0000I000.1000D003.5000!";

void setup() {
  // put your setup code here, to run once:
  Serial.begin(9600);
  Serial.println("=================================");
}


void loop() {
  // 'done' variable makes sure the code only runs once
  while (!done) {
    // Checking the formatting and values of the info char array to be parsed
    parseFormat(info, results.infoType, results.angleType, results.voltage, 
                results.angle, results.Kp, results.Ki, results.Kd, results.errMsg);
  
    // Print all of the struct variable values
    Serial.print("Info Type: ");
    Serial.println(results.infoType);
    Serial.print("Voltage: ");
    Serial.println(results.voltage, 4);
    Serial.print("Angle Type: ");
    Serial.println(results.angleType);
    Serial.print("Angle: ");
    Serial.println(results.angle, 4);
    Serial.print("Kp: ");
    Serial.println(results.Kp, 4);
    Serial.print("Ki: ");
    Serial.println(results.Ki, 4);
    Serial.print("Kd: ");
    Serial.println(results.Kd, 4);
    Serial.print("Error Message: ");
    Serial.println(results.errMsg);

    done = true;
  }

}