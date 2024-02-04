.ifndef CH395_COMMAND_PORT
    .include "ch395.inc"
.endif

.export _ch395_open_socket_sn
.export ch395_open_socket_sn

.proc _ch395_open_socket_sn
    ;;@proto void ch395_open_socket_sn(unsigned char ID_SOCKET)
.endproc

.proc ch395_open_socket_sn
    ;;@brief Open socket from arg
    ;;@inputA Socket id
    ;;@modifyY
    ldy     #CH395_OPEN_SOCKET_SN
    sty     CH395_COMMAND_PORT
    sta     CH395_DATA_PORT ; Send socket id
    rts
.endproc
