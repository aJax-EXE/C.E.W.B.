#ifndef CEWBEncoder_h
#define CEWBEncoder_h

#include <Arduino.h>

#define ENC1X 1
#define ENC2X 2
#define ENC4X 4

class CEWBEncoder {

public:

    CEWBEncoder(uint8_t pinA, uint8_t pinB, uint8_t mode);

    void begin();

    long getCount();
    int8_t getDirection();

    static void updatePort(uint8_t portIndex, uint8_t newState);

private:

    uint8_t _pinA;
    uint8_t _pinB;

    uint8_t _maskA;
    uint8_t _maskB;

    uint8_t _portIndex;
    uint8_t _mode;

    volatile long _count;
    volatile int8_t _direction;

    volatile uint8_t _lastState;

    static CEWBEncoder* _encoders[8];
    static uint8_t _encoderCount;

};

#endif