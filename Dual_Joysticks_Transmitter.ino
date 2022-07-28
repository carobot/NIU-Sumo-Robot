#include <SPI.h>
#include <RF24.h>
#include <nRF24L01.h>

int axisdata[8]; 


int upSwitchA = A5;   // left joystick is A
int downSwitchA = A3;  // right joystick is B
int leftSwitchA = A2;
int rightSwitchA = A4;

int upSwitchB = 2;
int downSwitchB = 3;
int leftSwitchB = 4;
int rightSwitchB = 5;

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
  radio.setChannel(76);

  
pinMode(upSwitchA, INPUT_PULLUP);
 pinMode(downSwitchA , INPUT_PULLUP);
  pinMode(leftSwitchA, INPUT_PULLUP);
  pinMode(rightSwitchA, INPUT_PULLUP);

  pinMode(upSwitchB, INPUT_PULLUP);
 pinMode(downSwitchB, INPUT_PULLUP);
  pinMode(leftSwitchB, INPUT_PULLUP);
  pinMode(rightSwitchB, INPUT_PULLUP);


}

void loop() {
  // put your main code here, to run repeatedly:

/*
 const char text[] = "Hello World";
  radio.write(&text, sizeof(text));
  delay(1000);
*/

int upValA= digitalRead(upSwitchA);
int downValA = digitalRead(downSwitchA);
int leftValA = digitalRead(leftSwitchA);
int rightValA = digitalRead(rightSwitchA);

int upValB= digitalRead(upSwitchB);
int downValB = digitalRead(downSwitchB);
int leftValB = digitalRead(leftSwitchB);
int rightValB = digitalRead(rightSwitchB);



axisdata[0] = upValA;
axisdata[1] = downValA;
axisdata[2] = leftValA;
axisdata[3] = rightValA;

axisdata[4] = upValB;
axisdata[5] = downValB;
axisdata[6] = leftValB;
axisdata[7] = rightValB;

Serial.print("Up switch A: ");
  Serial.println(upValA);
  Serial.print("Down switch A: ");
  Serial.println(downValA);
  Serial.print("Left switch A: ");
  Serial.println(leftValA);
  Serial.print("Right switch A: ");
  Serial.println(rightValA);


Serial.println("");

Serial.print("Up switch B: ");
  Serial.println(upValB);
  Serial.print("Down switch B: ");
  Serial.println(downValB);
  Serial.print("Left switch B: ");
  Serial.println(leftValB);
  Serial.print("Right switch B: ");
  Serial.println(rightValB);


Serial.println("");


radio.write(&axisdata, sizeof(axisdata));

}
