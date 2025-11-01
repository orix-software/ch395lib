---

## ch395_get_recv_len_sn

***Description***

Get the length received from socket

***Input***

* Accumulator : Socket id

 lda #$01 ; Socket 1
 jsr ch395_get_recv_len_sn
 ; check A and X for length received
ch395_connected:
```


***Modify***

* Accumulator * X Register 
***Returns***

* Accumulator : The length
* X Register : The length

