/* Código OBR 2023 - Nióbio */

/* Libraries section */
#include <U8g2lib.h>
#include <Wire.h>

/* General configurations section */
// Delay between each stepper step
const int velocity = 5;

/* Objects declaration*/
// OLED 128x64
U8G2_SSD1306_128X64_NONAME_F_HW_I2C display(U8G2_R0, /* reset=*/U8X8_PIN_NONE);


/* Pin declaration section */
// IR Sensor pins
uint8_t irSensorPins[] = {0};
// Grouped Stepper Motor pins {step pin, direction pin}, enable pin
uint8_t stepperPins[5] = {2,5,3,6,8};

/* Function declaration section */
// IR Sensor Reader receives the sensor pin
int irSensorRead(int pin);
// Stepper Controller receiver if the movement will be in a cross axis, the delay between the steps, the step quantity, the direction (0, 1, 2, 3) if the cross is true and the angle if it isn't
bool stepperControl(bool cross, int velocity, int steps, int direction, int angle);
// Servo Controller receives which servo to control and the movement method
bool servoControl(int current, char method);
// Prints all the important info
void infoPrint();
// Display Controller receives the mode (0: Startup, 1: Console, 2: Errors, 3: Logo, 4: Clear), the text if needed, the line to print it and the delay size between the buffers
bool displayControl(int mode, String text, int line, int delayTime);
// Try the function
bool tryFunction(bool current, String name, String error);
// Pins configuration setup
bool pinsSetup();
// OLED 128x64 setup
bool oledSetup();

/* Main code section */
void setup() {
  Serial.begin(115200);
  tryFunction(pinsSetup(), "pinsSetup()", "error");
  tryFunction(oledSetup(), "oledSetup()", "error");
  infoPrint();
  displayControl(0,"",0,0);
  displayControl(4,"",0,2000);
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
void infoPrint() {
  Serial.print("Código OBR 2023 - Nióbio\n");
  Serial.print("├ Pinos:\n");
  Serial.print("│ ├ IR: ");
  for (int i = 0; i < sizeof(irSensorPins) - 1; i++) {
  Serial.print(irSensorPins[i] );
    Serial.print(", ");
  }
  Serial.print(irSensorPins[sizeof(irSensorPins) - 1]);
  Serial.print("\n│ └ Stepper: ");
  for (int i = 0; i < sizeof(stepperPins) - 1; i++) {
    Serial.print(stepperPins[i]);
    Serial.print(", ");
  }
  Serial.print(stepperPins[sizeof(stepperPins) - 1]);
  Serial.print("\n└ Constantes:\n");
  Serial.print("  └ Velocidade: ");
  Serial.print(velocity);
  Serial.println("\n");
}
bool displayControl(int mode, String text, int line, int delayTime) {
  switch (mode) {
  case 0: // Startup
    display.clearBuffer();
    display.setFont(u8g2_font_helvB08_tf);
    display.drawStr(0, 12, "OBR 2023");
    display.drawStr(0, 24, "- Constantes:");
    display.drawStr(0, 36, "  - Velocidade: ");
    display.setCursor(78, 36);
    display.print(velocity);
    display.sendBuffer();
    delay(delayTime);
    break;
  case 1:
    display.clearBuffer();

    display.sendBuffer();
    delay(delayTime);
    break;
  case 2:
    display.clearBuffer();

    display.sendBuffer();
    delay(delayTime);
    break;
  case 3:
    display.clearBuffer();

    display.sendBuffer();
    delay(delayTime);
    break;
  case 4:
    delay(delayTime);
    display.clearBuffer();
    display.nextPage();
    display.sendBuffer();
    break;
  }
  return true;
}
bool tryFunction(bool current, String name, String error) {
  while (!current) {
    Serial.println(name + " " + error + "!");
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
  return true;
}
bool oledSetup() {
  display.begin();
  display.clearBuffer();
  return true;
}