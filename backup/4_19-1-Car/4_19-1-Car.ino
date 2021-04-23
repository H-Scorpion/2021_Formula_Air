#include <Servo.h>

/*** 方向 ***/
bool forward = 1;
bool back = 0;

/*** Stage：循跡階段***/
int stage = 0; //循跡階段
int stage_Time[5] = {1946, 0, 1946, 0, 0}; //stage_X 持續時間
int stage_Rotating_Angle[5] = {23, -23, -23, 23, 0}; //stage_X 旋轉角度


/*** Blower：鼓風機 ***/
/*鼓風機的 PWM 最低啟動數值：
  單向：1270， 接著開始往上調
  雙向：1500， 接著開始遠離 1500*/
Servo Blower;
const int Blower_Pin = 9; //控制 Blower 的腳位
int Blower_PWM = 0; //控制轉速的 PWM
      /*** 單向 ***/
/*const int Blower_PWM_Base = 1000; //Blower 的啟動 PWM 值
const int Blower_PWM_Run = 1370; //吹動車子的 PWM 值
const int Blower_PWM_Max = 1500; //Blower PWM 的最大值
const int Blower_PWM_Min = 0; //Blower PWM 的最小值 */

     /*** 雙向 ***/
const int Blower_PWM_Base = 1500; //Blower 的啟動 PWM 值
const int Blower_PWM_Run = Blower_PWM_Base + 110; //吹動車子的 PWM 值
const int Blower_PWM_Max = 1800; //Blower PWM 的最大值
const int Blower_PWM_Min = 1200; //Blower PWM 的最小值 




/*** Steering：轉向機構 ***/
Servo Steering; 
const int Steering_Pin = 3; //控制 Steering 的腳位
int Steering_Angle = 0; //Steering 的角度位置
int Steering_Rotating = 0; //Steering 的旋轉角度
const int Steering_Rotating_limit = 23; //Steering 的旋轉角度"極限"
const int Steering_Angle_Central = 95; //Steering 的中心角度
const int Steering_Angle_Max = Steering_Angle_Central + Steering_Rotating_limit; //Steering 的最大角度位置
const int Steering_Angle_Min = Steering_Angle_Central - Steering_Rotating_limit; //Steering 的最小角度位置
const int Steering_Angle_Initial = Steering_Angle_Max; //Steering 初始角度

/*** Brake：煞車 ***/
Servo Brake;
const int Brake_Pin = 5; //控制 Brake 的腳位
int Brake_Angle = 0; //Brake 的角度位置
int Brake_Rotating = 0; //Brake 的旋轉轉角度
const int Brake_Angle_Base = 60; //Brake 的基準角度
const int Brake_Rotating_limit = 40; //Brake 的旋轉角度極限
const int Brake_Angle_Max = Brake_Angle_Base + Brake_Rotating_limit; //Brake 的最大角度位置
const int Brake_Angle_Min = Brake_Angle_Base; //Brake 的最小角度位置
String Brake_Rotating_String = ""; //Brake 的旋轉角度字串


/*** IR_Tracking：循跡 ***/
const int IR_pin[7] = {2, 4, 6,     A3, A4, A5,   A6};
byte IR_black[7] = {0};
int forward_state = B000;
int back_state = B000;
int IR_Rotating_Angle[6] = {10, 0, -10,     -10, 0, 10};
int delta_Time = 0, t1 = 0, t2 = 0;
int Time_limit = 600;
int count = 2;


/*** passsTime：每一階段運行的時間 ***/
//  int t1, t2, deltatime;
  String s;
  bool countingTime = false;


void Start()
{
  Blower.writeMicroseconds(1230); //啟動 Blower
}

void Stop()
{
  Blower.writeMicroseconds(1000); //停止 Blower
  while (1)
  {
    //停止 Blower 後，停在這裡....
  }
}
void IR_Judge()
{
  if (forward)
  {
    while (1)
    {
      if (IR_black[0] == 1)
      {
        forward_state |= B100;
      }
      else
      {
        forward_state &= B011;
      }
  
      if (IR_black[1] == 1)
      {
        forward_state |= B010;
      }
      else
      {
        forward_state &= B101;
      }
  
      if (IR_black[2] == 1)
      {
        forward_state |= B001;
      }
      else
      {
        forward_state &= B110;
      }
  
      
      
      switch (forward_state)
      {
        case B100:
          Rotate(IR_Rotating_Angle[0]);
          break;
        case B010:
          Rotate(IR_Rotating_Angle[1]);
          break;
        case B001:
          Rotate(IR_Rotating_Angle[2]);
          break;
      }
    }
  }

  if (back)
  {
    while (1)
    {
      if (IR_black[3] == 1)
      {
        back_state |= B1000;
        t1 = millis();
      }
      else
      {
        back_state &= B0111;
      }
  
      if (IR_black[6] == 1)
      {
        back_state |= B0001;
      }
      else
      {
        back_state &= B1110;
      }
    }

    switch (back_state)
    {
      
    }
  }
}

