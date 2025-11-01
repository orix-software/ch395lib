---

## ch395_get_unreach_ipport
***Description***

This command is used to get an inaccessible IP, ports and protocol type. CH395 will generate an inaccessible interrupt when an inaccessible message is received. MCU can use this command to get inaccessible information. After receiving this command, CH395 will output 1 byte of inaccessible code, 1 byte of protocol type, 2 bytes of port number (low bytes in front), and 4 bytes of IP in turn. MCU can judge whether the protocol, port or IP is inaccessible according to the inaccessible codes. For inaccessible codes, refer to RFC792 (CH395INC.H defines four common inaccessible codes).
!!! failure "Does not work"
