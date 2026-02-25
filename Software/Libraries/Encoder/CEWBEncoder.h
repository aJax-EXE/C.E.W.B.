#ifndef CEWBENCODER_H
#define CEWBENCODER_H
#include <Arduino.h>

#define MAX_ENCODERS 6

// The class of the encoder that will have the corresponding variables and functions
// In the main project code, there is the main/base encoder and the pendulum encoder
class CEWBEncoder {
private:  // All variables of a class go to the private area
  uint8_t _ENC_A, _ENC_B;

  volatile long _encoderCount = 0;
  volatile int _encoderDirection = 0;
  volatile uint8_t _lastState = 0;

  volatile uint8_t* _portRegA;
  volatile uint8_t* _portRegB;
  uint8_t _maskA;
  uint8_t _maskB;

  uint8_t _pcintGroup = 255;  // 0=B,1=C,2=D
  bool _usingExtInterrupt = false;

  // A static pointer to the instance of the class
  static CEWBEncoder* _instances[MAX_ENCODERS];
  static uint8_t _instanceCount;

  static void extISRRouter();

  static void enablePCINT(uint8_t pin);

  static const int8_t quadTable[16];

public:  // Functions and displayed numbers go here
  // Constructor Function
  CEWBEncoder(uint8_t ENC_A, uint8_t ENC_B);

  // Functions that outputs the count of the encoder and the direction that it's spinning
  long getCount();
  int getDirection();
  void reset();

  void handleISR();

  static void pcintRouter(uint8_t group, uint8_t changedMask);
};

#endif