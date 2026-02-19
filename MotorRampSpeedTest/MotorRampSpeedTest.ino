// All this code/example is meant to do is ramp the motor up to 
// a set PMW Limit, run at that value for a while, then ramp back down

int PWMVal = 0; // Current PWM Value that is being read by the Motor
int PWMLim = 255; // The max limit that the PWM will go up to; change as desired

// Motor Drive PMW Pins
#define MotorIn1 9  // Orange Wire
#define MotorIn2 10 // Blue Wire

void setup() {
  // Setting the Motor Pins for the Motor Driver
  // They dictate the direct the motor spins in
  pinMode(MotorIn1, OUTPUT);
  pinMode(MotorIn2, OUTPUT);

  // Setup Serial Monitor
  Serial.begin(9600);
}

void loop() {
  // Ramp Up Forward
  digitalWrite(MotorIn1, LOW);
  for (PWMVal; PWMVal <= PWMLim; PWMVal++) {
    analogWrite(MotorIn2, PWMVal);
    Serial.println("Ramping up Speed Forward!");
    delay(10);
  }

  // Full Forward Speed for one second
  delay(1000);

  // Ramp Down Forward
  for (PWMVal; PWMVal >= 0; PWMVal--) {
    analogWrite(MotorIn2, PWMVal);
    Serial.println("Ramping down Speed Forward!");
    delay(10);
  }

  // Ramp Up Backward
  digitalWrite(MotorIn2, LOW);
  for (PWMVal; PWMVal <= PWMLim; PWMVal++) {
    analogWrite(MotorIn1, PWMVal);
    Serial.println("Ramping up Speed Backwards!");
    delay(10);
  }

  // Full Backwards Speed for one second
  delay(1000);

  // Ramp Down Backwards
  for (PWMVal; PWMVal >= 0; PWMVal--) {
    analogWrite(MotorIn1, PWMVal);
    Serial.println("Ramping down Speed Backwards!");
    delay(10);
  }

  // PWMVal = 0;
}