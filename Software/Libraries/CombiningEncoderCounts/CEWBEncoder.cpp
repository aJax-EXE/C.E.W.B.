#include "CEWBEncoder.h"

CEWBEncoder* CEWBEncoder::_encoders[8];
uint8_t CEWBEncoder::_encoderCount = 0;

static volatile uint8_t lastPortState[3] = { 0, 0, 0 };

static const int8_t quadTable[16] = { 0, -1, 1, 0,
                                       1,  0, 0, -1,
                                      -1,  0, 0,  1,
                                       0,  1,-1,  0 };

CEWBEncoder::CEWBEncoder(uint8_t pinA, uint8_t pinB, uint8_t mode) {
  _pinA = pinA;
  _pinB = pinB;
  _mode = mode;
  _count = 0;
  _direction = 0;
  _maskA = digitalPinToBitMask(pinA);
  _maskB = digitalPinToBitMask(pinB);

  uint8_t port = digitalPinToPort(pinA);
  switch (port) {
    case 2: _portIndex = 0; break;  // PORTB
    case 3: _portIndex = 1; break;  // PORTC
    case 4: _portIndex = 2; break;  // PORTD
  }
  _encoders[_encoderCount++] = this;
}

void CEWBEncoder::begin() {
  pinMode(_pinA, INPUT_PULLUP);
  pinMode(_pinB, INPUT_PULLUP);

  *digitalPinToPCMSK(_pinA) |= digitalPinToBitMask(_pinA);
  *digitalPinToPCMSK(_pinB) |= digitalPinToBitMask(_pinB);

  PCICR |= (1 << digitalPinToPCICRbit(_pinA));
  PCICR |= (1 << digitalPinToPCICRbit(_pinB));

  uint8_t A = digitalRead(_pinA);
  uint8_t B = digitalRead(_pinB);
  _lastState = (A << 1) | B;
}

long CEWBEncoder::getCount() {
 
  noInterrupts();
  long c = _count;
  interrupts();
  return c;
}

int8_t CEWBEncoder::getDirection() {
  noInterrupts();
  int8_t d = _direction;
  interrupts();
  return d;
}

void CEWBEncoder::updatePort(uint8_t portIndex, uint8_t newState) {
  uint8_t diff = newState ^ lastPortState[portIndex];
  if (diff == 0) return;

  lastPortState[portIndex] = newState;

  for (uint8_t i = 0; i < _encoderCount; i++) {
    CEWBEncoder* e = _encoders[i];
    if (e->_portIndex != portIndex) continue;

    // Only process if one of our pins changed
    if (!(diff & (e->_maskA | e->_maskB))) continue;

    uint8_t A = (newState & e->_maskA) ? 1 : 0;
    uint8_t B = (newState & e->_maskB) ? 1 : 0;
    uint8_t state = (A << 1) | B;

    if (e->_mode == ENC4X) {
      // Use full quadrature table for 4x counting
      uint8_t index = (e->_lastState << 2) | state;
      int8_t change = quadTable[index];
      if (change != 0) {
        e->_count += change;
        e->_direction = change;
      }
    }

    else if (e->_mode == ENC2X) {
      // Only count on rising and falling edges of A
      // Use B at the moment A changed to determine direction
      uint8_t lastA = (e->_lastState >> 1) & 1;
      uint8_t lastB = (e->_lastState) & 1;
      if (A != lastA) {
        // Direction: if A rising and B low, or A falling and B high → forward
        int8_t change = ((A == 1 && lastB == 0) || (A == 0 && lastB == 1)) ? 1 : -1;
        e->_count += change;
        e->_direction = change;
      }
    }

    else {
      // ENC1X — only count on rising edge of A
      uint8_t lastA = (e->_lastState >> 1) & 1;
      if (lastA == 0 && A == 1) {
        int8_t change = B ? 1 : -1;
        e->_count += change;
        e->_direction = change;
      }
    }

    e->_lastState = state;
  }
}

ISR(PCINT0_vect) { CEWBEncoder::updatePort(0, PINB); }
ISR(PCINT1_vect) { CEWBEncoder::updatePort(1, PINC); }
ISR(PCINT2_vect) { CEWBEncoder::updatePort(2, PIND); }
