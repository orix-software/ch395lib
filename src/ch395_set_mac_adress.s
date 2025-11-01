.include "ch395.inc"
.include "telestrat.inc"

.export ch395_set_mac_adress

.proc ch395_set_mac_adress
    ;;@brief This command is used to set MAC address for CH395. It is necessary to input 6 bytes of MAC, with low bytes of MAC address in front. CH395 chip will store MAC address in the internal EEPROM. It will take 100mS to execute this command.MAC address assigned by IEEE has been burned when CH395 chip is delivered. If it is not necessary, please do not set MAC address     
    ;;@inputA Low ptr mac adress
    ;;@inputX High ptr mac adress
    ;;@modifyA
    ;;@modifyY
    ;;@modifyMEM_RES Tmp
    ldy     #CH395_SET_MAC_ADDR
    sty     CH395_COMMAND_PORT
    sta     RES
    stx     RES + 1

    ldy     #$00

@loop:
    lda     (RES),y
    sta     CH395_DATA_PORT
    iny
    cpy     #$06
    bne     @loop

    rts
.endproc
