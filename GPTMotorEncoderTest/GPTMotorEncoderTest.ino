// ================= ENCODER =================
const int ENC_A = 2;   // CLK; Green Wire
const int ENC_B = 3;   // DT; White Wire

volatile long encoderCount = 0;
volatile int encoderDirection = 0;

// Store last encoder state (2-bit)
volatile uint8_t lastState = 0;

// Quadrature lookup table
// Index = (lastState << 2) | currentState
// Value = +1, -1, or 0
const int8_t quadTable[16] = {
  0, -1, +1,  0,
 +1,  0,  0, -1,
 -1,  0,  0, +1,
  0, +1, -1,  0
};

void encoderISR() {
  uint8_t currentState =
    (digitalRead(ENC_A) << 1) | digitalRead(ENC_B);

  uint8_t index = (lastState << 2) | currentState;
  int8_t delta = quadTable[index];

  if (delta != 0) {
    encoderCount += delta;
    encoderDirection = (delta > 0) ? 1 : -1;
  }

  lastState = currentState;
}

// ================= MOTOR =================
const int IN1 = 9;   // PWM-capable
const int IN2 = 10;  // PWM-capable

// speed: -255 to +255
void setMotor(int speed) {
  speed = constrain(speed, -255, 255);

  if (speed > 0) {
    analogWrite(IN1, speed);
    digitalWrite(IN2, LOW);
  }
  else if (speed < 0) {
    digitalWrite(IN1, LOW);
    analogWrite(IN2, -speed);
  }
  else {
    digitalWrite(IN1, LOW);
    digitalWrite(IN2, LOW);
  }
}

// ================= SETUP =================
void setup() {
  // Encoder
  pinMode(ENC_A, INPUT_PULLUP);
  pinMode(ENC_B, INPUT_PULLUP);

  lastState =
    (digitalRead(ENC_A) << 1) | digitalRead(ENC_B);

  attachInterrupt(digitalPinToInterrupt(ENC_A), encoderISR, CHANGE);
  attachInterrupt(digitalPinToInterrupt(ENC_B), encoderISR, CHANGE);

  // Motor
  pinMode(IN1, OUTPUT);
  pinMode(IN2, OUTPUT);

  Serial.begin(9600);
}

// ================= LOOP =================
void loop() {
  // Run motor (change sign to reverse)
  setMotor(150);

  // Read encoder values safely (real-time)
  long count;
  int dir;

  noInterrupts();
  count = encoderCount;
  dir = encoderDirection;
  interrupts();

  Serial.print("Count: ");
  Serial.print(count);
  Serial.print(" | Direction: ");
  Serial.println(dir == 1 ? "CW" : "CCW");

  delay(100);
}
