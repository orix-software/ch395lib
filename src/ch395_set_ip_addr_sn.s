.include "ch395.inc"
.include "telestrat.inc"

.export ch395_set_ip_addr_sn

.proc ch395_set_ip_addr_sn
    ;;@brief This command is used to set the destination IP address of Socket. It is necessary to input 1 byte of Socket index value and 4 bytes of destination IP address. When Socket works in IPRAW, UDP, or TCP Client mode, 0the destination IP must be set before the command CMD_OPEN_SOCKET_SN is sent
    ;;@inputA Socket id
    ;;@inputY low ptr ip
    ;;@inputX high ptr ip
    ;;@modifyMEM_RES tmp

    sty     RES
    stx     RES + 1

    ldx     #CH395_SET_IP_ADDR_SN
    stx     CH395_COMMAND_PORT
    sta     CH395_DATA_PORT ; Send socket id
    ; send IP
    ldy     #$00
@loop:
    lda     (RES),y
    sta     CH395_DATA_PORT
    iny
    cpy     #04
    bne     @loop

    rts
.endproc
