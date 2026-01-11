// Rotary Encoder Inputs
#define CLK 6 // White Wire
#define DT 5  // Green Wire


int counter = 0;
int currentStateCLK;
int lastStateCLK;
String currentDir = "";

void setup() {

  // Set encoder pins as inputs
  pinMode(CLK, INPUT);
  pinMode(DT, INPUT);

  // Setup Serial Monitor
  Serial.begin(9600);

  // Read the initial state of CLK
  lastStateCLK = digitalRead(CLK);
}

void loop() {

  // Read the current state of CLK
  currentStateCLK = digitalRead(CLK);

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
