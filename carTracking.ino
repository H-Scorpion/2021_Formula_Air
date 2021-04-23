#include <Servo.h>

/*** 時間差 ***/
int t1 = 0;
int t2 = 0;

/*** 方向 ***/
bool forward = 1;
bool back = 0;

/*** Stage：循跡階段***/
int stage = 0; //循跡階段
int stage_Time[6] = {1000, 0, 1946, 0, 0}; //stage_X 持續時間
int stage_Rotating_Angle[6] = {22, 0, -22, 23, 0}; //stage_X 旋轉角度


/*** Blower：鼓風機 ***/
/*鼓風機的 PWM 最低啟動數值：
  雙向：1500， 接著開始遠離 1500*/
Servo Blower;
const int Blower_Pin = 9; //控制 Blower 的腳位
int Blower_PWM = 0; //控制轉速的 PWM

     /*** 雙向 ***/
const int Blower_PWM_Base = 1500; //Blower 的啟動 PWM 值
const int Blower_PWM_Start = Blower_PWM_Base + 150; //吹動車子的 PWM 值
const int Blower_PWM_Start_Back = Blower_PWM_Base - 95; //吹動車子的 PWM 值
const int Blower_PWM_Run = Blower_PWM_Base + 80; //吹動車子的 PWM 值
const int Blower_PWM_RunBack = Blower_PWM_Base - 47; //吹動車子的 PWM 值
const int Blower_PWM_Max = Blower_PWM_Base + 160; //Blower PWM 的最大值
const int Blower_PWM_Min = Blower_PWM_Base - 160; //Blower PWM 的最小值




/*** Steering：轉向機構 ***/
Servo Steering; 
const int Steering_Pin = 3; //控制 Steering 的腳位
int Steering_Angle = 0; //Steering 的角度位置
int Steering_Rotating = 0; //Steering 的旋轉角度
const int Steering_Rotating_limit = 23; //Steering 的旋轉角度"極限"
const int Steering_Angle_Central = 95; //Steering 的中心角度
const int Steering_Angle_Max = Steering_Angle_Central + Steering_Rotating_limit; //Steering 的最大角度位置
const int Steering_Angle_Min = Steering_Angle_Central - Steering_Rotating_limit; //Steering 的最小角度位置
const int Steering_Angle_Initial = -15; //Steering 初始角度

/*** Brake：煞車 ***/
Servo Brake;
const int Brake_Pin = 5; //控制 Brake 的腳位
int Brake_Angle = 0; //Brake 的角度位置
const int Brake_Angle_Base = 85; //Brake 中心角度
const int Brake_Angle_Max = Brake_Angle_Base + 10; //Brake 的最大角度位置
const int Brake_Angle_Min = Brake_Angle_Base; //Brake 的最小角度位置
int Brake_Rotating_Angle = 0;


/*** IR_Tracking：循跡 ***/
const int IR_pin[7] = {2, 4, 6,     A0, A1, A2,   A3};
int IR_Rotating_Angle[7] = {-5, 0, 5,     -7, 8, 0, 7};



