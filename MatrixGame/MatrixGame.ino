#include "LedControl.h"
#include <LiquidCrystal.h>
#include <EEPROM.h>

// variabile pentru "emoticoane"
byte heart[8] = {
    0b00000, 
    0b01010, 
    0b11111, 
    0b11111, 
    0b11111, 
    0b01110, 
    0b00100, 
    0b00000 
};

byte smile[8] = {
    0b00000, 
    0b00100,
    0b10010, 
    0b00001,
    0b10001, 
    0b00010,
    0b00100, 
    0b00000 
}; 

byte skull[8] = {
    B00000,
    B01110,
    B10101,
    B11011,
    B01110,
    B01110,
    B00000,
    B00000
};

byte upArrow[8] = {
    B00100,
    B01010,
    B10001,
    B00000,
    B00000,
    B00000,
    B00000,
    B00000
};

byte downArrow[8] = {
    B00000,
    B00000,
    B00000,
    B00000,
    B00000,
    B10001,
    B01010,
    B00100
};

byte arrows[8] = {
    B00100,
    B01010,
    B10001,
    B00000,
    B00000,
    B10001,
    B01010,
    B00100
};

// LCD pins
const int v0Pin = 6;
const int RS = 12;
const int enable = 11;
const int d4 = 5;
const int d5 = 4;
const int d6 = 3;
const int d7 = 2;

LiquidCrystal lcd(RS, enable, d4, d5, d6, d7);

// variabile pentru mesajul de la sfarsitul jocului
String congratulationMessage1 = "Congratulations!";
String congratulationMessage2;
String congratulationMessage3 = "Press the button to exit!";
int indexMessage1 = 0;
int indexMessage2 = 0;
int indexMessage3 = 0;

// LED Matrix
int brightness = 1;
const int dinPin = 10;
const int clkPin = 9;
const int loadPin = 8;
const int noOfDrivers = 1;

// joystick
const int xPin = A0;
const int yPin = A1;
const int swPin = A2;
int contrast = 150;
int xValue = 0;
int yValue = 0;
bool xMoved = false;
bool yMoved = false;
int swValue;
bool swState = false;
int lastSwValue = HIGH;
int buttonState;
bool previousButtonPressed = false;
bool buttonPressed = false;
bool canBeCleared = true;
bool gameFinished = false;
bool startGame = false;
const int lowThreshold = 200;
const int highThreshold = 900;
bool inMenu = false;
bool inSettings = false;
String inGameName = "Cosmin";

unsigned long startTime = 0;
unsigned long incrementCursor = 0;
unsigned long lastDebounceTime = 0;
const int debounceDelay = 50;

// varibila pentru UI
const int arrowTimeDelay = 500;

// Menu characters variables 
char leftPart = ' ';
char rightPart = ' ';
char space = ' ';
char leftArrow = '<';
char rightArrow = '>';
int menuPosition = 0;

// Score variables 
int score = 0;
int highScore = 0;

// shooting block position
int yShootingBlockPosition = 6;
int prevYShootingBlockPosition = 6;
bool animationComplete = false;

// shootingAnimationDelay
const int animationTimeDelay = 20;
int animationTime = 0;

// shooters position
int xPrevPosition = 0;
int xPosition = 0;

// display Variables 
int countDisplay = 0;
int countForMatrixMap = 6;

// descending Timers
bool matrixMoved = false;
int descendingDelay = 6000;
unsigned long descendingTime = 0;

// variabile delay/debouce 
const int delayTime = 1000;
int previousTime = 0;
int debounceTime = 0;

// variabile de joc
bool gameEnded = false;
int scoreIncrement = 1;


// varibile pentru animatii
int animationCount = 0;
long animationMenuTime = 0;
unsigned long endGameAnimationTime = 0;
bool endGameAnimationComplete = false;
const int endGameAnimationDelay = 50;

// variabile pentru fiecare sectiune din meniu
int infoOption = 0;
int difficultyOption = 0;
int settingsOption = 0;
int maxCountDisplay = 0;

// contori pentru interactiunile din timpul jocului
int leftCount = 0;
int rightCount = 0;
unsigned long timePassed = 0; 
bool gameUpdated = false;

LedControl lc = LedControl(dinPin, clkPin, loadPin, noOfDrivers);

