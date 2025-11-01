---

## ch395_set_retran_period

***Description***

This command is used to set the retry cycle. It is necessary to input 2 bytes of number of cycles of (with low bytes in front) in milliseconds. The allowable maximum value is 1000. The total retry time is N * M, N is the number of retries, and M is the retry cycle. The default retry cycle is 500MS and retries are only valid in TCP mode.

***Input***

* Accumulator : Period 
* X Register : Period 


***Modify***

* Y Register 
 lda #$FF
 ldx #$FF
 jsr ch395_retran_period
 rts
```

