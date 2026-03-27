#include "CEWBEncoder.h"

// In order to declare an encoder counting type, 
// ENC1X is no quad
// ENC2X is 2x quad
// ENC4X is 4x quad

// Encoders can also have different pin values outside of 2 and 3,
// but the pins chosen need to be apart of the same pcint grouping (found on pinout sheet)
//
CEWBEncoder encoder(2,3,ENC2X);

void setup()
{
    Serial.begin(115200);

    pinMode(IN1,OUTPUT);
    pinMode(IN2,OUTPUT);

    encoder.begin();
}

void loop()
{
    digitalWrite(IN1,HIGH);
    digitalWrite(IN2,LOW);

    static unsigned long last = 0;

    if(millis() - last > 100)
    {
        last = millis();

        Serial.print("Count: ");
        Serial.print(encoder.getCount());

        Serial.print(" Dir: ");
        Serial.println(encoder.getDirection());
    }
}
