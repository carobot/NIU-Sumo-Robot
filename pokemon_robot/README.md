# Robot Control Code

![Activity diagram for main code loop](https://github.com/carobot/NIU-Sumo-Robot/blob/main/pokemon_robot/images/main_activity.png)

## Scope Requirements
The general scope of this project was to build a remotely controlled battlebot capable of being laser-cut and assembled at a small scale (8-10 robots). Primarily, the requirements were: separated and adjustable communication channels, differential drive motor control, wireless connectivity, low cost of parts, speaker/buzzer output, addressable lights, easily serviceable, and with the space to mount moderately sized plush animals.

## General Design Decisions
The design of the robot was settled after brief discussion on resources available. The chassis would be built from laser-cut MDF or hardboard, assembled with risers and screws, and easily accessible/modifiable. The electronics would be centered around a readily available Arduino/Geniuno Uno, CAROBOT's L298N motor driver, and standard gearmotors. The code had to be extensible for future expansions, and be able to remain responsive despite larger loads.

## Handling Input
Due to expected interference at an event of this scale, one of the goals was to use channel frequencies as little as possible while creating a system reasonably robust against dropped, weak, or corrupted signals. Rather than waiting for a packet from the controller at each loop, the system acts on packets broadcast from the controller at regular intervals, and holds its previous state otherwise. Waiting for packets would have required either the controller to operate at a faster broadcast frequency than the robot’s loop frequency, which would be hard to predict as project complexity increases.
To reliably work with occasional dropped packets, the robot updates its movement under either of two conditions - it has received a new packet, or it has not received a new packet for over 1 second. Otherwise, it keeps its previous state. This constant state keeps the robot’s current state of motion despite dropped packets, maintaining the illusion of responsiveness under unstable conditions. The only time it will shut down is after 1 second of silence - when the robot is at risk of driving off the suislide.

## Tick System(s)
As mentioned before, one of the primary goals of this robot was responsiveness within reasonable processor load. The main idea was to run a master loop which ran all critical tasks and set flags which triggered specific behaviour in slave loops at set intervals. While this is not an actual RTOS or scheduling kernel due to the absence of timed interrupts, it is likely one of the bare metal approaches as close to an RTOS as possible. As many non-critical tasks still had to be run at regular intervals to maintain program responsiveness, a timed cycle-based scheduling system was designed to handle non-critical tasks.

The robot primarily functions on a tick-tack-tock system with a constant input poll system accompanied by 3 separate tick cycles. Once setup has completed successfully without triggered error flags the main loop starts to run which executes all internal logic and checks. Any insignificant processing, such as sensor data parsing or light change instructions, are deferred outside of the loop to be handled on regularly scheduled ticks.

![Logic flowchart for tick control](https://github.com/carobot/NIU-Sumo-Robot/blob/main/pokemon_robot/images/flowchart.png)

The primary tick system runs every 50 milliseconds, or 20Hz. While this isn’t particularly fast compared to more modern systems like PCs and phones, a 20Hz rate provides moderate response times to non-critical tasks. The tasks delegated to the primary tick system include time-dependent speed ramps, LED status and action lights, and state resets. On every tick motor control is executed (independent of radio input) and the status byte is checked for active bit flags to process and reset the highest priority flag. On the event of a flag triggered, its logic is triggered either instantly, or after a certain offset from the start tick. This ensures non-critical tasks are still processed on a timely basis independently of critical tasks, retaining responsiveness.


## Final Words
While this system is likely overkill for simple radio input and sensor streams, it sets up a solid foundation for further expansion without significant change to the responsiveness of any critical component. For example, a constant IMU data stream can still be processed to a speed perceived as responsive while maintaining near-instantaneous input response times.

In more processing-heavy applications such as speaker output or display refreshes, one or more constant streams of data processed unnecessarily frequently would result in stuttering and obvious input lag. This tick system allows for a limited number of data streams and buffers to be processed concurrently with the addition of interrupts, keeping the system responsive despite possible future additions.
