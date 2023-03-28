int digital = 7;
int analog = A0;

void setup() {
  pinMode(digital, INPUT);
  pinMode(analog, INPUT);
  Serial.begin(9600);
  // put your setup c
}

void loop() {
  Serial.print("Digital ");
  Serial.println( digitalRead(digital) );
  
  Serial.print("Analogico ");
  Serial.println( analogRead(analog) );
  
  Serial.println("");
  delay(300); 
}
