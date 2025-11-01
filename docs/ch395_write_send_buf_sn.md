---

## ch395_write_send_buf_sn
***Description***

This command is used to write data to Socket transmit buffer. It is necessary to input 1 byte of Socket index value, 2 bytes of length (low bytes in front) and several bytes of data stream. The length of input data must not be larger than the size of transmit buffer. However, in MACRAW mode, the maximum length of input data can only be 1514, and any redundant data will be discarded. After the external MCU writes the data, CH395 will encapsulate the data packet according to the working mode of Socket, and then send it. Before MCU receives SINT_STAT_SENBUF_FREE, it is not allowed to write data into Socket transmit buffer again
***Input***

* Accumulator : Socket ID
* Y Register : Low length
* X Register : High length
* RES : Ptr of the data to send* RESB : Adress ptr to read
***Modify***

* RESTmp
***Example***

```ca65```
```

