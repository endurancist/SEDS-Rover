#include <Wire.h>
#include <Adafruit_MotorShield.h>
#include "utility/Adafruit_MS_PWMServoDriver.h"

#define led 11 //red
#define uhoh 7

Adafruit_MotorShield AFMS = Adafruit_MotorShield(); 
Adafruit_DCMotor *myMotorL = AFMS.getMotor(1);
Adafruit_DCMotor *myMotorR = AFMS.getMotor(3);

void setup() {
  Serial.begin (9600);
  AFMS.begin();
  pinMode(led, OUTPUT);
  pinMode(uhoh, INPUT);
  
}

void loop() {
  int number;
  while (digitalRead(uhoh) == HIGH) {
    
    myMotorL->run(RELEASE); // stop motors if close to wall
    myMotorR->run(RELEASE);
    delay(1000);
    myMotorL->setSpeed(255); //reverse full speed back
    myMotorR->setSpeed(255);
    myMotorL->run(BACKWARD);
    myMotorR->run(BACKWARD);
    delay(1000); //backing up 1 seconds
    myMotorL->run(RELEASE); 
    myMotorR->run(RELEASE);
    delay(1000);
    
    myMotorL->setSpeed(255);
    myMotorR->setSpeed(255);
    myMotorL->run(FORWARD);
    myMotorR->run(BACKWARD); //turning right 90 deg
    delay(1000);
    myMotorL->run(RELEASE); 
    myMotorR->run(RELEASE); //stop
    delay(1000);
    
    if (digitalRead(uhoh) == HIGH) {
      myMotorL->setSpeed(255);
      myMotorR->setSpeed(255);
      myMotorL->run(BACKWARD);
      myMotorR->run(FORWARD); //turning left 180 deg
      delay(2000);
      myMotorL->run(RELEASE); 
      myMotorR->run(RELEASE); //stop
      delay(1000);
      
      if (digitalRead(uhoh) == HIGH) {
        myMotorL->setSpeed(255);
        myMotorR->setSpeed(255);
        myMotorL->run(BACKWARD);
        myMotorR->run(FORWARD); //turning left
        delay(1000);
        myMotorL->run(RELEASE); 
        myMotorR->run(RELEASE); //stop
        delay(1000);
        break; //break out of while loop
      }
      else {
        break;
      }
    }
    else {
      break;
    }
  
  }
  myMotorL->setSpeed(255);
  myMotorR->setSpeed(255);
  myMotorL->run(FORWARD);
  myMotorR->run(BACKWARD); //full speed forward
  delay(100);
}

