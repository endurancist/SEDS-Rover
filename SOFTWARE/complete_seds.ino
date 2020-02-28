#include <Wire.h>
#include <math.h>
#include <Adafruit_Sensor.h>
#include <Adafruit_BNO055.h>
#include <utility/imumaths.h>
#include <Adafruit_MotorShield.h>
#include "utility/Adafruit_MS_PWMServoDriver.h"

#define trigPin 13
#define echoPin 12
#define BNO055_SAMPLERATE_DELAY_MS (100)


/* TODO:
 *  Check logic and turning amounts and behaviour
 *  Analyze code taken from git
 *  Write explicit turn left, right, forward, back, etc mothods using structure given here
 *  Add in functionality for multiple sensors?
 *  Actually test on rover
 *  Do multiple sensors for directions (non-infinite right turns)
 *  
 */



//setup motorshield, motors, sensors

Adafruit_BNO055 bno = Adafruit_BNO055(55);

Adafruit_MotorShield AFMS = Adafruit_MotorShield(); 
Adafruit_DCMotor *leftmotor = AFMS.getMotor(1);
Adafruit_DCMotor *rightmotor = AFMS.getMotor(3);

double targetXOrientation;

//Returns the current distance 
float getDistance(sensorNum) {
  float duration, distance;
  duration = pulseIn(sensorNum, HIGH);
  distance = (duration/2) / 29.1; //29.1 is a magic number - need to verify and justify
  return distance;
}

//sets the target orientation given the rotation amount and the current orientation
void setRelativeTargetRotation(double degreeDelta, double current)
{
  targetXOrientation = fmod(current + degreeDelta, 360.0);
}

//gets the amount of rotation needed(?)
double getTargetRotationDelta(double currentRotation)
{
  double inDelta = targetXOrientation - currentRotation;
  double outDelta = inDelta;
  double absDelta = abs(inDelta);

  if (absDelta > 180)
  {
    outDelta = 360 - absDelta;
    if (inDelta > 0)
    {
      outDelta = -outDelta;
    }
  }

  return outDelta;
}

//updates the required power amounts to get the motor moving (?)
void updateTargetRotationLoop(double currentRotation, double &leftPowerOut, double &rightPowerOut)
{
  const double minSpeed = 100;
  const double maxSpeed = 255;
  const double minSpeedDegrees = 1; //delta required to warrant moving at all
  const double maxSpeedDegrees = 30; //delta required to warrant going full speed
  
  double delta = getTargetRotationDelta(currentRotation);
  double deltaIntensity = constrain(abs(delta)/maxSpeedDegrees, 0.0, 1.0);

  double power = 0;
  if (abs(delta) >= minSpeedDegrees)
  {
    power = lerp(minSpeed, maxSpeed, deltaIntensity) * sign(delta);
  }
  else
  {
    power = 0;
  }

  //Assumes positive means forward
  leftPowerOut = power;
  rightPowerOut = -power;
}

//linearly interpolates from a to b based on percent (0-1)
double lerp(double a, double b, double percent)
{
  return a + (b-a)*percent;
}

//calculates the sign of an input
int sign(double input)
{
  if (input < 0)
  {
    return -1;
  }
  else if (input > 0)
  {
    return 1;
  }
  else
  {
    return 0;
  }
}

//moves forward 
void driveForward()
{
  
}

void setup() {
  // put your setup code here, to run once:
  Serial.begin(9600);
  //Setup for the sensors
  pinMode(trigPin, OUTPUT);
  pinMode(echoPin, INPUT);
  //Setup for the motors
  digitalWrite(trigPin, LOW); //initialize
  digitalWrite(echoPin, LOW); //initialize

  Serial.println("Orientation Sensor Test"); Serial.println("");

  /* Initialise the sensor */
  if(!bno.begin())
  {
    /* There was a problem detecting the BNO055 ... check your connections */
    Serial.print("Ooops, no BNO055 detected ... Check your wiring or I2C ADDR!");
    while(1);
  }
  AFMS.begin();

}

unsigned long beginStraightMillis = 0;

void loop() {
  // put your main code here, to run repeatedly:

  float distance1 = getDistance(echoPin); //get the distance from a sensor - can be adapted to multiple sensors

  if(distance1 <= 30) { //too close to an obstacle
    //do turning, other logic checking, etc
    double currentRotation = event.orientation.x;
    double deltaFromTarget = getTargetRotationDelta(currentRotation);
    
    if (abs(deltaFromTarget) < 1)
    {
      setRelativeTargetRotation(90, currentRotation);
      beginStraightMillis = millis();
      Serial.println("Set new target rotation");
    }
    else
    {
      double leftPower;
      double rightPower;
      updateTargetRotationLoop(currentRotation, leftPower, rightPower);
    
      leftMotor->setSpeed(leftPower);
      leftMotor->run(FORWARD);
      rightMotor->setSpeed(rightPower);
      rightMotor->run(FORWARD);

      Serial.print("currentRotation="); Serial.print(currentRotation);
      Serial.print(" | deltaFromTarget="); Serial.print(deltaFromTarget);
      Serial.print(" | leftPower="); Serial.print(leftPower);
      Serial.print(" | rightPower="); Serial.print(rightPower);
      Serial.println("");
    }
    while(!(millis() - beginStraightMillis < straightDuration))
    {delay(500);} //wait for the turn to complete before moving on
    //continue moving forward
    leftMotor->setSpeed(255);
    rightMotor->setSpeed(255);
    
  }
  else { //safe distance away
    //do nothing - keep on keeping on
  }


  delay(500); //wait 1/2 second before checking again

}

 

