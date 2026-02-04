// Setting the variables that the format will carry (Change the values to see the results)
// USES CHAR ARRAYS

// Struct of the different type of variables that will be given the information from the char array
struct parsedData {
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

  // Array that will hold an error message to tell the user what is wrong
  // Will deal with the error code messages
  char errMsg[100];
};

// Creating the struct
parsedData results;

// The size of the char array buffer for which will tell how big the formatted info will be
const int bufferSize = 50;

// The end character is '!', which signals the end of the information
// The complete information format that needs to be dissected (and the temp info variable)
char info[bufferSize] = " ";

// Check if all of the values are formatted correctly or can be formatted correctly
bool correctFormat, done = false;

// Binary counter to determine what values are incorrect and need to be fixed 
// (And with the different types of error values that can exist)
uint8_t errorVal = 0b000000;
enum Errors {
  INFO_TYPE_ERROR  = 0b100000,
  VOLT_ERROR       = 0b010000,
  ANGLE_TYPE_ERROR = 0b001000,
  KP_ERROR         = 0b000100,
  KI_ERROR         = 0b000010,
  KD_ERROR         = 0b000001
};

// Functions used for the formatting
void checkAndFormat();
void failState();

void setup() {
  // put your setup code here, to run once:
  Serial.begin(9600);
  Serial.println("=================================");
}

void loop() {
  // put your main code here, to run repeatedly:
  while(!done){
    checkAndFormat(results.infoType, results.voltage, results.angleType, results.angle,
                    results.Kp, results.Ki, results.Kd, info);
    
    // Print all of the struct variable values
    Serial.print("Formatted Info: ");
    Serial.println(info);

    done = true;
  }
}

void checkAndFormat(char infoType, float voltage, char angleType, float angle, 
                    float Kp, float Ki, float Kd, char* info) {
  if (infoType != '$' && infoType != '#') {
    errorVal |= INFO_TYPE_ERROR;
    failState();
    return;
  }
  if (voltage/100 >= 1) { 
    errorVal |= VOLT_ERROR;
    failState();
    return;
  }
  if (angleType != 'D' && angleType != 'R') {
    errorVal |= ANGLE_TYPE_ERROR;
    failState();
    return;
  }
  if (Kp/1000 >= 1 || Ki/1000 >= 1 || Kd/1000 >= 1){
    if (Kp/1000 >= 1) {
      errorVal |= KP_ERROR;
      failState();
      return;
    } 
    if (Ki/1000 >= 1) {
      errorVal |= KI_ERROR;
      failState();
      return;
    }
    if (Kd/1000 >= 1) {
      errorVal |= KD_ERROR;
      failState();
      return;
    }
  }

  // Variables for the whole number and decimal points of each value
  int wholeVolt = (int)voltage;
  int wholeAng = (int)angle;
  int wholeKp = (int)Kp;
  int wholeKi = (int)Ki;
  int wholeKd = (int)Kd;

  int fracVolt = round((voltage - wholeVolt) * 10000);
  int fracAng  = round((angle - wholeAng) * 10000);
  int fracKp  = round((Kp - wholeKp) * 10000);
  int fracKi  = round((Ki - wholeKi) * 10000);
  int fracKd  = round((Kd - wholeKd) * 10000);

  // Combining all of the variables together into the necessary format
  snprintf(info, bufferSize, "%cV%02d.%04dA%c%03d.%04d;P%03d.%04dI%03d.%04dD%03d.%04d!", 
           infoType, wholeVolt, fracVolt, angleType, wholeAng, fracAng, wholeKp, fracKp, 
           wholeKi, fracKi, wholeKd, fracKd);
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

  strcpy(results.errMsg, "Error with the given information");
}
