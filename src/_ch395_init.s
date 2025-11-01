.include "ch395.inc"

.export _ch395_init
.import ch395_init

.proc _ch395_init
    ;;@proto void          ch395_init();
    ;;@brief This command is used to initialize CH395, including initializing MAC, PHY and TCP/IP stack of CH395. Generally, it takes 350mS to execute the command. MCU can send GET_CMD_STATUS to query whether the execution has finished and the execution status.
    jmp     ch395_init
.endproc

