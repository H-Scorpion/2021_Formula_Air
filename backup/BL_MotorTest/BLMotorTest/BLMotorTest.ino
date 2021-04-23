#include <Servo.h>

Servo myservo;  // create servo object to control a servo

void setup() {
  myservo.attach(9);  //
  Serial.begin(9600);
  myservo. writeMicroseconds (1000);
}

void loop() {
  if(Serial.available()!=0){
    char a = Serial.read();
    if(a == 'A'){
      myservo. writeMicroseconds (1300);
      delay(20);
    }
    else if(a == 'B'){
      myservo. writeMicroseconds (1400);
      delay(20);
    }
    else if(a == 'S'){
      myservo. writeMicroseconds (1000);
      delay(20);
    }
  }
}
