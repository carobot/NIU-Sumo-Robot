# Pokemon Lifting Platform Code
This code controls the Pokemon game timer and music. It also raises and lowers a 2ftx2ft platform by extending and retracting an electric jack below it.

## Arduino Code
Below is a list of the system's general mechanism in chronological order during a game:
- An arduino will receive an input from a button to begin the mechanism
- The arduino will send a signal to the raspberry pi to begin its functions (After an audio countdown, the 60 second game timer and game music begin. This code is in the Raspberry Pi code section)
- At 30 seconds left, the arduino will extend the electric jack (and the 2x2 platform on it) upwards for 15 seconds by controlling its motor through a relay. This middle platform will remain here for 15 seconds until the timer reaches 0 seconds left.
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


In order to be able to reset the platform at any point during the code, the rest of the code is included in a series of while loops that are all broken out of when the reset button is pressed. Whenever the loops are broken out of, the final portion of the code is used which lowers the platform and effectively resets the system. Each loop contains the following code to check if the reset button is pressed (The reset() function just prints "reset" in the serial monitor):
```
// Check for reset
if (digitalRead(resetPin) == LOW){
reset();
break; 
}
```

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
This is not the code originally used to display a timer and play music (the original can be found in the old raspberry pi code folder). It has been greatly simplified and updated to play an mp4 file of the timer and music using the moviepy and pygame libraries. The mp4 file can be found here: 
https://drive.google.com/file/d/1cD2DQEMQYrCH2XB90sjX_IaXBVr2EDr2/view?usp=sharing

#### Code Explanation
The arduino is in serial connection with the raspberry pi, and as such can send it a signal with the Serial.print() command. The raspberry pi’s code is written on python, and establishes serial communication with the arduino using the serial library. This connection is estabilished in the following part of the code:
```
if __name__ == '__main__':
    ser = serial.Serial('/dev/ttyACM0', 9600, timeout=1)
    ser.reset_input_buffer()
    while True:
```

The code then continuously checks if the arduino sends the signal "activate" (this was written in the arduino code):
```
 while True:
        if ser.in_waiting > 0:
            line = ser.readline().decode('utf-8').rstrip()
            if line == "activate":
```

Once this signal is read, the code uses the moviepy and pygame libraries to play the mp4 file:
```
video = VideoFileClip('/home/pi/downloads/timer1.mp4')
video.preview()
```
