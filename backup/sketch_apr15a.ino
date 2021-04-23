#include <Timer.h>

Timer tcb;

int angle;
int t1,t2,deltaTime;
bool countingTime;
bool sensorLout,sensorLin;
bool rightside;

void startTime(){  //開始計時
  countingTime=true;
  t1=millis();
}
void endTime(){    //結束計時
  t2=millis();
  countingTime=false;
  deltaTime=t2-t1;//結算衝出去的時間
  angleTimeConvert();  
}

void angleTimeConvert(){
  if(deltaTime<200){//若很快衝出去則用最大角度轉回
    angle=23;
  }
  else{ //衝出去的時間越長，則偏的越少，需要轉的角度越小。
    angle=1000/deltaTime;
    angle=map(angle,0,5,0,23);
  }
}

void turn(){//轉向
  if(rightside){
    angle=-abs(angle);
  }
  else{
    angle=abs(angle);
  }
  servo.write(90+angle);
}

void setup() {
  Serial.begin(9600);
}

void loop() {
  tcb.update();
  if(sensorLout&&!countingTime){//左邊外側讀到則開始計時
    startTime();
    rightside=false;    //車子左偏
  }
  if(countingTime&&sensorLin){//左邊內側讀到則終止計時，計算衝出去的快慢並轉回來
    endTime();
    turn();
  }

  
}