void Rotate(int Rotate_Angle) //旋轉角度
{
  Steering_Angle = Steering_Angle_Central + Rotate_Angle;
  
  //判斷 Steering_Angle 是否為合法角度
  if (Steering_Angle <= Steering_Angle_Max && Steering_Angle >= Steering_Angle_Min)
  {
    //如果轉動 -23~23 度，則 Steering 轉動此角度
    //Serial.println("轉動" + (String)Rotate_Angle + "度");
    Steering.write(Steering_Angle); // 轉動 Steering    
  }
  else if(Steering_Angle > Steering_Angle_Max)
  {
    //如果向右轉超過 23 度，則 Steering 向右轉到最大值(23度)
    //Serial.println("向右轉到極限");
    Steering_Angle = Steering_Angle_Max;
    Steering.write(Steering_Angle); // 轉動 Steering    
  }
  else
  {
    Serial.println("向左轉到極限");
    //如果向左轉超過 23 度，則 Steering 向左轉到最大值(-23度)
    Steering_Angle = Steering_Angle_Min;
    Steering.write(Steering_Angle); // 轉動 Steering    
  }
}

void Run(int stage)
{
  switch (stage)
  {
    case 0:
      Serial.println("Stage 0...");
      Rotate(stage_Rotating_Angle[stage]);
      Blower.writeMicroseconds(Blower_PWM_Run); // 吹動車子
      //t1 = millis();
      //passTime();
      delay(stage_Time[0]);
      break;

    case 1:
      Serial.println("Stage 1...");
      Rotate(stage_Rotating_Angle[stage]);
      
      //如果都沒測到黑線，則停留在這
      while ( digitalRead(IR_pin[0]) == 0 &&
              digitalRead(IR_pin[1]) == 0 && 
              digitalRead(IR_pin[2]) == 0   )
      {
        
      }
      
      IR_Judge();
//      Blower.writeMicroseconds(Blower_PWM_Stop); // 停車
      break;

    case 2:
      Rotate(stage_Rotating_Angle[stage]);
      IR_Judge();

  }
}



/*
void passTime()
{
  
  
  

  while (1)
  {
    while (Serial.available() > 0)
    {
      char c = Serial.read();
      s += (String)c;
      delay(10); //等待一下，好讓 Serial 能夠接受新的資料
    }
    
    if (s != "")
    {
      t2=millis();
      Blower.writeMicroseconds(1000); //停止 Blower
      countingTime=false;
      Serial.println("t2：" + (String)t2);
      deltatime = t2 - t1;
      Serial.println("t2 - t1 = " + (String)deltatime);
      Serial.println("\n\n\n");
  
      s = "";
    }
  }
  
}*/

void Straight()
{
  Steering_Angle = Steering_Angle_Central;
  Steering.write(Steering_Angle); // Steering 轉到中間
}



void setup() {
  /*Blower 初始化*/
  Blower.attach(Blower_Pin);  // 將 Blower 連接到 Blower_Pin
  Blower.writeMicroseconds(Blower_PWM_Base); //啟動 Blower

  /*Steering 初始化*/
  Steering.attach(Steering_Pin); // 將 Steering 連接到 Steering_Pin
  Rotate(15); //初始化 Steering 角度

  /*Brake 初始化*/
  Brake.attach(Brake_Pin); // 將 Brake 連接到 Brake_Pin
  Brake.write(Brake_Angle_Base);

  Serial.begin(9600);
  Serial.println("按下 Enter，吹動車子");

  /*
  while (1)
  {
    while (Serial.available() > 0)
    {
      char c = Serial.read();
      s += (String)c;
      delay(10); //等待一下，好讓 Serial 能夠接受新的資料
    }
    
    if (s != "")
    {
      Serial.println("Start");
      s = "";
      break;
    }
  } */

  
}

void loop()
{
  Run(stage);
  stage++;
}
