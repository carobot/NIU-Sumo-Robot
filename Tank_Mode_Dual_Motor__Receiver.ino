#include <SPI.h>
#include <nRF24L01.h>
#include <RF24.h>


int axisdata[8];

RF24 radio(9, 10); // CE, CSN
const byte address[6] = "00001";


int dirA = 7;
int speedA = 6;
int dirB = 4;
int speedB = 5;

int accel = 2;
int speed1 = 40;

void setup(){

pinMode(dirA, OUTPUT);
pinMode(speedA, OUTPUT);
pinMode(dirB, OUTPUT);
pinMode(speedB, OUTPUT);


 Serial.begin(9600);
 radio.begin();
 //Serial.println("TEST nRF24L01+ receiver");
  radio.openReadingPipe(0, address);
  radio.setPALevel(RF24_PA_MIN);
  radio.startListening();
  radio.setChannel(76);
 
}

void loop(){

/*

if (axisdata[0] == HIGH) {
  speed1 = 40;
} else {
  speed1 = speed1>=255 ? 255 : speed1 + accel;
}

*/

if (radio.available()) {
    //Serial.println("TEST nRF24L01+ receiver");
      radio.read(&axisdata, sizeof(axisdata));

    

    Serial.print("Up switch: ");
  Serial.println(axisdata[0]);
  Serial.print("Down switch: ");
  Serial.println(axisdata[1]);
  Serial.print("Left switch: ");
  Serial.println(axisdata[2]);
  Serial.print("Right switch: ");
  Serial.println(axisdata[3]);
  

     /*
     char text[32] = "";
    radio.read(&text, sizeof(text));
    Serial.println(text);
  */
    
    

  } if (axisdata[0] == LOW && axisdata[4] == LOW){
       Serial.print("Up switch: ");
  //Serial.println(axisdata[0]);
      
      analogWrite(speedA, 255);
      digitalWrite(dirA, HIGH);
      analogWrite(speedB, 255);
      digitalWrite(dirB, HIGH);
      //delay(20);
      
  

  } else if (axisdata[1] == LOW && axisdata[5] == LOW){
      //for (int i = 0; i < 255; i++){
      Serial.print("Down switch: ");
  //Serial.println(axisdata[1]);
      analogWrite(speedA, 255);
      digitalWrite(dirA, LOW);
      analogWrite(speedB, 255);
      digitalWrite(dirB, LOW);
      //delay(20);
      
      
  } else if (axisdata[0] == LOW && axisdata[5] == LOW){
      analogWrite(speedA, 150);
      digitalWrite(dirA, HIGH);
      analogWrite(speedB, 100);
      digitalWrite(dirB, LOW);

  } else if (axisdata[1] == LOW && axisdata[4] == LOW){
      analogWrite(speedA, 150);
      digitalWrite(dirA, LOW);
      analogWrite(speedB, 150);
      digitalWrite(dirB, HIGH);

  } else if (axisdata[0] == LOW){
      analogWrite(speedA, 150);
      digitalWrite(dirA, HIGH);
      
  } else if (axisdata[4] == LOW){
      analogWrite(speedB, 150);
      digitalWrite(dirB, HIGH);

  } else if (axisdata[1] == LOW){
      analogWrite(speedA, 150);
      digitalWrite(dirA, LOW);
      
  } else if (axisdata[5] == LOW){
      analogWrite(speedB, 150);
      digitalWrite(dirB, LOW);


  } else if(axisdata[0] == HIGH && axisdata[1] == HIGH){
    analogWrite(speedA, 0);
    analogWrite(speedB, 0);
    
  } else{
    Serial.println("Fail");
    
  }

delay(20);

}



  //delay(20);

  
