.ifndef CH395_COMMAND_PORT
    .include "ch395.inc"
.endif

.export _ch395_set_fun_para

; void ch395_set_fun_para(unsigned char flag)

.proc _ch395_set_fun_para
    ldy     #CH395_SET_FUN_PARA
    sty     CH395_COMMAND_PORT
    ; Send four bytes
    sta     CH395_DATA_PORT 
    lda     #$00
    sta     CH395_DATA_PORT 
    sta     CH395_DATA_PORT 
    sta     CH395_DATA_PORT 
    rts
.endproc