#ifndef NEWCEWBENCODER_H
#define NEWCEWBENCODER_H

#include <Arduino.h>
#define MAX_ENCODERS 8

class NewCEWBEncoder {
public:
  NewCEWBEncoder(uint8_t pinA, uint8_t pinB);

  void begin();
  long getCount();
  int8_t getDirection();
  void reset();
  
  static void handlePort(volatile uint8_t* pinReg, uint8_t current);

private:
  void handle(uint8_t currentPortState);

  volatile uint8_t* _pinReg;
  volatile uint8_t* _ddrReg;
  volatile uint8_t* _portReg;
  volatile uint8_t* _pcmskReg;

  uint8_t _maskA;
  uint8_t _maskB;
  uint8_t _pcieMask;

  volatile long _count;
  volatile int8_t _direction;
  volatile uint8_t _lastState;

  static NewCEWBEncoder* _instances[MAX_ENCODERS];
  static uint8_t _instanceCount;

};

#endif