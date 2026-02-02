#include "CEWBFormatting.h"

// The end character is '!', which signals the end of the information
char endBit = '!';

// The size of the char array buffer for which will tell how big the formatted info will be
const int bufferSize = 50;

// Check if all of the values are formatted correctly or can be formatted correctly
// F for Format; P for Parsing
bool correctFormatF = false, correctFormatP = true;


// Variables used by the formatting function(s) only
  // Binary counter to determine what values are incorrect and need to be fixed
  // (And with the different types of error values that can exist)
uint8_t errorVal = 0b000000;
enum Errors {
  INFO_TYPE_ERROR = 0b100000,
  VOLT_ERROR = 0b010000,
  ANGLE_TYPE_ERROR = 0b001000,
  KP_ERROR = 0b000100,
  KI_ERROR = 0b000010,
  KD_ERROR = 0b000001
};


// Variables used by the parsing function(s)
  // Binary checker to see if the necessary variables are in place
uint16_t charCheck = 0b00000000000;
enum importantChars {
  incorrectChar = 0b10000000000,  // Error
  tooShort = 0b01000000000,       // Error
  foundInfoType = 0b00100000000,
  foundVolt = 0b00010000000,
  foundAngle = 0b00001000000,
  foundAngleType = 0b00000100000,
  foundSemi = 0b00000010000,
  foundKp = 0b00000001000,
  foundKi = 0b00000000100,
  foundKd = 0b00000000010,
  foundBang = 0b00000000001,
  goodToGo = 0b00111111111  // Correct everything state
};

  // Total number and decimal point counter for info format checker
const int corrNumAmount = 34, corrDeciAmount = 5;

static void failState(char* info, char& infoType, char& angleType, float& voltage,
                      float& angle, float& Kp, float& Ki, float& Kd, char* errMsg) {
  // Check failed somewhere...
  infoType = 'N';
  voltage = 0;
  angleType = 'N';
  angle = 0;
  Kp = 0;
  Ki = 0;
  Kd = 0;
   
  // Outputting the correct reason of error message
  if (charCheck & incorrectChar) {
    strcpy(errMsg, "Characters in the wrong order");
  } else if (charCheck & tooShort) {
    strcpy(errMsg, "Info too short");
  }

}

