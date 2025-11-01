---

## ch395_get_remot_ipp_sn

***Description***

This command is used to get the remote IP address and port number. It is necessary to input 1 byte of Socket index value. CH395 will output 4 bytes of IP address and 2 bytes of port number (low bytes in front). After Socket works in TCP Server mode and the connection is established, MCU can get the remote IP address and port number through this command

***Input***

* Accumulator : The Socket for the ip to get
* X Register : The ptr (high) to store the ip
* Y Register : The ptr (low) to store the ip


***Modify***

* Accumulator * X Register * Y Register * RES

 lda #$01 ; Socket 1
 ldx #<ip_dest
 ldy #>ip_dest
 jsr ch395_get_remot_ipp_sn
 rts
ipdest:
 .byte 192,168,0,1
```

