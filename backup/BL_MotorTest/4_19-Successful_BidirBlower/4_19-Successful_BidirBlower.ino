#include <Servo.h>

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

String PWM_Offset_String = ""; //PWM 偏移字串

void setup() {
  Blower.attach(Blower_Pin);  // 將 Blower 連接到 Blower_Pin
  Serial.begin(9600);
  Blower.writeMicroseconds(PWM_Base); //啟動 Blower
  Serial.println("START"); 
}

void loop() {
  //接收使用者輸入的數值，並傳到 PWM_String
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
