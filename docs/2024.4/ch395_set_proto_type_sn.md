---

## ch395_set_proto_type_sn

***Description***

This command is used to set the working mode of Socket. It is necessary to input 1 byte of Socket index value and 1 byte of working mode. The working mode is defined as follows:
Code Name Description
03H PROTO_TYPE_TCP TCP mode
02H PROTO_TYPE_UDP UDP mode
01H PROTO_TYPE_MAC_RAW MAC original message mode
00H PROTO_TYPE_IP_RAW IP original message mode
This command must be executed before CMD_OPEN_SOCKET_SN. Refer to 8.3 Application Reference
Steps for detailed steps.

***Input***

* Accumulator : Socket id
* X Register : Proto mode


***Modify***

* Y Register tmp
 lda #$00 ; Socket
 ldx #CH395_PROTO_TYPE_TCP
 jsr ch395_set_proto_type_sn
```

