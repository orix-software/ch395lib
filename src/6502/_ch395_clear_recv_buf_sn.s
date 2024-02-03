.ifndef CH395_COMMAND_PORT
    .include "ch395.inc"
.endif

.export _ch395_clear_recv_buf_sn
.export ch395_clear_recv_buf_sn

.proc _ch395_clear_recv_buf_sn
    ;;@proto void ch395_clear_recv_buf_sn(unsigned char ID_SOCKET)
    ;;@brief Clear receive buffer
.endproc


.proc ch395_clear_recv_buf_sn
    ;;@brief Clear receive buffer
    ;;@modifyA
    ;;@modifyY
    ldy     #CH395_CLEAR_RECV_BUF_SN
    sty     CH395_COMMAND_PORT
    sta     CH395_DATA_PORT ; Send socket id
    rts
.endproc
