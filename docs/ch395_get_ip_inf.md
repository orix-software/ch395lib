---

## ch395_get_ip_inf
***Description***

Get ip info
***Input***

* Accumulator : the ptr to modify (Ptr must be 20 bytes length)
* X Register : the ptr to modify (Ptr must be 20 bytes length)

***Modify***

* Accumulator * X Register * Y Register * RESTmp value* RESTmp value
***Example***

```ca65 lda #<ip_address ldx #>ip_address jsr ch395_get_ip_inf rtsip_address: .res 4```
```

