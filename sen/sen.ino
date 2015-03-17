int sen[8] = {3,2,4,5,11,9,10,12};
int data[8]  ={0};
int pdata[8] ={0};
int val;
void setup() {
  // put your setup code here, to run once:
  Serial.begin(115200);
}

void loop() {
  int val;
  // put your main code here, to run repeatedly:
  for(int i=0;i<8;i++){
    val = analogRead(sen[i]); 
    Serial.print(i);
    Serial.print(" ");
    data[i] = (data[i]*3+val)>>2;
    pdata[i] = (pdata[i]*31+val)>>5;
    Serial.println(pdata[i]);
    Serial.print(" ");
    Serial.println(data[i]);
  }    
  delay(100);
}
