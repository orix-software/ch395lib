---

## ch395_set_des_port_sn

***Description***

This command is used to set the Socket destination port. It is necessary to input 1 byte of Socket index value and 2 bytes of destination port (the low bytes are in front). When Socket works in UDP or TCP Client mode, this value must be set.

***Input***

* Accumulator : Socketid 
* Y Register : Low int
* X Register : high int


***Modify***

* REStmp

 lda #$01
 ldy #80
 ldx #$00
 jsr ch395_set_des_port_sn
 rts
```

