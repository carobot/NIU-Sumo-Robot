#include <Arduino.h>
#include <SPI.h>
#include <nRF24L01.h>
#include <RF24.h>

#include <stdlib.h>
#include <time.h>

enum Pins {MOTOR_LA = 6, MOTOR_LD = 7, MOTOR_RA = 5, MOTOR_RD = 4, RADIO_1 = 9, RADIO_2 = 10, DIP_0 = 3, DIP_1 = 14, DIP_2 = 8, DIP_3 = 2};
enum Channels {CHANNEL_A = 72, CHANNEL_B = 32, CHANNEL_C = 64, CHANNEL_D = 69};
enum DriveMode {TANK_DRIVE, RAMP_DRIVE, DUAL_AXIS_DRIVE};
enum Directions {BACKWARDS = 0, FORWARDS = 1};
enum Speeds {STOP = 0, MIN = 64, MID = 128, MAX = 200, OGREDRIVE = 255};

#define POLL_RATE_MS 16 // unused unless program complexity makes robot responsiveness poor
#define PACKETSIZE 1
#define INACTIVE_THRESHOLD 1000 // milliseconds until inactivity (1 second)
#define TICK_LENGTH 50 // milliseconds per timed tick
#define TURBO_DELAY 100 // delay for turbo kick in, ticks
#define SLEEP_DELAY 60000

RF24 radio(RADIO_1, RADIO_2);
const byte address[6] = "00001";

unsigned char * rf_input;
unsigned char * prev_input;
unsigned char * movement_vectors;

// BIT FIELD ~~~~~~
// 7: undefined ~ 128
// 6: undefined ~ 64
// 5: turbo active ~ 32
// 4: robot hit, slowed ~ 16
// 3: idle ~ 8
// 2: memory error ~ 4
// 1: signal not received ~ 2
// 0: fatal uncategorised error, exit ~ 1

unsigned char flags;

unsigned char input_type; // 0 = RF joystick, 1 = USB controller, 2 = bluetooth
unsigned char dip_switch;

unsigned long counter;
unsigned long last_transmission_clock;
unsigned long test_clock;

unsigned long last_tick;
unsigned long tick_counter;
unsigned long forward_start_tick;

unsigned long status;

void set_speed (char lefts, char rights, char leftd, char rightd) {
    analogWrite(MOTOR_LA, lefts); digitalWrite(MOTOR_LD, leftd);

    analogWrite(MOTOR_RA, rights); digitalWrite(MOTOR_RD, rightd);
}

void tick () {
    last_tick = millis(); // reset counter
    tick_counter++;

    if (*prev_input == *rf_input && *rf_input == 0x44) { // ramp backwards
        if (movement_vectors[0] < MAX) {
            movement_vectors[0] += 1; movement_vectors[1] += 1;
            Serial.println("Ramp back");
        }
            
    // if input is forwards, and turbo is timed in
    } else if (*rf_input == 0x88 && tick_counter - forward_start_tick >= TURBO_DELAY) {
        movement_vectors[0] = OGREDRIVE; movement_vectors[1] = OGREDRIVE;
    }

    // set LED statuses
    if (flags & 0x1 || flags & 0x4 >> 2) { // fatal code error, memory or other

    } else if (flags & 0x2 >> 1) { // signal not received
 
    } else if (flags & 0x8 >> 3) { // robot is idle

    } else if (flags & 0x16 >> 4) { // robot is hit, slowed

    } else if (flags & 0x32 >> 5) { // turbo active

    } else { // standard operation, no flags

    }
}

