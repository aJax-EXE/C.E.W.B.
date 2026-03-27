#include <CEWBEncoder.h>

// ==== ENCODER PINS ====
#define ENC_A 2
#define ENC_B 3

// ==== DRV8871 PINS ====
#define IN1 9   // PWM
#define IN2 10  // Direction

CEWBEncoder encoder(ENC_A, ENC_B, ENC4X);

// ==== SETTINGS ====
const float CPR = 512 * 4;
const int MOTOR_SPEED = 80;

unsigned long lastPrint = 0;

// ===== MOTOR CONTROL =====

void motorForward(int speed) {
  analogWrite(IN1, speed);
  digitalWrite(IN2, LOW);
}

void motorReverse(int speed) {
  analogWrite(IN1, speed);
  digitalWrite(IN2, HIGH);
}

void motorCoast() {
  analogWrite(IN1, 0);
  digitalWrite(IN2, LOW);
}

void motorBrake() {
  digitalWrite(IN1, HIGH);
  digitalWrite(IN2, HIGH);
}

void setup() {
  Serial.begin(115200);

  pinMode(IN1, OUTPUT);
  pinMode(IN2, OUTPUT);

  encoder.begin();
  encoder.setCPR(CPR);
  encoder.resetCount();

  // ===== SELECT STOP MODE =====
  // encoder.setTargetDegrees(180.0);
  encoder.setTargetCounts(10000);
  // encoder.setTargetRadians(PI);

  // encoder.enableTargetStop(true);

  // Start motor
  motorForward(MOTOR_SPEED);

  Serial.println("DRV8871 motor running... stopping at target");
}

void loop() {

  long count = encoder.getCount();
  float degrees = (count / CPR) * 360.0;

  Serial.println(encoder.getCount());


  // ===== HARD STOP =====
  if (encoder.reachedTarget()) {
    Serial.println("STOP TRIGGERED");
    motorBrake();
    while(1);
  }

  // ===== DEBUG PRINT =====
  if (millis() - lastPrint > 100) {
    Serial.print("Count: ");
    Serial.print(count);

    Serial.print(" | Deg: ");
    Serial.println(degrees, 2);

    lastPrint = millis();
  }
}

