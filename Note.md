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

> **PROBLEM 2**: The `process` function cause the library to loose some degrees of freedom, forcing the user to create `process` functions
> with restricted input types. Moreover, implementing various algorithms inside the library using this paradigm appears
> to be difficult and requires some strange tricks that might leads to an increasing and unwanted complexity. This is something
> that I would not like to have in this project. On the other hand, using a message queue shared between the receiver and the
> user program, makes things simpler. In this case the user may or may not define a process function.


## Todo

### Basic Development

- [x] Implement IP Packet (Header + Payload)
- [x] Implement ICMP Packet (Header + Payload) and wrap ICMP into IP
- [x] Create a Sender which sends the packet
- [x] Create a crafter for IP and ICMP packet
- [x] Consider this aspect of `memcpy` when filling `ByteBuffer` and computing the Checksum
- [ ] TCP Packet (Header + Payload)
- [x] UDP Packet (Header + Payload)
- [x] Decoder for already implemented Packets
- [?] Update the crafter for TCP and UDP
- [?] Update the sender to TCP and UDP
- [x] Create a receiver
- [?] Check for memory safetiness using Valgrind
- [?] Fragmenter for UDP Packets (not 100% sure it is the correct way)
- [?] Add context to the functions (e.g., `__attribute__ ((__noreturn__))`)
- [?] Add Documentation Strings to Function declaration in .h files
- [x] Find a way to handle the problem above (**PROBLEM 1**)
- [x] Create a simple Linked List
- [x] Change all the functions returning char pointers to function taking a destination buffer
- [x] Change the receiver so that it uses a Message Queue instead of `process` function (**PROBLEM 2**)
- [x] Re-engineering applied

Legend:

- [x] Task completed
- [?] Still under development/to be tested
- []  Not started Yet (aka, to be done)

### Advanced Development

- [x] Implement custom ping message
- [?] Test Fragmentation
- [x] Test Linked List
- [ ] Test UDP
- [?] Path MTU Discovery (Implemented, still to be tested)