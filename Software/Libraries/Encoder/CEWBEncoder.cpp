#include "CEWBEncoder.h"


CEWBEncoder* CEWBEncoder::_instances[MAX_ENCODERS];
uint8_t CEWBEncoder::_instanceCount = 0;

// Quadrature state transition table
// index = (lastState << 2) | currentState
// value = +1, -1, or 0
const int8_t CEWBEncoder::quadTable[16] = {
  0, -1, 1, 0,
  1, 0, 0, -1,
  -1, 0, 0, 1,
  0, 1, -1, 0
};

// Constructor Function
CEWBEncoder::CEWBEncoder(uint8_t ENC_A, uint8_t ENC_B) {
  _ENC_A = ENC_A;
  _ENC_B = ENC_B;

  pinMode(_ENC_A, INPUT_PULLUP);
  pinMode(_ENC_B, INPUT_PULLUP);

  _lastState = (digitalRead(_ENC_A) << 1) | digitalRead(_ENC_B);

  if (_instanceCount < MAX_ENCODERS) {_instances[_instanceCount++] = this;}

  int intA = digitalPinToInterrupt(_ENC_A);
  int intB = digitalPinToInterrupt(_ENC_B);
  
  if (intA >= 0) {attachInterrupt(intA, staticISR, CHANGE);}
  else { enablePCINT(_ENC_A); }

  if (intB >= 0) {attachInterrupt(intB, staticISR, CHANGE);}
  else { enablePCINT(_ENC_B); }
  
}

// Functions that gets the encoder count and direction
long CEWBEncoder::getCount() {
    noInterrupts();
    long val = _encoderCount;
    interrupts();
    return val;
}

int CEWBEncoder::getDirection() {
    return _encoderDirection;
}

// Function that deals with the pin(s) that don't have interrupts automatically enabled
void CEWBEncoder::enablePCINT(uint8_t pin) {
    *digitalPinToPCMSK(pin) |= bit(digitalPinToPCMSKbit(pin));
    PCIFR |= bit(digitalPinToPCICRbit(pin));
    PCICR |= bit(digitalPinToPCICRbit(pin));
}

ISR(PCINT0_vect) { CEWBEncoder::staticISR(); }
ISR(PCINT1_vect) { CEWBEncoder::staticISR(); }
ISR(PCINT2_vect) { CEWBEncoder::staticISR(); }


//
void CEWBEncoder::staticISR() {
  for (uint8_t i = 0; i < _instanceCount; i++) {
    _instances[i]->encoderISR();
  }
}

// The interrupt function
void CEWBEncoder::encoderISR() {
  uint8_t currentState = (digitalRead(_ENC_A) << 1) | digitalRead(_ENC_B);
  uint8_t index = (_lastState << 2) | currentState;
  int8_t delta = quadTable[index];

  if (delta) {
    _encoderCount += delta;
    _encoderDirection = (delta > 0) ? 1 : -1;
  }

  _lastState = currentState;
}