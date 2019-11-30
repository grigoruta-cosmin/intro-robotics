#include <LiquidCrystal.h>
#include <EEPROM.h>
const int RS = 12;
const int enable = 11;
const int d4 = 5;
const int d5 = 4;
const int d6 = 3;
const int d7 = 2;

const int xPin = A0;
const int yPin = A1;
const int swPin = A2;


int xValue = 0;
int yValue = 0;
int swValue;
int lastSwValue = HIGH;
int buttonState;
bool previousButtonPressed = false;
bool buttonPressed = false;
bool canBeCleared = true;
bool gameFinished = false;
bool startGame = false;
bool updatedLevel = false;
unsigned long incrementCursor = 0;
unsigned long lastDebounceTime = 0;
unsigned long lastIncrementTime = 0;
unsigned long startTime = 0;
bool inMenu = false;
bool inSettings = false;
const int incrementLevelDelay = 2000;
const int debounceDelay = 50;
const int highThreshold = 750;
const int lowThreshold = 350;
bool xMoved = true;
bool yMoved = false;
unsigned long previousTime = 0;
const int delayTime = 500;
String inGameName = "Cosmin";

char leftPart = ' ';
char rightPart = ' ';
char space = ' ';
char leftArrow = '<';
char rightArrow = '>';
int menuPosition = 0;

int currentLives = 3;
int startingLevelValue = 0;
int levelValue = 0;
int score;
int highScore = 0;

LiquidCrystal lcd(RS, enable, d4, d5, d6, d7);

String congratulationMessage1 = "Congratulations!";
String congratulationMessage2 = "You finished the game!";
String congratulationMessage3 = "Press the button to exit!";
int indexMessage1 = 0;
int indexMessage2 = 0;
int indexMessage3 = 0;

void setup() {
  Serial.begin(9600);
  lcd.begin(16, 2);
  pinMode(swPin, INPUT_PULLUP);
  highScore = EEPROM.read(0);
  EEPROM.write(0,0);
}

void readValuesFromJoy(){
  xValue = analogRead(xPin);
  yValue = analogRead(yPin);
  swValue = digitalRead(swPin);
}

void menuArrowDisplay(){
  if (menuPosition == 0){
    if (millis() - previousTime >= delayTime){
      if (rightPart == space){
        rightPart = rightArrow;
      }else{
        rightPart = space;
      }
      lcd.setCursor(0,1);
      lcd.print(space);
      lcd.setCursor(15,1);
      lcd.print(rightPart);
      previousTime = millis();
    }
  }else if (menuPosition == 1){
    if (millis() - previousTime >= delayTime){
      if (rightPart == space && leftPart == space){
        rightPart = rightArrow;
        leftPart = leftArrow;
      }else{
        rightPart = space;
        leftPart = space;
      }
      lcd.setCursor(15,1);
      lcd.print(rightPart);
      lcd.setCursor(0,1);
      lcd.print(leftPart);
      previousTime = millis();
    }
  }else if (menuPosition == 2){
    if (millis() - previousTime >= delayTime){
      if (leftPart == space){
        leftPart = leftArrow;
      }else{
        leftPart = space;
      }
      lcd.setCursor(0,1);
      lcd.print(leftPart);
      lcd.setCursor(15,1);
      lcd.print(space);
      previousTime = millis();
    }
  }
}

void analyzeDataFromJoy(){
  if (yValue >= highThreshold && yMoved == false && buttonPressed == false){
    menuPosition++;
    yMoved = true;
  }
  if (yValue <= lowThreshold && yMoved == false && buttonPressed == false){
    menuPosition--;
    yMoved = true;
  }
  if (yValue >= lowThreshold && yValue <= highThreshold && buttonPressed == false){
    yMoved = false;
  }
  if (menuPosition == 3){
    menuPosition = 2;
  }
  if (menuPosition == -1){
    menuPosition = 0;
  }
  if (swValue != lastSwValue){
    lastDebounceTime = millis();
  }
  if (millis() - lastDebounceTime > debounceDelay){
    if (swValue != buttonState){
      buttonState = swValue;
      if (buttonState == LOW){
        buttonPressed = !buttonPressed;
      }
    }
  }
  lastSwValue = swValue;
}

