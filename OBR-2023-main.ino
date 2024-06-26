/* Código OBR 2023 - Nióbio */

/* Libraries section */
#include <U8g2lib.h>
#include "Adafruit_TCS34725softi2c.h"
#include <MPU6050_light.h>
#include "I2Cdev.h"
#include "Wire.h"

/* General configurations section */
int stepperVelocity = 2; // ms/step
int slowedStepperVelocity = 5;
int primarySteps = 20;
int secundarySteps = 5;
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
bool positiveArrayIrRead(int minorIndex, int majorIndex);
// IR Sensor Reader from index to index, returns true if all the sensors are in white
bool negativeArrayIrRead(int minorIndex, int majorIndex);
// IR Sensor Reader from index to index, returns true if some of the sensors are in black
bool someArrayIrRead(int minorIndex, int majorIndex);
// Ultrassonic Sensor Reader receives the trigger and the echo pin
float ultraSensorRead(int trigPin, int echoPin);
// Color Sensor Reader receives the side to be read (0: Left, 1: Right) and the color to be read (0: Red, 1: Green, 2: Blue, 3: Clarity)
int colorSensorRead(int side, int color);
// MPU6050 Sensor Reader receives the axis to return (0: x, 1: y, 2: z)
float mpuSensorRead(int axis);
// Stepper Controller receiver if the movement will be in a cross axis, the delay between the steps, the step quantity, the direction (Left: 0, Right: 1, Forward: 2, Backward: 3) if the cross is true and the angle if it isn't
bool stepperControl(int velocity, int steps, int direction);
// Check a T case
bool checkTCase(bool back);
// Straight Turn receives the side to make a turn
void straightTurn(int side);
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
void loop() {

  for (int i = sizeof(irSensorPins) - 1; i >= 0; i--) { Serial.print(irSensorRead(irSensorPins[i]));Serial.print(" "); }
  Serial.print("\n");Serial.print(greenCheck(0));Serial.print(greenCheck(1));
  
  if /* 1 */(positiveArrayIrRead(0,7)) {
    if /* DG */(greenCheck(0) && greenCheck(1)) {
      Serial.println("aikshdj");
      stepperControl(slowedStepperVelocity, primarySteps * 100, 0);
    } else if /* LG */(greenCheck(0)) {
      stepperControl(slowedStepperVelocity, primarySteps * 10, 0);
      stepperControl(stepperVelocity, primarySteps * 4, 2);
    } else if /* RG */(greenCheck(1)) {
      stepperControl(slowedStepperVelocity, primarySteps * 10, 1);
      stepperControl(stepperVelocity, primarySteps * 4, 2);
    } else if /* NG */(!greenCheck(0) && !greenCheck(1)) {
      stepperControl(stepperVelocity, primarySteps * 4, 2);
    }
    Serial.print(" 1");
  } else if /* 2L */(positiveArrayIrRead(3,7) && negativeArrayIrRead(0,1)) {
    if /* LG */(greenCheck(0)) {
      stepperControl(slowedStepperVelocity, primarySteps * 10, 0);
      stepperControl(stepperVelocity, primarySteps * 4, 2);
    } else if /* NG */(!greenCheck(0) && !greenCheck(1)) {
      if(checkTCase(true)) {
        stepperControl(stepperVelocity, primarySteps * 4, 2);
      } else {
        stepperControl(slowedStepperVelocity, primarySteps * 10, 0);
        stepperControl(stepperVelocity, primarySteps * 4, 2);
      }
    }
    Serial.print(" 2L");
  } else if /* 2R */(positiveArrayIrRead(0,4) && negativeArrayIrRead(6,7)) {
    if /* RG */(greenCheck(1)) {
      stepperControl(slowedStepperVelocity, primarySteps * 10, 1);
      stepperControl(stepperVelocity, primarySteps * 4, 2);
    } else if /* NG */(!greenCheck(0) && !greenCheck(1)) {
      if(checkTCase(true)) {
        stepperControl(stepperVelocity, primarySteps * 4, 2);
      } else {
        stepperControl(slowedStepperVelocity, primarySteps * 10, 1);
        stepperControl(stepperVelocity, primarySteps * 4, 2);
      }
    }
    Serial.print(" 2R");
  } else if /* 3L */(!positiveArrayIrRead(3,4) && !negativeArrayIrRead(5,7)) {
    stepperControl(slowedStepperVelocity, primarySteps, 0);
    Serial.print(" 3L");
  } else if /* 3R */(!positiveArrayIrRead(3,4) && !negativeArrayIrRead(0,2)) {
    stepperControl(slowedStepperVelocity, primarySteps , 1);
    Serial.print(" 3R");
  } else if /* 4 */(!negativeArrayIrRead(1,6) && !irSensorRead(irSensorPins[0]) && !irSensorRead(irSensorPins[7])) {
    stepperControl(stepperVelocity, primarySteps, 2);
    Serial.print(" 4");
  } else if /* 5 */(negativeArrayIrRead(0,7)) {
    stepperControl(stepperVelocity, primarySteps, 2);
    Serial.print(" 5");
  }
  Serial.print("\n");
}

