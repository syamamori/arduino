#include <TimerThree.h>
#include <TimerOne.h>
#include <Encoder.h>

#define LIMTE 700

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
int KP = 300;
int KI = 3;
int tarvA = 0;
int tarvB = 0;
int flagRestWindup = 0;


// A相ピン割り当て
#define ENC1_A 18
// B相ピン割り当て
#define ENC1_B 19

//motorB 回転方向をそろえるためA,B逆
// A相ピン割り当て
#define ENC2_A 20
// B相ピン割り当て
#define ENC2_B 21
//encoder
Encoder myEnc1(ENC1_A, ENC1_B);
Encoder myEnc2(ENC2_A, ENC2_B);


//photo sensors
int sen[8] = {3, 2, 4, 5, 11, 9, 10, 12};
int data[8] = {0};
int pdata[8] = {0};

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
  if (abs(trueout) < 100) trueout = 0;
  antiWR = out - trueout;
  return trueout;
}

void doEncoderCounter(void) {
  long encA, encB;
  int outA, outB;
  int val;
  digitalWrite(13, HIGH);
  encA = myEnc1.read();
  encB = myEnc2.read();
  myEnc1.write(0);
  myEnc2.write(0);
  outA = calcPID(tarvA, encA);
  motorMoveA(outA);
  outB = calcPID(tarvB, encB);
  motorMoveB( outB );
  for (int i = 0; i < 8; i++) {
    val = analogRead(sen[i]);
    //pdata[i] = (63*pdata[i] + val)>>6;
    data[i] = (3 * data[i] + val) >> 2;
  }
  digitalWrite(13, LOW);
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
void setup() {
  pinMode(dirA, OUTPUT);
  pinMode(dirB, OUTPUT);
  pinMode(speA, OUTPUT);
  pinMode(speB, OUTPUT);
  pinMode(blaA, OUTPUT);
  pinMode(blaB, OUTPUT);
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
int state(void) {
  int b = 0;
  for (int i = 0; i < 8; i++) {
    if ( data[i] > LIMTE) {
      b += (1 << i);
    }
  }
  Serial.print("state");
  Serial.println(b);
  return b;
}
int trac(int a)
{
  int t = 0;
  for (int i = 0; i < 8; i++) {
    t += a % 2;
    a = a >> 1;
  }
  return t;
}
void loop() {
  int mA = 0, mB = 0;
  int s = 0;
  s = state();
  if (s == 0) {
    mA = -8;
    mB = -8;
  } else {
    if (trac(s >> 4) > 2) {
      mA = -8;
      mB = -5;
    } else if(trac(s&0x0f)>2) {
      mA = 8;
      mB = 5;
    }else{
      for (int i = 0; i < 8; i++) {
        if (s & (0x01 << i)) {
          switch (i) {
            case 0:
            case 1:
            case 6:
            case 7:
              mB = 8;
              mA = -8;
            case 2:
            case 3:
            case 4:
            case 5:
              mA = 8;
              mB = -8;
            break;
          }
        }
      }
    }
  }
  tarvA = mA;
  tarvB = mB;
  delay (100);
}
