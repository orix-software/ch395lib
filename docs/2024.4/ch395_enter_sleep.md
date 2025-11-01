---

## ch395_enter_sleep

***Description***

This command enables CH395 chip in a low-power sleep suspended state. When MCU writes a new command to CH395 (no data input command, such as CMD_GET_IC_VER), it will exit the low-power state. For the parallel port and SPI interface communication modes, active SCS chip selection will also cause CH395 to exit the low-power state, so MCU shall immediately disable the SCS chip selection after sending the command CMD_ENTER_SLEEP. In sleep state, MAC and PHY of CH395 will be in power off mode and disconnect Ethernet. Typically, it takes several milliseconds for CH395 to exit the low-power state.
!!! failure "Does not work"
