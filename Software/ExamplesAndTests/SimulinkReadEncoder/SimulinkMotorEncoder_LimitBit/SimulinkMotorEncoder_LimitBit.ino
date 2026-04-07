// Custom Libraries
#include <CEWBEncoder.h>
#include <CEWBFunctions.h>

// Motor Drive PWM Pins
#define MotorIn1 9   // Orange Wire
#define MotorIn2 10  // Blue Wire

// Encoder
CEWBEncoder enc1(2, 3, ENC2X);

// ── Change this to set the encoder limit ──────────────────
#define TARGET_POS 0   // encoder count to stop at (0 = disabled)
// ──────────────────────────────────────────────────────────

// State
long v_motor   = 0;
bool limit_hit = false;

uint8_t PWMVal = 0;

void spinMotor(long v);

void setup() {
  pinMode(MotorIn1, OUTPUT);
  pinMode(MotorIn2, OUTPUT);
  enc1.begin();
  Serial.begin(115200);
}

void loop() {

  // ---- Wait for request from Simulink ----
  if (Serial.available()) {
    String line = Serial.readStringUntil('\n');

    if (line.length() > 0 && line.charAt(0) == '#') {

      // Parse incoming: #voltage  (single value only)
      long v_cmd = 0;
      int parsed = sscanf(line.c_str(), "#%ld", &v_cmd);

      if (parsed == 1) {
        // Reset limit flag if Simulink commands zero voltage
        if (v_cmd == 0) {
          limit_hit = false;
        }
        v_motor = v_cmd;
      }

      // ---- Read encoder ----
      long p_encoder = enc1.getCount();

      // ---- Check limit ----
      if (TARGET_POS != 0 && !limit_hit) {
        bool passed = (v_motor > 0 && p_encoder >= TARGET_POS) ||
                      (v_motor < 0 && p_encoder <= -TARGET_POS);
        if (passed) {
          limit_hit = true;
          v_motor   = 0;   // cut motor
        }
      }

      // ---- Drive motor ----
      spinMotor(v_motor);

      // ---- Send response back to Simulink ----
      // Format: #±XXXXXX,±XXXXXX:B\n
      //   first value  = encoder count
      //   second value = voltage being applied
      //   B            = 1 if limit reached, 0 otherwise
      char tx_buf[24];
      snprintf(tx_buf, sizeof(tx_buf), "#%+07ld,%+07ld:%d\n",
               p_encoder, v_motor, limit_hit ? 1 : 0);
      Serial.print(tx_buf);
    }
  }
}

// Spin motor based on signed voltage value
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
