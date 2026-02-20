// ================= ENCODER PINS =================
#define ENC_A 2   // INT0; Green Wire
#define ENC_B 3   // INT1; White Wire

// ================= ENCODER VARIABLES =================
volatile long encoderCount = 0;
volatile int encoderDirection = 0;
volatile uint8_t lastState = 0;

// Quadrature state transition table
// index = (lastState << 2) | currentState
// value = +1, -1, or 0
const int8_t quadTable[16] = {
   0, -1,  1,  0,
   1,  0,  0, -1,
  -1,  0,  0,  1,
   0,  1, -1,  0
};

// ================= ENCODER ISR =================
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

// ================= SETUP =================
void setup() {
  Serial.begin(9600);

  pinMode(ENC_A, INPUT_PULLUP);
  pinMode(ENC_B, INPUT_PULLUP);

  // Read initial encoder state
  lastState =
    (digitalRead(ENC_A) << 1) | digitalRead(ENC_B);

  // Attach interrupts for full 4× decoding
  attachInterrupt(digitalPinToInterrupt(ENC_A), encoderISR, CHANGE);
  attachInterrupt(digitalPinToInterrupt(ENC_B), encoderISR, CHANGE);

  Serial.println("4x Quadrature Encoder Ready");
}

// ================= LOOP =================
void loop() {
  long count;
  int dir;

  // Atomic read
  noInterrupts();
  count = encoderCount;
  dir = encoderDirection;
  interrupts();

  // Convert count to angle (degrees)
  float angle = (count * 360.0) / 2048.0;
  // Convert count to radians
  float rads = (count * 2 * PI) / 2048.0;

  Serial.print("Count: ");
  Serial.print(count);
  Serial.print(" | Angle: ");
  Serial.print(angle, 3);
  Serial.print(" deg | Direction: ");
  Serial.println(dir == 1 ? "CW" : "CCW");

  delay(100);
}