void loop () {
    
    if (radio.available()) { // new packet is available to be processed

        *prev_input = *rf_input;
        radio.read(rf_input, PACKETSIZE);
        //Serial.println(*rf_input);
        last_transmission_clock = millis(); // update last time updated

        if (*prev_input != *rf_input && *rf_input == 0x44) { // if new, updated value backs up, initialise ramp
            Serial.println("backup");
            movement_vectors[0] = MID; movement_vectors[1] = MID; // start at 64, gradually ramp up
            movement_vectors[2] = BACKWARDS; movement_vectors[3] = BACKWARDS;

        } else if (*prev_input != *rf_input && *rf_input == 0x88) { // if new, updated value goes forwards, set to limited speed
            movement_vectors[0] = MAX; movement_vectors[1] = MAX;
            movement_vectors[2] = FORWARDS; movement_vectors[3] = FORWARDS;
            forward_start_tick = tick_counter; // set startup timer mark for turbo mode

        } else if (*rf_input != 0x44 && *rf_input != 0x88) {
            movement_vectors[0] = (*rf_input >> 0x4) ? (MID) : (STOP);
            movement_vectors[1] = (*rf_input & 0x0F) ? (MID) : (STOP);

            movement_vectors[2] = (*rf_input >> 4) & 0x8; // get direction by masking whether forward is pressed
            movement_vectors[3] = (*rf_input & 0x0F) & 0x8;
        }
        
    } else { // if no signal received
        
        if (millis() - last_transmission_clock >= INACTIVE_THRESHOLD) { // no transmission for short time

            set_speed (0, 0, 0, 0);
            flags |= 0x02; // set signal flag

            if (millis() - last_transmission_clock >= SLEEP_DELAY) {
                // if inactive for extended time, enter sleep

                // TODO: LED update before sleep
                while (!radio.available()) {
                    Serial.println("Entering low power sleep mode.");
                    delay(10000);
                }

            
            } else {
                // TODO: LED flash per wait tick
                Serial.println("No activity from transmitter.");
                delay(1000);
            }

            
        }
    }
    
    if (millis() - last_tick >= TICK_LENGTH) {
        // TICK!
        last_tick = millis();
        tick_counter++;
        //Serial.println("Tick!");
        tick();
    }
    
    // set speed per loop to allow for software modulated speed
    set_speed(movement_vectors[0], movement_vectors[1], movement_vectors[2], movement_vectors[3]);
}

int main () {
    // setup sequence    
    init(); // i honestly don't know what this is for but it helps with the serial output

    { // clear buffer
        unsigned char temp_dump = 0;
        radio.read(&temp_dump, PACKETSIZE);
    }

    flags = 0x00;
    counter = 0uL;
    test_clock = 0uL;

    last_tick = millis();
    status = 0x0;
    tick_counter = 0;


    Serial.begin(9600);
    Serial.println("Program start.");

    // initialise global variables
    rf_input = (unsigned char*)calloc(PACKETSIZE, sizeof(unsigned char));
    prev_input = (unsigned char*)calloc(PACKETSIZE, sizeof(unsigned char));
    movement_vectors = (unsigned char*)calloc(4, sizeof(unsigned char));

    if (rf_input == nullptr || movement_vectors == nullptr || prev_input == nullptr) {
        Serial.println("Not enough memory, program will now exit.");
        flags |= 0x05;
        return(1);
    }

    // initialise motor outputs
    pinMode(MOTOR_LA, OUTPUT);
    pinMode(MOTOR_LD, OUTPUT);
    pinMode(MOTOR_RA, OUTPUT);
    pinMode(MOTOR_RD, OUTPUT);

    //initialise dipshits
    pinMode(DIP_0, INPUT_PULLUP);
    pinMode(DIP_1, INPUT_PULLUP);
    pinMode(DIP_2, INPUT_PULLUP);
    pinMode(DIP_3, INPUT_PULLUP);


    radio.begin();
    radio.openReadingPipe(0, address);
    radio.setPALevel(RF24_PA_MIN);
    radio.startListening();

    // read DIP switch bit field for channel
    unsigned char channel_mode = 0x0;
        
    channel_mode |= (~digitalRead(DIP_0) & 0x1); channel_mode |= (~digitalRead(DIP_1) & 0x1) << 0X1;
    Serial.print("Channel ");
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
        default:
            flags |= 0x1; // something is not supposed to happen, kill code
    }
    
    // conserve power, turn off switches after read
    digitalWrite(DIP_0, 0); digitalWrite(DIP_1, 0);
    digitalWrite(DIP_2, 0); digitalWrite(DIP_3, 0);

    while (!(flags & 0x01)) { // while no fatal error has been triggered
        loop();
    }

    

    Serial.println("Program has exit. Reupload or press the reset button to start again.");
    if (flags & 0x02) Serial.println("Fatal memory error.");
    radio.stopListening();
    return(1);
}