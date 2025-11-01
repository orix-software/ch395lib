---

## ch395_set_phy
***Description***

This command is used to set Ethernet PHY connection mode of CH395. The connection mode is automated negotiation mode by default. This command needs to input 1 byte of data, which is the connection mode code: Disconnect PHY when the connection mode code is 01H; PHY is 10M full duplex when the connection mode code is 02H; PHY is 10M half duplex when the connection mode code is 04H; PHY is 100M full duplex when the connection mode code is 08H; PHY is 100M half duplex when the connection mode code is 10H; PHY is automated negotiation when the connection mode code is 20H. When CH395 receives this command, it will reset MAC and PHY and reconnect according to the newly set connection mode. If Ethernet is already connected, it will be disconnected and reconnected.
!!! failure "Does not work"
