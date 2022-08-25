#include <Arduino.h>
#include <SPI.h>
#include <nRF24L01.h>
#include <RF24.h>

#include <stdio.h>
#include <stdlib.h>
#include <time.h>

enum Pins {MOTOR_L1 = 5, MOTOR_L2 = 6, MOTOR_R1 = 2, MOTOR_R2 = 9 , RADIO_1 = 7, RADIO_2 = 8};
enum Channels {CHANNEL_A = 69, CHANNEL_B = 97, CHANNEL_C = 125, CHANNEL_D = 179};

#define POLL_RATE_MS 16 // unused unless program complexity makes robot responsiveness poor
#define PACKETSIZE 2
#define INACTIVE_THRESHOLD 16000000 // cycles until inactivity on a 16MHz system (1 second)

RF24 radio(RADIO_1, RADIO_2);
const byte address[6] = "00001";

int * rf_input;
int * movement_vectors;

// BIT FIELD ~~~~~~
// 7: undefined
// 6: undefined
// 5: undefined
// 4: undefined
// 3: undefined
// 2: memory error ~ 4
// 1: signal not received ~ 2
// 0: fatal error, exit ~ 1

char flags;

char input_type; // 0 = RF joystick, 1 = USB controller, 2 = bluetooth
char dip_switch;

unsigned long counter;
unsigned long last_transmission_clock;

void set_speed (char left_speed, char right_speed, char left_direction, char right_direction) {

}

void loop () {
    if (radio.available()) { // new packet is available to be processed
        radio.read(rf_input, PACKETSIZE);
        last_transmission_clock = clock();
    } else {
        flags |= 0x02; // set signal flag
        if (clock() - last_transmission_clock >= INACTIVE_THRESHOLD) { // no transmission for quite some time
            set_speed (0, 0, 0, 0);
        }
    }
    
    // process accelerometer input
}

int main () {
    // setup sequence    
    init(); // i honestly don't know what this is for but it helps with the serial output

    flags = 0x00;
    counter = 0uL;

    Serial.begin(9600);
    Serial.println("Program start.");

    // initialise global variables
    rf_input = (int*)calloc(PACKETSIZE, sizeof(int));
    movement_vectors = (int*)calloc(2, sizeof(int));

    if (rf_input == nullptr || movement_vectors == nullptr) {
        Serial.println("Not enough memory, program will now exit.");
        flags |= 0x05;
        return(1);
    }

    // initialise motor outputs
    pinMode(MOTOR_L1, OUTPUT);
    pinMode(MOTOR_L2, OUTPUT);
    pinMode(MOTOR_R1, OUTPUT);
    pinMode(MOTOR_R2, OUTPUT);

    pinMode(LED_BUILTIN, OUTPUT);

    radio.begin();
    radio.openReadingPipe(0, address);
    radio.setPALevel(RF24_PA_LOW);
    radio.startListening();

    switch (dip_switch & 0x3) {
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
        default:
            flags |= 0x1; // something is not supposed to happen, kill code
    }

    Serial.println("Radio receiver initialised.");
    pinMode(LED_BUILTIN, HIGH);
    delay(400);
    pinMode(LED_BUILTIN, LOW);
    delay(200);
    pinMode(LED_BUILTIN, HIGH);
    delay(400);
    pinMode(LED_BUILTIN, LOW);

    while (!(flags & 0x01)) { // while no fatal error has been triggered
        loop();
    }

    Serial.println("Program has exit. Reupload or press the reset button to start again.");
    if (flags & 0x02) Serial.println("Fatal memory error.");
    radio.stopListening();
    return(1);
}