#include <Servo.h>



/*** Blower：鼓風機 ***/
//鼓風機的 PWM 最低啟動數值：1230
Servo Blower;
const int Blower_Pin = 9; //控制 Blower 的腳位
int Blower_PWM = 0; //控制轉速的 PWM
const int Blower_PWM_Active = 1000; //Blower 的啟動電壓
const int Blower_PWM_Max = 1600; //Blower PWM 的最大值
const int Blower_PWM_Min = 0; //Blower PWM 的最小值
String Blower_PWM_String = ""; //Blower PWM 字串


/*** Steering：轉向機構 ***/
Servo Steering; 
const int Steering_Pin = 3; //控制 Steering 的腳位
int Steering_Angle = 0; //Steering 的角度位置
int Steering_Rotating = 0; //Steering 的旋轉角度
const int Steering_Rotating_limit = 25; //Steering 的旋轉角度"極限"
const int Steering_Angle_Central = 95; //Steering 的中心角度
const int Steering_Angle_Initial = Steering_Angle_Central ; //Steering 的初始角度
const int Steering_Angle_Max = Steering_Angle_Central + Rotating_Angle_limit; //Steering 的最大角度位置
const int Steering_Angle_Min = Steering_Angle_Central - Rotating_Angle_limit; //Steering 的最小角度位置
String Steering_Angle_String= ""; //Steering 的字串角度


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


void setup() {
  /*Blower 初始化*/
  Blower.attach(Blower_Pin);  // 將 Blower 連接到 Blower_Pin
  Blower.writeMicroseconds(Blower_PWM_Active); //啟動 Blower

  /*Steering 初始化*/
  Steering.attach(Steering_Pin); // 將 Steering 連接到 Steering_Pin
  Steering.write(Steering_Angle_Initial);//初始化 Steering 角度

  /*Brake 初始化*/
  Brake.attach(Brake_Pin); // 將 Brake 連接到 Brake_Pin
  Brake.write(Brake_Angle_Base);
}

void loop() {
  // put your main code here, to run repeatedly:

}
