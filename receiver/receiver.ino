
#include <SPI.h>
#include <nRF24L01.h>
#include <RF24.h>

#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#define MOTOR_L1 5
#define MOTOR_L2 6

#define MOTOR_R1 3
#define MOTOR_R2 9

#define RADIO_1 7
#define RADIO_2 8

#define POLL_RATE_MS 16 // unused unless program complexity makes robot responsiveness poor
#define PACKETSIZE 2


RF24 radio(7, 8);
const byte address[6] = "00001";

int * rf_input;
int * movement_vectors;

char input_type; // 0 = RF joystick, 1 = USB controller, 2 = bluetooth

void error (char* error) { // holding loop for fatal errors
    while (1) {
        Serial.println(error);
        delay(5000);
    }
}

void setup () {

    // initialise global variables
    rf_input = (int*)malloc(PACKETSIZE * sizeof(int));
    movement_vectors = (int*)calloc(2, sizeof(int));
    if (rf_input == nullptr || movement_vectors == nullptr) error("NOT ENOUGH MEMORY");

    input_type = 0;

    // initialise motor outputs
    pinMode(MOTOR_L1, OUTPUT);
    pinMode(MOTOR_L2, OUTPUT);
    pinMode(MOTOR_R1, OUTPUT);
    pinMode(MOTOR_R2, OUTPUT);

    Serial.begin(9600);
    Serial.println("Program start.");

    radio.begin();
    radio.openReadingPipe(0, address);
    radio.setPALevel(RF24_PA_MIN);
    radio.startListening();
    Serial.println("Radio receiver initialised.");
}

int update_movement () {
    // motor movement is mapped into range of +/-
}

void radio_loop () {
    if (!radio.available()) error("RADIO MISSING");


}

void loop () {
    switch (input_type) {
        case 0:
            radio_loop();
            break;
        case 1:

            break;
        case 2:

            break;
        default:
            error("INVALID INPUT TYPE");
            break;
    }
}

 
