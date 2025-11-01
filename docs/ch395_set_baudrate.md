---

## ch395_set_baudrate
***Description***

This command is used to set the baud rate of CH395 for serial communication. When CH395 works in serial communication mode, the default communication baud rate is set by the level combination of SDO, SDI and SCK pins (refer to Section 6.4 of this datasheet) after reset. When these pins are suspended, the baud rate is 9600bps by default. If MCU supports high communication speed, the serial communication baud rate can be dynamically regulated through this command. This command requires the input of three data, namely, baudrate coefficient 0, baud rate coefficient 1 and baud rate coefficient 2. The following table shows the corresponding relationship with baud rates.

