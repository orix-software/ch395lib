---

## ch395_set_ttl

***Description***

This command is used to set Socket TTL. It is necessary to input 1 byte of Socket index value and 1 byte of TTL value. It shall be set after the Socket is opened, and the maximum value is 128

***Input***

* Accumulator : Socket ID
* X Register : TTL value


***Modify***

* Y Register 
