---

## ch395_get_socket_status_sn

***Description***

This command is used to get Socket status. It is necessary to input a 1 byte of Socket index value. CH395 will output a 2-byte status code when receiving this command. The first status code is the status code of Socket. The status code of Socket is defined as follows:
00H SOCKET_CLOSED
05H SOCKET_OPEN
The second status code is TCP status code, which is only meaningful when TCP mode has been on. TCP
status code is defined as follows:
Code Name Description
00H TCP_CLOSED Closed
01H TCP_LISTEN Monitoring
02H TCP_SYN_SENT SYN sent
03H TCP_SYN_RCVD SYN received
04H TCP_ESTABLISHED TCP connection established
05H TCP_FIN_WAIT_1 The active closing side first sends FIN
06H TCP_FIN_WAIT_2 The active closing side receives an ACK from FIN
07H TCP_CLOSE_WAIT The passive closing side receives FIN
08H TCP_CLOSING Closing
09H TCP_LAST_ACK The passive closing side sends FIN
0AH TCP_TIME_WAIT 2MLS waiting status
TCP status is defined in TCP/IP protocol. Please refer to TCP/IP protocol for the detailed meaning

***Input***

* Accumulator : Socket id


***Modify***

* X Register * Accumulator 
***Returns***

* Accumulator : The status
* X Register : The state

 lda #$01 ; Socket 1
 jsr ch395_get_socket_status_sn
 ; check A and X for the state
 cmp #CH395_SOCKET_CLOSED
 beq @soclet is closed
```

