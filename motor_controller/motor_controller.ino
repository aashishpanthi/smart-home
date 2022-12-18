//MOTOR1 PINS
int ena = 5;
int in1 = 6;
int in2 = 7;

void setup(){
  pinMode(ena, OUTPUT);
  pinMode(in1, OUTPUT);
  pinMode(in2, OUTPUT);

  //CLOCKWISE MAX SPEED
  digitalWrite(in1,HIGH);
  digitalWrite(in2,LOW);
  analogWrite(ena, 255);
  delay(2000);  
}

void loop() {

 
}
