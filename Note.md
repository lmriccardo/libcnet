# Note on Development

`memcpy` in an LSB machine (Little-Endian) it will copy LSB and then MSB

```c
unsigned short x = 0x1234;
char buf[2]; // 2 bytes
memcpy(buf, &x, 2); // --> buf = 0x3412

unsigned short x1 = htons(0x1234); // --> x = 0x3412
memcpy(buf, &x, 2); // --> buf = 0x1234
```

In current implementation of the sender-receiver paradigm, once the sender has finished
sending the message, we need to wait 1 seconds in order to the receiver being able to
receive the response. 


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
- [] Update the crafter for TCP and UDP
- [] Update the sender to TCP and UDP
- [?] Create a receiver
- [?] Check for memory safetiness using Valgrind
- [] Maybe we should create a Thread also for the sender
- [] Fragmenter for UDP Packets
- [] Implements a simple Linked Lists for UDP Fragments and other application
- [] Add context to the functions (e.g., `__attribute__ ((__noreturn__))`)

Legend:

- [x] Task completed
- [?] Still under development/to be tested
- []  Not started Yet (aka, to be done)

### Advanced Development

- [] Implement custom ping message