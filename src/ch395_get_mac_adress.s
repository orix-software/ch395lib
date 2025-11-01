.include "ch395.inc"
.include "telestrat.inc"

.export ch395_get_mac_adress

.proc ch395_get_mac_adress
    ;;@brief Get mac address
    ;;@inputA the ptr
    ;;@inputX the ptr
    ;;@modifyA
    ;;@modifyX
    ;;@modifyY
    ;;@modifyMEM_RES Tmp value
    ;;@returnsA the content of ptr modified
    ;;@returnsX the content of ptr modified

    ;;@```ca65
    ;;@`  lda  #<mac_address
    ;;@`  ldx  #>mac_address
    ;;@`  ldx  #$00
    ;;@`  jsr  ch395_get_mac_adress
    ;;@`  rts
    ;;@`mac_address:
    ;;@`  .res 10
    ;;@```

    sta     RES
    stx     RES + 1
	lda     #CH395_GET_MAC_ADDR
    sta     CH395_COMMAND_PORT

    ldy     #$00

@loop:
    lda     CH395_DATA_PORT
    sta     (RES),y

    iny
    cpy     #$06
    bne     @loop

    rts
.endproc
