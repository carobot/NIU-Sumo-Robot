#include <Arduino.h>
#include <SPI.h>
#include <nRF24L01.h>
#include <RF24.h>
#include "SparkFun_MMA8452Q.h"

#include <Wire.h>
#include <FastLED.h>

#include <stdlib.h>
#include <time.h>

enum Pins {MOTOR_LA = 6, MOTOR_LD = 7, MOTOR_RA = 5, MOTOR_RD = 4, RADIO_1 = 9, RADIO_2 = 10, DIP_0 = 3, DIP_1 = 14, DIP_2 = 8, DIP_3 = 2, LED_DATA = 15};
enum Channels {CHANNEL_A = 72, CHANNEL_B = 40, CHANNEL_C = 64, CHANNEL_D = 69};
enum DriveMode {TANK_DRIVE, RAMP_DRIVE, DUAL_AXIS_DRIVE};
enum Directions {BACKWARDS = 0, FORWARDS = 1};
enum Speeds {STOP = 0, MIN = 64, MID = 128, MAX = 200, OGREDRIVE = 255};

#define PACKETSIZE 1
#define INACTIVE_THRESHOLD 1000 // milliseconds until inactivity (1 second)
#define TICK_LENGTH 50 // milliseconds per timed tick
#define TURBO_DELAY 100 // delay for turbo kick in, ticks
#define SLEEP_DELAY 30000
#define SLOW_PERCENTAGE 50 // percentage of slow when hit

RF24 radio(RADIO_1, RADIO_2);
const byte address[6] = "00001";

unsigned char * rf_input;
unsigned char * prev_input;
unsigned char * movement_vectors;

// BIT FIELD ~~~~~~
// 7: undefined ~ 128
// 6: undefined ~ 64
// 5: turbo active ~ 32
// 4: robot hit, stunned ~ 16
// 3: idle, sleep ~ 8
// 2: memory error ~ 4
// 1: signal not received ~ 2
// 0: fatal uncategorised error, exit ~ 1

unsigned char flags;

unsigned char input_type; // 0 = RF joystick, 1 = USB controller, 2 = bluetooth
unsigned char dip_switch;

unsigned long last_transmission_clock;

unsigned long last_tick;
unsigned long tick_counter;
unsigned long forward_start_tick;

unsigned char light_strip;
CRGB * light_array;
unsigned char light_brightness;

unsigned char pokemon_mode;

MMA8452Q accel;
unsigned long hit_start_tick;

void set_light (char lights, char brightness, CRGB colour) {
    FastLED.setBrightness(brightness); // set lights on a per-bit basis based on input char mask
    for (int i = 0; i < 8; i++) {
        if ((lights & (0x1 << i)) >> i) light_array[i] = colour;
        else light_array[i] = CRGB::Black;
    } FastLED.show();
}

void set_speed (char lefts, char rights, char leftd, char rightd) {
    if (flags & 0x16 >> 4) { // if slowed, reduce input speed
        lefts /= SLOW_PERCENTAGE; rights /= SLOW_PERCENTAGE;
    }
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
    if (((flags & 0x1) || (flags & 0x4)) >> 2) { // fatal code error, memory or other
        set_light(0x01, 100, CRGB::Red);
    } else if ((flags & 0x2) >> 1) { // signal not received
        if ((tick_counter + 2) % 40 == 0) {
            set_light(0x01, 50, CRGB::Green);
        } else if (tick_counter % 40 == 0) {
            set_light(0x00, 50, CRGB::White);
        }
    } else if ((flags & 0x8) >> 3) { // robot is idle

    } else if ((flags & 0x10) >> 4) { // robot is hit, stunned
        Serial.println(tick_counter);
        Serial.println(hit_start_tick);
        if (tick_counter == hit_start_tick + 1) { // flash red if hit
            set_light(0xFF, 50, CRGB::Red);
        } else if (tick_counter >= hit_start_tick + 5) {
            Serial.println("Reset phase");
            switch (pokemon_mode) { // set active operation colour to pokemon shade
                case (0x0): // pikachu or dragonite
                    set_light(0xFF, 25, CRGB::Yellow);
                    break;
                case (0x1): // blastoise
                    set_light(0xFF, 25, CRGB::DarkCyan);
                    break;
                case (0x2): // snorlaxative
                    set_light(0xFF, 25, CRGB::HotPink);
                    break;
                case (0x3): // eevee
                    set_light(0xFF, 25, CRGB::Chocolate);
                    break;
            }
            flags &= ~0x10; // reset flags to normal operation after light flash
        }
    } else if ((flags & 0x20) >> 5) { // turbo active

    } else { // standard operation, no flags
        switch (pokemon_mode) { // set active operation colour to pokemon shade
            case (0x0): // pikachu or dragonite
                set_light(0xFF, 25, CRGB::Yellow);
                break;
            case (0x1): // blastoise
                set_light(0xFF, 25, CRGB::DarkCyan);
                break;
            case (0x2): // snorlaxative
                set_light(0xFF, 25, CRGB::HotPink);
                break;
            case (0x3): // eevee
                set_light(0xFF, 25, CRGB::Chocolate);
                break;
        }
    }
}

