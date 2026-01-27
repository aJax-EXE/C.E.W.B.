// USES THE STRING VARIABLE
// Setting the variables that the format will carry
float voltage, angle, Kp, Ki, Kd;
String infoLabel, angleType;

// The complete information format that needs to be dissected (and the temp info variable)
String info, tempInfo;
int startIndex;

// This is an example of the format that is used: $V12.3220AD044.8745;P009.0000I000.1000D003.5000!

void setup() {
  // put your setup code here, to run once:
  Serial.begin(9600);
}

void loop() {
  // Starting the loop only when data is received
  while (Serial.available() > 0) {
    // Getting the message and cutting off the end character
    info = Serial.readStringUntil('!');

    // Initial String size check
    if (info.length() != 47) {
      Serial.println("Sorry this data is not formatted correctly. Please try again.");
      break;
    }

    // Determining the label (Setting or Feedback)
    if (info.charAt(0) == '$') {
      infoLabel = "Master Info";
    } else if (info.charAt(0) == '#') {
      infoLabel = "Feedback Info";
    }

    // Remove the first division label character
    info.remove(0,1);

    // Getting the different bits of information - voltage
    startIndex = info.indexOf('V');
    // Serial.println(startIndex);

    tempInfo = info.substring(startIndex+1, startIndex+8);
    voltage = tempInfo.toFloat();
    // Serial.println(voltage, 4);


    // Getting the different bits of information - angle
    startIndex = info.indexOf('A');
    // Serial.println(startIndex);

    tempInfo = info.substring(startIndex+2, startIndex+10);
    // Determining the label (Degrees or Radians)
    if (info.charAt(startIndex+1) == 'D') {
      angleType = "Degrees";
    } else if (info.charAt(0) == 'R') {
      angleType = "Radians";
    }
    angle = tempInfo.toFloat();
    Serial.print("Label: ");
    // Serial.println(angleType);
    // Serial.println(angle, 4);

    
    // Getting the proportional constant
    startIndex = info.indexOf('P');
    // Serial.println(startIndex);

    tempInfo = info.substring(startIndex+1, startIndex+9);
    Kp = tempInfo.toFloat();
    // Serial.println(Kp, 4);


    // Getting the integral constant
    startIndex = info.indexOf('I');
    // Serial.println(startIndex);

    tempInfo = info.substring(startIndex+1, startIndex+9);
    Ki = tempInfo.toFloat();
    // Serial.println(Ki, 4);


    // Getting the derivative constant
    startIndex = info.lastIndexOf('D');
    // Serial.println(startIndex);

    tempInfo = info.substring(startIndex+1, startIndex+9);
    Kd = tempInfo.toFloat();
    // Serial.println(Kd, 4);

     //Printing all of the information
  Serial.print("Entered Data: ");
  Serial.println(info);
  Serial.print("Label: ");
  Serial.println(infoLabel);

  Serial.print("Voltage: ");
  Serial.println(voltage, 4);

  Serial.print("Angle Type: ");
  Serial.println(angleType);

  Serial.print("Angle: ");
  Serial.println(angle, 4);

  Serial.print("Kp: ");
  Serial.println(Kp, 4);

  Serial.print("Ki: ");
  Serial.println(Ki, 4);

  Serial.print("Kd: ");
  Serial.println(Kd, 4);
  }
}
