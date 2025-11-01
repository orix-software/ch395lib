---

## ch395_clear_recv_buf_sn

***Description***

This command is used to clear the Socket receive buffer. It is necessary to input 1 byte of Socket index value. Upon receiving this command, CH395 will reset the receiving length of this Socket, and the receiving pointer will point to the buffer head.

***Input***

* Accumulator : Socket id


***Modify***

* Accumulator * Y Register 
 lda #CH395_SOCKET1 ; Socket ID
 jsr ch395_clear_recv_buf_sn
```

