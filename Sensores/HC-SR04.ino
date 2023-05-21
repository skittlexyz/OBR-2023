const int ultraPins[] = {2, 3, 4, 5, 6, 7, 8, 9};  // Trig, Echo, Trig, Echo, Trig, Echo, Trig, Echo

long distances[4];

void setup() {
  Serial.begin(9600);

  for (int i = 0; i < 8; i += 2) {
    pinMode(ultraPins[i], OUTPUT);    // Trig pins
    pinMode(ultraPins[i + 1], INPUT); // Echo pins
  }
}

void loop() {
  for (int i = 0; i < 4; i++) {
    int trigPin = ultraPins[i * 2];
    int echoPin = ultraPins[i * 2 + 1];
    distances[i] = ultraSensorRead(trigPin, echoPin);
  }

  Serial.print("Sensor 1: ");
  Serial.print(distances[0]);
  Serial.print(" cm, Sensor 2: ");
  Serial.print(distances[1]);
  Serial.print(" cm, Sensor 3: ");
  Serial.print(distances[2]);
  Serial.print(" cm, Sensor 4: ");
  Serial.print(distances[3]);
  Serial.println(" cm");

  delay(500);
}

long ultraSensorRead(int trigPin, int echoPin) {
  digitalWrite(trigPin, LOW);
  delayMicroseconds(2);
  digitalWrite(trigPin, HIGH);
  delayMicroseconds(10);
  digitalWrite(trigPin, LOW);

  long duration = pulseIn(echoPin, HIGH);

  long distance = duration * 0.034 / 2;

  return distance;
}