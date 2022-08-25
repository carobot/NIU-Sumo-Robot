#include <SPI.h>
#include <nRF24L01.h>
#include <RF24.h>


RF24 radio(9, 10); // CE, CSN
const byte address[6] = "00001";

int axisdata[4];

int dirA = 7;
int speedA = 6;
int dirB = 4;
int speedB = 5;

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
  //radio.setChannel(76);
 
}

void loop(){

if (radio.available()) {
    //Serial.println("TEST nRF24L01+ receiver");
      radio.read(&axisdata, sizeof(axisdata));
     /*

    Serial.print("Up switch: ");
  Serial.println(axisdata[0]);
  Serial.print("Down switch: ");
  Serial.println(axisdata[1]);
  Serial.print("Left switch: ");
  Serial.println(axisdata[2]);
  Serial.print("Right switch: ");
  Serial.println(axisdata[3]);
        
     char text[32] = "";
    radio.read(&text, sizeof(text));
    Serial.println(text);

    */

  } if (axisdata[0] == HIGH){
      //for (int i = 0; i < 255; i++){
       Serial.print("Up switch: ");
  Serial.println(axisdata[0]);
      analogWrite(speedA, 100);
      digitalWrite(dirA, LOW);
      analogWrite(speedB, 100);
      digitalWrite(dirB, LOW);
      //delay(20);
  

  } if (axisdata[1] == HIGH){
      //for (int i = 0; i < 255; i++){
      Serial.print("Down switch: ");
  Serial.println(axisdata[1]);
      analogWrite(speedA, 100);
      digitalWrite(dirA, HIGH);
      analogWrite(speedB, 100);
      digitalWrite(dirB, HIGH);
      //delay(20);
      
  } if(axisdata[0] == LOW and axisdata[1] == LOW){
    analogWrite(speedA, 0);
    analogWrite(speedB, 0);
    
  } else{
    Serial.println("Fail");
  }

}
