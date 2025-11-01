.include "ch395.inc"
.include "telestrat.inc"

.export ch395_read_recv_buf_sn

.proc ch395_read_recv_buf_sn
    ;;@brief This command is used to read data from Socket receive buffer. It is necessary to input 1 byte of Socket index value and 2 bytes of length (low bytes in front). CH395 will output several bytes of data stream based on the length value. In actual application, the command RECV_LEN_SN can be firstly sent to get the actual effective length of the current buffer. The length of the read data can be less than the actual effective length of the buffer, the unread data is still reserved in the receive buffer, and MCU can continue to read through this command. In MACRAW mode, the processing modes are different. In MACRAW mode, the receive buffer is a frame buffer, which can cache only 1 frame of Ethernet data. After CH395 processes the command READ_RECV_BUF_SN, Socket0 receive buffer will be cleared, so MCU shall read all valid data of the buffer at a time.
    ;;@inputA Socket id
    ;;@inputY Length low
    ;;@inputX length high
    ;;@inputMEM_RES ptr to fill
    ;;@modifyMEM_RESB tmp value
    sty     RESB        ; Save length
    stx     RESB + 1

    ldy     #CH395_READ_RECV_BUF_SN
    sty     CH395_COMMAND_PORT
    sta     CH395_DATA_PORT

    lda     RESB
    sta     CH395_DATA_PORT ; set length
    stx     CH395_DATA_PORT ; set length to receive

    lda     RESB + 1
    beq     @decrement

    ldy     #$00
    ldx     #$00

@loop2:
    lda     CH395_DATA_PORT ; set ptr2
    sta     (RES),y
    iny
    bne     @loop2
    inc     RES + 1
    inx
    cpx     RESB+1
    bne     @loop2

@decrement:
    ldy     #$00

@L1:
    lda     CH395_DATA_PORT
    sta     (RES),y
    iny
    cpy     RESB
    bne     @L1

    rts
.endproc
