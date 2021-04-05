void setup() {
  Serial.begin(9600);
}

void loop() {
  int value = analogRead(A1);
  Serial.println(value);
  delay(500);
}
