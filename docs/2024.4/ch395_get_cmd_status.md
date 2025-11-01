---

## ch395_get_cmd_status

***Description***

This command is used to get the command execution status. CH395 will output 1 byte of data, which is the command execution state. The command execution status is as follows:

 jsr ch395_get_cmd_status
 ; Check A for STATUS
 rts
```


***Modify***

* Accumulator 
