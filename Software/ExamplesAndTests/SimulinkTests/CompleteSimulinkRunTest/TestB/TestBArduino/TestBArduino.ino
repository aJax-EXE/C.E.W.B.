// Custom Libraries
#include <CEWBEncoder.h>
#include <CEWBFunctions.h>

// Motor Drive PWM Pins
#define MotorIn1 9   // Orange Wire
#define MotorIn2 10  // Blue Wire

// Encoder
CEWBEncoder enc1(2, 3, ENC2X);

// State
long    v_motor    = 0;
long    target_pos = 0;   // received from MATLAB block dialog via packet
bool    limit_hit  = false;
uint8_t PWMVal     = 0;

void spinMotor(long v);

void setup() {
  pinMode(MotorIn1, OUTPUT);
  pinMode(MotorIn2, OUTPUT);
  enc1.begin();
  Serial.begin(115200);
}

void loop() {

  if (Serial.available()) {
    String line = Serial.readStringUntil('\n');

    if (line.length() > 0 && line.charAt(0) == '#') {

      // Parse: #voltage,target_position
      int v_cmd      = 0;
      int target_cmd = 0;
      int parsed = sscanf(line.c_str(), "#%d,%d", &v_cmd, &target_cmd);

      if (parsed == 2) {
        target_pos = (long)target_cmd;

        // Reset limit if voltage commanded to zero
        if (v_cmd == 0) {
          limit_hit = false;
        }
        v_motor = (long)v_cmd;
      }

      // ---- Read encoder ----
      long p_encoder = enc1.getCount();

      // ---- Check limit ----
      // target_pos > 0: check positive direction
      // target_pos < 0: check negative direction
      // target_pos = 0: disabled
      if (target_pos != 0 && !limit_hit) {
        bool passed = (target_pos > 0 && p_encoder >= target_pos) ||
                      (target_pos < 0 && p_encoder <= target_pos);
        if (passed) {
          limit_hit = true;
          v_motor   = 0;
        }
      }

      // ---- Drive motor ----
      spinMotor(v_motor);

      // ---- Send response: #±XXXXXX,±XXXXXX:B\n ----
      char tx_buf[24];
      snprintf(tx_buf, sizeof(tx_buf), "#%+07ld,%+07ld:%d\n",
               p_encoder, v_motor, limit_hit ? 1 : 0);
      Serial.print(tx_buf);
    }
  }
}

void spinMotor(long v) {
  PWMVal = abs(voltToPWM((int)v));

  if (v >= 0) {
    digitalWrite(MotorIn1, LOW);
    analogWrite(MotorIn2, PWMVal);
  } else {
    digitalWrite(MotorIn2, LOW);
    analogWrite(MotorIn1, PWMVal);
  }
}
