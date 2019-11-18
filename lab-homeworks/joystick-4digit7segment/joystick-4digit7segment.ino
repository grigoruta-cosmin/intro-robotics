
const int pinA = 12;
const int pinB = 8;
const int pinC = 5;
const int pinD = 3;
const int pinE = 2;
const int pinF = 11;
const int pinG = 6;
const int pinDP = 4;
const int pinD1 = 7;
const int pinD2 = 9;
const int pinD3 = 10;
const int pinD4 = 13;

const int xPin = A0;
const int yPin = A1;
const int swPin = A2;

const int segSize = 8;

const int noOfDisplays = 4;
const int noOfDigits = 10;

int dpState = LOW;
const int timeForBlinking = 200;
int currentNumber = 0;

struct PartOfStatus{
  int pos;
  int digit;
};

PartOfStatus Status[4];

const int debounce = 200;
unsigned long currentTime;
unsigned long previousTime = 0;
unsigned long previousTimeForDebounce = 0;

bool swPressed;
int xValueChangedFlag = 0;
int yValueChangedFlag = 0;
int xValue = 0;
int yValue = 0;
int swValue = 0;
int prevSwValue = 1;


const int highThreshold = 750;
const int lowThreshold = 350;


int auxPosition = 3;

// segments array, similar to before
int segments[segSize] = {
  pinA, pinB, pinC, pinD, pinE, pinF, pinG, pinDP
};
// digits array, to switch between them easily
int digits[noOfDisplays] = {
  pinD1, pinD2, pinD3, pinD4
 };   
 
byte digitMatrix[noOfDigits][segSize - 1] = {
// a  b  c  d  e  f  g
  {1, 1, 1, 1, 1, 1, 0}, // 0
  {0, 1, 1, 0, 0, 0, 0}, // 1
  {1, 1, 0, 1, 1, 0, 1}, // 2
  {1, 1, 1, 1, 0, 0, 1}, // 3
  {0, 1, 1, 0, 0, 1, 1}, // 4
  {1, 0, 1, 1, 0, 1, 1}, // 5
  {1, 0, 1, 1, 1, 1, 1}, // 6
  {1, 1, 1, 0, 0, 0, 0}, // 7
  {1, 1, 1, 1, 1, 1, 1}, // 8
  {1, 1, 1, 1, 0, 1, 1}  // 9
};

void displayNumber(byte digit, byte decimalPoint) {
  for (int i = 0; i < segSize - 1; i++) {
    digitalWrite(segments[i], digitMatrix[digit][i]);
  }

  // write the decimalPoint to DP pin
  digitalWrite(segments[segSize - 1], decimalPoint);
}

// activate the display no. received as param
void showDigit(int num) {
  for (int i = 0; i < noOfDisplays; i++) {
    digitalWrite(digits[i], HIGH);
  }
  digitalWrite(digits[num], LOW);
}

void workOnStatus(int xValue, int yValue){
  //Serial.print("pos ");
  //Serial.println(auxPosition);
  //Serial.print(yValueChangedFlag);
  if (yValue >= highThreshold && yValueChangedFlag == 0 && swPressed == 1){
    auxPosition--;
    yValueChangedFlag = 1;
  }
  if (yValue <= lowThreshold && yValueChangedFlag == 0 && swPressed == 1){
    auxPosition++;
    yValueChangedFlag = 1;
  }
  if (yValue >= lowThreshold && yValue <= highThreshold){
    yValueChangedFlag = 0;  
  }
  if (xValue >= highThreshold && xValueChangedFlag == 0 && swPressed == 0){
    Status[auxPosition].digit++;
    xValueChangedFlag = 1;
  }
  if (xValue <= lowThreshold && xValueChangedFlag == 0 && swPressed == 0){
    Status[auxPosition].digit--;
    xValueChangedFlag = 1;
  }
  if (xValue >= lowThreshold && xValue <= highThreshold){
    xValueChangedFlag = 0;  
  }
  if (auxPosition > 3){
    auxPosition = 0;
  }
  if (auxPosition < 0){
    auxPosition = 3;
  }
  if (Status[auxPosition].digit > 9){
    Status[auxPosition].digit = 0;  
  }
  if (Status[auxPosition].digit < 0){
    Status[auxPosition].digit = 9;
  }
}

void displayOnSevenSegment(){
  for (int i = 0; i < 4; i++){
    showDigit(Status[i].pos); 
    if (i == auxPosition && swPressed == true){
      if (millis() - previousTime >= timeForBlinking){
        dpState = !dpState;
        previousTime = millis();
      }
      displayNumber(Status[i].digit, dpState);
    }else{
      displayNumber(Status[i].digit, LOW); 
    }
    delay(5);
  }
}

void setup() {
  pinMode(swPin, INPUT_PULLUP);
  for (int i = 0; i < segSize; i++)
  {
    pinMode(segments[i], OUTPUT);  
  }
  for (int i = 0; i < noOfDisplays; i++)
  {
    pinMode(digits[i], OUTPUT);  
  }
  for(int i = 0; i < 4; i++){
    Status[i].pos = i;
    Status[i].digit = 0;
  }
  Serial.begin(9600);
}

void loop() {
  Serial.println(swPressed);
  /*Serial.print(" | ");
  Serial.print(prevSwValue);
  Serial.print(" | ");
  Serial.print("X-axis: ");
  Serial.print(xValue);
  Serial.print(" | ");
  Serial.print("Y-axis: ");
  Serial.print(yValue);
  Serial.print(" | ");
  Serial.print(currentTime);
  Serial.print("---");
  Serial.print(previousTimeForDebounce);
  Serial.println(" | ");*/
  xValue = analogRead(xPin);
  yValue = analogRead(yPin);
  swValue = digitalRead(swPin);
  currentTime = millis();
  if (swValue != prevSwValue && currentTime - previousTimeForDebounce >= debounce){
    swPressed = !swPressed;
    previousTimeForDebounce = currentTime;
  }
  prevSwValue = swValue;
  workOnStatus(xValue, yValue);
  displayOnSevenSegment();
}
