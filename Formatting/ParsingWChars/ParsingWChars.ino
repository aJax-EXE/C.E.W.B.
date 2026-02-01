// Setting the variables that the format will carry
// USES CHAR ARRAYS

// Struct of the different type of variables that will be given the information from the char array
struct parsedData {
  float voltage, angle, Kp, Ki, Kd;
  char infoLabel, angleType;
};

// Creating the struct
parsedData results;

// The size of the char array buffer for which will tell how big the formatted info will be
const int bufferSize = 50;

// The complete information format that needs to be dissected (and the temp info variable)
char info[bufferSize] = "$V12.3220AD044.8745;P009.0000I000.1000D003.5000!";
char tempInfo[16] = " ";
int startIndex;

// This is an example of the format that is used: $V12.3220AD044.8745;P009.0000I000.1000D003.5000!
// Check if all of the values are formatted correctly or can be formatted correctly
bool correctFormat, done = false;

// Binary checker to see if the necessary variables are in place
uint8_t charCheck = 0b00000000000;
enum importantChars {
  incorrectChar  = 0b10000000000,  // Error
  tooShort       = 0b01000000000,  // Error
  foundInfoType  = 0b00100000000,
  foundVolt      = 0b00010000000,
  foundAngle     = 0b00001000000,
  foundAngleType = 0b00000100000,
  foundSemi      = 0b00000010000,
  foundKp        = 0b00000001000,
  foundKi        = 0b00000000100,
  foundKd        = 0b00000000010,
  foundBang      = 0b00000000001,
  goodToGo       = 0b00111111111  // Correct everything state
};

// Total number and decimal point counter for info format checker
const int corrNumAmount = 34, corrDeciAmount = 5;

// Functions used for the formatting
bool formatCheckAndParse(char*, uint8_t&);

void setup() {
  // put your setup code here, to run once:
  Serial.begin(9600);
  Serial.println("=================================");
}

void loop() {
  // 'done' variable makes sure the code only runs once
  while (!done) {
    // Checking the formatting and values of the info char array to be parsed
    correctFormat = formatCheckAndParse(info, charCheck);

    if (!correctFormat) {
      // Check failed somewhere...
      Serial.println("Sorry user, but one or more issues with the inputted info string are incorrect: ");

      // Outputting the correct reason of error message
      if (charCheck & incorrectChar) {
        // Give the variable later...
        Serial.println("* The characters around a variable are in the wrong order");
      } else if (charCheck & tooShort) {
        Serial.println("* The formatted information array has too few characters in it");
      }
      Serial.println("Please review and try again.");
      done = true;
    } else {
      Serial.print("Info Type: ");
      Serial.println(results.infoLabel);
      Serial.print("Voltage: ");
      Serial.println(results.voltage, 4);
      Serial.print("Angle Tpye: ");
      Serial.println(results.angleType);
      Serial.print("Angle: ");
      Serial.println(results.angle, 4);
      Serial.print("Kp: ");
      Serial.println(results.Kp, 4);
      Serial.print("Ki: ");
      Serial.println(results.Ki, 4);
      Serial.print("Kd: ");
      Serial.println(results.Kd, 4);

      done = true;
    }
  
  }
}

// Function that checks if the given info char array has all of the needed parts
bool formatCheckAndParse(char* info, uint8_t& check) {
  // Initial String size check
  if (strlen(info) != 48) { check |= tooShort; return false; }

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
          check |= foundInfoType;
          results.infoLabel = 'M';
          break;
        } else { check |= incorrectChar; return false; }

      case '#':
        if (info[i] == info[0]) {
          check |= foundInfoType;
          results.infoLabel = 'F';
          break;
        } else { check |= incorrectChar; return false; }

      case 'V':
        if (info[i] == info[1]) { check |= foundVolt; break; } 
        else { check |= incorrectChar; return false; }
      
      case 'A': 
        if (info[i] == info[9]) { 
          check |= foundAngle;
          results.voltage = value;
          intPart = 0;
          fracPart = 0;
          fracDiv = 1;
          decimal = false;
          break;
        } else { check |= incorrectChar; return false; }

      case 'D':
        // Need some special logic to check if the D is for "degrees" of "Kd" 
        if (info[i-1] == 'A') { 
          check |= foundAngleType;
          results.angleType = 'D';
          break;
        } 
        else if (info[i] == info[38]) {
          check |= foundKd;
          results.Ki = value;
          intPart = 0;
          fracPart = 0;
          fracDiv = 1;
          decimal = false;
          break;
        }
        else { check |= incorrectChar; return false; }

      case 'R': 
        if (info[i] == info[10]) {
          check |= foundAngleType;
          results.angleType = 'R';
          break;
        }
        else { check |= incorrectChar; return false; }

      case ';':
        if (info[i] == info[19]) {
          check |= foundSemi;
          results.angle = value;
          intPart = 0;
          fracPart = 0;
          fracDiv = 1;
          decimal = false;
          break;
        }
        else { check |= incorrectChar; return false; }

      case 'P':
        if (info[i] == info[20]) { check |= foundKp; break; }
        else { check |= incorrectChar; return false; }

      case 'I':
        if (info[i] == info[29]) {
          check |= foundKi;
          results.Kp = value;
          intPart = 0;
          fracPart = 0;
          fracDiv = 1;
          decimal = false;
          break;
        }
        else { check |= incorrectChar; return false; }

      case '!':
        if (info[i] == info[47]) {
          check |= foundBang;
          results.Kd = value;
          intPart = 0;
          fracPart = 0;
          fracDiv = 1;
          decimal = false;
          break;
        }
        else { check |= incorrectChar; return false; }

      default:
        // Counting the total amounts of digits and decimal points
        if (info[i] >= 48 && info[i] <= 57) {
          totNum++;
          if (!decimal) {
            intPart = intPart * 10 + (info[i] - '0');
          } else {
            fracPart = fracPart * 10 + (info[i] - '0');
            fracDiv *= 10;
          }
          break;
        } else if (info[i] == 46) {
          totDeci++;
          decimal = true;
          break;
        }
        else { check |= incorrectChar; return false; }
      
    }
  }

  // Last number and decimal point check
  if ((totNum == corrNumAmount) && (totDeci == corrDeciAmount) && (check & goodToGo)) { return true; }
}
