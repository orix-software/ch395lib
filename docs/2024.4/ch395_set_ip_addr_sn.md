---

## ch395_set_ip_addr_sn

***Description***

This command is used to set the destination IP address of Socket. It is necessary to input 1 byte of Socket index value and 4 bytes of destination IP address. When Socket works in IPRAW, UDP, or TCP Client mode, 0the destination IP must be set before the command CMD_OPEN_SOCKET_SN is sent

***Input***

* Accumulator : Socket id
* Y Register : low ptr ip
* X Register : high ptr ip


***Modify***

* REStmp

