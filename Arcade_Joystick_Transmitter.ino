#include <SPI.h>
#include <RF24.h>
#include <nRF24L01.h>

int axisdata[4];

int upSwitch = A5;
int downSwitch = A3;
int leftSwitch = A2;
int rightSwitch = A4;

RF24 radio(9, 10); // CE, CSN
const byte address[6] = "00001";

void setup() {
  // put your setup code here, to run once:

  Serial.begin(9600); //try 115200
  radio.begin();
  radio.openWritingPipe(address);
  radio.setPALevel(RF24_PA_MIN);
  radio.stopListening();
  //Serial.println("TEST nRF24L01+ transmitter");
  //radio.setChannel(76);

  
pinMode(A2, INPUT_PULLUP);
 pinMode(A3, INPUT_PULLUP);
  pinMode(A4, INPUT_PULLUP);
  pinMode(A5, INPUT_PULLUP);


}

void loop() {
  // put your main code here, to run repeatedly:

/*
 const char text[] = "Hello World";
  radio.write(&text, sizeof(text));
  delay(1000);
*/

int upVal = digitalRead(upSwitch);
int downVal = digitalRead(downSwitch);
int leftVal = digitalRead(leftSwitch);
int rightVal = digitalRead(rightSwitch);


axisdata[0] = upVal;
axisdata[1] = downVal;
axisdata[2] = leftVal;
axisdata[3] = rightVal;

Serial.print("Up switch: ");
  Serial.println(upVal);
  Serial.print("Down switch: ");
  Serial.println(downVal);
  Serial.print("Left switch: ");
  Serial.println(leftVal);
  Serial.print("Right switch: ");
  Serial.println(rightVal);

radio.write(&axisdata, sizeof(axisdata));

}
