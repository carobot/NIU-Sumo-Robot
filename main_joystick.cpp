#include <Arduino.h>
#include <SPI.h>
#include <RF24.h>
#include <nRF24L01.h>

enum Channels {CHANNEL_A = 72, CHANNEL_B = 32, CHANNEL_C = 64, CHANNEL_D = 69};
enum Pins {DIP_0 = 4, DIP_1 = 5};

int upSwitchA = A5;   // left joystick is A
int downSwitchA = A3;  // right joystick is B

int upSwitchB = 2;
int downSwitchB = 3;

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
      
    pinMode(upSwitchA, INPUT_PULLUP);
    pinMode(downSwitchA, INPUT_PULLUP);

    pinMode(upSwitchB, INPUT_PULLUP);
    pinMode(downSwitchB, INPUT_PULLUP);

    pinMode(DIP_0, INPUT_PULLUP);
    pinMode(DIP_1, INPUT_PULLUP);

    // read DIP switch bit field for channel
    unsigned char channel_mode = 0x0;
    channel_mode |= (~digitalRead(DIP_0) & 0x1); channel_mode |= (~digitalRead(DIP_1) & 0x1) << 0X1;
    Serial.println(channel_mode);
    switch (channel_mode & 0x3) {
        case (0x0):
            radio.setChannel(CHANNEL_A);
            break;
        case (0x1):
            radio.setChannel(CHANNEL_B);
            break;
        case (0x2):
            radio.setChannel(CHANNEL_C);
            break;
        case (0x3):
            radio.setChannel(CHANNEL_D);
            break;
    }
    channel_mode = 0x0;

}

void loop() {

    // convert input values to char bit field for transmission
    register unsigned char output_field = 0x00;
    output_field |= (~digitalRead(upSwitchA) & 0x01) << 0x7; output_field |= (~digitalRead(downSwitchA) & 0x01) << 0x6;
    //output_field |= digitalRead(rightSwitchA) << 0x5; output_field |= digitalRead(leftSwitchA) << 0x4;

    output_field |= (~digitalRead(upSwitchB) & 0x01) << 0x3; output_field |= (~digitalRead(downSwitchB) & 0x01) << 0x2;
    //output_field |= digitalRead(rightSwitchB) << 0x1; output_field |= digitalRead(leftSwitchB);

    //output_field = ~output_field;
    radio.write(&output_field, sizeof(unsigned char));
}
