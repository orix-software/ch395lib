---

## ch395_get_mac_adress
***Description***

Get mac address
***Input***

* Accumulator : the ptr
* X Register : the ptr

***Modify***

* Accumulator * X Register * Y Register * RESTmp value
***Returns***

* Accumulator : the content of ptr modified
* X Register : the content of ptr modified
* X Register : the content of ptr modified

***Example***

```ca65 lda #<mac_address ldx #>mac_address ldx #$00 jsr ch395_get_mac_adress rtsmac_address: .res 10```
```

