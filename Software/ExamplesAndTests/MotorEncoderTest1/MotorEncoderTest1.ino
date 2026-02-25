#define ENC_A 2
#define ENC_B 3

#define IN1 9
#define IN2 8

volatile long encoderCount = 0;
volatile uint8_t lastState = 0;


// ----- Encoder ISR -----
void encoderISR() {

  uint8_t currentState =
      (digitalRead(ENC_A) << 1) | digitalRead(ENC_B);

  uint8_t transition = (lastState << 2) | currentState;

  switch (transition) {
    case 0b0001:
    case 0b0111:
    case 0b1110:
    case 0b1000:
      encoderCount++;
      break;

    case 0b0010:
    case 0b0100:
    case 0b1101:
    case 0b1011:
      encoderCount--;
      break;

    default:
      break;  // ignore invalid transitions (noise)
  }

  lastState = currentState;
}


void setup() {
  Serial.begin(115200);

  pinMode(ENC_A, INPUT_PULLUP);
  pinMode(ENC_B, INPUT_PULLUP);

  pinMode(IN1, OUTPUT);
  pinMode(IN2, OUTPUT);

  // Attach interrupts
  attachInterrupt(digitalPinToInterrupt(ENC_A), encoderISR, CHANGE);

  lastState = (digitalRead(ENC_A) << 1) | digitalRead(ENC_B);


  // Spin motor forward
  analogWrite(IN1, 150);
  digitalWrite(IN2, LOW);
}

void loop() {

  static long lastCount = 0;
  static unsigned long lastTime = 0;

  if (millis() - lastTime >= 100) {
    unsigned long now = millis();
    long currentCount;

    noInterrupts();
    currentCount = encoderCount;
    interrupts();

    long delta = currentCount - lastCount;

    float rpm = (delta * 600.0) / COUNTS_PER_REV;

    Serial.println(rpm);

    lastCount = currentCount;
    lastTime = now;
  }
}

