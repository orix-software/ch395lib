.include "ch395.inc"

.export ch395_get_glob_int_status

.proc ch395_get_glob_int_status
    ;;@brief This command is used to get the global interrupt status. CH395 will output 1 byte of global interrupt status after receiving this command. Global interrupt status is defined as follows:
    ;;@returnsA int status state CH395_GINT_STAT_SOCK0 CH395_GINT_STAT_SOCK1 CH395_GINT_STAT_SOCK2 CH395_GINT_STAT_SOCK3 CH395_GINT_STAT_DHCP CH395_GINT_STAT_PHY_CHANGE CH395_GINT_STAT_IP_CONFLI CH395_GINT_STAT_UNREACH
    ;;@modifyA
    ;;@explain Bit Name Description
    ;;@explain 7 GINT_STAT_SOCK3 Socket3 interrupt
    ;;@explain 6 GINT_STAT_SOCK2 Socket2 interrupt
    ;;@explain 5 GINT_STAT_SOCK1 Socket1 interrupt
    ;;@explain 4 GINT_STAT_SOCK0 Socket0 interrupt
    ;;@explain 3 GINT_STAT_DHCP DHCP interrupt
    ;;@explain 2 GINT_STAT_PHY_CHANGE PHY status change interrupt
    ;;@explain 1 GINT_STAT_IP_CONFLI IP conflict
    ;;@explain 0 GINT_STAT_UNREACH Inaccessible interrupt
    ;;@explain ① GINT_STAT_UNREACH: Inaccessible interrupt. When CH395 receives ICMP inaccessible interrupt
    ;;@explain message, it saves the IP address, port and protocol type of the inaccessible IP packet in the inaccessible
    ;;@explain information table, and then generates an interrupt. When the MCU receives the interrupt, it can send the
    ;;@explain command GET_UNREACH_IPPORT to get the inaccessible information.
    ;;@explain ② GINT_STAT_IP_CONFLI: IP conflict interrupt. This interrupt is generated when CH395 detects that its
    ;;@explain IP address is the same as that of other network devices in the same network segment.
    ;;@explain ③ GINT_STAT_PHY_CHANGE: PHY change interrupt. This interrupt is generated when PHY connection
    ;;@explain of CH395 changes, for example, PHY state changes from the connected state to the disconnected state or
    ;;@explain from the disconnected state to the connected state. MCU can send GET_PHY_STATUS command to get
    ;;@explain the current PHY connection status.
    ;;@explain ④ GINT_STAT_DHCP: DHCP interrupt. If MCU enables DHCP function of CH395, CH395 will generate
    ;;@explain this interrupt. MCU can send the command CMD_GET_DHCP_STATUS to get the DHCP status. If the
    ;;@explain status is 0, it will indicate success; otherwise, it will indicate timeout failure.
    ;;@explain ⑤ GINT_STAT_SOCK0 - GINT_STAT_SOCK3: Socket interrupt. When there is an interrupt event in
    ;;@explain Socket, CH395 will generate this interrupt. MCU needs to send GET_INT_STATUS_SN to get the
    ;;@explain interrupt status of Socket. Please refer to GET_INT_STATUS_SN.
    ;;@explain When this command is completed, CH395 will set INT# pin to high level and clear the global interrupt
    ;;@explain status
    ;;@modifyA
    ;;@returnsA Status
    ;;@```ca65
    ;;@`  jsr       ch395_get_glob_int_status
    ;;@`  ; check accumulator to get interrupts states
    ;;@```
    lda     #CH395_GET_GLOB_INT_STATUS
    sta     CH395_COMMAND_PORT
    lda     CH395_DATA_PORT
    rts
.endproc
