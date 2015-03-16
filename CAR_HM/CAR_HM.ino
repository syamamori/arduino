#define DIR_A 12
#define PWM_A 3
#define DIR_B 13
#define PWM_B 11

#define FORWARD 1
#define BACK 0


int mode=FORWARD;
int wait = 0;

void setup() {
  pinMode(DIR_A,OUTPUT);
  pinMode(PWM_A,OUTPUT);
  pinMode(DIR_B,OUTPUT);
  pinMode(PWM_B,OUTPUT);
  Serial.begin(9600) ;
}

void loop() {
  int i;
  //digitalWrite(DIR_B,HIGH);
  
  int field=analogRead(5);
  
  if(field>900 && wait ==0){
    if(mode==FORWARD){
      mode=BACK;
      wait=1;
    }else if(mode==BACK){
     mode=FORWARD;
     wait=1;
    }
    delay(100);
  } else if(field<800){
    wait=0;
  }
  switch (mode){
   case FORWARD: digitalWrite(DIR_A,HIGH);break;
   case BACK:digitalWrite(DIR_A,LOW);break;
   default: break;
   }
   analogWrite(PWM_A,255);
   //analogWrite(PWM_B,255);
   delay(10);
}
