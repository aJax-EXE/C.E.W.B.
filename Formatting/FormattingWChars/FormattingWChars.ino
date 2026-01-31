// Setting the variables that the format will carry (Change the values to see the results)
// USES CHAR ARRAYS
// The first value should indicate if the info is the Master/Setting form '$' or the Feedback form '#'
char infoType = '$';

// Voltage should have the form XX.XXXX
float voltage = 12.3220;

// The angle type either be a 'D' for degrees or 'R' for radians
char angleType = 'D';
// Angle, regardless of the type, should have the form XXX.XXXX
float angle = 44.8745;

// The Kp, Ki, and Kd values should have the form XXX.XXXX
float Kp = 9;
float Ki = 0.1;
float Kd = 3.5;

// The size of the char array buffer for which will tell how big the formatted info will be
const int bufferSize = 50;

// The end character is '!', which signals the end of the information

// The complete information format that needs to be dissected (and the temp info variable)
char info[bufferSize] = " ";
// char tempInfo[16] = " ";

// Check if all of the values are formatted correctly or can be formatted correctly
bool correctFormat, done = false;

// Binary counter to determine what values are incorrect and need to be fixed (And with the different types of error values that can exist)
uint8_t errorVal = 0b000000;
enum {
  INFO_TYPE_ERROR = 0b100000,
  VOLT_ERROR = 0b010000,
  ANGLE_TYPE_ERROR = 0b001000,
  KP_ERROR = 0b000100,
  KI_ERROR = 0b000010,
  KD_ERROR = 0b000001
} ;


bool formatCheck(char, float, char, float, float, float, float, byte&);
void transferValues(char, float, char, float, float, float, float, char*, int);

void setup() {
  // put your setup code here, to run once:
  Serial.begin(9600);
}

void loop() {
  // 'done' variable makes sure the code only runs once
  while (!done) {
    // Checking the formatting and values of the input values to be compared
    correctFormat = formatCheck(infoType, voltage, angleType, Kp, Ki, Kd, errorVal);
    
    if (!correctFormat) {
      // Checks failed somewhere...
      Serial.println("Sorry user, but one or more of the following variables inputted for proper formatting are incorrect: ");

      // Check each bit and print corresponding message
      if (errorVal & INFO_TYPE_ERROR) {
        Serial.println("* Incorrect information type character given");
      }
      if (errorVal & VOLT_ERROR) {
        Serial.println("* Incompatiable voltage value given");
      }
      if (errorVal & ANGLE_TYPE_ERROR) {
        Serial.println("* Incorrect angle type given");
      }
      if (errorVal & KP_ERROR) {
        Serial.println("* Incompatiable Kp value given");
      }
      if (errorVal & KI_ERROR) {
        Serial.println("* Incompatiable Ki value given");
      }
      if (errorVal & KD_ERROR) {
        Serial.println("* Incompatiable Kd value given");
      }
      
      Serial.println("Please review and try again.");
      done = true;
    }
    else {
      // Check succeeds
      transferValues(infoType, voltage, angleType, angle, Kp, Ki, Kd, info, bufferSize);

      // The overall length of the formatted information array should be 49 characters long (48 info characters + '\0' (null character))
      Serial.print("Formatted Info: ");
      Serial.println(info);
      Serial.print("Number of info characters: ");
      Serial.println(strlen(info));
      Serial.print("Number of total characters: ");
      Serial.println(strlen(info) + 1);
      done = true;
    }

    // done == true
  }
}

// Function that checks if the given values are valid and can be formatted correctly
bool formatCheck(char infoType, float voltage, char angleType, float Kp, float Ki, float Kd, byte& error) {
  if (infoType != '$' && infoType != '#') {
    error |= 0b100000;
  }
  if (voltage/100 >= 1) { 
    error |= 0b010000;
  }
  if (angleType != 'D' && angleType != 'R') {
    error |= 0b001000;
  }
  if (Kp/1000 >= 1 || Ki/1000 >= 1 || Kd/1000 >= 1){
    if (Kp/1000 >= 1) {
      error |= 0b000100;
    } 
    if (Ki/1000 >= 1) {
      error |= 0b000010;
    }
    if (Kd/1000 >= 1) {
      error |= 0b000001;
    }
  }

  return error == 0;
}

// Function that turns the float values into there proper form of XXX.XXXX into a char array
void transferValues(char infoType, float voltage, char angleType, float angle, float Kp, float Ki, float Kd, char* info, int bufferSize) {
  // Variables for the whole number and decimal points of each value
  int wholeVolt = (int)voltage;
  int wholeAng = (int)angle;
  int wholeKp = (int)Kp;
  int wholeKi = (int)Ki;
  int wholeKd = (int)Kd;

  int fracVolt  = round((voltage - wholeVolt) * 10000);
  int fracAng  = round((angle - wholeAng) * 10000);
  int fracKp  = round((Kp - wholeKp) * 10000);
  int fracKi  = round((Ki - wholeKi) * 10000);
  int fracKd  = round((Kd - wholeKd) * 10000);

  // Combining all of the variables together into the necessary format
  snprintf(info, bufferSize, "%cV%02d.%04dA%c%03d.%04d;P%03d.%04dI%03d.%04dD%03d.%04d!", infoType, wholeVolt, fracVolt, angleType, wholeAng, fracAng, wholeKp, fracKp, wholeKi, fracKi, wholeKd, fracKd);
} 
