int stepPin[2] = {2,3};
int dirPin[2] = {5,6};
int enable = 8;
bool toggleOne = HIGH;
bool toggleTwo = LOW;

void setup() {
// Sets the two pins as Outputs
  for (int i = 0; i < 2; i++) {
    pinMode(stepPin[i], OUTPUT);
    pinMode(dirPin[i], OUTPUT);
  }
  pinMode(enable,OUTPUT);
  digitalWrite(enable,LOW);
  digitalWrite(dirPin[0], HIGH);
  digitalWrite(dirPin[1], LOW);
}
void loop() {
  for (int i = 0; i <= 1000; i++) {
    digitalWrite(stepPin[0], HIGH);
    digitalWrite(stepPin[1], HIGH);
    delay(1);
    digitalWrite(stepPin[0], LOW);
    digitalWrite(stepPin[1], LOW);
    delay(1);
    if (i == 1000) {
      digitalWrite(dirPin[0], !toggleOne);
      digitalWrite(dirPin[1], !toggleTwo);
      toggleOne = !toggleOne;
      toggleTwo = !toggleTwo;
    }
  }
}
