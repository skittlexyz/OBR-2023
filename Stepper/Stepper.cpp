#include "Stepper.h"
#include "Arduino.h"

Stepper:Stepper(int stepPinA, int directionPinA, int stepPinB, int directionPinB, int enablePin) {
  int _stepPinA = stepPinA;
  int directionPinA = directionPinA;
  int _stepPinB = stepPinB;
  int directionPinB = directionPinAB
  int _enablePin = enablePin;
  pinMode(_stepPinA, OUTPUT);
  pinMode(_directionPinA, OUTPUT);
  pinMode(_stepPinB, OUTPUT);
  pinMode(_directionPinB, OUTPUT);
  pinMode(_enablePin, OUTPUT);
}
bool Stepper:run(bool cross, int velocity, int steps, int direction, int angle) {
  if (cross) {
    for (int i = 0; i < steps; i++) {
      switch (direction) {
      case 0:
        digitalWrite(directionPinA, HIGH);
        digitalWrite(directionPinB, LOW);
        digitalWrite(stepPinA, HIGH);
        digitalWrite(stepPinB, HIGH);
        delay(velocity);
        digitalWrite(stepPinA, LOW);
        digitalWrite(stepPinB, LOW);
        delay(velocity);      
        break;
      case 1:
        digitalWrite(directionPinA, LOW);
        digitalWrite(directionPinB, HIGH);
        digitalWrite(stepPinA, HIGH);
        digitalWrite(stepPinB, HIGH);
        delay(velocity);
        digitalWrite(stepPinA, LOW);
        digitalWrite(stepPinB, LOW);
        delay(velocity);      
        break;
      case 2:
        digitalWrite(directionPinA, HIGH);
        digitalWrite(directionPinB, HIGH);
        digitalWrite(stepPinA, HIGH);
        digitalWrite(stepPinB, HIGH);
        delay(velocity);
        digitalWrite(stepPinA, LOW);
        digitalWrite(stepPinB, LOW);
        delay(velocity);      
        break;
      case 3:
        digitalWrite(directionPinA, LOW);
        digitalWrite(directionPinB, LOW);
        digitalWrite(stepPinA, HIGH);
        digitalWrite(stepPinB, HIGH);
        delay(velocity);
        digitalWrite(stepPinA, LOW);
        digitalWrite(stepPinB, LOW);
        delay(velocity);      
        break;
      }
    }
    return true;
  } else {
    return true;
  }
  return false;
}