void loop () {
    
    if (radio.available()) { // new packet is available to be processed
        flags &= ~0x02;
        *prev_input = *rf_input;
        radio.read(rf_input, PACKETSIZE);
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

                // breathe to indicate entry into LP
                for (float i = 0; i < 6.28f; i += 0.05f) {
                    float brightness_val = (sin(i - 1.57f) + 1.0f) * 128.0f;
                    set_light(0x01, brightness_val, CRGB::Green);
                    delay(20);
                } set_light(0x00, 50, CRGB::White);

                // occassional reminder of LP mode
                while (!radio.available()) {
                    Serial.println("Low power sleep mode.");
                    delay(5000);
                    for (float i = 0; i < 6.28f; i += 0.25f) {
                        set_light(0x01, (sin(i - 1.57f) + 1.0f) * 32.0f, CRGB::Green);
                        delay(20);
                    } set_light(0x00, 50, CRGB::White);
                }
                // breathe white when normal transmission resumed
                for (float i = 0; i < 6.28f; i += 0.5f) {
                    float brightness_val = (sin(i - 1.57f) + 1.0f) * 128.0f;
                    set_light(0x01, brightness_val, CRGB::White);
                    delay(10);
                } set_light(0x00, 50, CRGB::White);
            }

            
        }
    }
    if (accel.available() && accel.readTap() > 0) {
        hit_start_tick = tick_counter;
        flags |= 0x10;
    }
    if (millis() - last_tick >= TICK_LENGTH) {
        // TICK!
        last_tick = millis();
        //Serial.println("Tick!");
        tick();
    }
    
    // set speed per loop to allow for software modulated speed
    set_speed(movement_vectors[0], movement_vectors[1], movement_vectors[2], movement_vectors[3]);
}

int main () {
    // setup sequence    
    init(); // i honestly don't know what this is for but it helps with the serial output
    Serial.begin(9600);
    Serial.println("Program start.");

    /*{ // clear buffer
        unsigned char temp_dump = 0;
        radio.read(&temp_dump, PACKETSIZE);
    }*/

    flags = 0x00;

    last_tick = millis();
    tick_counter = 0;
    light_strip = 0x0;
    light_brightness = 0;

    Wire.begin();
    

    // initialise global variables
    rf_input = (unsigned char*)calloc(PACKETSIZE, sizeof(unsigned char));
    prev_input = (unsigned char*)calloc(PACKETSIZE, sizeof(unsigned char));
    movement_vectors = (unsigned char*)calloc(4, sizeof(unsigned char));
    light_array = (CRGB*)malloc(6 * sizeof(CRGB));

    if (rf_input == nullptr || movement_vectors == nullptr || prev_input == nullptr || light_array == nullptr) {
        Serial.println("Not enough memory, program will now exit.");
        flags |= 0x05;
        return(1);
    }

    if (!accel.begin()) {
        Serial.println("Accelerometer not integrated.");
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
    radio.setPALevel(RF24_PA_LOW);
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

    // read DIP switch bit field for channel
    pokemon_mode = 0x0;
        
    pokemon_mode |= (~digitalRead(DIP_2) & 0x1); pokemon_mode |= (~digitalRead(DIP_3) & 0x1) << 0X1;
    Serial.print("Pokemon ");
    Serial.println(pokemon_mode);
    
    // conserve power, turn off switches after read
    digitalWrite(DIP_0, 0); digitalWrite(DIP_1, 0);
    digitalWrite(DIP_2, 0); digitalWrite(DIP_3, 0);

    accel.init(SCALE_4G, ODR_50); // Initialize accelerometer with lower sensitivity

    FastLED.addLeds<NEOPIXEL, LED_DATA>(light_array, 6);
    
    for (int i = 0; i < 8; i++) { // end of start sequence
        set_light(0x1 << i, 100, CRGB::White);
        delay(75);
    } for (int i = 7; i >= 0; i--) { // end of start sequence
        set_light(0x1 << i, 100, CRGB::White);
        delay(75);
    } set_light(0x00, 50, CRGB::White);

    while (!(flags & 0x01)) { // while no fatal error has been triggered
        loop();
    }

    

    Serial.println("Program has exit. Reupload or press the reset button to start again.");
    if (flags & 0x02) Serial.println("Fatal memory error.");
    radio.stopListening();
    set_light(0x01, 50, CRGB::Red);
    return(1);
}