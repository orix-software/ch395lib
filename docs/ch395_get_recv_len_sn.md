---

## ch395_get_recv_len_sn
***Description***

Get the length received from socket
***Input***

* Accumulator : Socket id

***Example***

```ca65 lda #$01 ; Socket 1 jsr ch395_get_recv_len_sn ; check A and X for length receivedch395_connected:```

***Modify***

* Accumulator * X Register 
***Returns***

* Accumulator : The length
* X Register : The length

