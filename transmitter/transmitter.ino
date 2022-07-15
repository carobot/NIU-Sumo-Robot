// transmitter code must standardise radio communication to 0 = neutral, +/- 512 in cardinal directions

#include <Arduino.h>
#include <SPI.h>
#include <RF24.h>
#include <nRF24L01.h>

#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#define Y_PIN 0
#define X_PIN 1
#define CLICK_PIN 2

#define RADIO_1 7
#define RADIO_2 8
#define PACKETSIZE 2

RF24 radio(RADIO_1, RADIO_2);
const byte address[6] = "00001";

char click_down;
long click_time;

void setup () {
    // initialise global variables
    click_down = 0;
    click_time = 0L;

    Serial.begin(9600); // start debug output

    Serial.println("Serial monitor initialised. 1");
    radio.begin();
    radio.openWritingPipe(address);
    radio.setPALevel(RF24_PA_MIN);
    radio.stopListening();
    Serial.println("Radio module initialised. 2");

    pinMode(X_PIN, INPUT);
    pinMode(Y_PIN, INPUT);
    pinMode(CLICK_PIN, INPUT);
    Serial.println("Pin inputs initialised. 3");
    Serial.println("done.");

    // possibly set up pairing sequence if needed later
}

void calibrate () {
    
}

void loop () {


    int * reads = (int*)malloc(PACKETSIZE * sizeof(int));
    reads[0] = analogRead(X_PIN); reads[1] = analogRead(Y_PIN);

    //Serial.print("Joystick: ("); Serial.print(reads[0]); Serial.print(", "); Serial.print(reads[1]); Serial.println(")");

    attachInterrupt(digitalPinToInterrupt(CLICK_PIN), calibrate, LOW); // interrupt for sync

    radio.write(reads, PACKETSIZE * sizeof(int));
    delay(100);
}