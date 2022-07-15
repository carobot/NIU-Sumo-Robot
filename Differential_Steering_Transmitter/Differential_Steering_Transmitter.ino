#include <Arduino.h>
#include <SPI.h>
#include <RF24.h>
#include <nRF24L01.h>

int xAxisPin = A0;
int yAxisPin = A1;

int axisdata[2];

RF24 radio(7, 8); // CE, CSN
const byte address[6] = "00001";


void setup() {

  
  Serial.begin(9600); //try 115200
  radio.begin();
  radio.openWritingPipe(address);
  radio.setPALevel(RF24_PA_MIN);
  radio.stopListening();
  //Serial.println("TEST nRF24L01+ transmitter");
  //radio.setChannel(76);

  pinMode(xAxisPin, INPUT);
  pinMode(yAxisPin, INPUT);

}
void loop() {


int xAxisRead = analogRead(xAxisPin);
int yAxisRead = analogRead(yAxisPin);


Serial.print("X Axis: ");
Serial.println(xAxisRead);

Serial.print("Y Axis: ");
Serial.println(yAxisRead);


axisdata[0] = xAxisRead;
axisdata[1] = yAxisRead;

radio.write(&axisdata, sizeof(axisdata));

/*
 const char text[] = "Hello World";
  radio.write(&text, sizeof(text));
  delay(1000);
*/

}
