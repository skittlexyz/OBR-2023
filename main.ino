/* Código OBR 2023 - Nióbio */

/* General configurations section */
// WIP
const int velocity = 300;

/* Pin declaration section */
// IR Sensor pins
uint8_t irSensorPins[] = {};
// Grouped Stepper Motor pins {step pin, direction pin}, enable pin
uint8_t stepperPins[5] = {2,5,3,6,8};

/* Function declaration section */
// IR Sensor Reader receives the sensor pin
int irSensorRead(int pin);

// Stepper Controller receiver if the movement will be in a cross axis, the delay between the steps, the step quantity, the direction (F, B, L, R) if the cross is true and the angle if it isn't.
bool stepperControl(bool cross, int velocity, int steps, int direction, int angle);
// Servo Controller receives which servo to control and the movement method
bool servoControl(int current, char method);
// Prints all the important info
void infoPrint();
// Try the function
bool tryFunction(bool current, String name, String error);
// Pins configuration setup
bool setupPins();

/* Main code section */
void setup() {
  Serial.begin(115200);
  infoPrint();
  tryFunction(setupPins(), "setupPins()", "error");
}
void loop() {
  stepperControl(true, velocity, 500, 0, 0);
  stepperControl(true, velocity, 500, 1, 0);
  stepperControl(true, velocity, 500, 2, 0);
  stepperControl(true, velocity, 500, 3, 0);
}

/* Functions section */
int irSensorRead(int pin) {
  return 0;
}
bool stepperControl(bool cross, int velocity, int steps, int direction, int angle) {
  if (cross) {
    for (int i = 0; i < steps; i++) {
      switch (direction) {
      case 0:
        digitalWrite(stepperPins[1], HIGH);
        digitalWrite(stepperPins[3], LOW);
        digitalWrite(stepperPins[0], HIGH);
        digitalWrite(stepperPins[2], HIGH);
        delay(velocity);
        digitalWrite(stepperPins[0], LOW);
        digitalWrite(stepperPins[2], LOW);
        delay(velocity);      
        break;
      case 1:
        digitalWrite(stepperPins[1], LOW);
        digitalWrite(stepperPins[3], HIGH);
        digitalWrite(stepperPins[0], HIGH);
        digitalWrite(stepperPins[2], HIGH);
        delay(velocity);
        digitalWrite(stepperPins[0], LOW);
        digitalWrite(stepperPins[2], LOW);
        delay(velocity);      
        break;
      case 2:
        digitalWrite(stepperPins[1], HIGH);
        digitalWrite(stepperPins[3], HIGH);
        digitalWrite(stepperPins[0], HIGH);
        digitalWrite(stepperPins[2], HIGH);
        delay(velocity);
        digitalWrite(stepperPins[0], LOW);
        digitalWrite(stepperPins[2], LOW);
        delay(velocity);      
        break;
      case 3:
        digitalWrite(stepperPins[1], LOW);
        digitalWrite(stepperPins[3], LOW);
        digitalWrite(stepperPins[0], HIGH);
        digitalWrite(stepperPins[2], HIGH);
        delay(velocity);
        digitalWrite(stepperPins[0], LOW);
        digitalWrite(stepperPins[2], LOW);
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
bool servoControl(int current, char method) {
  return true;
}
bool tryFunction(bool current, String name, String error) {
  while (!current) {
    Serial.println(name + " " + error + "!");
    while (true) {}
  }
  return true;
}
bool setupPins() {
  for (int i = 0; i < sizeof(stepperPins) - 1; i++) {
    pinMode(stepperPins[i], OUTPUT);
  }
  pinMode(stepperPins[sizeof(stepperPins) - 1], OUTPUT);
  for (int i = 0; i < sizeof(irSensorPins); i++) {
    pinMode(irSensorPins[i], INPUT);
  }
  return true;
}