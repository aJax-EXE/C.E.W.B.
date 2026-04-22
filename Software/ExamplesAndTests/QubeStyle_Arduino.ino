// Custom Libraries
#include <CEWBEncoder.h>
#include <CEWBFunctions.h>

// Motor Drive PWM Pins
#define MotorIn1 9   // Orange Wire
#define MotorIn2 10  // Blue Wire

// Encoder
CEWBEncoder enc1(2, 3, ENC2X);

// State
float         v_motor   = 0.0;
uint8_t       PWMVal    = 0;
unsigned long last_recv = 0;
#define WATCHDOG_MS 500

void spinMotor(float v);

void setup() {
  pinMode(MotorIn1, OUTPUT);
  pinMode(MotorIn2, OUTPUT);
  enc1.begin();
  Serial.begin(115200);
  last_recv = millis();
}

void loop() {

  // ---- Watchdog — stop motor if Simulink stops sending ----
  if (millis() - last_recv > WATCHDOG_MS) {
    v_motor = 0.0;
    spinMotor(0.0);
  }

  // ---- Wait for voltage command from Simulink ----
  if (Serial.available()) {
    String line = Serial.readStringUntil('\n');

    if (line.length() > 0 && line.charAt(0) == '#') {

      // Parse raw scaled integer: #sXXXXXX
      // Use library's makeVoltage() to descale to real volts
      // e.g. #+037500 → makeVoltage(37500) → 3.75V
      long v_scaled = 0;
      int  parsed   = sscanf(line.c_str(), "#%ld", &v_scaled);

      if (parsed == 1) {
        v_motor   = makeVoltage(v_scaled);  // descale using library function
        last_recv = millis();
      }
    }
  }

  // ---- Always drive motor with latest voltage ----
  long p_encoder = enc1.getCount();
  spinMotor(v_motor);

  // ---- Send data back to Simulink ----
  // Format: #sXXXXXX,sXXXXXX\n
  //   first value  = encoder count (raw long, no scaling)
  //   second value = voltage × 10000 (MATLAB calls makeVoltage equivalent to descale)
  if (Serial.availableForWrite() > 0) {
    long v_scaled_out = (long)(v_motor * 10000.0);
    char tx_buf[20];
    snprintf(tx_buf, sizeof(tx_buf), "#%+07ld,%+07ld\n",
             p_encoder, v_scaled_out);
    Serial.print(tx_buf);
  }
}

// Convert signed voltage to PWM and drive motor
// voltToPWM receives float directly as the library expects
void spinMotor(float v) {
  PWMVal = abs(voltToPWM(v));   // pass float directly — no int cast

  if (v >= 0.0) {
    digitalWrite(MotorIn1, LOW);
    analogWrite(MotorIn2, PWMVal);
  } else {
    digitalWrite(MotorIn2, LOW);
    analogWrite(MotorIn1, PWMVal);
  }
}
