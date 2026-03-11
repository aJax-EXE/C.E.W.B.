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


float v_motor = 23;
long p_encoder = 456;
int int_aux = 0;
char str_aux[23];                // each number is defined as a string "XX.XXXX,XXXX\n", where X is a digit [0:9]

uint8_t PWMVal = 0;
  
void setup() {
  // Setting the Motor Pins for the Motor Driver
  // They dictate the direct the motor spins in
  pinMode(MotorIn1, OUTPUT);
  pinMode(MotorIn2, OUTPUT);

  // Starting the encoder up
  enc1.begin();

  Serial.begin(115200);
  Serial.println("Beginning");
}

void loop() {

    // ---- Send data to Linux ----
    p_encoder = enc1.getCount();

    // ---->>>> read encoder here <<<<<---- 
    Serial.print(v_motor,3);
    Serial.print(",");
    Serial.println(p_encoder);
  
    // ---- Read data from Linux ----
    if (Serial.available()) {
      String line = Serial.readStringUntil('\n');
      sscanf(line.c_str(), "%04f,%04d", &v_motor, &p_encoder);
      // ---->>>> send voltage to the motor <<<<<----
      // Turn the voltage into a PWM Value
      PWMVal = voltToPWM(v_motor);

      // Make the motor spin at that value
      digitalWrite(MotorIn1, LOW);
      analogWrite(MotorIn2, PWMVal);
      
    }
    delay(1);
    
}


