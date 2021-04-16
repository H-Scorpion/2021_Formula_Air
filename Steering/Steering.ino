#include <Servo.h>

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

void setup() {
  Steering.attach(Steering_Pin); // 將 Steering 連接到 Steering_Pin
  Serial.begin(9600); 
  Serial.println("START"); 
  Steering_Angle = Steering_Angle_Central;//初始化 Steering 角度
  Steering.write(Steering_Angle);
}

void loop() {
  while (Serial.available() > 0)
  {
    char c = Serial.read();
    Angle_String += (String)c;
    delay(10); //等待一下，好讓 Serial 能夠接受新的資料
  }

  if (Angle_String != "")
  {
    Rotating_Angle = Angle_String.toInt();
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
}
