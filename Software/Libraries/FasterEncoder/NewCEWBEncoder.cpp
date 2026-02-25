#include "NewCEWBEncoder.h"

NewCEWBEncoder* NewCEWBEncoder::_instances[MAX_ENCODERS];
uint8_t NewCEWBEncoder::_instanceCount = 0;

NewCEWBEncoder::NewCEWBEncoder(uint8_t pinA, uint8_t pinB)
: _count(0), _direction(0)
{
  uint8_t port = digitalPinToPort(pinA);

  _pinReg  = portInputRegister(port);
  _ddrReg  = portModeRegister(port);
  _portReg = portOutputRegister(port);

  _maskA = digitalPinToBitMask(pinA);
  _maskB = digitalPinToBitMask(pinB);

  // Assign correct PCMSK + PCIE bit
  if (_pinReg == &PINB) {
    _pcmskReg = &PCMSK0;
    _pcieMask = (1 << PCIE0);
  }
  else if (_pinReg == &PINC) {
    _pcmskReg = &PCMSK1;
    _pcieMask = (1 << PCIE1);
  }
  else {
    _pcmskReg = &PCMSK2;
    _pcieMask = (1 << PCIE2);
  }

  _instances[_instanceCount++] = this;
}

void NewCEWBEncoder::begin() {

  *_ddrReg  &= ~(_maskA | _maskB);
  *_portReg |= (_maskA | _maskB);

  _lastState = *_pinReg;

  PCICR |= _pcieMask;
  *_pcmskReg |= _maskA;
}

long NewCEWBEncoder::getCount() {
  noInterrupts();
  long val = _count;
  interrupts();
  return val;
}

int8_t NewCEWBEncoder::getDirection() {
  return _direction;
}

void NewCEWBEncoder::reset() {
  noInterrupts();
  _encoderCount = 0;
  interrupts();
}


void NewCEWBEncoder::handle(uint8_t current) {

  uint8_t changed = current ^ _lastState;

  if (changed & _maskA) {

    bool A = current & _maskA;
    bool B = current & _maskB;

    if (A == B) {
      _count++;
      _direction = 1;
    } else {
      _count--;
      _direction = -1;
    }
  }

  _lastState = current;
}

void NewCEWBEncoder::handlePort(volatile uint8_t* pinReg, uint8_t current) {

  for (uint8_t i = 0; i < _instanceCount; i++) {

    if (_instances[i]->_pinReg == pinReg) {
      _instances[i]->handle(current);
    }
  }
}

/* -------- ISR ROUTING -------- */

ISR(PCINT0_vect) {
  uint8_t current = PINB;
  NewCEWBEncoder::handlePort(&PINB, current);
}

ISR(PCINT1_vect) {
  uint8_t current = PINC;
  NewCEWBEncoder::handlePort(&PINC, current);
}

ISR(PCINT2_vect) {
  uint8_t current = PIND;
  NewCEWBEncoder::handlePort(&PIND, current);
}