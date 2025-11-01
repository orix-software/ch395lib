.include "ch395.inc"
.include "telestrat.inc"

.importzp ptr1
.import popax

.export _ch395_get_remot_ipp_sn
.import ch395_get_remot_ipp_sn

.proc _ch395_get_remot_ipp_sn
    ;;@proto void          ch395_get_remot_ipp_sn(unsigned char *ptr, unsigned char socket);
    ;;@brief does not work Get remote ip connected to the socket
    ;;@inputPARAM_socketid The socketid
    pha
    jsr     popax
    sta     ptr1
    stx     ptr1 + 1
    pla
    ; FIXME
    rts
    ;jmp     ch395_get_remot_ipp_sn::enter_c_proto
.endproc

