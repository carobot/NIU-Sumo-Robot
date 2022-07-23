#include <SD.h> // need to include the SD library
#define SD_ChipSelectPin 4 //connect pin 4 of arduino to cs pin of sd card
#include <TMRpcm.h> //Arduino library for asynchronous playback of PCM/WAV files
#include <SPI.h> //  need to include the SPI library

TMRpcm tmrpcm; // create an object for use in this sketch
int temp=1;


// pins 8/9, 11/12, 4/5 (outdated)
// Motor A connections
//A=4, B=3, c=1, D=2
int dirA = 7;
int speedA = 6;
int dirB = A0;
int speedB = 5;
int dirC = 2;
int speedC = 3;
int dirD = 8;
int speedD = 3; //


void setup (){
 
 tmrpcm.speakerPin = 9; //5,6,11 or 46 on Mega, 9 on Uno, Nano, etc
 Serial.begin(9600);
 if (!SD.begin(SD_ChipSelectPin)) // returns 1 if the card is present
 {
  Serial.println("SD fail");
  return;
 }

 tmrpcm.setVolume(5); //
 tmrpcm.play("music1.wav"); //the sound file "song" will play each time the arduino powers up, or is reset
                          //try to provide the file name with extension
                     

Serial.begin(9600);
pinMode(dirA, OUTPUT);
pinMode(speedA, OUTPUT);
pinMode(dirB, OUTPUT);
pinMode(speedB, OUTPUT);
pinMode(dirC, OUTPUT);
pinMode(speedC, OUTPUT);
pinMode(dirD, OUTPUT);
pinMode(speedD, OUTPUT);


  delay(20000);
  analogWrite(speedA, 170);
  digitalWrite(dirA, LOW);
  analogWrite(speedB, 120);
  digitalWrite(dirB, LOW);
  analogWrite(speedC, 110);
  digitalWrite(dirC, LOW);
  analogWrite(speedD, 110);
  digitalWrite(dirD, LOW);
  delay(10000);

  analogWrite(speedA, 170);
  digitalWrite(dirA, HIGH);
  analogWrite(speedB, 120);
  digitalWrite(dirB, HIGH);
  analogWrite(speedC, 110);
  digitalWrite(dirC, HIGH);
  analogWrite(speedD, 110);
  digitalWrite(dirD, HIGH);
  
}


void loop() {
  // put your main code here

}
