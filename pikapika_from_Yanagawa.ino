#include <MsTimer2.h>
#include <TimerOne.h>

bool flagPWM = false,flagLED = false;
long count = 0;

void test(){
  flagPWM = !flagPWM;
  if(flagLED){
    if(flagPWM)
      digitalWrite(5,HIGH);
    else 
      digitalWrite(5,LOW);
  }
  else
      digitalWrite(5,LOW);
   
    /*For the test: 1s pikapika.*/
    if(count++>125000){
     count = 0;
     flagLED = !flagLED;
    }
}

void setup() {
  // put your setup code here, to run once:
  pinMode(5,OUTPUT);
  Serial.begin(230400);
// MsTimer2::set(20,test);
// MsTimer2::start();
  Timer1.initialize(8);
  Timer1.attachInterrupt(test);
  Timer1.start();
}

void loop() {
  // put your main code here, to run repeatedly:
  int a=analogRead(A0);
  Serial.println(a);
  delay(10);
}
