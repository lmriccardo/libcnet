# Todo C++ development

## Utils Development

- [x] Implements ByteBuffer class (*To be Tested*)
- [x] Implements MessageQueue buffer (*To be Tested*)
- [x] Implements Timer class (*To be Tested*)
- [x] Test ByteBuffer Implementation
- [x] Test MessageQueue Implementation
- [x] Test Timer Implementation

## Packets Development

### IP Packet

- [ ] Implements IP Packet and IP Header class

### TCP Packet

- [ ] Implements TCP Packet and TCP Header class

### ICMP Packet

- [?] Implements ICMP Packet and ICMP Header class

### UDP Packet

- [ ] Implements UDP Packet and UDP Header class


## Communication Development

- [?] Implements Sender
- [ ] Implements Receiver
- [ ] Synchrnonization function Sender-Receiver (*NOTA 1*)

*NOTA 1*

To perform thread synchronization between sender and receiver the idea is to
set the same mutex and condition variable the sender has to the receiver.
Whenever the receiver needs to receive it acquire the lock and wait to be
notified by the sender, once received it release the lock and notify the sender.