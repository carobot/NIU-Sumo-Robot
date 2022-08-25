# Pokemon Lifting Platform Code

## Arduino Code
Below is a list of the system's general mechanism in chronological order during a game:
- An arduino will receive an input from a button to begin the mechanism
- After an audio countdown, the arduino will send a signal to the raspberry pi to begin its functions (game timer and music, this code is in the Raspberry Pi code section)
- At 30 seconds, the arduino will extend the electric jack upwards for 15 seconds by controlling its motor through a relay. This middle 2x2 platform will remain here for 15 seconds until the timer reaches 0 seconds left.
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


To activate the jack through the relay, the relay requires a High/Low signal to relay switch 1 and 2. 
