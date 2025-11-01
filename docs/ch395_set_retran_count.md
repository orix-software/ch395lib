---

## ch395_set_retran_count
***Description***

This command is used to set the number of retries. It is necessary to input 1 byte of number of retries. The allowable maximum value is 20. If the input data is more than 20, it will be processed as 20. The default number of retries is 12, and retries are only valid in TCP mode.
***Input***

* Accumulator : Retran period

***Modify***

* X Register 
