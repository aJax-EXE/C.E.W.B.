// ================= CONFIG =================
#define ENC_A 2
#define ENC_B 3

#define IN1 9   // PWM pin
#define IN2 8

// encoder resolution AFTER quadrature
// example: 600 CPR encoder → 2400 counts
const float COUNTS_PER_REV = 2400.0;

// PID gains (tune these!)
float Kp = 4.0;
float Ki = 0.02;
float Kd = 0.2;

// ================= GLOBALS =================

volatile long encoderCount = 0;

float targetAngle = 90.0;   // degrees (change this!)

float integral = 0;
float lastError = 0;

unsigned long lastTime;

// ================= ENCODER ISR =================

void encoderISR() {
  bool A = digitalRead(ENC_A);
  bool B = digitalRead(ENC_B);

  if (A == B) encoderCount++;
  else encoderCount--;
}

// ================= SETUP =================

void setup() {
  Serial.begin(115200);

  pinMode(ENC_A, INPUT_PULLUP);
  pinMode(ENC_B, INPUT_PULLUP);

  pinMode(IN1, OUTPUT);
  pinMode(IN2, OUTPUT);

  attachInterrupt(digitalPinToInterrupt(ENC_A), encoderISR, CHANGE);
  attachInterrupt(digitalPinToInterrupt(ENC_B), encoderISR, CHANGE);

  lastTime = millis();
}

// ================= MOTOR DRIVER =================

void setMotor(float value) {

  value = constrain(value, -255, 255);

  if (value > 0) {
    // Forward
    analogWrite(IN1, value);
    digitalWrite(IN2, LOW);
  }
  else if (value < 0) {
    // Reverse
    analogWrite(IN1, -value);
    digitalWrite(IN2, HIGH);
  }
  else {
    // Brake / coast
    digitalWrite(IN1, LOW);
    digitalWrite(IN2, LOW);
  }
}


// ================= MAIN LOOP =================

void loop() {

  // ----- Time step -----
  unsigned long now = millis();
  float dt = (now - lastTime) / 1000.0;
  lastTime = now;

  // ----- Get angle -----
  noInterrupts();
  long counts = encoderCount;
  interrupts();

  float angle = counts * 360.0 / COUNTS_PER_REV;

  // ----- PID -----
  float error = targetAngle - angle;

  integral += error * dt;
  integral = constrain(integral, -100, 100);   // anti-windup

  float derivative = (error - lastError) / dt;
  lastError = error;

  float output = Kp * error + Ki * integral + Kd * derivative;

  // ----- Drive motor -----
  setMotor(output);

  // ----- Debug -----
  Serial.print("Angle: ");
  Serial.print(angle);
  Serial.print("  Output: ");
  Serial.println(output);

  delay(5);
}