/* Functions section */
bool irSensorRead(int pin) {
  return digitalRead(pin);
}
bool positiveArrayIrRead(int minorIndex, int majorIndex) {
  for (int i = minorIndex; i <= majorIndex; i++) {
    if (!irSensorRead(irSensorPins[i])) {
      return false;
    }
  }
  return true;
}
bool negativeArrayIrRead(int minorIndex, int majorIndex) {
  for (int i = minorIndex; i <= majorIndex; i++) {
    if (irSensorRead(irSensorPins[i])) {
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
bool greenCheck(int side) {
  if (side == 0 && colorSensorRead(0,1) < 200 && colorSensorRead(0,1) > colorSensorRead(0,0) && colorSensorRead(0,1) > colorSensorRead(0,2)) {
    return true;
  }
  if (side == 1 && colorSensorRead(1,1) < 200 && colorSensorRead(1,1) > colorSensorRead(1,0) && colorSensorRead(1,1) > colorSensorRead(1,2)) {
    return true;
  }
  return false;
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
      case 0: // left
        digitalWrite(stepperPins[1], LOW);digitalWrite(stepperPins[3], LOW);
        digitalWrite(stepperPins[0], HIGH);digitalWrite(stepperPins[2], HIGH);
        delay(velocity);
        digitalWrite(stepperPins[0], LOW);digitalWrite(stepperPins[2], LOW);
        delay(velocity);      
        break;
      case 1: // right
        digitalWrite(stepperPins[1], HIGH);digitalWrite(stepperPins[3], HIGH);
        digitalWrite(stepperPins[0], HIGH);digitalWrite(stepperPins[2], HIGH);
        delay(velocity);
        digitalWrite(stepperPins[0], LOW);digitalWrite(stepperPins[2], LOW);
        delay(velocity);      
        break;
      case 2: // forward
        digitalWrite(stepperPins[1], HIGH);digitalWrite(stepperPins[3], LOW);
        digitalWrite(stepperPins[0], HIGH);digitalWrite(stepperPins[2], HIGH);
        delay(velocity);
        digitalWrite(stepperPins[0], LOW);digitalWrite(stepperPins[2], LOW);
        delay(velocity);      
        break;
      case 3: // backward
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
bool checkTCase(bool back) {
  stepperControl(stepperVelocity, 100, 2);
  if (!negativeArrayIrRead(2,5)) {
      if (back) {
      stepperControl(stepperVelocity, 100, 3);
    }
    return true;
  }
  if (back) {
    stepperControl(stepperVelocity, 100, 3);
  }
  return false;
}
void straightTurn(int side) {
  stepperControl(stepperVelocity, 100, 2);
  do {
    stepperControl(slowedStepperVelocity, secundarySteps * 4, side);
  } while (!positiveArrayIrRead(3,4) && (!positiveArrayIrRead(0,2) || !positiveArrayIrRead(5,7)));
  stepperControl(slowedStepperVelocity, secundarySteps * 2, side);
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