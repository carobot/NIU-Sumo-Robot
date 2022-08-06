#include <SD.h> // need to include the SD library
#define SD_ChipSelectPin 4 //connect pin 4 of arduino to cs pin of sd card
#include <TMRpcm.h> //Arduino library for asynchronous playback of PCM/WAV files
#include <SPI.h> //  need to include the SPI library

//pins 9-13 required for the audio system
int trig_pin = 7;
int echo_pin = 6;
int relay1 = 2; // to lower jack Set pin to LOW to activate relay 
int relay2 = 3; // to raise jack Set pin to LOW to activate relay 
int upSwitch = A4; // to raise the platform manually 
int downSwitch = A5; // to lower the platform manually
const int resetPin = A0; // click to reset the mechanism
const int buttonPin = A1; // click to start the mechanism

TMRpcm tmrpcm; // create an object for use by the music library
int temp=1;



void setup() {

pinMode(A0, INPUT_PULLUP);
pinMode(A1, INPUT_PULLUP);
pinMode(A4, INPUT_PULLUP);
pinMode(A5, INPUT_PULLUP);
pinMode(2, OUTPUT);
pinMode(3,OUTPUT);
pinMode(trig_pin, OUTPUT);
pinMode(echo_pin, INPUT);


// Setup the speakers
 tmrpcm.speakerPin = 9; //5,6,11 or 46 on Mega, 9 on Uno, Nano, etc
 Serial.begin(9600);
 if (!SD.begin(SD_ChipSelectPin)) // returns 1 if the card is present
 {
  Serial.println("SD fail");
  return;

   // Deactivate jack
   digitalWrite(2, HIGH);
   digitalWrite(3, HIGH);
  
 }
}

//Making the reset function
void reset(){
  Serial.println("reset");
  tmrpcm.disable();
  }
 


void loop() {

//If the button is pressed, start the game
  if (digitalRead(buttonPin) == LOW){

    // Creating a loop that the reset button can break out of
    int a = 0;
    while (a<=1){

      //Play the music
      tmrpcm.setVolume(5); //
      tmrpcm.play("music1.wav"); //the sound file "song" will play each time the arduino powers up, or is reset
                            //try to provide the file name with extension
  
      
      //Start the RPI timer
      Serial.println("activate");
      delay(300);
  
  
      //Begin the platform mechanism (don't move for 45 seconds)
      int b = 0;
      while (b<= 7500){

       // Deactivate jack
       digitalWrite(2, HIGH);
       digitalWrite(3, HIGH);
        
        // Check for reset
        if (digitalRead(resetPin) == LOW){
        reset();
        break; 
        }

        delay(1);
        Serial.println(b);  
        b++;
      }

       // Check for reset
      if (digitalRead(resetPin) == LOW){
      reset();
      break; 
      }

      
      // Raise jack (15 seconds)
      int c = 0;
      while (c<= 2500){
      digitalWrite(2, HIGH);
      digitalWrite(3, LOW);
      
        // Check for reset
        if (digitalRead(resetPin) == LOW){
        reset();
        break; 
        }

        delay(1);
        Serial.println(c);  
        c++;
      }

      
      // Check for reset
      if (digitalRead(resetPin) == LOW){
      reset();
      break; 
      }

  
      // Deactivate jack
      digitalWrite(2, HIGH);
      digitalWrite(3, HIGH);
      
  
      // 15 seconds to manually adjust height after platform is lifted
      int d = 0;
      while (d<= 2500){
        // Manually raise jack
        if (digitalRead(upSwitch) == LOW){
          digitalWrite(2, HIGH);
          digitalWrite(3, LOW);
          Serial.print("up");
        }
    
        // Manually lower jack
        if (digitalRead(downSwitch) == LOW){
          digitalWrite(2, LOW);
          digitalWrite(3, HIGH);
          Serial.print("down");
        }
    
        if (digitalRead(downSwitch) == HIGH and digitalRead(upSwitch) == HIGH){
          digitalWrite(2, HIGH);
          digitalWrite(3, HIGH);
        }
        
        // Check for reset
        if (digitalRead(resetPin) == LOW){
        reset();
        break; 
        }
        
        delay(1);
        Serial.println(d);  
        d++;
        }
        
      // Check for reset
      if (digitalRead(resetPin) == LOW){
      reset();
      break;     
      }

  
      // Deactivate jack (in case up or down was pressed at end of loop)
      digitalWrite(2, HIGH);
      digitalWrite(3, HIGH);
    

    // Reset the platform
    reset();
    break;
    }

  // Lower jack
  digitalWrite(2, LOW);
  digitalWrite(3, HIGH);
  delay(30000);

  Serial.println("wait");
     

  }

}
