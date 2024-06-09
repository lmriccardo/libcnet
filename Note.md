# Note on Development

`memcpy` in an LSB machine (Little-Endian) it will copy LSB and then MSB

```c
unsigned short x = 0x1234;
char buf[2]; // 2 bytes
memcpy(buf, &x, 2); // --> buf = 0x3412

unsigned short x1 = htons(0x1234); // --> x = 0x3412
memcpy(buf, &x, 2); // --> buf = 0x1234
```

> In current implementation of the sender-receiver paradigm, once the sender has finished
> sending the message, we need to wait 1 seconds in order to the receiver being able to
> receive the response. 

> **PROBLEM 1**: It happens that, when calling `getInterfaceIp` (or in general any other function returning
> a pointer to a static portion of memory) and `getHostnameIp` in this sequence, the content
> of value from the first call is overwritten by the second function. It is also discussed
> in the documentation of `gethostbyname`. 


## Todo

### Basic Development

- [x] Implement IP Packet (Header + Payload)
- [x] Implement ICMP Packet (Header + Payload) and wrap ICMP into IP
- [x] Create a Sender which sends the packet
- [x] Create a crafter for IP and ICMP packet
- [x] Consider this aspect of `memcpy` when filling `ByteBuffer` and computing the Checksum
- [] TCP Packet (Header + Payload)
- [?] UDP Packet (Header + Payload)
- [x] Decoder for already implemented Packets
- [?] Update the crafter for TCP and UDP
- [?] Update the sender to TCP and UDP
- [?] Create a receiver
- [?] Check for memory safetiness using Valgrind
- [] Maybe we should create a Thread also for the sender
- [?] Fragmenter for UDP Packets (not 100% sure it is the correct way)
- [?] Add context to the functions (e.g., `__attribute__ ((__noreturn__))`)
- [?] Add Documentation Strings to Function declaration in .h files
- [] Find a way to handle the problem above (**PROBLEM 1**)

Legend:

- [x] Task completed
- [?] Still under development/to be tested
- []  Not started Yet (aka, to be done)

### Advanced Development

- [?] Implement custom ping message