// Setting the variables that the format will carry
// USES CHAR ARRAYS

// Struct of the different type of variables that will be given the information from the char array
struct parsedData {
  float voltage, angle, Kp, Ki, Kd;
  char infoType, angleType;
  char errMsg[100];
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
bool correctFormat = true, done = false;

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

// Functions used for the formatting
void formatCheckAndParse(char*);
void failState();

void setup() {
  // put your setup code here, to run once:
  Serial.begin(9600);
  Serial.println("=================================");
}

void loop() {
  // 'done' variable makes sure the code only runs once
  while (!done) {
    // Checking the formatting and values of the info char array to be parsed
    formatCheckAndParse(info);

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

// Function that checks if the given info char array has all of the needed parts
void formatCheckAndParse(char* info) {
  // Initial String size check
    if (strlen(info) != 48) {
      charCheck |= tooShort;
      Serial.print("Fuck my life: ");
      Serial.println(charCheck);
      failState();
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
            results.infoType = 'M';
            break;
          } else {
            charCheck |= incorrectChar;
            failState();
            return;
          }

        case '#':
          if (info[i] == info[0]) {
            charCheck |= foundInfoType;
            results.infoType = 'F';
            break;
          } else {
            charCheck |= incorrectChar;
            failState();
            return;
          }

        case 'V':
          if (info[i] == info[1]) {
            charCheck |= foundVolt;
            break;
          } else {
            charCheck |= incorrectChar;
            failState();
            return;
          }

        case 'A':
          if (info[i] == info[9]) {
            charCheck |= foundAngle;
            results.voltage = value;
            intPart = 0;
            fracPart = 0;
            fracDiv = 1;
            decimal = false;
            break;
          } else {
            charCheck |= incorrectChar;
            failState();
            return;
          }

        case 'D':
          // Need some special logic to check if the D is for "degrees" of "Kd"
          if (info[i - 1] == 'A') {
            charCheck |= foundAngleType;
            results.angleType = 'D';
            break;
          } else if (info[i] == info[38]) {
            charCheck |= foundKd;
            results.Ki = value;
            intPart = 0;
            fracPart = 0;
            fracDiv = 1;
            decimal = false;
            break;
          } else {
            charCheck |= incorrectChar;
            failState();
            return;
          }

        case 'R':
          if (info[i] == info[10]) {
            charCheck |= foundAngleType;
            results.angleType = 'R';
            break;
          } else {
            charCheck |= incorrectChar;
            failState();
            return;
          }

        case ';':
          if (info[i] == info[19]) {
            charCheck |= foundSemi;
            results.angle = value;
            intPart = 0;
            fracPart = 0;
            fracDiv = 1;
            decimal = false;
            break;
          } else {
            charCheck |= incorrectChar;
            failState();
            return;
          }

        case 'P':
          if (info[i] == info[20]) {
            charCheck |= foundKp;
            break;
          } else {
            charCheck |= incorrectChar;
            failState();
            return;
          }

        case 'I':
          if (info[i] == info[29]) {
            charCheck |= foundKi;
            results.Kp = value;
            intPart = 0;
            fracPart = 0;
            fracDiv = 1;
            decimal = false;
            break;
          } else {
            charCheck |= incorrectChar;
            failState();
            return;
          }

        case '!':
          if (info[i] == info[47]) {
            charCheck |= foundBang;
            results.Kd = value;
            intPart = 0;
            fracPart = 0;
            fracDiv = 1;
            decimal = false;
            break;
          } else {
            charCheck |= incorrectChar;
            failState();
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
            failState();
            return;
          }
      }
    }

    // Last number and decimal point check
    if ((totNum == corrNumAmount) && (totDeci == corrDeciAmount) && ((charCheck & goodToGo)) == goodToGo) {
      strcpy(results.errMsg, "All conditions good");
    }

}

void failState() {
  // Check failed somewhere...
  results.infoType = 'N';
  results.voltage = 0;
  results.angleType = 'N';
  results.angle = 0;
  results.Kp = 0;
  results.Ki = 0;
  results.Kd = 0;

  Serial.print("Check val: ");
  Serial.println(charCheck);
  Serial.print("incorrect val: ");
  Serial.println(incorrectChar);
  Serial.print("short val: ");
  Serial.println(tooShort);
  // Outputting the correct reason of error message
  if (charCheck & incorrectChar) {
    Serial.println(charCheck);
    strcpy(results.errMsg, "Characters in the wrong order");
  } else if (charCheck & tooShort) {
    Serial.println(charCheck);
    strcpy(results.errMsg, "Info too short");
  }
}
