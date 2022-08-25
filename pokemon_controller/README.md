# Radio Controller

## Setup and limitations
Once finished with the standard setup to initialize pins and radio module, the controller operates with two basic restrictions in mind - radio interference and packet size limitations.

The first, radio interference, is a concern that the broadcasted packets from the controller box would completely saturate the channel it occupied, heavily interfering or even disabling other devices using these unofficial frequencies at the event. This concern was completely addressed in a single line of code: delay(100);, which spaced out the broadcasts between packets.

The other concern with radio interference was that other devices broadcasting on the same channel would also send “foreign data” or corrupt packets - an issue addressable by including a checksum using any simple hash function. A checksum would have also enabled the code to reconstruct corrupted packets. While this would have made radio transmissions immune to occasional corruption, it would not have made the robots immune to constant packet loss - an issue I thought would have been more likely than packet corruption at this event. As a result we did not go through the additional effort of encrypting and decrypting a checksum on both ends, instead focusing our attention on more urgent goals. If constant total packet loss occurred at the event, we could just give up instead.

## Packet Design
As for packet size limitations, it must be kept to a minimum for the joystick control. We knew the radio module had a packet size limit of 32 bytes, and four directions for each of the two joysticks encoded inefficiently with 4-byte integers would completely max out our packet bandwidth. On top of that, we expected the project scope to increase as it progressed, so the radio packets would likely have been used for other purposes as well. These additional requirements would have made a 32-byte integer array infeasible for the future.

Imagine a 600-pound man in a Smart car, with 2 guests carrying bags trying to get inside. No space.

Instead, each direction on each joystick is encoded into its own position of an 8-bit bit field, condensing 32 bytes down to 1 byte, while maintaining the same data in transmission.

Diving deeper into the technicals, the 8 bits could technically have been fully encoded into 4 bits, although with a single limitation. With 2 bits per joystick to represent 4 unique states, the joysticks’ range of motion could only be represented as a single direction at a time. But with a single bit for each cardinal direction for each joystick (8 bits total), multiple directions could be active at a time, retaining the possibility of a joystick being pushed diagonally. This direction-to-bitfield was done through bit shifts. The input was parsed and combined into the transmission buffer using tricks commonly seen in embedded applications - a negation, bit mask, shift, and then ORed into the existing buffer value (the negation and mask was necessary due to the pullup resistors, which flips the values contrary to common logic). On the receiver side it was done more simply - initial checks for special cases (straight forwards/backwards) by comparing constants, or bit masked and compared if not a special case.

![Joystick encoding logic](https://github.com/carobot/NIU-Sumo-Robot/blob/main/pokemon_robot/images/encoding.jpg)

The final product results in a packet transmission containing all necessary information, yet lean enough to integrate other data into the same transmission with minimal modification to either side of the transmission.
