/* Código OBR 2023 - Nióbio */

/* Libraries section */
#include <U8g2lib.h>
#include "Adafruit_TCS34725softi2c.h"
#include <MPU6050_light.h>
#include "I2Cdev.h"
#include "Wire.h"

/* General configurations section */
// Delay between each stepper step
int stepperVelocity = 2; // ms/step
int slowedStepperVelocity = 5;
int stepperVelocityBackup = stepperVelocity;
#define colorVelocity TCS34725_INTEGRATIONTIME_50MS

/* Pin declaration and variables section */
// IR Sensor pins
uint8_t irSensorPins[8] = {34,35,36,37,38,39,40,41};
// Ultrassonic Pins {trig, echo, ...}
uint8_t ultraPins[8] = {42,43,44,45,46,47,48,49}; 
// Grouped Stepper Motor pins {step pin, direction pin, ..., enable pin}
uint8_t stepperPins[5] = {2,5,3,6,8};
// TCS34725 Pins {sda, scl, ...}
uint8_t colorPins[4] = {50,51,52,53};

/* Objects declaration*/
// TCS34725
Adafruit_TCS34725softi2c leftColor = Adafruit_TCS34725softi2c(colorVelocity, TCS34725_GAIN_1X, colorPins[0], colorPins[1]);
Adafruit_TCS34725softi2c rightColor = Adafruit_TCS34725softi2c(colorVelocity, TCS34725_GAIN_1X, colorPins[2], colorPins[3]);
// MPU6050
MPU6050 mpu(Wire);

/* Function declaration section */
// IR Sensor Reader receives the sensor pin
bool irSensorRead(int pin);
// IR Sensor Reader from index to index, returns true if all the sensors are in black
bool arrayIrRead(int minorIndex, int majorIndex);
// Ultrassonic Sensor Reader receives the trigger and the echo pin
float ultraSensorRead(int trigPin, int echoPin);
// Color Sensor Reader receives the side to be read (0: Left, 1: Right) and the color to be read (0: Red, 1: Green, 2: Blue, 3: Clarity)
int colorSensorRead(int side, int color);
// MPU6050 Sensor Reader receives the axis to return (0: x, 1: y, 2: z)
float mpuSensorRead(int axis);
// Stepper Controller receiver if the movement will be in a cross axis, the delay between the steps, the step quantity, the direction (Left: 0, Right: 1, Forward: 2, Backward: 3) if the cross is true and the angle if it isn't
bool stepperControl(int velocity, int steps, int direction);
// Try the function
bool tryFunction(bool current, String name, String error);
// Pins configuration setup
bool pinsSetup();
// TCS34725 setup
bool colorSetup();
// MPU6050 setup
bool mpuSetup();

/* Main code section */
void setup() {
  Wire.begin();
  Serial.begin(115200);
  tryFunction(pinsSetup(), "pinsSetup()", "Pins setup Error");
  tryFunction(colorSetup(), "colorSetup()", "TCS34725 Malfunction");
  //tryFunction(mpuSetup(), "mpuSetup()", "MPU6050 Malfunction");
}
void loop() {/*
  Serial.print("\nInfrared | ");
  for (int i = 0; i < sizeof(irSensorPins); i++) {
    Serial.print(i);Serial.print(": ");Serial.print(irSensorRead(irSensorPins[i]));Serial.print(" | ");
  }
  Serial.print("\nColor | ");
  for (int i = 0; i < sizeof(colorPins) / 2; i++) {
    Serial.print(i);Serial.print(": R: ");Serial.print(colorSensorRead(i, 0));Serial.print(" G: ");Serial.print(colorSensorRead(i, 1));Serial.print(" B: ");Serial.print(colorSensorRead(i, 2));Serial.print(" | ");
  }
  Serial.print("\n");*/
  for (int i = 0; i < sizeof(irSensorPins); i++)
  {
    Serial.print(irSensorRead(irSensorPins[i]));
    Serial.print(" ");
  }
  Serial.print("\n");
  

  if /* FORWARD */((irSensorRead(irSensorPins[4]) || irSensorRead(irSensorPins[3])) && !arrayIrRead(5, 7) && !arrayIrRead(0, 2)) {
      Serial.print("\nForward ");
      stepperControl(stepperVelocity, 10, 2);
  } else if /* GAP */((!irSensorRead(irSensorPins[4]) && !irSensorRead(irSensorPins[3])) && !arrayIrRead(5, 6) && !arrayIrRead(1, 2)) {
    Serial.print("\nForward Gap ");
    stepperControl(stepperVelocity, 10, 2);
  } else if /* SMOOTH LEFT */(((irSensorRead(irSensorPins[4]) || irSensorRead(irSensorPins[3])) && arrayIrRead(5, 6) && !arrayIrRead(1, 2)) || (arrayIrRead(5, 6) && !arrayIrRead(1, 2))) { 
    stepperVelocity = slowedStepperVelocity;
    stepperControl(stepperVelocity, 5, 1);
    Serial.print("\nSmooth Left ");
    stepperVelocity = stepperVelocityBackup;
  } else if /* SMOOTH RIGHT */(((irSensorRead(irSensorPins[4]) || irSensorRead(irSensorPins[3])) && !arrayIrRead(5, 6) && arrayIrRead(1, 2)) || (arrayIrRead(5, 6) && !arrayIrRead(1, 2))) {
    stepperVelocity = slowedStepperVelocity;
    stepperControl(stepperVelocity, 5, 0);
    Serial.print("\nSmooth Right ");
    stepperVelocity = stepperVelocityBackup;
  } else if /* LEFT */((arrayIrRead(2,7) && !irSensorRead(irSensorPins[0])) || arrayIrRead(4,7)) {
    stepperVelocity = slowedStepperVelocity;
    stepperControl(stepperVelocity, 5, 1);
    Serial.print("\nLeft ");
    stepperVelocity = stepperVelocityBackup;
  } else if /* RIGHT */((arrayIrRead(0,5) && !irSensorRead(irSensorPins[7])) || arrayIrRead(0,3)) {
    stepperVelocity = slowedStepperVelocity;
    stepperControl(stepperVelocity, 5, 1);
    Serial.print("\nLeft ");
    stepperVelocity = stepperVelocityBackup;
  }
  
  //stepperControl(stepperVelocity, 100, 0);
  //delay(150);
  /*
  * stepperControl(stepperVelocity, 250, 0);
  * stepperControl(stepperVelocity, 250, 1);
  * stepperControl(stepperVelocity, 250, 2);
  * stepperControl(stepperVelocity, 250, 3);
  */
}

