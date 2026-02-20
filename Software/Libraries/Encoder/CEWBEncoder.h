#ifndef CEWBENCODER_H
#define CEWBENCODER_H
#include <Arduino.h>

#define MAX_ENCODERS 4

// The class of the encoder that will have the corresponding variables and functions
// In the main project code, there is the main/base encoder and the pendulum encoder
class CEWBEncoder {
  private: // All variables of a class go to the private area
    volatile long _encoderCount = 0;
    volatile int _encoderDirection = 0;
    volatile uint8_t _lastState = 0;
    uint8_t _ENC_A, _ENC_B;

    // A static pointer to the instance of the class
    static CEWBEncoder* _instances[MAX_ENCODERS];
    static uint8_t _instanceCount;

    static const int8_t quadTable[16];

    static void enablePCINT(uint8_t pin);

  public: // Functions and displayed numbers go here
    // Constructor Function
    CEWBEncoder(uint8_t ENC_A, uint8_t ENC_B);

    // Functions that outputs the count of the encoder and the direction that it's spinning
    long getCount();
    int getDirection();

    // The static function that will be the actual ISR
    static void staticISR();
    // The interrupt function
    void encoderISR();
    // Function that deals with pins that don't have interrupts
    // static void pcintISR();
};


#endif