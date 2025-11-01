---

## ch395_set_sour_port_sn

***Description***

This command is used to set the source port of Socket. It is necessary to input 1 byte of Socket index value and 2 bytes of source port (low bytes in front). If two or more Sockets are in the same mode, the source port numbers must not be the same. For example, Socket 0 is in UDP mode, the source port number is 600, and Socket 1 is also in UDP mode. The source port number 600 cannot be used again, otherwise it may cause the0 opening failure.

***Input***

* Accumulator : Socket id
* Y Register : Low port
* X Register : High port


***Modify***

* RES

