#include <ArduinoRobot.h>
#include <Wire.h>

#define led 11 //red
#define uhoh 7

void setup() {
  Serial.begin (9600);
  Robot.begin();
  pinMode(led, OUTPUT);
  pinMode(uhoh, INPUT);
  
}

void loop() {
  int number;
  while (digitalRead(uhoh) == HIGH) {
    
    Robot.motorsStop(); // stop motors if close to wall
    delay(1000);
    Robot.motorsWrite(-255,-255); //reverse full speed back
    delay(1000); //backing up 1 seconds
    Robot.motorsStop(); //stop motors
    delay(1000);
    
    Robot.motorsWrite(255,-255); //turning right
    delay(1000);
    Robot.motorsStop(); //stop
    delay(1000);
    
    if (digitalRead(uhoh) == HIGH) {
      Robot.motorsWrite(-255,255); //if still a wall turn left 180degrees
      delay(2000); //one second will turn back forward next second will turn left
      Robot.motorsStop();
      delay(1000);
      
      if (digitalRead(uhoh) == HIGH) {
        Robot.motorsWrite(-255,255); //continuing turning left until opp of original orientation
        delay(1000);
        Robot.motorsStop();
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
  Robot.motorsWrite(255,255) //full speed forward
  delay(100);
}
