#include "CEWBEncoder.h"

#define IN1 5
#define IN2 6

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


//   // Convert count to angle (degrees)
//   float angle = (count * 360.0) / 2048.0;
//   // Convert count to radians
//   float rads = (count * 2 * PI) / 2048.0;

//   Serial.print("Count: ");
//   Serial.print(count);
//   Serial.print(" | Angle: ");
//   Serial.print(angle, 3);
//   Serial.print(" deg | Direction: ");
//   Serial.println(dir == 1 ? "CW" : "CCW");
}
