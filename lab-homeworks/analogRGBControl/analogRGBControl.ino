const int redPotPin = A0;
const int greenPotPin = A1;
const int bluePotPin = A2;

const int redLedPin = 9;
const int greenLedPin = 10;
const int blueLedPin = 11;

int redPotValue = 0;
int greenPotValue = 0;
int bluePotValue = 0;

int redLedValue = 0;
int greenLedValue = 0;
int blueLedValue = 0;

void setup() {
  pinMode(redPotPin, INPUT);
  pinMode(greenPotPin, INPUT);
  pinMode(bluePotPin, INPUT);
  pinMode(redLedValue, OUTPUT);
  pinMode(greenLedValue, OUTPUT);
  pinMode(blueLedValue, OUTPUT);
  Serial.begin(9600);
}

void loop() {
  redPotValue = analogRead(redPotPin);
  greenPotValue = analogRead(greenPotPin);
  bluePotValue = analogRead(bluePotPin);

  redLedValue = map(redPotValue, 0, 1023, 0, 255);
  greenLedValue = map(greenPotValue, 0, 1023, 0, 255);
  blueLedValue = map(bluePotValue, 0, 1023, 0, 255);
  
  setColor(redLedValue, greenLedValue, blueLedValue);
  delay(1);
}

void setColor(int redValue, int greenValue, int blueValue){
  analogWrite(redLedPin, redValue);
  analogWrite(greenLedPin, greenValue);
  analogWrite(blueLedPin, blueValue);
}
