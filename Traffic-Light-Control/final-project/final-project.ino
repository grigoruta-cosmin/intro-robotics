unsigned long globalTime = 0;
unsigned long startMillis = 0;
unsigned long slowMillis = 0;
unsigned long stopMillis = 0;
const int startTime = 11000;
const int slowTime = 1000;
const int stopTime = 4000;
int movingState = 0; // 0 - start, 1 - slow, 2 - stop
int prevMovingState = 0;
class CarTrafficLight{
private:
  int greenPin;
  int yellowPin;
  int redPin;
  bool greenLight;
  bool yellowLight;
  bool redLight;
public:
  CarTrafficLight(){
    greenLight = true;
    yellowLight = false;
    redLight = false;
  }
  int getGreenPin(){
    return greenPin;
  }
  int getYellowPin(){
    return yellowPin;
  }
  int getRedPin(){
    return redPin;
  }
  bool getGreenLight(){
    return greenLight;
  }
  bool getYellowLight(){
    return yellowLight;
  }
  bool getRedLight(){
    return redLight;
  }
  void setGreenPin(int g){
    greenPin = g;
  }
  void setYellowPin(int y){
    yellowPin = y;
  }
  void setRedPin(int r){
    redPin = r; 
  }
  void startState(){
    greenLight = true;
    redLight = false;
    yellowLight = false;
  }
  void slowState(){
    greenLight = false;
    yellowLight = true;
    redLight = false;
  }
  void stopState(){
    greenLight = false;
    yellowLight = false;
    redLight = true;
  }
  void showLights(){
    digitalWrite(greenPin, greenLight);
    digitalWrite(yellowPin, yellowLight);
    digitalWrite(redPin, redLight);
  }
  ~CarTrafficLight() = default;
};

class Traffic{
  CarTrafficLight semaforMasinaFirst;
  CarTrafficLight semaforMasinaSecond;
public:
  Traffic(){
    semaforMasinaFirst.setGreenPin(10);
    semaforMasinaFirst.setYellowPin(9);
    semaforMasinaFirst.setRedPin(8);
    semaforMasinaSecond.setGreenPin(22);
    semaforMasinaSecond.setYellowPin(24);
    semaforMasinaSecond.setRedPin(26);
  }
  void going(){
    semaforMasinaFirst.showLights();
    semaforMasinaSecond.showLights();
    if(movingState == 0){
      semaforMasinaFirst.startState();
      semaforMasinaSecond.stopState();
      if(millis() - startMillis > startTime){
        movingState = 1; 
      }
    }else if(movingState == 1){
      semaforMasinaFirst.slowState();
      if(millis() - slowMillis > slowTime){
        movingState = 2;
      }
    }else if(movingState == 2){
      semaforMasinaFirst.stopState();
      semaforMasina2.startState();
      if(millis() - stopMillis > stopT-ime){
        movingState = 0;
      }
    }
  }
  CarTrafficLight& getSemaforMasinaFirst(){
    return semaforMasinaFirst;
  }
  CarTrafficLight& getSemaforMasinaSecond(){
    return semaforMasinaSecond;
  }
};

void timeFunction(){
  if(movingState == 0){
    stopMillis = millis();
    slowMillis = millis();
  }else if(movingState == 1){
    startMillis = millis();
    stopMillis = millis();
  }else if(movingState == 2){
    startMillis = millis();
    slowMillis = millis();
  }
}

Traffic traffic;

void setup() {
  pinMode(traffic.getSemaforMasinaFirst().getGreenPin(), OUTPUT);
  pinMode(traffic.getSemaforMasinaFirst().getYellowPin(), OUTPUT);
  pinMode(traffic.getSemaforMasinaFirst().getRedPin(), OUTPUT);
  pinMode(traffic.getSemaforMasinaSecond().getGreenPin(), OUTPUT);
  pinMode(traffic.getSemaforMasinaSecond().getYellowPin(), OUTPUT);
  pinMode(traffic.getSemaforMasinaSecond().getRedPin(), OUTPUT);
}

void loop() {
  timeFunction();
  traffic.going();
}
