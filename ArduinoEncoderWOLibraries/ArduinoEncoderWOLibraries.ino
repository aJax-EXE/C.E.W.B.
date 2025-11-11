// Rotary Encoder Inputs
#define CLK 6
#define DT 5

// Motor Drive PMW Pins
#define MotorIn1 9
#define MotorIn2 10

int counter = 0;
int currentStateCLK;
int lastStateCLK;
String currentDir = "";

int PWMVal = 0;

void setup() {

  // Set encoder pins as inputs
  pinMode(CLK, INPUT);
  pinMode(DT, INPUT);

  // Setting the Motor Pins
  pinMode(MotorIn1, OUTPUT);
  pinMode(MotorIn2, OUTPUT);

  // Setup Serial Monitor
  Serial.begin(9600);

  // Read the initial state of CLK
  lastStateCLK = digitalRead(CLK);
}

void loop() {

  // Read the current state of CLK
  currentStateCLK = digitalRead(CLK);

  // Ramp Up Forward
  digitalWrite(MotorIn1, LOW);
  for (PWMVal; PWMVal <= 255; PWMVal++) {
    currentStateCLK = digitalRead(CLK);
    analogWrite(MotorIn2, PWMVal);
    positionCheck();
    delay(10);
  }

  // Full Forward Speed for one second
  // Figure out how to pause the process for this time while keeping 
  // The counter from the encoder going
  delay(1000);

  // Ramp Down Forward
  for (PWMVal; PWMVal >= 0; PWMVal--) {
    currentStateCLK = digitalRead(CLK);
    analogWrite(MotorIn2, PWMVal);
    positionCheck();
    delay(10);
  }

  // Ramp Up Backward
  digitalWrite(MotorIn2, LOW);
  for (PWMVal = 0; PWMVal <= 255; PWMVal++) {
    currentStateCLK = digitalRead(CLK);
    analogWrite(MotorIn1, PWMVal);
    positionCheck();
    delay(10);
  }

  // Full Backwards Speed for one second
  // Figure out how to pause the process for this time while keeping 
  // The counter from the encoder going
  delay(1000);

  // Ramp Down Forward
  for (PWMVal = 255; PWMVal >= 0; PWMVal--) {
    currentStateCLK = digitalRead(CLK);
    analogWrite(MotorIn1, PWMVal);
    positionCheck();
    delay(10);
  }

  // // If last and current state of CLK are different, then pulse occurred
  // // React to only 1 state change to avoid double count
  // if (currentStateCLK != lastStateCLK && currentStateCLK == 1) {

  //   // If the DT state is different than the CLK state then
  //   // the encoder is rotating CCW so decrement
  //   if (digitalRead(DT) != currentStateCLK) {
  //     counter--;
  //     currentDir = "CCW";
  //   } else {
  //     // Encoder is rotating CW so increment
  //     counter++;
  //     currentDir = "CW";
  //   }

  //   Serial.print("Direction: ");
  //   Serial.print(currentDir);
  //   Serial.print(" | Counter: ");
  //   Serial.println(counter);
  // } //else {
  // //   Serial.print("Direction: ");
  // //   Serial.print(currentDir);
  // //   Serial.print(" | Counter: ");
  // //   Serial.println(counter);
  // // }

  // // Remember last CLK state
  // lastStateCLK = currentStateCLK;
  // // counter++;

}

void positionCheck () {
  // If last and current state of CLK are different, then pulse occurred
  // React to only 1 state change to avoid double count
  if (currentStateCLK != lastStateCLK && currentStateCLK == 1) {

    // If the DT state is different than the CLK state then
    // the encoder is rotating CCW so decrement
    if (digitalRead(DT) != currentStateCLK) {
      counter--;
      currentDir = "CCW";
    } else {
      // Encoder is rotating CW so increment
      counter++;
      currentDir = "CW";
    }

    Serial.print("Direction: ");
    Serial.print(currentDir);
    Serial.print(" | Counter: ");
    Serial.println(counter);
  } //else {
  //   Serial.print("Direction: ");
  //   Serial.print(currentDir);
  //   Serial.print(" | Counter: ");
  //   Serial.println(counter);
  // }

  // Remember last CLK state
  lastStateCLK = currentStateCLK;
}