void menuOption(){
  lcd.setCursor(2,0);
  lcd.print("Hi,");
  lcd.setCursor(6,0);
  lcd.print(inGameName);
  String optionName = "";
  if (menuPosition == 0){
    optionName = "Start Game";
    lcd.setCursor(3, 1);
    lcd.print(optionName);
  }else if (menuPosition == 1){
    optionName = "HighScore";
    lcd.setCursor(3, 1);
    lcd.print(optionName);
  }else if (menuPosition == 2){
    optionName = "Settings";
    lcd.setCursor(3,1);
    lcd.print(optionName);
  }
  if (yMoved == true){
    lcd.setCursor(3,1);
    lcd.print("            ");
  }
}

void menuActions(){
  if (menuPosition == 0){
    startGame = true;
    if (updatedLevel == false){
      levelValue = startingLevelValue;
      updatedLevel = true;
    }
    if (gameFinished == false){
      lcd.setCursor(0,0);
      lcd.print("Lives: ");
      lcd.setCursor(6,0);
      lcd.print(currentLives);
      lcd.setCursor(9,0);
      lcd.print("Level:");
      lcd.setCursor(15,0);
      lcd.print(levelValue);
      lcd.setCursor(3,1);
      lcd.print("Score:");
      lcd.setCursor(9,1);
      lcd.print(score);
      if (millis() - lastIncrementTime > incrementLevelDelay){
        levelValue++;
        lastIncrementTime = millis();
      }
      if (millis() - startTime > 10000){
        gameFinished = true;
        lcd.clear();
      }
      score = levelValue * 3; 
    }else{
      lcd.setCursor(16, 0);
      lcd.autoscroll();
      highScore = score;
      EEPROM.write(0, highScore);
      canBeCleared = true;
      startGame = false;
      while (indexMessage1 < congratulationMessage1.length()){
        if (millis() - incrementCursor > 500){
          lcd.print(congratulationMessage1[indexMessage1]);
          indexMessage1++;
          incrementCursor = millis();
        } 
      }
      lcd.clear();
      lcd.setCursor(16,1);
      while (indexMessage2 < congratulationMessage2.length()){
        if (millis() - incrementCursor > 500){
          lcd.print(congratulationMessage2[indexMessage2]);
          indexMessage2++;
          incrementCursor = millis();
        } 
      }
      lcd.clear();
      lcd.setCursor(16,0);
      while (indexMessage3 < congratulationMessage3.length()){
        if (millis() - incrementCursor > 500){
          lcd.print(congratulationMessage3[indexMessage3]);
          indexMessage3++;
          incrementCursor = millis();
        } 
      }
      lcd.noAutoscroll();
      startTime = millis();
    }
  }
  if (menuPosition == 1){
    lcd.setCursor(0,0);
    lcd.print("HighScore:");
    lcd.setCursor(10, 0);
    lcd.print(highScore);
    canBeCleared = true;
  }
  if (menuPosition == 2){
    lcd.setCursor(0,1);
    lcd.print("Set Level:");
    lcd.setCursor(10,1);
    lcd.print(startingLevelValue);
    analyzeDataFromJoyForSettings();
    canBeCleared = true;
    updatedLevel = false;
    gameFinished = false;
    score = 0;
    indexMessage1 = indexMessage2 = indexMessage3 = 0;
  }
}

void updateTimes(){
  if (startGame == false){
    startTime = lastIncrementTime = millis();
  }
}

void analyzeDataFromJoyForSettings(){
  if (xValue >= highThreshold && xMoved == false){
    startingLevelValue++;
    xMoved = true;
  }
  if (xValue <= lowThreshold && xMoved == false){
    startingLevelValue--;
    xMoved = true;
  }
  if (xValue >= lowThreshold && xValue <= highThreshold){
    xMoved = false;
  }
  if (startingLevelValue == -1){
    startingLevelValue = 0;
  }
}


void loop() {
  readValuesFromJoy();
  analyzeDataFromJoy();
  updateTimes();
  if (previousButtonPressed != buttonPressed && canBeCleared == true){
    lcd.clear();
    inMenu = !inMenu;
    previousButtonPressed = buttonPressed;
  }
  if (inMenu == false){
    menuArrowDisplay();    
    menuOption();
  }else if (inMenu == true){
    canBeCleared = false;
    menuActions();
  }
}
