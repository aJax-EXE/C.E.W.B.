// Custom Libraries
#include <CEWBEncoder.h>
#include <CEWBFunctions.h>

// Motor Drive PMW Pins
#define MotorIn1 9  // Orange Wire
#define MotorIn2 10 // Blue Wire

// Encoder Creation
CEWBEncoder enc1(2, 3, ENC2X);

// // The max voltage value that the motor can go to
// const int maxVolt = 24;


int v_motor = 123;
int p_encoder = 456;
int int_aux = 0;
char str_aux[11];     // each number is defined as a string "XX.XXXX,XXXX\n", where X is a digit [0:9]

uint8_t PWMVal = 0;

// Function to spin the motor
void spinMotor(int);
  
void setup() {
  // Setting the Motor Pins for the Motor Driver
  // They dictate the direct the motor spins in
  pinMode(MotorIn1, OUTPUT);
  pinMode(MotorIn2, OUTPUT);

  // Starting the encoder up
  enc1.begin();

  Serial.begin(115200);
  // Serial.println("Beginning");
}

void loop() {

    // ---- Send data to Linux ----
    p_encoder = enc1.getCount();

    // ---->>>> read encoder here <<<<<---- 
    sprintf (str_aux, "%04d,%04d\n", v_motor , p_encoder );
    Serial.print(str_aux);
  
    // ---- Read data from Linux ----
    if (Serial.available()) {
      String line = Serial.readStringUntil('\n');
      sscanf(line.c_str(), "%04d,%04d", &v_motor, &p_encoder);
      // ---->>>> send voltage to the motor <<<<<----
      spinMotor(v_motor);
      
    }
    delay(1);
    
}

// Function to make the motor spin based on a given voltage value
void spinMotor(int v_motor) {
  // Turn the voltage into a PWM Value
  PWMVal = abs(voltToPWM(v_motor));

  // Make the motor spin at that value
  if (v_motor >= 0) {
    digitalWrite(MotorIn1, LOW);
    analogWrite(MotorIn2, PWMVal);
  } else {
    digitalWrite(MotorIn2, LOW);
    analogWrite(MotorIn1, PWMVal);
  }
}