byte matrixMap[7][8] = {
    {1, 0, 1, 0, 1, 0, 1, 1},
    {1, 0, 0, 1, 0, 1, 0, 1},
    {1, 0, 1, 1, 1, 1, 1, 1},
    {1, 1, 1, 0, 1, 0, 1, 1},
    {1, 0, 0, 1, 0, 0, 1, 1},
    {1, 1, 1, 0, 1, 1, 1, 1},
    {1, 1, 0, 0, 0, 1, 1, 1}
};

byte matrix[8][8];

byte animationMatrix[5][8][8] = {
    {
        {1, 1, 0, 0, 1, 1, 0, 1},
        {0, 1, 0, 1, 1, 1, 1, 1},
        {0, 0, 1, 0, 1, 1, 0, 1},
        {0, 0, 0, 0, 0, 0, 0, 0},
        {0, 0, 0, 0, 0, 0, 0, 0},
        {0, 0, 0, 0, 0, 0, 0, 0},
        {0, 0, 0, 0, 0, 0, 0, 0},
        {1, 0, 0, 0, 0, 0, 0, 0}
    },
    {
        {1, 1, 1, 0, 1, 0, 0, 1},
        {0, 1, 0, 1, 1, 1, 0, 1},
        {1, 1, 1, 0, 0, 1, 0, 1},
        {0, 1, 0, 1, 1, 1, 1, 1},
        {0, 0, 0, 0, 0, 0, 0, 0},
        {0, 0, 0, 0, 0, 0, 0, 0},
        {0, 0, 0, 0, 0, 0, 0, 0},
        {0, 1, 0, 0, 0, 0, 0, 0}
    },
    {
        {1, 1, 1, 0, 1, 0, 0, 1},
        {0, 1, 0, 1, 1, 1, 1, 1},
        {1, 0, 1, 0, 1, 1, 0, 1},
        {1, 0, 0, 1, 0, 1, 0, 1},
        {0, 0, 0, 0, 0, 0, 0, 0},
        {0, 0, 0, 0, 0, 0, 0, 0},
        {0, 0, 0, 0, 0, 0, 0, 0},
        {0, 0, 0, 0, 1, 0, 0, 0}
    },
    {
        {1, 1, 1, 1, 1, 0, 1, 1},
        {1, 1, 0, 1, 0, 1, 1, 1},
        {0, 1, 0, 0, 1, 0, 0, 1},
        {0, 0, 0, 1, 0, 1, 1, 1},
        {0, 1, 0, 1, 0, 1, 1, 1},
        {0, 0, 0, 0, 0, 0, 0, 0},
        {0, 0, 0, 0, 0, 0, 0, 0},
        {0, 0, 0, 0, 0, 1, 0, 0}
    },
    {
        {1, 0, 1, 0, 1, 0, 0, 1},
        {1, 1, 0, 1, 0, 0, 1, 1},
        {0, 1, 1, 0, 1, 1, 0, 1},
        {1, 0, 0, 1, 0, 1, 1, 1},
        {0, 1, 1, 1, 0, 1, 1, 1},
        {0, 0, 1, 0, 0, 1, 0, 1},
        {0, 0, 0, 0, 0, 0, 0, 0},
        {0, 0, 0, 0, 0, 0, 0, 1}
    }
};

// functia ce updateaza timpii
void updateTime(){
    if(startGame == false){
        descendingTime = startTime = millis();
    }
}

