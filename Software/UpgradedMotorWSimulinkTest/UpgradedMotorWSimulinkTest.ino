#include <CEWBEncoder.h>

#define CMD_HEADER  0xAA
#define DATA_HEADER 0x55

#define IN1 5
#define IN2 6

CEWBEncoder encoder(2,3,ENC2X);

float voltageCommand = 0.0;
long encoderCount = 0;

unsigned long lastCommandTime = 0;

void setup()
{
    Serial.begin(115200);

    pinMode(IN1,OUTPUT);
    pinMode(IN2,OUTPUT);

    encoder.begin();
}

void readCommand()
{
    if (Serial.available() >= 5)
    {
        if (Serial.read() == CMD_HEADER)
        {
            Serial.readBytes((char*)&voltageCommand, sizeof(float));
            lastCommandTime = millis();
        }
    }

    if (millis() - lastCommandTime > 200)
        voltageCommand = 0;
}

void driveMotor(float voltage)
{
    voltage = constrain(voltage,-24.0,24.0);

    int pwm = abs(voltage) * 255.0 / 24.0;

    if(voltage >= 0)
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

void sendTelemetry()
{
    encoderCount = encoder.getCount();

    Serial.write(DATA_HEADER);
    Serial.write((byte*)&voltageCommand,sizeof(float));
    Serial.write((byte*)&encoderCount,sizeof(long));
}

void loop()
{
    readCommand();

    driveMotor(voltageCommand);

    sendTelemetry();

    delay(10);
}