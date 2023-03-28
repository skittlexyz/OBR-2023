/* Código OBR 2023 - Nióbio */

/* General configurations section */
// WIP

/* Pin declaration section */
// IR Sensor pins
byte irSensorPins [] = {};
// Grouped Stepper Motor pins {step pin, direction pin}, enable pin
byte stepperPins [] = {{2,5},{3,6},8};

/* Function declaration section */
// IR Sensor Reader receives the sensor pin
int irSensorRead(byte pin);
// Stepper Controller receiver if the movement will be in a cross axis, the step quantity, the direction (F, B, L, R) if the cross is true and the angle if it isn't.
bool stepperControl(bool cross, int steps, char direction, int angle);
// Servo Controller receives which servo to control and the movement method
bool servoControl(byte current, char method);
// Pins configuration setup
bool setupPins();

/* Main code section */
void setup() {
  setupPins();
}
void loop() {
  stepperControl(true, 500, "F", 0);
  stepperControl(true, 500, "B", 0);
  stepperControl(true, 500, "L", 0);
  stepperControl(true, 500, "R", 0);
}

/* Functions section */
int irSensorRead(byte pin) {
  return 0;
}
bool stepperControl(bool cross, int steps, char direction, int angle) {
  bool directionToggleAssistant = HIGH;
  if (cross) {
    for (int i = 0; i < steps; i++) {
      switch (direction) {
      case "F":
        digitalWrite(stepperPins[0][1], directionToggleAssistant);
        digitalWrite(stepperPins[1][1], directionToggleAssistant);
        digitalWrite(stepperPins[0][0], HIGH);
        digitalWrite(stepperPins[1][0], HIGH);
        delay(1);
        digitalWrite(stepperPins[0][0], LOW);
        digitalWrite(stepperPins[1][0], LOW);
        delay(1);      
        break;
      case "B":
        digitalWrite(stepperPins[0][1], !directionToggleAssistant);
        digitalWrite(stepperPins[1][1], !directionToggleAssistant);
        digitalWrite(stepperPins[0][0], HIGH);
        digitalWrite(stepperPins[1][0], HIGH);
        delay(1);
        digitalWrite(stepperPins[0][0], LOW);
        digitalWrite(stepperPins[1][0], LOW);
        delay(1);      
        break;
      case "L":
        digitalWrite(stepperPins[0][1], directionToggleAssistant);
        digitalWrite(stepperPins[1][1], !directionToggleAssistant);
        digitalWrite(stepperPins[0][0], HIGH);
        digitalWrite(stepperPins[1][0], HIGH);
        delay(1);
        digitalWrite(stepperPins[0][0], LOW);
        digitalWrite(stepperPins[1][0], LOW);
        delay(1);      
        break;
      case "R":
        digitalWrite(stepperPins[0][1], !directionToggleAssistant);
        digitalWrite(stepperPins[1][1], directionToggleAssistant);
        digitalWrite(stepperPins[0][0], HIGH);
        digitalWrite(stepperPins[1][0], HIGH);
        delay(1);
        digitalWrite(stepperPins[0][0], LOW);
        digitalWrite(stepperPins[1][0], LOW);
        delay(1);      
        break;
      }
    }
    return true;
  } else {
    return true;
  }
  return false;
}
bool servoControl(byte current, char method) {
  return true;
}
bool setupPins() {
  for (int i = 0; i < getSize(stepperPins) - 1; i++) {
    for (int j = 0; j < getSize(stepperPins[i]); j++) {
      pinMode(stepperPins[i][j], OUTPUT);
    }
  }
  pinMode(stepperPins[getSize(stepperPins) - 1], OUTPUT);
  for (int i = 0; i < getSize(irSensorPins); i++) {
    pinMode(irSensorPins[i], INPUT);
  }
  return true;
}