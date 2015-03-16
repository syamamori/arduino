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
//#define KI 3
//#define KP 300
int KP = 300;
int KI = 3;

Encoder myEnc1(ENC1_A, ENC1_B);
Encoder myEnc2(ENC2_A, ENC2_B);

int tarvA = 0;
int tarvB = 0;
//long encA,encB;
int flagRestWindup = 0;
int calcPID(int tar, int now)
{
  int error;
  static int antiWR = 0;
  int out, trueout;
  static int integral = 0;
  error = (tar - now) << 3 ;
  if (!flagRestWindup) {
    integral += error - antiWR / KP;
    integral = constrain(integral, -2500, 2500);
  }
  out = KI * integral + KP * error ;
  out += 300 * now;
  out = out >> 3;
  trueout = constrain(out, -1023, 1023);
  if(abs(trueout)<100) trueout=0;
  antiWR = out - trueout;
  return trueout;
}

void doEncoderCounter(void) {
  long encA, encB;
  int outA,outB;
  digitalWrite(13, HIGH);
  encA = myEnc1.read();
  encB = myEnc2.read();
  myEnc1.write(0);
  myEnc2.write(0);
  //motorMoveB(500);
  //motorMoveA(500);
  outA = calcPID(tarvA, encA);
  motorMoveA(outA);
  outB = calcPID(tarvB, encB);
  motorMoveB( outB );
  Serial.print("tarA ");
  Serial.print(tarvA);
  Serial.print(" encA ");
  Serial.print(encA);
  Serial.print(" outA ");
  Serial.print(outA);
 // Serial.print('\n');
  Serial.print(" encB ");
  Serial.print(encB);
  Serial.print(" outB ");
  Serial.println(outB);
  //Serial.print(" analog ");
  //Serial.print(analogRead(senA));
  //Serial.print(" ");
  //Serial.print(analogRead(senB));
  Serial.print('\n');
  digitalWrite(13, LOW);
}

void setup() {
  pinMode(dirA,OUTPUT);
  pinMode(dirB,OUTPUT);
  pinMode(speA,OUTPUT);
  pinMode(speB,OUTPUT);
  pinMode(blaA,OUTPUT);
  pinMode(blaB,OUTPUT);
  myEnc1.write(0);
  myEnc2.write(0);
  Serial.begin(115200);
  Timer1.initialize(10000);
  Timer1.attachInterrupt(doEncoderCounter);
  Timer3.initialize(10000);
  Timer3.pwm(speA, 0);
  Timer1.pwm(speB, 0);
  Timer1.pwm(dirA, 0);
  Timer1.pwm(dirB, 0);
}
void motorMoveA(int velo)
{
  if (velo < 0) {
    Timer1.setPwmDuty(dirA, 0);
//    digitalWrite(dirA, LOW);
    digitalWrite(blaA, LOW);
    velo *= -1;
  } else if (velo > 0) {
    Timer1.setPwmDuty(dirA, 1023);
//    digitalWrite(dirA, HIGH);
    digitalWrite(blaA, LOW);
  } else {
    digitalWrite(blaA, HIGH);
  }
  Timer3.setPwmDuty(speA, velo);
}
void motorMoveB( int velo)
{
  if (velo < 0) {
    Timer1.setPwmDuty(dirB, 0);
//    digitalWrite(dirB, LOW);
    digitalWrite(blaB, LOW);
    velo *= -1;
  } else if (velo > 0) {
    Timer1.setPwmDuty(dirB, 1023);
    //  digitalWrite(dirB, HIGH);
    digitalWrite(blaB, LOW);
  } else {
    digitalWrite(blaB, HIGH);
  }
  Timer1.setPwmDuty(speB, velo);
}
void loop() {
  static int vel = -200;
  static int dir = 1;

  //motorMoveA(vel);
  //  tarvB = vel;
  //  tarvA = vel;
  //motorMoveB(vel);
  //  char c;
  //  if(Serial.available()){
  //     c = Serial.read();
  //     Serial.flush();
  //     Serial.print(c);
  //  }
//  vel += dir;
//  if ( abs(vel) == 500 ) dir *= -5;
  delay(50);
  if ( 5 < Serial.available()) {
    int c;
    c = Serial.read();
    if ( c == 'p') {
      c = (Serial.read() - 48) * 10000;
      c += (Serial.read() - 48) * 1000;
      c += (Serial.read() - 48) * 100;
      c += (Serial.read() - 48) * 10;
      c += (Serial.read() - 48);
      KP = c;
      Serial.print("KP");
      Serial.println(KP);
    } else if ( c == 'i') {
      c = (Serial.read() - 48) * 10000;
      c += (Serial.read() - 48) * 1000;
      c += (Serial.read() - 48) * 100;
      c += (Serial.read() - 48) * 10;
      c += (Serial.read() - 48);
      KI = c;
      Serial.print("Ki");
      Serial.println(KI);
    } else if (c == 't') {
      c = (Serial.read() - 48) * 100;
      c += (Serial.read() - 48) * 10;
      c += (Serial.read() - 48);
      Serial.read();
      Serial.read();
      tarvB = c;
      tarvA = c;
      //Serial.print("tarvB");
      //Serial.println(tarvB);
      //        Serial.print("encB");
      //        Serial.println(encB);
    }
    Serial.flush();
  }
}
