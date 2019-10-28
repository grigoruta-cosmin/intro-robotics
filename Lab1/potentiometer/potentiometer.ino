int ledValue = 0;
int potValue = 0;
const int potPin = A0;
const int ledPin = 9;


void setup() {
  // put your setup code here, to run once:
  Serial.begin(9600);
}

void loop() {
  // put your main code here, to run repeatedly:
  float voltage;
  potValue = analogRead(potPin);
  voltage = potValue * (5.0 / 1023);
  Serial.println(potValue);
  ledValue = map(potValue, 0, 1023, 0, 255);
  analogWrite(ledPin, ledValue);
  delay(10);
}
