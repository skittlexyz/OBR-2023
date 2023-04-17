#include "Stepper.h"
Stepper stepper(2,5,3,6,8);

void setup() {

}
void loop() {
  run(true, 1, 50, 3, 0);
}