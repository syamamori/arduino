#include <TimerThree.h>
#include <TimerOne.h>
#include <Encoder.h>
 
// A相ピン割り当て
#define ENC1_A 18
// B相ピン割り当て
#define ENC1_B 19

//motorB 回転方向をそろえるためA,B逆
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

//PID
#define KI 3
#define KP 80
//int KP = 100;
//int KI = 3;

Encoder myEnc1(ENC1_A, ENC1_B);
Encoder myEnc2(ENC2_A, ENC2_B);

int tarvA;
int tarvB;
//long encA,encB;
int flagRestWindup = 0;
int calcPID(int tar, int now)
{
  int error;
  static int antiWR;
  int out,trueout;
  static int integral = 0;
  error = (tar - now)<<3 ;
  if(!flagRestWindup){  
    integral += error - antiWR/KP;
    integral = constrain(integral, -2500, 2500);
  }
  out = ( KI*integral + KP*error)>>3 ;
  out += 7*now;
  trueout = constrain(out, -1023, 1023);
  antiWR = out - trueout;
  return trueout;
}

void doEncoderCounter(void){
  long encA,encB;
  int out;
  encA = myEnc1.read();
  encB = myEnc2.read();
  myEnc1.write(0);
  myEnc2.write(0);
  //motorMoveB(500);
  motorMoveA(calcPID(tarvA, encA));
  out = calcPID(tarvB, encB);
  motorMoveB( out );
//  Serial.print("encA");
//  Serial.print(encA);
//  Serial.print('\n');
  Serial.print("encB ");
  Serial.print(encB);
  Serial.print(" tar ");
  Serial.print(tarvB);
  Serial.print(" out ");
  Serial.print(out);
  Serial.print(" analog ");
  Serial.print(analogRead(0));
  Serial.print(" ");
  Serial.print(analogRead(1));
  Serial.print('\n');
}

void setup() {
  myEnc1.write(0);
  myEnc2.write(0);
  Serial.begin(115200);
  Timer1.initialize(50000);
  Timer1.attachInterrupt(doEncoderCounter); 
  Timer3.initialize(50000);
  Timer3.pwm(speA,0);
  Timer1.pwm(speB,0);
}
void motorMoveA(int velo)
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
  Timer3.setPwmDuty(speA, velo);    
}
void motorMoveB( int velo)
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
  static int vel = 0;
  static int dir = 1;

  //motorMoveA(vel);
//  tarvB = vel;
//  tarvA = vel;
//  motorMoveB(vel);
//  char c;
//  if(Serial.available()){
//     c = Serial.read();
//     Serial.flush();
//     Serial.print(c);
//  }
//  vel += dir;
  if( abs(vel)==1023 ) dir *= -1;
  delay(50);
  if( 5<Serial.available()){
      int c;
      c = Serial.read();
//      if( c =='p'){
//        c = (Serial.read()-48)*10000;
//        c += (Serial.read()-48)*1000;
//        c += (Serial.read()-48)*100;
//        c += (Serial.read()-48)*10;
//        c += (Serial.read()-48);
//        KP = c;
//        Serial.print("KP");
//        Serial.println(KP);
//      }else if( c =='i'){
//        c = (Serial.read()-48)*10000;
//        c += (Serial.read()-48)*1000;
//        c += (Serial.read()-48)*100;
//        c += (Serial.read()-48)*10;
//        c += (Serial.read()-48);
//        KI = c;
//        Serial.print("Ki");
//        Serial.println(KI);
//      }else 
      if(c=='t'){
        c = (Serial.read()-48)*100;
        c += (Serial.read()-48)*10;
        c += (Serial.read()-48);
        Serial.read();
        Serial.read();
        tarvB = c;
        Serial.print("tarvB");
        Serial.println(tarvB);
//        Serial.print("encB");
//        Serial.println(encB);
      }
      Serial.flush();
  }
}
