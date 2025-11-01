.include "ch395.inc"

.export ch395_set_gwip_addr

.proc ch395_set_gwip_addr
    ;;@brief This command is used to set the gateway address for CH395. It is necessary to input 4 bytes of IP address
    ;;@failure Does not work
	lda     #CH395_SET_GWIP_ADDR
    rts
.endproc
