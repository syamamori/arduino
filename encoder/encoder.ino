//#include <TimerThree.h>
#include <TimerOne.h>
#include <Encoder.h>
 
// A相ピン割り当て
#define ENC1_A 18
// B相ピン割り当て
#define ENC1_B 19
// A相ピン割り当て
#define ENC2_A 20
// B相ピン割り当て
#define ENC2_B 21

//motor pin 
#define dirA 12
#define speA 3
#define blaA 9
#define senA 0

#define dirB 13
#define speB 11
#define blaB 8
#define senB 1

Encoder myEnc1(ENC1_A, ENC1_B);
Encoder myEnc2(ENC2_A, ENC2_B);

void doEncoderCounter(void){
  long encA,encB;
  encA = myEnc1.read();
  encB = myEnc2.read();
  Serial.print("encA");
  Serial.print(encA);
  Serial.print("encB");
  Serial.print(encB);
  myEnc1.write(0);
  myEnc2.write(0);
}

void setup() {
  myEnc1.write(0);
  myEnc2.write(0);
  Serial.begin(115200);
  Timer1.initialize(2000);
  Timer1.attachInterrupt(doEncoderCounter); // blinkLED to run every 0.15 seconds
// Timer3.pwm(speA,0);
  analogWrite(speA,0);
  Timer1.pwm(speB,0);
}
void motorMoveA( long velo)
{
  if(velo<0){
    digitalWrite(dirA,LOW);
    digitalWrite(blaA,LOW);
    velo *= -1;
  }else if(velo>0){
    digitalWrite(dirA,HIGH);
    digitalWrite(blaA,LOW);
  }else{
    digitalWrite(blaA,HIGH); 
  }
  analogWrite(speA,velo*255/1023);
 // Timer3.setPwmDuty(speA, velo);    
}
void motorMoveB( long velo)
{
  if(velo<0){
    digitalWrite(dirB,LOW);
    digitalWrite(blaB,LOW);
    velo *= -1;
  }else if(velo>0){
    digitalWrite(dirB,HIGH);
    digitalWrite(blaB,LOW);
  }else{
    digitalWrite(blaB,HIGH); 
  }
  Timer1.setPwmDuty(speB, velo);    
}
void loop() {
  motorMoveA(1023);
  motorMoveB(1023);
  delay(5000);
  motorMoveA(0);
  motorMoveB(0);
  delay(5000);  
  motorMoveA(-1023);
  motorMoveB(-1023);
}
