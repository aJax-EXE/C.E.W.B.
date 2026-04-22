// Custom Libraries
#include <CEWBEncoder.h>
#include <CEWBFunctions.h>

// Motor Drive PWM Pins
#define MotorIn1 9   // Orange Wire
#define MotorIn2 10  // Blue Wire

// Encoder
CEWBEncoder enc1(2, 3, ENC2X);

// State
long    v_motor         = 0;
uint8_t PWMVal          = 0;
unsigned long last_recv = 0;  // timestamp of last received packet
#define WATCHDOG_MS 200       // stop motor if no packet for 200ms

void spinMotor(long v);

void setup() {
  pinMode(MotorIn1, OUTPUT);
  pinMode(MotorIn2, OUTPUT);
  enc1.begin();
  Serial.begin(115200);
}

void loop() {

  // ---- Watchdog — stop motor if Simulink stops sending ----
  if (millis() - last_recv > WATCHDOG_MS) {
    v_motor = 0;
    spinMotor(0);
  }

  // ---- Wait for voltage command from Simulink ----
  if (Serial.available()) {
    String line = Serial.readStringUntil('\n');

    if (line.length() > 0 && line.charAt(0) == '#') {

      // Parse incoming voltage command: #±XXXXXXX
      int v_cmd  = 0;
      int parsed = sscanf(line.c_str(), "#%d", &v_cmd);

      if (parsed == 1) {
        v_motor   = (long)v_cmd;
        last_recv = millis();  // reset watchdog timer
      }

      // ---- Read encoder ----
      long p_encoder = enc1.getCount();

      // ---- Execute voltage command ----
      spinMotor(v_motor);

      // ---- Send data back to Simulink ----
      // Format: #±XXXXXX,±XXXXXX:0\n
      //   first value  = encoder count
      //   second value = voltage being applied
      //   :0           = limit bit always 0 (no limit logic here)
      //                  Simulink handles all limit/control logic
      char tx_buf[24];
      snprintf(tx_buf, sizeof(tx_buf), "#%+07ld,%+07ld:0\n",
               p_encoder, v_motor);
      Serial.print(tx_buf);
    }
  }
}

// Convert signed voltage to PWM and drive motor
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
