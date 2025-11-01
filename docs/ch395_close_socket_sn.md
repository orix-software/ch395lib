---

## ch395_close_socket_sn
***Description***

This command is used to close Socket. It is necessary to input a 1 byte of Socket index value. After Socket is closed, the receive buffer and transmit buffer of Socket are emptied, but the configuration information is still reserved, and you just need to open the Socket again when using the Socket the next time. In TCP mode, CH395 will automatically disconnect TCP before turning off Socket.
***Input***

* Accumulator : The id of the socket to close

***Modify***

* Y Register 
***Example***

```ca65 lda #$01 ; Socket ID jsr ch395_close_socket_sn```

