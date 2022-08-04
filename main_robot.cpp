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
enum Speeds {STOP = 0, MIN = 128, MID = 200, MAX = 255};

#define POLL_RATE_MS 16 // unused unless program complexity makes robot responsiveness poor
#define PACKETSIZE 1
#define INACTIVE_THRESHOLD 1000 // milliseconds until inactivity (1 second)
#define TICK_LENGTH 50 // milliseconds per timed tick

RF24 radio(RADIO_1, RADIO_2);
const byte address[6] = "00001";

unsigned char * rf_input;
unsigned char * movement_vectors;

// BIT FIELD ~~~~~~
// 7: undefined ~ 128
// 6: undefined ~ 64
// 5: undefined ~ 32
// 4: robot hit, slowed ~ 16
// 3: idle ~ 8
// 2: memory error ~ 4
// 1: signal not received ~ 2
// 0: fatal error, exit ~ 1

unsigned char flags;

unsigned char input_type; // 0 = RF joystick, 1 = USB controller, 2 = bluetooth
unsigned char dip_switch;

unsigned long counter;
unsigned long last_transmission_clock;
unsigned long test_clock;

unsigned long last_tick;
unsigned char tick_counter;

unsigned long status;

void set_speed (char left_speed, char right_speed, char left_direction, char right_direction) {
    analogWrite(MOTOR_LA, left_speed); digitalWrite(MOTOR_LD, left_direction);
    analogWrite(MOTOR_RA, right_speed); digitalWrite(MOTOR_RD, right_direction);
}

void tick () {
    last_tick = millis(); // reset counter
    tick_counter++;

    if (flags & 0x1 || flags & 0x4) { // fatal code error, memory or other

    } else if (flags & 0x2) { // signal not received

    }
}

void loop () {
    
    if (radio.available()) { // new packet is available to be processed
        radio.read(rf_input, PACKETSIZE);
        Serial.println(*rf_input);
        last_transmission_clock = millis(); // update last time used
        unsigned char left_motor = (*rf_input & 0xF0) >> 0x4;
        unsigned char right_motor = *rf_input & 0x0F;
        unsigned char straight = (left_motor & 0x8 && right_motor & 0x8) || (left_motor & 0x4 && right_motor & 0x4);
        set_speed(straight ? MAX : (left_motor == 0x0 ? 0 : MID), straight ? MAX : (right_motor == 0x0 ? 0 : MID),
         left_motor >> 0x3, right_motor >> 0x3);

        // update motor state
    } else { // if no signal received
        flags |= 0x02; // set signal flag
        if (millis() - last_transmission_clock >= INACTIVE_THRESHOLD) { // no transmission for quite some time
            set_speed (0, 0, 0, 0);
            Serial.println("No activity from transmitter.");
            delay(1000);
        }
    }
    
    // send motor signals outside of receive loop to allow for ramping of speeds
    if (!(flags & 0x02)) {

    }
    
    if (last_tick + 50 >= millis()) {
        // TICK!
        
    }
    // process accelerometer input
}

int main () {
    // setup sequence    
    init(); // i honestly don't know what this is for but it helps with the serial output

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
    movement_vectors = (unsigned char*)calloc(2, sizeof(unsigned char));

    if (rf_input == nullptr || movement_vectors == nullptr) {
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
    

    while (!(flags & 0x01)) { // while no fatal error has been triggered
        loop();
    }

    

    Serial.println("Program has exit. Reupload or press the reset button to start again.");
    if (flags & 0x02) Serial.println("Fatal memory error.");
    radio.stopListening();
    return(1);
}