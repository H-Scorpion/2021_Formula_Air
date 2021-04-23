#include <Servo.h>

/*** Blower：鼓風機 ***/
//鼓風機的 PWM 最低啟動數值：1230
//1500是停(中間值)
Servo Blower;
const int Blower_Pin = 9; //控制 Blower 的腳位
int PWM_Width = 0; //控制轉速的脈寬長度
const int PWM_Active_Max = 1500; //Blower 的啟動上限電壓
const int PWM_Active_Min = 1300; //Blower 的啟動下限電壓
const int PWM_Width_Max = 1600; //手動最大
const int PWM_Width_Min = 0; //手動最小

String PWM_String = ""; //PWM字串

void setup() {
  Blower.attach(Blower_Pin);  // 將 Blower 連接到 Blower_Pin
  Serial.begin(9600);
  Blower.writeMicroseconds(PWM_Active_Max); //啟動 Blower
  //Blower.writeMicroseconds(PWM_Active_Min); //啟動 Blower
  Serial.println("START"); 
}

void loop() {
  //接收使用者輸入的數值，並傳到 PWM_String
  while (Serial.available() > 0)
  {
    char c = Serial.read();
    PWM_String += (String)c;
    delay(10); //等待一下，好讓 Serial 能夠接受新的資料
  }
  
  if (PWM_String != "")
  {
    PWM_Width = PWM_String.toInt();
    Serial.println("PWM_Width(String)：" + (String)PWM_Width);
    
    //判斷 PWM_Width 是否在合理範圍內：0~1600
    if (PWM_Width <= PWM_Width_Max && PWM_Width >= PWM_Width_Min)
    {
      Blower.writeMicroseconds(PWM_Width); // 轉動 Blower
    }
    else
    {
      Serial.println("無效PWM值，重新輸入PWM值：0~1600");
    }
    Serial.println("\n\n\n\n");
    PWM_String = ""; //PWM字串重置
  }
}