void Blow_up(int pwm)
{
  if (pwm <= Blower_PWM_Max && pwm >= Blower_PWM_Min)
  {
    Blower.writeMicroseconds(pwm);
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

void brake_Car(int Rotate_Angle) //旋轉角度
{
  Brake_Angle = Brake_Angle_Base + Rotate_Angle;
  
  //判斷 Brake_Angle 是否為合法角度
  if (Brake_Angle <= Brake_Angle_Max && Brake_Angle >= Brake_Angle_Min)
  {
    Brake.write(Brake_Angle);     
  }
}

void IR_Judge()
{
  int back_touch = 0;
  int back_black = 0;
  int track_line = 1;

  Serial.println("forward..");

  
  while (forward)
  {
    if (digitalRead(IR_pin[0]) == 1)
    {
      Rotate(IR_Rotating_Angle[0]);
    }
  
    if (digitalRead(IR_pin[1]) == 1)
    {
      Rotate(IR_Rotating_Angle[1]);
    }
  
    if (digitalRead(IR_pin[2]) == 1)
    {
      Rotate(IR_Rotating_Angle[2]);
    }

    if (digitalRead(IR_pin[3]) == 1 && digitalRead(IR_pin[6]) == 1)
    {
      back_black = 1;
    }

    if (digitalRead(IR_pin[3]) == 0 && digitalRead(IR_pin[6]) == 0 && back_black == 1)
    {
      back_touch++;
      back_black = 0;

      //黑線觸碰 2 次，進行 S 型轉彎
      if (back_touch == 1)
      {
        Serial.println("back1 back4 touch 2");
        if (track_line == 1)
        { 
          track_line++;

          //向左轉
          Rotate(-22);
          delay(950);

          //車頭擺正
          Rotate(0);
          delay(1250);

          //向右轉
          Rotate(22);
          delay(950);

          //車頭擺正
          Rotate(0);

          if (digitalRead(IR_pin[0]) == 1 && digitalRead(IR_pin[2]) == 1)
          {
            //碰到終點第一條黑線，斷電 0.3 秒
            Blow_up(1500);
            delay(300);

            Blow_up(1430);
            delay(300);
          }

          //在第一停止區等一下
          delay(8000);
          
          back_touch = 0;
          forward = 0;
          back = 1;
        }
      }
    }
   }
   back_black = 0;
   back_touch = 0;

   //從第一停止區向後加速
   Blow_up(Blower_PWM_Start_Back);
   delay(700);

   //降成一般速
   Blow_up(Blower_PWM_RunBack);
   Serial.println("back..");

   int forward_black = 0;
   int forward_touch = 0;
   int back2_black = 0;
   int back3_black = 0;
   while (back)
   {  
      if (digitalRead(IR_pin[3]) == 1)
      {
        Rotate(IR_Rotating_Angle[3]);
      }
    
      if (digitalRead(IR_pin[5]) == 1)
      {
        Rotate(IR_Rotating_Angle[5]);
      }
    
      if (digitalRead(IR_pin[6]) == 1)
      {
        Rotate(IR_Rotating_Angle[6]);
      }

      if (digitalRead(IR_pin[0]) == 1 && digitalRead(IR_pin[2]) == 1)
      {
        forward_black = 1;
      }
  
      if (digitalRead(IR_pin[0]) == 0 && digitalRead(IR_pin[2]) == 0 && forward_black == 1)
      {
        forward_touch++;
        forward_black = 0;
        if (forward_touch == 3)
        {
          Blow_up(1500);
          delay(50);
          Blow_up(1585);
          delay(1290);
          Blow_up(1500);
          while (1)
          {
            
          }
        }
      }
   }
}



void setup() {
  /*Blower 初始化*/
  Blower.attach(Blower_Pin);  // 將 Blower 連接到 Blower_Pin
  Blow_up(Blower_PWM_Base);

  /*Steering 初始化*/
  Steering.attach(Steering_Pin); // 將 Steering 連接到 Steering_Pin
  Rotate(Steering_Angle_Initial); //初始化 Steering 角度

  /*Brake 初始化*/
  Brake.attach(Brake_Pin); // 將 Brake 連接到 Brake_Pin
  brake_Car(0);

  Serial.begin(9600);
  Serial.println("按下 Enter，吹動車子");

  for (int i = 0; i < 7; i++)
  {
    pinMode(IR_pin[i], INPUT);
  }

  delay(3000); //停 3 秒

}

void loop()
{
  //啟動加速
  Rotate(0);
  Blow_up(1620);
  delay(800);

  //降成一般速，向右轉
  Blow_up(1580);
  Rotate(22);
  delay(1250);

  //車頭擺正
  Rotate(0);
  delay(950);

  //向左轉
  Rotate(-22);
  delay(950);

  //車頭擺正
  Rotate(0);
  
  IR_Judge();
  
}
