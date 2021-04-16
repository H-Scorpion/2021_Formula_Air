#include <Servo.h>
int IRSensor1 = 2; // connect ir sensor to arduino pin 2
int IRSensor2 = 4; // connect ir sensor to arduino pin 4

int LED = 13; // conect Led to arduino pin 13

/*** Steering：轉向機構 ***/
Servo Steering; 
const int Steering_Pin = 3; //控制 Steering 的腳位
int Steering_Angle = 0; //Steering 的角度位置
int Rotating_Angle = 0; //Steering 旋轉角度
const int Steering_Angle_Central = 95; //Steering 的中心角度
const int Steering_Angle_limit = 25; //Steering 的中心角度
const int Steering_Angle_Max = Steering_Angle_Central + Steering_Angle_limit; //Steering 的最大角度位置
const int Steering_Angle_Min = Steering_Angle_Central - Steering_Angle_limit; //Steering 的最小角度位置

String Angle_String = ""; //字串角度



void setup() 
{

  Serial.begin(9600);
  pinMode (IRSensor1, INPUT); // sensor pin INPUT
  pinMode (IRSensor2, INPUT); // sensor pin INPUT
  pinMode (LED, OUTPUT); // Led pin OUTPUT

  Steering.attach(Steering_Pin); // 將 Steering 連接到 Steering_Pin
  Serial.println("START"); 
  Steering_Angle = Steering_Angle_Central;//初始化 Steering 角度
  Steering.write(Steering_Angle);
}

void loop()
{
  // initial statusSensor to store IR Sensor value
  int statusSensor[4] ;
  for (int i =0;i<4;i++){
    statusSensor[i]=0;  
  }
  //get IR sensor value
  getIRValue(&statusSensor[0]);

  for (int i =0;i<4;i++){    
    Serial.print(statusSensor[i]);  
  }
  int rotAng=calSteerAng(&statusSensor[0]);
  steering(rotAng);
  
  



  delay(250);
}

int calSteerAng(int *irInput){
  int ang=0;
  if (irInput[0]==1){
    ang=-10;
  }
  else if(irInput[1]==1){
    ang=10;
  }
  else{
    ang=0;
  }
  return ang;
}

void irLedControl(int toLight){
  if (toLight == 1){
    digitalWrite(LED, LOW); // LED LOW
  }  
  else
  {
    digitalWrite(LED, HIGH); // LED High
  }
}

void getIRValue(int *pArray){
  pArray[0]=digitalRead(IRSensor1);
  pArray[1]=digitalRead(IRSensor2);
  for(int i=2;i<4;i++){
    pArray[i]=i;
  }
}

void steering(int Rotating_Angle){
  Steering_Angle = Steering_Angle_Central + Rotating_Angle;

  //判斷 Steering_Angle 是否為合法角度
  if (Steering_Angle <= Steering_Angle_Max || Steering_Angle >= Steering_Angle_Min)
  {
    Serial.println("Rotating Angle：" + (String)Rotating_Angle);
    Serial.println("Steering Angle：" + (String)Steering_Angle);
    Steering.write(Steering_Angle); // 轉動 Steering
  
   
  }
  else
  {
     Serial.println("無效角度，重新輸入旋轉角度---範圍：" + (String)(-abs(Steering_Angle_limit)) + "~" + (String)(abs(Steering_Angle_limit)));
  }
  Serial.println("\n\n\n\n");
  Angle_String = ""; //字串角度重置
}

//void printArray(int *ArrayToPrint){
//  for(int i=0;i<<4;i++){
//    Serial.print(ArrayToPrint[i]);
//  }
//  Serial.print('hi');
//}
