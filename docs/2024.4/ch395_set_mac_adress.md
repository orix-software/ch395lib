---

## ch395_set_mac_adress

***Description***

This command is used to set MAC address for CH395. It is necessary to input 6 bytes of MAC, with low bytes of MAC address in front. CH395 chip will store MAC address in the internal EEPROM. It will take 100mS to execute this command.MAC address assigned by IEEE has been burned when CH395 chip is delivered. If it is not necessary, please do not set MAC address

***Input***

* Accumulator : Low ptr mac adress
* X Register : High ptr mac adress


***Modify***

* Accumulator * Y Register * RESTmp

