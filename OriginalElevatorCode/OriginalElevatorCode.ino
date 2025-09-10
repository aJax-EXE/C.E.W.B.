#include <Arduino.h>
#include <TB6612_ESP32.h>
#include <ESP32Encoder.h>
// Pins for all inputs, keep in mind the PWM defines must be on PWM pins
// the default pins listed are the ones used on the Redbot (ROB-12097) with
// the exception of STBY which the Redbot controls with a physical switch/*
#define AIN1 26 // PCB 25
#define AIN2 22// PCB 39
#define PWMA 25// PCB 36
#define STBY 21// PCB 26
#define A 18 // PCB 21 // Green
#define B 5 // PCB 22 // Yellow

ESP32Encoder encoder;
double numSteps = 1.6875*PI/280.0;
double currentPos = 0;

// these constants are used to allow you to make your motor configuration
// line up with function names like forward. Value can be 1 or -1
const int offsetA = 1;
Motor motor1 = Motor(AIN1, AIN2, PWMA, offsetA, STBY, 5000 , 8, 1);

// Initializing motors. The library will allow you to initialize as many
// motors as you have memory for. If you are using functions like forward
// that take 2 motors as arguments you can either write new functions or
// call the function more than once.
void move(Motor &motor, ESP32Encoder &encoder, double &numSteps, double &currentPos, double dist) {
  if(dist > 0) {
  while(dist+currentPos >= encoder.getCount()*numSteps) {
    int speed = constrain(map(dist+currentPos-encoder.getCount()*numSteps, 0, 18, 100, 255), 100, 255);
    motor.drive(speed);
    //Serial.println(encoder.getCount()/2);
  }

  } else {
  while(dist+currentPos <= encoder.getCount()*numSteps) {
    int speed = constrain(map(dist+currentPos+encoder.getCount()*numSteps, 0, 18, 100, 255), 100, 255);
    motor.drive(-speed);
    //Serial.println(encoder.getCount()/2);
  }
}
motor.drive(0);
currentPos = encoder.getCount()*numSteps;
}
void setup() {
encoder.attachHalfQuad(A,B);
encoder.setCount(0);
Serial.begin(115200);
Serial.println("System Ready");
}
void loop() {
if(Serial.available() > 0){
String moveDist = Serial.readString();
moveDist.trim();
Serial.println(moveDist.toDouble());
move(motor1, encoder, numSteps, currentPos, moveDist.toDouble());
Serial.println("Move Completed");
}
}
