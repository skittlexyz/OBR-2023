void setup() {
  Serial.begin(9600);
  while (!Serial) {
    // Wait for Serial Monitor to open
  }for(int pin = 2; pin < 10; pin++) {
    pinMode(pin, INPUT);
  }
}

void loop() {
  Serial.println("");
  Serial.println("Digital Pin Readings:");
  Serial.println("---------------");
  Serial.println("| Pin | Value |");
  Serial.println("---------------");
  for (int pin = 2; pin < 10; pin++) {
    int value = digitalRead(pin);
    printTableRow(pin, value);
  }
  delay(1000);
}

void printTableRow(int pin, int value) {
  Serial.print("|  ");
  Serial.print(pin);
  Serial.print("  |  ");
  Serial.print(value);
  Serial.println("   |");
  Serial.println("---------------");
}