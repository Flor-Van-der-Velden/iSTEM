void setup() {
  pinMode(0,OUTPUT);
  pinMode(1,OUTPUT);
  pinMode(2,OUTPUT);
  pinMode(3,OUTPUT);
  pinMode(4,OUTPUT);
  pinMode(5,OUTPUT);
  pinMode(6,OUTPUT);
  pinMode(7,OUTPUT);
}

void loop() {
  for(int i=0; i<256;i++){
    PORTD = i;
    delayMicroseconds(40);
  }
  for(int j=255; j>0;j--){
    PORTD = j;
    delayMicroseconds(40);
  }
}
