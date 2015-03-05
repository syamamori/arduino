#include <MsTimer2.h>
 
// A相ピン割り当て
#define ENC_A 12
// B相ピン割り当て
#define ENC_B 13
 
// 前回値（Bit1:A相、Bit0:B相）
volatile int oldEnc;
 
// エンコーダ値
volatile int encValue;
 
void doEncoderCounter(void){
  int newEnc = (digitalRead(ENC_A)?0:2) | (digitalRead(ENC_B)?0:1);
   
  if (0) {
    // do nothing
  } else if (0 == oldEnc) {
    if (2 == newEnc) { // up
      encValue++;
    } else if (1 == newEnc) { // down
      encValue--;
    }
  } else if (1 == oldEnc) {
    if (0 == newEnc) { // up
      encValue++;
    } else if (3 == newEnc) { // down
      encValue--;
    }
  } else if (2 == oldEnc) {
    if (3 == newEnc) { // up
      encValue++;
    } else if (0 == newEnc) { // down
      encValue--;
    }
  } else if (3 == oldEnc) {
    if (1 == newEnc) { // up
      encValue++;
    } else if (2 == newEnc) { // down
      encValue--;
    }
  }    
  oldEnc = newEnc;
}
 
void setup() {
  pinMode(ENC_A, INPUT);
  pinMode(ENC_B, INPUT);
  encValue = 0;
  oldEnc = (digitalRead(ENC_A)?0:2) | (digitalRead(ENC_B)?0:1);
  MsTimer2::set(1, doEncoderCounter);
  MsTimer2::start();
  Serial.begin(115200);
}
 
void loop() {
  Serial.println(encValue,DEC);
  delay(1000);
}
