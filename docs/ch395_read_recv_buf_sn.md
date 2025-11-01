---

## ch395_read_recv_buf_sn
***Description***

This command is used to read data from Socket receive buffer. It is necessary to input 1 byte of Socket index value and 2 bytes of length (low bytes in front). CH395 will output several bytes of data stream based on the length value. In actual application, the command RECV_LEN_SN can be firstly sent to get the actual effective length of the current buffer. The length of the read data can be less than the actual effective length of the buffer, the unread data is still reserved in the receive buffer, and MCU can continue to read through this command. In MACRAW mode, the processing modes are different. In MACRAW mode, the receive buffer is a frame buffer, which can cache only 1 frame of Ethernet data. After CH395 processes the command READ_RECV_BUF_SN, Socket0 receive buffer will be cleared, so MCU shall read all valid data of the buffer at a time.
***Input***

* Accumulator : Socket id
* Y Register : Length low
* X Register : length high
* RES : ptr to fill
***Modify***

* RESBtmp value
