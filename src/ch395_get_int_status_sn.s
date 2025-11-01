.include "../include/ch395.inc"

.export ch395_get_int_status_sn

.proc ch395_get_int_status_sn
    ;;@brief This command is used to get the interrupt status of Socket. It is necessary to input 1 byte of Socket index value. After receiving this command, CH395 will output 1 byte of Socket interrupt code. The interrupt code bits are defined as follows: 
    ;;Bit Name Description
    ;;@explain A7 - Reserved
    ;;@explain 6 SINT_STAT_TIM_OUT Timeout
    ;;@explain 5 - Reserved
    ;;@explain 4 SINT_STAT_DISCONNECT TCP disconnected
    ;;@explain 3 SINT_STAT_CONNECT TCP connected
    ;;@explain 2 SINT_STAT_RECV Receive buffer not empty
    ;;@explain 1 SINT_STAT_SEND_OK Send Success
    ;;@explain 0 SINT_STAT_SENBUF_FREE Transmit buffer free
    ;;@explain ① SINT_STAT_SENBUF_FREE, transmit buffer free interrupt. After MCU writes data to the transmit
    ;;@explain buffer of Socket, CH395 will quickly copy the data to the internal protocol stack or MAC buffer, in order
    ;;@explain to encapsulate the data. When the data copying is finished, this interrupt will be generated. MCU can
    ;;@explain continue to write the subsequent data to the transmit buffer. After MCU writes data to the transmit buffer
    ;;@explain of Socket once, it must write the next data until the interruption is generated.
    ;;@explain ② SINT_STAT_SEND_OK, send OK interrupt. This interrupt indicates that the data packet is sent
    ;;@explain successfully. This interrupt will be generated after Sokcet sends a packet of data successfully. After MCU
    ;;@explain writes data to Socket buffer once, CH395 may encapsulate the data into several data packets for sending,
    ;;@explain so several send OK interrupts may be generated.
    ;;@explain ③ SINT_STAT_CONNECT, TCP linkage interrupt, active only in TCP mode. It indicates that TCP
    ;;@explain connection is successful, and MCU can transmit data only after the interrupt is generated.
    ;;@explain ④ SINT_STAT_DISCONNECT, TCP disconnection interrupt, only active in TCP mode, indicating TCP
    ;;@explain disconnection.
    ;;@explain ⑤ SINT_STAT_TIM_OUT. This interrupt will be generated in TCP mode when a timeout occurs during a
    ;;@explain TCP connection, disconnection, data sending and other processes. This interrupt will be generated if the
    ;;@explain data is not sent successfully in IPRAW or UDP mode.
    ;;@explain After the interrupts SINT_STAT_DISCONNECT and SINT_STAT_TIM_OUT are generated, CH395 takes
    ;;@explain different actions depending on whether the FUN_PARA_FLAG_SOCKET_CLOSE bit is 1 or 0. If
    ;;@explain FUN_PARA_FLAG_SOCKET_CLOSE is 0, CH395 will actively set the Socket state to the OFF state and
    ;;@explain clear all relevant buffers after the above two interrupts are generated. Otherwise, it will not do any operation
    ;;@explainon the Socket status and the relevant buffer, so as to facilitate the external MCU to read the residual data
    ;;@explainafter TCP is disconnected or timed out. When the external MCU reads the data, it must send a close
    ;;@explain command to close the Socket
    ;;
    ;;@inputA Socket id
    ;;@modifyA
    ;;@modifyX
    ;;@returnsA Status of selected socket
    ;;@```ca65
    ;;@`  lda     #CH395_SOCKET1 ; Check socket 1
    ;;@`  jsr     ch395_get_int_status_sn
    ;;@`  ; Check interrupt type
    ;;@`  and     #CH395_SINT_STAT_SEND_OK
    ;;@`  cmp     #CH395_SINT_STAT_SEND_OK
    ;;@`  beq     @send_ok
    ;;@`  rts
    ;;@```
    ldx     #CH395_GET_INT_STATUS_SN
    stx     CH395_COMMAND_PORT
    sta     CH395_DATA_PORT
    lda     CH395_DATA_PORT
    rts
.endproc

