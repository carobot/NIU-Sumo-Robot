
// pins 8/9, 11/12, 
// Motor A connections
int dirA = 7;
int speedA = 6;
int dirB = 4;
int speedB = 5;


void setup() {
  // put your setup code here, to run once:
Serial.begin(9600);
pinMode(dirA, OUTPUT);
pinMode(speedA, OUTPUT);
pinMode(dirB, OUTPUT);
pinMode(speedB, OUTPUT);

  delay(60000);
  analogWrite(speedA, 150);
  digitalWrite(dirA, LOW);
  analogWrite(speedB, 150);
  digitalWrite(dirB, LOW);
  delay(60000);
  
  analogWrite(speedA, 150);
  digitalWrite(dirA, HIGH);
  analogWrite(speedB, 150);
  digitalWrite(dirB, HIGH);
  delay(8000);
}


void loop() {
  // put your main code here

}
