.include "ch395.inc"
.include "telestrat.inc"

.export ch395_get_ip_inf

.proc ch395_get_ip_inf
    ;;@brief Get ip info
    ;;@inputA the ptr to modify (Ptr must be 20 bytes length)
    ;;@inputX the ptr to modify (Ptr must be 20 bytes length)
    ;;@modifyA
    ;;@modifyX
    ;;@modifyY
    ;;@modifyMEM_RES Tmp value

    ;;@```ca65
    ;;@`  lda  #<ip_address
    ;;@`  ldx  #>ip_address
    ;;@`  jsr  ch395_get_ip_inf
    ;;@`  rts
    ;;@`ip_address:
    ;;@`  .res 4
    ;;@```

    sta     RES
    stx     RES + 1

	lda     #CH395_GET_IP_INF
    sta     CH395_COMMAND_PORT

    ldy     #$00

@loop:
    lda     CH395_DATA_PORT
    sta     (RES),y
    iny
    cpy     #20
    bne     @loop

    rts
.endproc
