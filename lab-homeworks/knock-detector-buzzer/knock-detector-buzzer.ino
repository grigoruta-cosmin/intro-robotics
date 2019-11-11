const int passiveBuzzerPin = A4;
const int activeBuzzerPin = 11;
const int pushButtonPin = 2;
const int passiveBuzzerThreshold = 100;
const long interval = 5000;

long previousMillis = 0;
int timePassed = 0;
int passiveBuzzerFlag = 0;
int passiveBuzzerValue = 0;
int activeBuzzerTone = 1500;
int pushButtonState = 0;
int buttonPressed = 0;

void setup() {
  // put your setup code here, to run once:
  pinMode(passiveBuzzerPin, INPUT);
  pinMode(activeBuzzerPin, OUTPUT);
  pinMode(pushButtonPin, INPUT_PULLUP);
  Serial.begin(9600);
}

void loop() {
  // put your main code here, to run repeatedly:
  passiveBuzzerValue = analogRead(passiveBuzzerPin);
  Serial.print("passiveBuzzerValue = ");
  Serial.println(passiveBuzzerValue);
  if (passiveBuzzerValue > passiveBuzzerThreshold){
    passiveBuzzerFlag = 1;
    Serial.println("ajung aici ulterior?");
    Serial.print("passiveBuzzerFlag = ");
    Serial.println(passiveBuzzerFlag);
  }
  if (passiveBuzzerFlag == 1){
    unsigned long currentMillis = millis();
    Serial.println(currentMillis);
    if (currentMillis - previousMillis >= interval){
      previousMillis = currentMillis;
      timePassed = 1;
    }
    Serial.println(timePassed);
    Serial.println(previousMillis);
  }
  if (timePassed == 1){
    pushButtonState = digitalRead(pushButtonPin);
    if (pushButtonState == LOW){
      buttonPressed = 1;
    }
    Serial.print("buttonPressed = ");
    Serial.println(buttonPressed);
    if(buttonPressed == 1){
      noTone(activeBuzzerPin);
      passiveBuzzerFlag = 0;
      timePassed = 0;
      Serial.print("passiveBuzzerFlag = ");
      Serial.println(passiveBuzzerFlag);
      Serial.print("timePassed = ");
      Serial.println(timePassed);
      buttonPressed = 0;
    }else{
      tone(activeBuzzerPin, activeBuzzerTone);
    }  
  }
  //Serial.println(pushButtonState);
  //Serial.println(passiveBuzzerValue);
  delay(500);
}
