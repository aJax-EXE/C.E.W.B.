// Custom Libraries
#include <CEWBEncoder.h>
#include <CEWBFunctions.h>

// Motor Drive PWM Pins
#define MotorIn1 9   // Orange Wire
#define MotorIn2 10  // Blue Wire

// Encoder
CEWBEncoder enc1(2, 3, ENC4X);

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

  // ---- Serial check first — highest priority ----
  if (Serial.available()) {
    String line = Serial.readStringUntil('\n');

    if (line.length() > 0 && line.charAt(0) == '#') {

      // Parse scaled integer: #+XXXXXX or #-XXXXXX
      // makeVoltage divides by 10000.0 to recover float voltage
      long v_scaled = 0;
      int  parsed   = sscanf(line.c_str(), "#%ld", &v_scaled);

      if (parsed == 1) {
        v_motor   = makeVoltage(v_scaled);
        last_recv = millis();
        spinMotor(v_motor);  // apply immediately on receive
      }
    }
  }

  // ---- Watchdog — stop motor if Simulink stops sending ----
  if (millis() - last_recv > WATCHDOG_MS) {
    v_motor = 0.0;
    spinMotor(0.0);
  }

  // ---- Read encoder ----
  long p_encoder = enc1.getCount();

  // ---- Send data back to Simulink ----
  // Format: #sXXXXXX,sXXXXXX\n
  //   first value  = encoder count (raw long)
  //   second value = voltage x 10000 (MATLAB descales back to volts)
  if (Serial.availableForWrite() > 0) {
    long v_scaled_out = (long)(v_motor * 10000.0);
    char tx_buf[20];
    snprintf(tx_buf, sizeof(tx_buf), "#%+07ld,%+07ld\n",
             p_encoder, v_scaled_out);
    Serial.print(tx_buf);
  }
}

// Drive motor based on signed voltage
// voltToPWM only ever receives the magnitude — sign sets direction
void spinMotor(float v) {
  // Use abs(v) so voltToPWM always gets a positive value
  // The if/else below handles motor direction from the sign of v
  PWMVal = voltToPWM(fabsf(v));

  if (v > 0.0f) {
    digitalWrite(MotorIn1, LOW);
    analogWrite(MotorIn2, PWMVal);
  } else if (v < 0.0f) {
    digitalWrite(MotorIn2, LOW);
    analogWrite(MotorIn1, PWMVal);
  } else {
    // Exactly zero — brake motor
    digitalWrite(MotorIn1, LOW);
    digitalWrite(MotorIn2, LOW);
  }
}
