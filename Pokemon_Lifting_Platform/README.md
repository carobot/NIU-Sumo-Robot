# Pokemon Lifting Platform Code
This code controls the Pokemon game timer and music. It also raises and lowers a 2ftx2ft platform by extending and retracting an electric jack below it.

## Arduino Code
Below is a list of the system's general mechanism in chronological order during a game:
- An arduino will receive an input from a button to begin the mechanism
- The arduino will send a signal to the raspberry pi to begin its functions (After an audio countdown, the 60 second game timer and game music begin. This code is in the Raspberry Pi code section)
- At 30 seconds left, the arduino will extend the electric jack upwards for 15 seconds by controlling its motor through a relay. This middle 2x2 platform will remain here for 15 seconds until the timer reaches 0 seconds left.
- A joystick can be used to manually raise and lower the platform once it is fully raised.
- At 0 seconds, the middle platform will lower until it’s flush with the rest of the 6x6 platform

#### Code Explanation
The following pins were defined:
- 2 pins for the start and reset buttons
- 2 pins for the up and down switches for the joystick
- 2 pins for the relay
- 2 pins for the distance sensor (optional, it was not used in this project)


To have the entire mechanism only begin when activated, the code was entirely placed in the following if statement which checks if the start button is pressed:
```
//If the button is pressed, start the game
if (digitalRead(buttonPin) == LOW){
```


In order to be able to reset the platform at any point during the code, the rest of the code is included in a series of while loops that are all broken out of when the reset button is pressed. Whenever the loops are broken out of, the final code is used which lowers the platform and effectively resets the system. Each loop contains the following code to check if the reset button is pressed:
```
// Check for reset
if (digitalRead(resetPin) == LOW){
reset();
break; 
}
```
The reset() function just prints "reset" in the serial monitor. 

To control the electric jack through the relay, the relay requires a High or Low signal to relay switch 1 and 2.
- A High/Low signal raises the jack
- A Low/High signal lowers the jack
- A High/High signal deactivates the jack
An example of raising the jack in seen below (recall it is timed with a while loop to allow the mechanism to be reset):
```
// Raise jack (for 15 seconds)
      int c = 0;
      while (c<= 2500){
      digitalWrite(2, HIGH);
      digitalWrite(3, LOW);
      
        // Check for reset
        if (digitalRead(resetPin) == LOW){
        Serial.println("reset");
        break; 
        }

        delay(1);
      }
```
These basic principles are repeated to raise and lower the platform as needed in the mechanism.


## Raspberry Pi Code