/* Functions section */
bool irSensorRead(int pin) {
  return digitalRead(pin);
}
bool arrayIrRead(int minorIndex, int majorIndex) {
  for (int i = minorIndex; i <= majorIndex; i++) {
    if (!irSensorRead(irSensorPins[i])) {
      return false;
    }
  }
  return true;
}
float ultraSensorRead(int trigPin, int echoPin) {
  digitalWrite(trigPin, LOW);
  delayMicroseconds(2);
  digitalWrite(trigPin, HIGH);
  delayMicroseconds(10);
  digitalWrite(trigPin, LOW);
  return pulseIn(echoPin, HIGH) * 0.0343 / 2;
}
int colorSensorRead(int side, int color) {
  uint16_t r, g, b, c;
  switch (side) {
  case 0:
    leftColor.getRawData(&r, &g, &b, &c);
    switch (color) {
    case 0:
      return r;
      break;
    case 1:
      return g;
      break;
    case 2:
      return b;
      break;
    case 3:
      return c;
      break;
    }
    break;  
  case 1:
    rightColor.getRawData(&r, &g, &b, &c);
    switch (color) {
    case 0:
      return r;
      break;
    case 1:
      return g;
      break;
    case 2:
      return b;
      break;
    case 3:
      return c;
      break;
    }
    break;
  }
}
float mpuSensorRead(int axis) {
  mpu.update();
  switch (axis)
  {
  case 0:
    return mpu.getAngleX();
    break;
  case 1:
    return mpu.getAngleY();
    break;
  case 2:
    return mpu.getAngleZ();
    break;
  }
}
bool stepperControl(int velocity, int steps, int direction) {
  for (int i = 0; i < steps; i++) {
    switch (direction) {
      case 0:
        digitalWrite(stepperPins[1], HIGH);digitalWrite(stepperPins[3], HIGH);
        digitalWrite(stepperPins[0], HIGH);digitalWrite(stepperPins[2], HIGH);
        delay(velocity);
        digitalWrite(stepperPins[0], LOW);digitalWrite(stepperPins[2], LOW);
        delay(velocity);      
        break;
      case 1:
        digitalWrite(stepperPins[1], LOW);digitalWrite(stepperPins[3], LOW);
        digitalWrite(stepperPins[0], HIGH);digitalWrite(stepperPins[2], HIGH);
        delay(velocity);
        digitalWrite(stepperPins[0], LOW);digitalWrite(stepperPins[2], LOW);
        delay(velocity);      
        break;
      case 2:
        digitalWrite(stepperPins[1], HIGH);digitalWrite(stepperPins[3], LOW);
        digitalWrite(stepperPins[0], HIGH);digitalWrite(stepperPins[2], HIGH);
        delay(velocity);
        digitalWrite(stepperPins[0], LOW);digitalWrite(stepperPins[2], LOW);
        delay(velocity);      
        break;
      case 3:
        digitalWrite(stepperPins[1], LOW);digitalWrite(stepperPins[3], HIGH);
        digitalWrite(stepperPins[0], HIGH);digitalWrite(stepperPins[2], HIGH);
        delay(velocity);
        digitalWrite(stepperPins[0], LOW);digitalWrite(stepperPins[2], LOW);
        delay(velocity);      
        break;
    }
  }
  return true;
}
bool tryFunction(bool current, String name, String error) {
  while (!current) {
    Serial.println(name + " - " + error + "!");
    while (true) {}
  }
  return true;
}
bool pinsSetup() {
  for (int i = 0; i < sizeof(stepperPins) - 1; i++) {
    pinMode(stepperPins[i], OUTPUT);
  }
  pinMode(stepperPins[sizeof(stepperPins) - 1], OUTPUT);
  for (int i = 0; i < sizeof(irSensorPins); i++) {
    pinMode(irSensorPins[i], INPUT);
  }
  for (int i = 0; i < 8; i += 2) {
    pinMode(ultraPins[i], OUTPUT);
    pinMode(ultraPins[i + 1], INPUT);
  }
  return true;
}
bool colorSetup() {
  if (leftColor.begin() && rightColor.begin()) {
    return true;
  } else {
    return false;
  }
}
bool mpuSetup() {
  byte status = mpu.begin();
  delay(50);
  if (status != 0) {
    return false;
  } else {
    mpu.calcOffsets();
    return true;
  }
}