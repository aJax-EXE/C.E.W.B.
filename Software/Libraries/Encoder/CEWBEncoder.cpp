#include "CEWBEncoder.h"


CEWBEncoder* CEWBEncoder::_instances[MAX_ENCODERS];
uint8_t CEWBEncoder::_instanceCount = 0;

// Port Snapshots
static volatile uint8_t lastPortB;
static volatile uint8_t lastPortC;
static volatile uint8_t lastPortD;


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

  _portRegA = portInputRegister(digitalPinToPort(_ENC_A));
  _portRegB = portInputRegister(digitalPinToPort(_ENC_B));
  _maskA = digitalPinToBitMask(_ENC_A);
  _maskB = digitalPinToBitMask(_ENC_B);

  uint8_t state = (((*_portRegA & _maskA) ? 1 : 0) << 1) | ((*_portRegB & _maskB) ? 1 : 0);

  _lastState = state;

  if (_instanceCount < MAX_ENCODERS) { _instances[_instanceCount++] = this; }

  int extInt = digitalPinToInterrupt(_ENC_A);

  if (extInt != NOT_AN_INTERRUPT) {
    _usingExtInterrupt = true;
    attachInterrupt(extInt, extISRRouter, CHANGE);
  } else {
    _usingExtInterrupt = false;

    uint8_t group = digitalPinToPCICRbit(_ENC_A);
    _pcintGroup = group;

    enablePCINT(_ENC_A);

    // Initializing the port snapshots only once
    lastPortB = PINB;
    lastPortC = PINC;
    lastPortD = PIND;
  }
}


// Function that deals with the pin(s) that don't have interrupts automatically enabled
void CEWBEncoder::enablePCINT(uint8_t pin) {
  *digitalPinToPCMSK(pin) |= bit(digitalPinToPCMSKbit(pin));
  PCIFR |= bit(digitalPinToPCICRbit(pin));
  PCICR |= bit(digitalPinToPCICRbit(pin));
}

void CEWBEncoder::extISRRouter() {
  for (uint8_t i = 0; i < _instanceCount; i++) {
    if (_instances[i]->_usingExtInterrupt) { _instances[i]->handleISR(); }
  }
}

// PCINT Vectors with Port-Diff
ISR(PCINT0_vect) {
  uint8_t current = PINB;
  uint8_t changed = current ^ lastPortB;
  lastPortB = current;

  if(changed) { CEWBEncoder::pcintRouter(0, changed); }
}

ISR(PCINT1_vect) {
  uint8_t current = PINC;
  uint8_t changed = current ^ lastPortC;
  lastPortC = current;

  if(changed) { CEWBEncoder::pcintRouter(1, changed); }
}

ISR(PCINT2_vect) {
  uint8_t current = PIND;
  uint8_t changed = current ^ lastPortD;
  lastPortD = current;

  if(changed) { CEWBEncoder::pcintRouter(2, changed); }
}


void CEWBEncoder::pcintRouter(uint8_t group, uint8_t changedMask) {
  for(uint8_t i = 0; i < _instanceCount; i++) {
    CEWBEncoder* enc = _instances[i];

    if (enc->_usingExtInterrupt) { continue; }

    if (enc->_pcintGroup != group) { continue; }

    // A pin did not change
    if (!(changedMask & enc->_maskA)) { continue; }
    enc->handleISR();
  }
}



// The interrupt function
void CEWBEncoder::handleISR() {
  uint8_t state = (((*_portRegA & _maskA) ? 1 : 0) << 1) | ((*_portRegB & _maskB) ? 1 : 0);

  // EXIT EARLY if nothing changed
  if (state == _lastState) { return; }

  uint8_t index = (_lastState << 2) | state;
  int8_t delta = quadTable[index];

  if (delta != 0) {
    _encoderCount += delta;
    _encoderDirection = (delta > 0) ? 1 : -1;
  }

  _lastState = state;
}

// Public Functions
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

void CEWBEncoder::reset() {
  noInterrupts();
  _encoderCount = 0;
  interrupts();
}
