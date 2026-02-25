#include <FasterEncoder2x.h>

#define IN1 5
#define IN2 6

const long TARGET = 10000;
const int PWM_VALUE = 255;     // 0–255

enum State {
  FORWARD,
  BRAKE_FWD,
  REVERSE,
  BRAKE_REV
};

State state = FORWARD;
unsigned long stateTimer = 0;

void motorForward(int pwm) {
  analogWrite(IN1, pwm);
  digitalWrite(IN2, LOW);
}

void motorReverse(int pwm) {
  analogWrite(IN2, pwm);
  digitalWrite(IN1, LOW);
}

void motorBrake() {
  digitalWrite(IN1, HIGH);
  digitalWrite(IN2, HIGH);
}

void motorCoast() {
  digitalWrite(IN1, LOW);
  digitalWrite(IN2, LOW);
}

void setup() {

  Serial.begin(115200);

  pinMode(IN1, OUTPUT);
  pinMode(IN2, OUTPUT);

  pinMode(2, INPUT_PULLUP);
  pinMode(3, INPUT_PULLUP);


  FastEncoder2x::begin();
  FastEncoder2x::write(0);

  motorForward(PWM_VALUE);
}

void loop() {

  long count = FastEncoder2x::read();

  switch (state) {

    case FORWARD:
      if (count >= TARGET) {
        motorBrake();
        stateTimer = millis();
        state = BRAKE_FWD;
      }
      break;

    case BRAKE_FWD:
      if (millis() - stateTimer > 200) {
        motorReverse(PWM_VALUE);
        state = REVERSE;
      }
      break;

    case REVERSE:
      if (count <= 0) {
        motorBrake();
        stateTimer = millis();
        state = BRAKE_REV;
      }
      break;

    case BRAKE_REV:
      if (millis() - stateTimer > 200) {
        FastEncoder2x::write(0);
        motorForward(PWM_VALUE);
        state = FORWARD;
      }
      break;
  }

  // Debug output (reduce frequency to avoid serial slowdown)
  static unsigned long lastPrint = 0;
  if (millis() - lastPrint > 100) {
    Serial.println(count);
    lastPrint = millis();
  }
}