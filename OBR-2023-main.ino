/* Código OBR 2023 - Nióbio */

/* Libraries section */
#include <U8g2lib.h>
#include "Adafruit_TCS34725softi2c.h"
#include <MPU6050_light.h>
#include "I2Cdev.h"
#include "Wire.h"

/* General configurations section */
// Delay between each stepper step
#define stepperVelocity 1
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
int irSensorRead(int pin);
// Ultrassonic Sensor Reader receives the trigger and the echo pin
float ultraSensorRead(int trigPin, int echoPin);
// Color Sensor Reader receives the side to be read (0: Left, 1: Right) and the color to be read (0: Red, 1: Green, 2: Blue, 3: Clarity)
int colorSensorRead(int side, int color);
// MPU6050 Sensor Reader receives the axis to return (0: x, 1: y, 2: z)
float mpuSensorRead(int axis);
// Stepper Controller receiver if the movement will be in a cross axis, the delay between the steps, the step quantity, the direction (0, 1, 2, 3) if the cross is true and the angle if it isn't
bool stepperControl(bool cross, int velocity, int steps, int direction, int angle);
// Servo Controller receives which servo to control and the movement method
bool servoControl(int current, char method);
// Prints all the important info in serial monitor
void infoPrint();
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
  infoPrint();
  tryFunction(pinsSetup(), "pinsSetup()", "Pins setup Error");
  tryFunction(colorSetup(), "colorSetup()", "TCS34725 Malfunction");
  tryFunction(mpuSetup(), "mpuSetup()", "MPU6050 Malfunction");
}
void loop() {
  Serial.print("\n");
  Serial.print("IR | ");
  for (int i = 0; i < sizeof(irSensorPins); i++) {
    Serial.print(i);
    Serial.print(": ");
    Serial.print(irSensorRead(irSensorPins[i]));
    Serial.print(" | ");
  }

  Serial.print("\n");
  Serial.print("COLOR | ");
  for (int i = 0; i < sizeof(colorPins) / 2; i++) {
    Serial.print(i);
    Serial.print(": R: ");
    Serial.print(colorSensorRead(i, 0));
    Serial.print(" G: ");
    Serial.print(colorSensorRead(i, 1));
    Serial.print(" B: ");
    Serial.print(colorSensorRead(i, 2));
    Serial.print(" | ");
  }

  Serial.print("\n");
  Serial.print("ULTRA | ");
  for (int i = 0; i < sizeof(ultraPins); i+=2) {
    Serial.print(i);
    Serial.print(": ");
    Serial.print(ultraSensorRead(ultraPins[i], ultraPins[i+1]));
    Serial.print(" | ");
  }
  
  Serial.print("\n");
  Serial.print("GYRO | ");
  for (int i = 0; i < 3; i++)
  {
    Serial.print(i);
    Serial.print(": ");
    Serial.print(mpuSensorRead(i));
    Serial.print(" | ");
  }
  
  delay(2500);
}

/* Functions section */
int irSensorRead(int pin) {
  return digitalRead(pin);
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
void infoPrint() {
  Serial.print("IR Pins: ");
  for (int i = 0; i < sizeof(irSensorPins); i++) {
    Serial.print(irSensorPins[i] );
    Serial.print(" ");
  }
  Serial.print("\nStepper Pins: ");
  for (int i = 0; i < sizeof(stepperPins) - 1; i++) {
    Serial.print(stepperPins[i]);
    Serial.print(" ");
  }
  Serial.print("\nStepper Velocity: ");
  Serial.print(stepperVelocity, DEC);
  Serial.println("\n");
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