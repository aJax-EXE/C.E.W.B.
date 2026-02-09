#include <iostream>
#include <cstring>
#include "CEWBFormatting.h"

using namespace std;

int main() {
    // creating the struct of the individual variables that make up the info char array
    parsedData results;

    // The size of the char array buffer for which will tell how big the formatted info will be
    const int bufferSize = 50;

    // The complete information format that needs to be dissected (and the temp info variable)
    char startInfo[bufferSize] = "$V12.3220AD044.8745;P009.0000I000.1000D003.5000!";
    char finalInfo[bufferSize] = " ";

    cout << "=================================" << endl;
    // Printing the initial information as it is
    printf("Initial Info State: %s\n", startInfo);

    // Checking the formatting and values of the info char array to be parsed
    parseFormat(startInfo, results.infoType, results.angleType, results.voltage, 
                results.angle, results.Kp, results.Ki, results.Kd, results.errMsg);
  
    // Print all of the struct variable values
    printf("Info Type: %c\n", results.infoType);
    printf("Voltage: %.4f\n", results.voltage);
    printf("Angle Type: %c\n", results.angleType);
    printf("Angle: %.4f\n", results.angle);
    printf("Kp: %.4f\n", results.Kp);
    printf("Ki: %.4f\n", results.Ki);
    printf("Kd: %.4f\n", results.Kd);
    printf("Error Message: %s\n", results.errMsg);
    cout << "=================================" << endl;

    formatInfo(finalInfo, results.infoType, results.angleType,  results.voltage, results.angle,
                    results.Kp, results.Ki, results.Kd, results.errMsg);
    
    // Print all of the struct variable values
    printf("Formatted Info: %s\n", finalInfo);

    // Checking if the char arrays are the exact same
    if(strcmp(startInfo, finalInfo) == 0) {
        cout << "They are the same info" << endl;
    } else {
        cout << "They are not the same info" << endl;
    }
    return 0;
}