// Setting the variables that the format will carry (Change the values to see the results)
// USES THE STRING VARIABLE
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

// The end character is '!', which signals the end of the information

// The complete information format that needs to be dissected (and the temp info variable)
String info, tempInfo;

// Check if all of the values are formatted correctly or can be formatted correctly
bool correctFormat, done = false;

bool formatCheck(char, float, char, float, float, float, float);
String transferVoltage(float);
String transferOthers(float);

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
      tempInfo = String(infoType) + "V" + transferVoltage(voltage);
      info = tempInfo;

      // Adding the angle info
      tempInfo = "A" + String(angleType) + transferOthers(angle);
      info += tempInfo;

      // Adding the PID controller info
      tempInfo = ";P" + transferOthers(Kp) + "I" + transferOthers(Ki) + "D" + transferOthers(Kd) + "!";
      info += tempInfo;
      Serial.println(info);

      // The overall length of the formatted information string should be 48 characters long
      Serial.println(info.length());
      done = true;
    }
  }
}

// Function that checks if the given values 
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

String transferVoltage(float voltage) {
  if (voltage < 10) {
    return "0" + String(voltage, 4);
  } else { return String(voltage, 4); }
}

String transferOthers(float value) {
  if (value < 100 && value > 10) {
    return "0" + String(value, 4);
  } else if (value < 100 && value < 10) {
    return "00" + String(value, 4);
  } else { return String(value, 4); }
} 