#include "Stepper.h"
#include "Arduino.h"

class Stepper {
  public:
    Stepper(int stepPinA, int directionPinA, int stepPinB, int directionPinB, int enablePin);
    bool run(bool cross, int velocity, int steps, int direction, int angle);
  private:
    int _stepPinA;
    int _directionPinA;
    int _stepPinB;
    int _directionPinB;
    int _enablePin;
}