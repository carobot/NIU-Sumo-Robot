
#include <SPI.h>
#include <nRF24L01.h>
#include <RF24.h>
#include <stdio.h>


RF24 radio(7, 8); // CE, CSN
const byte address[6] = "00001";

float mSpeedA;
float mSpeedB;


// motor A is left, B is right
int motorA1A = 5;
int motorA1B = 6; //4 pwm pins in use..required to change speed of motor
int motorB1A = 3;
int motorB1B = 9;


int axisdata[1];

void backward(){
          
     digitalWrite(motorA1A,LOW);
     digitalWrite(motorA1B,HIGH);
     digitalWrite(motorB1A,HIGH);
     digitalWrite(motorB1B,LOW);

     mSpeedA= (-250./508.)*(axisdata[0])+255.;
     mSpeedB= (-250./508.)*(axisdata[0])+255.;
     analogWrite(motorA1A ,mSpeedA);
     analogWrite(motorA1B ,0);
     analogWrite(motorB1A ,0);
     analogWrite(motorB1B ,mSpeedB);
}

void forward(){
          
     digitalWrite(motorA1A,HIGH);
     digitalWrite(motorA1B,LOW);
     digitalWrite(motorB1A,LOW);
     digitalWrite(motorB1B,HIGH);

     mSpeedA=(250./508.)*(axisdata[0])-255.;
     mSpeedB=(250./508.)*(axisdata[0])-255.;
     analogWrite(motorA1A ,0);
     analogWrite(motorA1B ,mSpeedA);
     analogWrite(motorB1A ,mSpeedB);
     analogWrite(motorB1B ,0);
}

void right(){
     digitalWrite(motorA1A,HIGH); //A forward
     digitalWrite(motorA1B,LOW);
     digitalWrite(motorB1A,HIGH); //B backward
     digitalWrite(motorB1B,LOW);

     mSpeedA=(250.0f/508.0f) * (axisdata[0]) - 255.0f;
     analogWrite(motorA1A ,0);
     analogWrite(motorA1B ,mSpeedA);

     mSpeedB= (-250./508.)*(axisdata[0])+255.;
     analogWrite(motorB1A ,0);
     analogWrite(motorB1B ,mSpeedB);

}

void left(){
     digitalWrite(motorB1A,LOW); //B forward
     digitalWrite(motorB1B,HIGH);
     digitalWrite(motorA1A,LOW);  //A backward
     digitalWrite(motorA1B,HIGH);

     mSpeedB=(250./508.)*(axisdata[0])-255.;
     analogWrite(motorB1A ,mSpeedB);
     analogWrite(motorB1B ,0);

     mSpeedA= (-250./508.)*(axisdata[0])+255.;
     analogWrite(motorA1A ,mSpeedA);
     analogWrite(motorA1B ,0);


}

void brake(){
     digitalWrite(motorA1A,LOW);
     digitalWrite(motorA1B,LOW);
     digitalWrite(motorB1A,LOW);
     digitalWrite(motorB1B,LOW);
}

void setup () {
     pinMode(motorA1A, OUTPUT);
     pinMode(motorA1B, OUTPUT);
     pinMode(motorB1A, OUTPUT);
     pinMode(motorB1B, OUTPUT);

     Serial.begin(9600);
     radio.begin();
     //Serial.println("TEST nRF24L01+ receiver");
     radio.openReadingPipe(0, address);
     radio.setPALevel(RF24_PA_MIN);
     radio.startListening();
     //radio.setChannel(76);
     

}
 

void loop() {
  if (radio.available()) {
    Serial.println("TEST nRF24L01+ receiver");
      radio.read(&axisdata, sizeof(axisdata));
  Serial.print("X axis: ");
      Serial.println(axisdata[0]);
       Serial.print("Y axis: ");
      Serial.println(axisdata[1]);

    /*
     char text[32] = "";
    radio.read(&text, sizeof(text));
    Serial.println(text);
   */
      /*
 } if (axisdata[0]<507){
      backward();
      Serial.print("X axis: ");
      Serial.println(axisdata[0]);

 } if(axisdata[0]>509){
      forward();
      Serial.print("X axis: ");
      Serial.println(axisdata[0]);

 } if(axisdata[0] > 506 and axisdata[0] < 509 and axisdata[1] > 510 and axisdata[1] < 513){
      brake();
      Serial.print("X axis: ");
      Serial.println(axisdata[0]);
      Serial.print("Y axis: ");
      Serial.println(axisdata[1]);
 
 } if (axisdata[1]<510){
      right();
      Serial.print("Y axis: ");
      Serial.println(axisdata[1]);
      
 } if (axisdata[1]>513){
      left();
      Serial.print("Y axis: ");
      Serial.println(axisdata[1]);

 }
 */
  }
}

 
