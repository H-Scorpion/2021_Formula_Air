#include <Servo.h>
int IRSensor1 = 2; // connect ir sensor to arduino pin 2 left
int IRSensor2 = 4; // connect ir sensor to arduino pin 4
int IRSensorMid= 5;

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

/*** Blower：鼓風機 ***/
//鼓風機操作：在序列附輸入比 1500 還高和還低的數值，數值離 1500 越遠，轉速越大
//1500是停(中間值)
Servo Blower;
const int Blower_Pin = 9; //控制 Blower 的腳位
int PWM = 0; //控制轉速的脈寬長度
const int PWM_Base = 1500; //Blower 的中間值、啟動值
int PWM_Offset = 0; //PWM 偏移量
const int PWM_Offset_Max = 150; //PWM 最大偏移量
const int PWM_Max = PWM_Base + PWM_Offset_Max; //PWM 最大值
const int PWM_Min = PWM_Base - PWM_Offset_Max; //PWM 最小值
const int PWM_Default_Start = 100; //PWM 最大偏移量
String PWM_Offset_String = ""; //PWM 偏移字串

int time1=0;


void setup() 
{
  //IR SENSOR
  Serial.begin(9600);
  pinMode (IRSensor1, INPUT); // sensor pin INPUT
  pinMode (IRSensor2, INPUT); // sensor pin INPUT
  pinMode (LED, OUTPUT); // Led pin OUTPUT

  //Steering
  Steering.attach(Steering_Pin); // 將 Steering 連接到 Steering_Pin
  Serial.println("START Steering"); 
  Steering_Angle = Steering_Angle_Central;//初始化 Steering 角度
  Steering.write(Steering_Angle);

  //BL motor
  Blower.attach(Blower_Pin);  // 將 Blower 連接到 Blower_Pin
  Blower.writeMicroseconds(PWM_Base); //啟動 Blower
//  Blower.writeMicroseconds(1540);
  Serial.println("START"); 
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
  Serial.print("\n"); 
  int rotAng=calSteerAng(&statusSensor[0]);
  BLMotorControl();
  steering(rotAng);


  if(millis() > 5000){
    if(millis()<60
    
    00){
      Blower.writeMicroseconds(1600);
    }
  }

  
  if(millis() > 20000){
    if(millis()<21000){
      Blower.writeMicroseconds(1500);
    }
  }
  
  



  delay(250);
}

//void countTime(){
//  
//}

int calSteerAng(int *irInput){
  int ang=0;
  if (irInput[0]==1){
    ang=-10;
  }
  else if(irInput[1]==1){
    ang=10;
  }
  else if(irInput[2]==1){
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
  pArray[2]=digitalRead(IRSensorMid);
  pArray[3]=3;

}

void steering(int Rotating_Angle){
  Steering_Angle = Steering_Angle_Central + Rotating_Angle;

  //判斷 Steering_Angle 是否為合法角度
  if (Steering_Angle <= Steering_Angle_Max || Steering_Angle >= Steering_Angle_Min)
  {
    Steering.write(Steering_Angle); // 轉動 Steering
  
   
  }
  else
  {
     Serial.println("無效角度，重新輸入旋轉角度---範圍：" + (String)(-abs(Steering_Angle_limit)) + "~" + (String)(abs(Steering_Angle_limit)));
  }
}

int dynamicIntInput(){
    String inputString="";
    int retInt;
    while (Serial.available() > 0){      
      char c = Serial.read();
      inputString += (String)c;
      delay(10); //等待一下，好讓 Serial 能夠接受新的資料
    }
    if (inputString != "")
    {
      retInt = inputString.toInt();
    }
    else{
      retInt=0;
    }
//    Serial.println("PWM_Width(String)：" + (String)retInt);
    inputString = "";
    return retInt;
}

void BLMotorControl(){
  while (Serial.available() > 0)
  {
    char c = Serial.read();
    PWM_Offset_String += (String)c;
    delay(10); //等待一下，好讓 Serial 能夠接受新的資料
  }
  
  if (PWM_Offset_String != "")
  {
    PWM_Offset = PWM_Offset_String.toInt();
    PWM = PWM_Base + PWM_Offset;
    Serial.println("PWM_Offset：" + (String)PWM_Offset);
    Serial.println("PWM：" + (String)PWM);
    
    //判斷 PWM_Width 是否在合理範圍內：0~1600
    if (PWM <= PWM_Max && PWM >= PWM_Min)
    {
      Blower.writeMicroseconds(PWM); // 轉動 Blower
    }
    else
    {
      Serial.println("無效 PWM_Offset，重新輸入值：" + (String)(-abs(PWM_Offset_Max)) + "~" + (String)(abs(PWM_Offset_Max)));
    }
    Serial.println("\n\n\n\n");
    PWM_Offset_String = ""; //PWM 偏移字串重置
  }
}

//void printArray(int *ArrayToPrint){
//  for(int i=0;i<<4;i++){
//    Serial.print(ArrayToPrint[i]);
//  }
//  Serial.print('hi');
//}
