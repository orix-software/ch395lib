---

## ch395_get_int_status_sn

***Description***

This command is used to get the interrupt status of Socket. It is necessary to input 1 byte of Socket index value. After receiving this command, CH395 will output 1 byte of Socket interrupt code. The interrupt code bits are defined as follows:
A7 - Reserved
6 SINT_STAT_TIM_OUT Timeout
5 - Reserved
4 SINT_STAT_DISCONNECT TCP disconnected
3 SINT_STAT_CONNECT TCP connected
2 SINT_STAT_RECV Receive buffer not empty
1 SINT_STAT_SEND_OK Send Success
0 SINT_STAT_SENBUF_FREE Transmit buffer free
① SINT_STAT_SENBUF_FREE, transmit buffer free interrupt. After MCU writes data to the transmit
buffer of Socket, CH395 will quickly copy the data to the internal protocol stack or MAC buffer, in order
to encapsulate the data. When the data copying is finished, this interrupt will be generated. MCU can
continue to write the subsequent data to the transmit buffer. After MCU writes data to the transmit buffer
of Socket once, it must write the next data until the interruption is generated.
② SINT_STAT_SEND_OK, send OK interrupt. This interrupt indicates that the data packet is sent
successfully. This interrupt will be generated after Sokcet sends a packet of data successfully. After MCU
writes data to Socket buffer once, CH395 may encapsulate the data into several data packets for sending,
so several send OK interrupts may be generated.
③ SINT_STAT_CONNECT, TCP linkage interrupt, active only in TCP mode. It indicates that TCP
connection is successful, and MCU can transmit data only after the interrupt is generated.
④ SINT_STAT_DISCONNECT, TCP disconnection interrupt, only active in TCP mode, indicating TCP
disconnection.
⑤ SINT_STAT_TIM_OUT. This interrupt will be generated in TCP mode when a timeout occurs during a
TCP connection, disconnection, data sending and other processes. This interrupt will be generated if the
data is not sent successfully in IPRAW or UDP mode.
After the interrupts SINT_STAT_DISCONNECT and SINT_STAT_TIM_OUT are generated, CH395 takes
different actions depending on whether the FUN_PARA_FLAG_SOCKET_CLOSE bit is 1 or 0. If
FUN_PARA_FLAG_SOCKET_CLOSE is 0, CH395 will actively set the Socket state to the OFF state and
clear all relevant buffers after the above two interrupts are generated. Otherwise, it will not do any operation
command to close the Socket

***Input***

* Accumulator : Socket id


***Modify***

* Accumulator * X Register 
***Returns***

* Accumulator : Status of selected socket

 lda #CH395_SOCKET1 ; Check socket 1
 jsr ch395_get_int_status_sn
 ; Check interrupt type
 and #CH395_SINT_STAT_SEND_OK
 cmp #CH395_SINT_STAT_SEND_OK
 beq @send_ok
 rts
```

