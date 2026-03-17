#include <CEWBEncoder.h>

#define IN1 9
#define IN2 10

#define CMD_HEADER 0xAA
#define DATA_HEADER 0x55

CEWBEncoder encoder(2,3,ENC2X);

float voltage = 0;

void setup()
{
    Serial.begin(115200);

    pinMode(IN1,OUTPUT);
    pinMode(IN2,OUTPUT);

    encoder.begin();
}

void driveMotor(float v)
{
    int pwm = constrain(abs(v)*255.0/12.0,0,255);

    if(v >= 0)
    {
        analogWrite(IN1,pwm);
        analogWrite(IN2,0);
    }
    else
    {
        analogWrite(IN1,0);
        analogWrite(IN2,pwm);
    }
}

void readCommand()
{
    static uint8_t state = 0;
    static uint8_t buffer[4];
    static uint8_t index = 0;

    while(Serial.available())
    {
        uint8_t b = Serial.read();

        switch(state)
        {
            case 0:     // wait for header
                if(b == CMD_HEADER)
                {
                    index = 0;
                    state = 1;
                }
                break;

            case 1:     // read float bytes
                buffer[index++] = b;

                if(index >= 4)
                {
                    memcpy(&voltage,buffer,4);
                    state = 0;
                }
                break;
        }
    }
}

void sendData()
{
    long count = encoder.getCount();

    Serial.write(DATA_HEADER);
    Serial.write((uint8_t*)&voltage,4);
    Serial.write((uint8_t*)&count,4);
}

void loop()
{
    readCommand();

    driveMotor(voltage);

    sendData();

    delay(10);
}