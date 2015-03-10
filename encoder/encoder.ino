#include <Encoder.h>
#include <MsTimer2.h>
 
// A相ピン割り当て
#define ENC1_A 18
// B相ピン割り当て
#define ENC1_B 19
 
// A相ピン割り当て
#define ENC2_A 20
// B相ピン割り当て
#define ENC2_B 21

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
  MsTimer2::set(2, doEncoderCounter);
  MsTimer2::start();
  Serial.begin(115200);
}
 
void loop() {
}
