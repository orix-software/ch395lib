---

## ch395_get_glob_int_status_all
***Description***

This command is used to get the global interrupt status. CH395 will output 2 bytes of global interrupt status after receiving this command. Global interrupt status is defined as follows:

***Returns***

* Accumulator : int status state CH395_GINT_STAT_SOCK0 CH395_GINT_STAT_SOCK1 CH395_GINT_STAT_SOCK2 CH395_GINT_STAT_SOCK3 CH395_GINT_STAT_DHCP CH395_GINT_STAT_PHY_CHANGE CH395_GINT_STAT_IP_CONFLI CH395_GINT_STAT_UNREACH

***Modify***

* Accumulator * X Register : int status state CH395_GINT_STAT_SOCK4 CH395_GINT_STAT_SOCK5 CH395_GINT_STAT_SOCK6 CH395_GINT_STAT_SOCK7
* X Register 
***Example***

```ca65 jsr ch395_get_glob_int_status_all ; check accumulator to get interrupts states ; check X to get interrupts states```
```
!!! failure "Does not work"
