// Custom Libraries
#include <CEWBEncoder.h>
#include <CEWBFunctions.h>

// Motor Drive PWM Pins
#define MotorIn1 9
#define MotorIn2 10

// Encoder
CEWBEncoder enc1(2, 3, ENC4X);

// State
long         v_motor   = 0;
long         p_encoder = 0;

float voltage = 0.0;
uint8_t       PWMVal    = 0;
unsigned long last_recv = 0;
char str_aux[30];
char voltWord[15];
uint8_t PWD_input;

void spinMotor(float v);



void setup() {
  pinMode(MotorIn1, OUTPUT);
  pinMode(MotorIn2, OUTPUT);
  enc1.begin();
  Serial.begin(115200);
  last_recv = millis();

}

void loop() {

  p_encoder = enc1.getCount();

  // Always use abs(voltage) for PWM display; sign handled by direction
  PWD_input = voltToPWM(abs(voltage));
  dtostrf(voltage, 8, 4, voltWord);

  // Single consistent format for both positive and negative
//  sprintf(str_aux, "#%+07ld,%+07ld,%d,%s\n", v_motor, p_encoder, PWD_input, voltWord);
  sprintf(str_aux, "#%+07ld,%+07ld\n", v_motor, p_encoder);
  Serial.print(str_aux);

  if (Serial.available()) {
    String line = Serial.readStringUntil('\n');
    sscanf(line.c_str(), "#%ld,%ld", &v_motor, &p_encoder);
    voltage = makeVoltage(v_motor);
    spinMotor(voltage);
  }

  delay(1);
}


// Convert signed voltage to PWM and drive motor
// voltToPWM receives float directly as the library expects
void spinMotor(float v) {
  PWMVal = voltToPWM(abs(v));  // abs here so voltToPWM always gets positive value

  if (v >= 0.0) {
    digitalWrite(MotorIn1, LOW);
    analogWrite(MotorIn2, PWMVal);
  } else {
    digitalWrite(MotorIn2, LOW);
    analogWrite(MotorIn1, PWMVal);
  }
}