// functie reutilizate in afisarea UI-ului, a fost folosita si in meniul principal
// dar si in parcurgerea informatiilor din subsectiunea info
void arrowDisplay(int &locationFromMenu){
    if (locationFromMenu == 0){
        if (millis() - previousTime >= arrowTimeDelay){
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
    }else if (locationFromMenu == 1 || locationFromMenu == 2){
        if (millis() - previousTime >= arrowTimeDelay){
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
    }else if (locationFromMenu == 3){
        if (millis() - previousTime >= arrowTimeDelay){
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

// functie ce analizeaza inputul de la joystick atunci cand playerul se afla in meniu
void analyzeDataFromJoyForMenu(){
    if (xValue >= highThreshold && xMoved == false && buttonPressed == false){
        menuPosition++;
        xMoved = true;
    }
    if (xValue <= lowThreshold && xMoved == false && buttonPressed == false){
        menuPosition--;
        xMoved = true;
    }
    if (xValue >= lowThreshold && xValue <= highThreshold && buttonPressed == false){
        xMoved = false;
    }
    if (menuPosition == 4){
        menuPosition = 3;
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

// Functia pentru parcurgerea meniului
void menuOption(){
    gameFinished = false;
    score = 0;
    indexMessage1 = indexMessage2 = indexMessage3 = 0;
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
    }else if (menuPosition == 3){
        optionName = "Info";
        lcd.setCursor(3,1);
        lcd.print(optionName);
    }
    if (xMoved == true){
        lcd.setCursor(3,1);
        lcd.print("            ");
    }
}

// functie pentru afisarea unui mic UI(sageti) pe LCD
void showSettingsDetails(){
    if(settingsOption == 0){
        lcd.setCursor(3, 0);
        lcd.print("Difficulty");
        lcd.setCursor(2, 1);
        lcd.setCursor(15, 0);
        lcd.print(rightArrow);
        if(difficultyOption == 0){
            lcd.write(4);
            lcd.setCursor(4,1);
            lcd.print("Easy");
            lcd.setCursor(9,1);
            lcd.write(2);
        }else if(difficultyOption == 1){
            lcd.write(5);
            lcd.setCursor(4,1);
            lcd.print("Medium");
        }else if(difficultyOption == 2){
            lcd.write(6);
            lcd.setCursor(4,1);
            lcd.print("Hard");
            lcd.setCursor(9,1);
            lcd.write(3);
        }
        if(yMoved == true){
            lcd.setCursor(2,1);
            lcd.print("            ");
        }
    }else if(settingsOption == 1){
        lcd.setCursor(0,0);
        lcd.print(leftArrow);
        lcd.setCursor(3, 0);
        lcd.print("Brightness");
        lcd.setCursor(15, 0);
        lcd.print(rightArrow);
        lcd.setCursor(4, 1);
        if(brightness == 1){
            lcd.write(4);
        }
        if(brightness > 1 && brightness < 14){
            lcd.write(5);
        }
        if(brightness == 14){
            lcd.write(6);
        }
        lcd.setCursor(5,1);
        lcd.print(brightness);
        if(yMoved == true){
            lcd.setCursor(2,1);
            lcd.print("            ");
        }        
    }else if(settingsOption == 2){
        lcd.setCursor(0, 0);
        lcd.print(leftArrow);
        lcd.setCursor(3, 0);
        lcd.print("LCD Contrast");
        lcd.setCursor(4, 1);
        if(contrast == 10){
            lcd.write(4);
        }
        if(contrast > 10 && contrast < 240){
            lcd.write(5);
        }
        if(contrast == 240){
            lcd.write(6);
        }
        lcd.setCursor(5, 1);
        lcd.print(contrast);
        if(yMoved == true){
            lcd.setCursor(2,1);
            lcd.print("            ");
        }
    }
    if(xMoved == true){
        lcd.clear();
    }
}
// functia ce permite playerului sa interactioneze cu meniul
void menuActions(){
    if (menuPosition == 0){
        startGame = true;
        if (gameFinished == false){
            // afisarea pe LCD a Scorului si a Timpului
            lcd.setCursor(0,1);
            lcd.print(leftArrow);
            lcd.setCursor(1,1);
            lcd.print(leftArrow);
            lcd.setCursor(2,1);
            lcd.print("R");
            lcd.setCursor(15,1);
            lcd.print(rightArrow);
            lcd.setCursor(14,1);
            lcd.print(rightArrow);
            lcd.setCursor(13,1);
            lcd.print("E");
            lcd.setCursor(4,0);
            lcd.print("Time:");
            lcd.setCursor(9,0);
            lcd.print((millis() - startTime) / 1000);
            lcd.setCursor(4,1);
            lcd.print("Score:");
            lcd.setCursor(10,1);
            lcd.print(score);
            safeRules();
            analyzeValuesForGame();
            MoveShooter();
            printMatrix();
            updateMatrix();
            checkIfGameEnded();
            inGameActions();
            if(animationComplete == false){
                animateShootingBlock();
            }
        }else{
            // Afisarea mesajului de final
            for(int row = 0; row < 8; row++)
                for(int col = 0; col < 8; col++){
                    matrix[row][col] = 0;
                    lc.setLed(0, row, col, false);
                }
            canBeCleared = true;
            lc.setLed(0, 7, xPosition, false);
            xPosition = 3;
            xPrevPosition = 3;
            lcd.setCursor(16, 0);
            lcd.autoscroll();
            // salvarea highscore-ului
            highScore = EEPROM.read(0);
            if(score >= highScore){
                highScore = score;
                EEPROM.write(0, highScore);
                congratulationMessage2 = "NEW HIGHSCORE  ";
            }else{
                congratulationMessage2 = "You finished the game!";
            }
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
        gameUpdated = false;
        analyzeDataFromJoyForSettings();
        showSettingsDetails();
        canBeCleared = true;
    }
    if(menuPosition == 3){
        arrowDisplay(infoOption);
        analyzeDataForInfo();
        showInfoDetails();
        canBeCleared = true;
    }
}

// Functie ce analizeaza inputul de la joystick
// atunci cand playerul se afla in setari
void analyzeDataFromJoyForSettings(){
    if (xValue >= highThreshold && xMoved == false && yMoved == false){
        settingsOption++;
        xMoved = true;
    }
    if (xValue <= lowThreshold && xMoved == false && yMoved == false){
        settingsOption--;
        xMoved = true;
    }
    if (xValue >= lowThreshold && xValue <= highThreshold){
        xMoved = false;
    }
    if(settingsOption == 3){
        settingsOption = 2;
    }
    if(settingsOption == -1){
        settingsOption = 0;
    }
    if(yValue < lowThreshold && yMoved == false && xMoved == false){
        if(settingsOption == 0){
            difficultyOption++;
        }else if(settingsOption == 1){
            brightness++;
        }else if(settingsOption == 2){
            contrast += 10;
        }
        yMoved = true;
    }
    if(yValue > highThreshold && yMoved == false && xMoved == false){
        if(settingsOption == 0){
            difficultyOption--;
        }else if(settingsOption == 1){
            brightness--;
        }else if(settingsOption == 2){
            contrast -= 10;
        }
        yMoved = true;
    }
    if(yValue >= lowThreshold && yValue <= highThreshold)
        yMoved = false;
    if(difficultyOption == 3){
        difficultyOption = 2;
    }
    if(difficultyOption == -1){
        difficultyOption = 0;
    }
    if(brightness == 0){
        brightness = 1;
    }
    if(brightness == 15){
        brightness = 14;
    }
    if(contrast == 0){
        contrast = 10;
    }
    if(contrast == 250){
        contrast = 240;
    }
}

// Functie ce genereaza animatia din meniu
void menuMatrixAnimation(){
    if(animationCount == 5){
        animationCount = 0;
    }    
    for(int row = 0; row < 8; row++)
        for(int col = 0; col < 8; col++)
            lc.setLed(0, row, col, animationMatrix[animationCount][row][col]);
    if(millis() - animationMenuTime > 1000){
        for(int row = 0; row < 8; row++)
            for(int col = 0; col < 8; col++)
                lc.setLed(0, row, col, 0);
        animationCount++;
        animationMenuTime = millis();
    }
}

// functia de print a matricii
void printMatrix(){
    for(int row = 0; row < 8; row++)
        for(int col = 0; col < 8; col++)
            lc.setLed(0, row, col, matrix[row][col]);
}

// ?functia de generare a mapei
void generateMaps(){
    for(int row = 0; row < 63; row++)
        for(int col = 0; col < millis() % 8; col++)
            matrixMap[row][millis() % 8] = 1;
}

// functia de citire a valorilor
void readAnalogValues(){
    xValue = analogRead(xPin);
    yValue = analogRead(yPin);
    swValue = digitalRead(swPin);
}

// functia care updateaza matricea(ia ultima linie din mapa si o pune pe prima linie din matrice)
void lastLineToFirstLine(){
    if(countForMatrixMap == -1){
        countForMatrixMap = 6;
    }
    for(int col = 0; col < 8; col++)
        matrix[0][col] = matrixMap[countForMatrixMap][col];
}

// functia care muta insula de 1 in jos
void moveMatrixDown(){
    int aux;
    bool flag = false;
    if(matrixMoved == false){
        for(int row = maxCountDisplay; row >= 0; row--){
            for(int col = 7; col >= 0 && flag == false; col--)
                if(matrix[row][col] == 1){
                    aux = row;
                    flag = true;
                }
            if(flag == true)
                break;
        }
        for(int row = aux; row >= 0; row--){
            for(int col = 0; col < 8; col++)
                matrix[row + 1][col] = matrix[row][col];
        }
        if(millis() % 2 == 0){
            for(int col = 0; col < 8; col++){
                matrix[0][col] = 0;
                if(col % 2 == 0){
                    matrix[0][col] = 1;
                }
            }
        }else if(millis() % 2 == 1){
            for(int col = 0; col < 8; col++){
                matrix[0][col] = 0;
                if(col % 3 == 1){
                    matrix[0][col] = 1;
                }else if(col % 3 == 2){
                    matrix[0][col] = 1;
                }
            }
        }else if(millis() % 3 == 0){
            for(int col = 0; col < 8; col++){
                matrix[0][col] = 0;
                if(col % 4 == 1){
                    matrix[0][col] == 1;
                }else if(col % 4 == 2){
                    matrix[0][col] == 1;
                }
            }
        }else if(millis() % 3 == 1){
            for(int col = 0; col < 8; col++){
                matrix[0][col] = 0;
                if(col % 3 == 0){
                    matrix[0][col] = 1;
                }else if(col % 3 == 1){
                    matrix[0][col] = 1;
                }
            }
        }else if(millis() % 3 == 2){
            for(int col = 0; col < 8; col++)
                matrix[0][col] = 0;
            for(int col = 2; col < 6; col++)
                matrix[0][col] = 1;
        }
        maxCountDisplay++;
        matrixMoved = true;
    }
}

// functia care updateaza matricea. In cazul in care avem o linie plina de 1 mat. se muta in sus 
// alt rol, odata la un timp insula de 1 se muta in jos pentru a mari dificultatea jocului
void updateMatrix(){
    int aux;
    for(int row = 0; row < 7; row++){
        int ok = 1;
        for(int col = 0; col < 8; col++)
            if(matrix[row][col] == 0){
                ok = 0;
                break;
            } 
        if(ok == 1){
            score += scoreIncrement;
            if(score != 0 && score % 10 == 0){
                descendingDelay -= 50;
            }
            moveMatrixUp(row);
            maxCountDisplay--;
        }
    }
    if(millis() - descendingTime > descendingDelay){
        matrixMoved = false;
        moveMatrixDown();
        descendingTime = millis();
    }
}

// se apeleaza cand avem o linie full de 1. se muta insula de 1 in sus 
void moveMatrixUp(int aux){
    for(int row = aux; row < 7; row++){
        for(int col = 0; col < 8; col++){
            if(row == 6){
                matrix[row][col] = 0;
            }else{
                matrix[row][col] = matrix[row + 1][col];
            }
        }
    }
}

// functia de mutare a jucatorului
void MoveShooter(){
    if(xPrevPosition != xPosition){
        matrix[7][xPrevPosition] = 0;
        xPrevPosition = xPosition;
    }
    matrix[7][xPosition] = 1;
}

// functia de animatie atunci cand jucatorul trage cu un block
void animateShootingBlock(){
    yShootingBlockPosition = prevYShootingBlockPosition = 6;
    for(yShootingBlockPosition = 6; yShootingBlockPosition >= countDisplay; ){
        if(millis() - animationTime > animationTimeDelay){
            yShootingBlockPosition--;
            animationTime = millis();
        }
        lc.setLed(0, yShootingBlockPosition, xPosition, 1);
        if(yShootingBlockPosition != prevYShootingBlockPosition){
            lc.setLed(0, prevYShootingBlockPosition, xPosition, false);
            prevYShootingBlockPosition = yShootingBlockPosition;
        }         
    }
    animationComplete = true;
}

// functie utilizata pentru a interactiona cu LCD-ul 
// si a afisa subsectiunile din "Info"
void showInfoDetails(){
    if(infoOption == 0){
        lcd.setCursor(3,0);
        lcd.print("Coded by");
        lcd.setCursor(4,1);
        lcd.print("Cosmin");
    }else if(infoOption == 1){
        lcd.setCursor(4,0);
        lcd.print("bit.do/");
        lcd.setCursor(4,1);
        lcd.print("cosmin-g");
    }else if(infoOption == 2){
        lcd.setCursor(4,0);
        lcd.print("This is");
        lcd.setCursor(3,1);
        lcd.print("MatrixSide");
    }else if(infoOption == 3){
        lcd.setCursor(0,0);
        lcd.print("Made with");
        lcd.setCursor(10,0);
        lcd.write(1);
        lcd.setCursor(12,0);
        lcd.print("for");
        lcd.setCursor(1,1);
        lcd.print("UnibucRobotics");
    }
    if(xMoved == true){
        lcd.clear();
    }
}

// functia ce reseteaza jocul
void restartGame(){
    score = 0;
    startTime = millis();
    for(int row = 0; row < 7; row++)
        for(int col = 0; col < 8; col++)
            matrix[row][col] = 0;
    maxCountDisplay = 0;
    countDisplay = 0;
}

// functia ce termina jocul
void endGame(){
    restartGame();
    canBeCleared = true;
    gameFinished = true;
}

void inGameActions(){
    // daca playerul se afla pe cea mai din dreapta pozitie
    // si da de 3 ori in dreapta de joystick jocul se va sfarsi
    if(rightCount > 2){
        endGame();
        rightCount = 0;
    }
    // daca playerul se afla pe cea mai din stanga pozitie
    // si da de 3 ori in stanga de joystick jocul se va reseta
    if(leftCount > 2){
        restartGame();
        lcd.setCursor(9,0);
        lcd.print("   ");
        leftCount = 0;
    }
}

// functia care adauga 1 in matrice
void putBlock(){
    countDisplay = 0;
    for(int row = 5; row >= 0; row--){
        if(matrix[row][xPosition] == 1){
            countDisplay = row + 1;
            if(countDisplay >= maxCountDisplay){
                maxCountDisplay = countDisplay;
            }
            break;
        }
    }
    matrix[countDisplay][xPosition] = 1;
}

// functia care analizeaza valorile citite
void analyzeDataForInfo(){
    if(xValue >= highThreshold && xMoved == false){
        infoOption++;
        xMoved = true;
    }
    if(xValue <= lowThreshold && xMoved == false){
        infoOption--;
        xMoved = true;
    }
    if(xValue > lowThreshold && xValue < highThreshold)
        xMoved = false;
    if(infoOption == 4){
        infoOption = 3;
    }
    if(infoOption == -1){
        infoOption = 0;
    }
}

void analyzeValuesForGame(){
    if(xValue >= highThreshold && xMoved == false && yMoved == false){
        xPosition++;
        xMoved = true;
    }
    if(xValue <= lowThreshold && xMoved == false && yMoved == false){
        xPosition--;
        xMoved = true;
    }
    if(xValue > lowThreshold && xValue < highThreshold){
        xMoved = false;
    }
    if(yValue < lowThreshold && yMoved == false && xMoved == false){
        animationComplete = false;
        putBlock();
        yMoved = true;
    }
    if(yValue >= lowThreshold)
        yMoved = false;
}

// functia indeplineste rolul de a pastra valorile integre
void safeRules(){
    if(xPosition == 8){
        rightCount++;
        xPosition = 7;
    }
    if(xPosition == -1){
        leftCount++;
        xPosition = 0;
    }
    if(millis() - timePassed > 4000){
        rightCount = 0;
        leftCount = 0;
        timePassed = millis();
    }

}

// se verifica daca s-a terminat jocul
void checkIfGameEnded(){
    if(countDisplay == 6){
        gameFinished = true;
    }
}

void setup(){
    // crearea emoticoanelor
    lcd.createChar(1, heart);
    lcd.createChar(2, smile);
    lcd.createChar(3, skull);
    lcd.createChar(4, upArrow);
    lcd.createChar(5, arrows);
    lcd.createChar(6, downArrow);
    Serial.begin(9600);
    lc.shutdown(0, false); //turn off power saving, enables display
    lc.clearDisplay(0);
    pinMode(swPin, INPUT_PULLUP);
    highScore = EEPROM.read(0);
    if(highScore == 255){
        EEPROM.write(0,0);
        highScore = EEPROM.read(0);
    }
    lcd.begin(16, 2);
    pinMode(v0Pin, OUTPUT);
}

void loop(){
    analogWrite(v0Pin, contrast);
    lc.setIntensity(0, brightness);
    if(gameUpdated == false){
        if(difficultyOption == 0){
            scoreIncrement = 1;
            descendingDelay = 7000;
        }else if(difficultyOption == 1){
            scoreIncrement = 2;
            descendingDelay = 4000;
        }else if(difficultyOption == 2){
            scoreIncrement = 4;
            descendingDelay = 2500;
        }
        gameUpdated = true;
    }
    updateTime();
    readAnalogValues();
    analyzeDataFromJoyForMenu();
    // selectarea si deselectarea sectiunii din meniu
    if (previousButtonPressed != buttonPressed && canBeCleared == true){
        lcd.clear();
        inMenu = !inMenu;
        previousButtonPressed = buttonPressed;
    }
    if (inMenu == false){
        maxCountDisplay = 0;
        countDisplay = 0;
        arrowDisplay(menuPosition);    
        menuOption();
        menuMatrixAnimation();
    }else if (inMenu == true){
        canBeCleared = false;
        menuActions();
    }
}