// Function to break the info up into its smaller components
void parseFormat(char* info, char& infoType, char& angleType, float& voltage,
                 float& angle, float& Kp, float& Ki, float& Kd, char* errMsg) {
  // Initial String size check
  if (strlen(info) != 48) {
    charCheck |= tooShort;
    failState(info, infoType, angleType, voltage, angle, Kp, Ki, Kd, errMsg);
    return;
  }

  // Total number and decimal point counter for info format checker
  int totNum = 0, totDeci = 0;

  // Variables for combining the digits into the full float value
  long intPart = 0;
  long fracPart = 0;
  int fracDiv = 1;
  bool decimal = false;
  float value = 0;

  // Checking for each value component($/#, V, A, D/R, ;, P, I, D, !, and .(for the decimal points))
  for (int i = 0; info[i] != '\0'; i++) {

    if (info[i] == '\0' || info[i] == ';' || info[i] == '!' || (info[i] >= 'A' && info[i] <= 'Z')) {
      value = intPart + ((float)fracPart / fracDiv);
    }

    switch (info[i]) {
      case '$':
        if (info[i] == info[0]) {
          charCheck |= foundInfoType;
          infoType = 'M';
          break;
        } else {
          charCheck |= incorrectChar;
          failState(info, infoType, angleType, voltage, angle, Kp, Ki, Kd, errMsg);
          return;
        }

      case '#':
        if (info[i] == info[0]) {
          charCheck |= foundInfoType;
          infoType= 'F';
          break;
        } else {
          charCheck |= incorrectChar;
          failState(info, infoType, angleType, voltage, angle, Kp, Ki, Kd, errMsg);
          return;
        }

      case 'V':
        if (info[i] == info[1]) {
          charCheck |= foundVolt;
          break;
        } else {
          charCheck |= incorrectChar;
          failState(info, infoType, angleType, voltage, angle, Kp, Ki, Kd, errMsg);
          return;
        }

      case 'A':
        if (info[i] == info[9]) {
          charCheck |= foundAngle;
          voltage = value;
          intPart = 0;
          fracPart = 0;
          fracDiv = 1;
          decimal = false;
          break;
        } else {
          charCheck |= incorrectChar;
          failState(info, infoType, angleType, voltage, angle, Kp, Ki, Kd, errMsg);
          return;
        }

      case 'D':
        // Need some special logic to check if the D is for "degrees" of "Kd"
        if (info[i - 1] == 'A') {
          charCheck |= foundAngleType;
          angleType = 'D';
          break;
        } else if (info[i] == info[38]) {
          charCheck |= foundKd;
          Ki = value;
          intPart = 0;
          fracPart = 0;
          fracDiv = 1;
          decimal = false;
          break;
        } else {
          charCheck |= incorrectChar;
          failState(info, infoType, angleType, voltage, angle, Kp, Ki, Kd, errMsg);
          return;
        }

      case 'R':
        if (info[i] == info[10]) {
          charCheck |= foundAngleType;
          angleType = 'R';
          break;
        } else {
          charCheck |= incorrectChar;
          failState(info, infoType, angleType, voltage, angle, Kp, Ki, Kd, errMsg);
          return;
        }

      case ';':
        if (info[i] == info[19]) {
          charCheck |= foundSemi;
          angle = value;
          intPart = 0;
          fracPart = 0;
          fracDiv = 1;
          decimal = false;
          break;
        } else {
          charCheck |= incorrectChar;
          failState(info, infoType, angleType, voltage, angle, Kp, Ki, Kd, errMsg);
          return;
        }

      case 'P':
        if (info[i] == info[20]) {
          charCheck |= foundKp;
          break;
        } else {
          charCheck |= incorrectChar;
          failState(info, infoType, angleType, voltage, angle, Kp, Ki, Kd, errMsg);
          return;
        }

      case 'I':
        if (info[i] == info[29]) {
          charCheck |= foundKi;
          Kp = value;
          intPart = 0;
          fracPart = 0;
          fracDiv = 1;
          decimal = false;
          break;
        } else {
          charCheck |= incorrectChar;
          failState(info, infoType, angleType, voltage, angle, Kp, Ki, Kd, errMsg);
          return;
        }

      case '!':
        if (info[i] == info[47]) {
          charCheck |= foundBang;
          Kd = value;
          intPart = 0;
          fracPart = 0;
          fracDiv = 1;
          decimal = false;
          break;
        } else {
          charCheck |= incorrectChar;
          failState(info, infoType, angleType, voltage, angle, Kp, Ki, Kd, errMsg);
          return;
        }

      default:
        // Counting the total amounts of digits and decimal points
        if (info[i] >= '0' && info[i] <= '9') {
          totNum++;
          if (!decimal) {
            intPart = intPart * 10 + (info[i] - '0');
          } else {
            fracPart = fracPart * 10 + (info[i] - '0');
            fracDiv *= 10;
          }
          break;
        } else if (info[i] == '.') {
          totDeci++;
          decimal = true;
          break;
        } else {
          charCheck |= incorrectChar;
          failState(info, infoType, angleType, voltage, angle, Kp, Ki, Kd, errMsg);
          return;
        }
    }
  }

  // Last number and decimal point check
  if ((totNum == corrNumAmount) && (totDeci == corrDeciAmount) && ((charCheck & goodToGo) == goodToGo)) {
    strcpy(errMsg, "All conditions good");
  }
}

// Checker function that makes sure the given values are valid and can be formatted correctly (Formatting only)
static bool formatCheck(char infoType, float voltage, char angleType, float Kp,
                        float Ki, float Kd, byte& error) {
}


// Function to put all of the info together to be sent off
void formatInfo(char infoLabel, char angleType, float voltage,
                float angle, float Kp, float Ki, float Kd, char* info,
                int bufferSize) {
}