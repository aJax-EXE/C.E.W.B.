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
char tempInfo[16] = " ";
// String info, tempInfo;

// Check if all of the values are formatted correctly or can be formatted correctly
bool correctFormat, done = false;

// Binary counter for ease of function access and char insertion
bool count = 0b000;

bool formatCheck(char, float, char, float, float, float, float);
void transferValue(float, char*, bool&);

void setup() {
  // put your setup code here, to run once:
  Serial.begin(9600);
}

void loop() {
  // 'done' variable makes sure the code only runs once
  while (done == false) {
    // Checking the formatting and values of the input values to be compared
    correctFormat = formatCheck(infoType, voltage, angleType, angle, Kp, Ki, Kd);
    
    if (correctFormat == false) {
      // Checks failed somewhere...
      Serial.println("Sorry user, but one or more variables inputted for proper formatting are incorrect. Please review and try again.");
      done = true;
    } else if (correctFormat == true) {
      // Check succeeds - Adding the voltage info
      info[0] = infoType;
      info[strlen(info)] = 'V';
      transferValue(voltage, tempInfo);

      strcat(info, tempInfo);
      tempInfo[0] = '\0';

      // Adding the angle info
      info[strlen(info)] = 'A';
      info[strlen(info)] = angleType;
      transferValue(angle, tempInfo);

      strcat(info, tempInfo);
      tempInfo[0] = '\0';
      
      // Adding the PID controller info
      strcat(info, ";P");
      transferValue(Kp, tempInfo);

      strcat(info, tempInfo);
      tempInfo[0] = '\0';

      info[strlen(info)] = 'I';
      transferValue(Ki, tempInfo);

      strcat(info, tempInfo);
      tempInfo[0] = '\0';

      info[strlen(info)] = 'D';
      transferValue(Kd, tempInfo);

      strcat(info, tempInfo);
      info[strlen(info)] = '!';
      Serial.println(info);

      // // The overall length of the formatted information array should be 49 characters long
      Serial.println(strlen(info));
      done = true;
    }
  }
}

// Function that checks if the given values are valid and can be formatted correctly
bool formatCheck(char infoType, float voltage, char angleType, float angle, float Kp, float Ki, float Kd) {
  if (infoType != '$' && infoType != '#') {
    return false;
  } else if (voltage/100 >= 1) { 
    return false; 
  } else if (angleType != 'D' && angleType != 'R') {
    return false;
  } else if (Kp/1000 >= 1 && Ki/1000 >= 1 && Kd/1000 >= 1){
    return false;
  } else { return true; }
}

// Function that turns the float values into there proper form of XXX.XXXX into a char array
void transferValue(float value, char* temp, bool& count) {
  int whole = (int)value;
  int frac  = round((value - whole) * 10000);

  switch (count) {
    case 1:
    
  }
  if (count == 0) {
    snprintf(temp, 16, "%03d.%04d", whole, frac);
  }
  
} 
