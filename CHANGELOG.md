# CHANGELOG

## libcnet v01.01r -- 29/06/2024 - 04/07/2024

##### Addition

- Added TCP Header and Packets structure and functions to `ip.h` and `ip.c`
- Added functions for IP Packets to include TCP Packets into `ip.h` and `ip.c`
- Added a version header files containing the current version of the library `utils/version.h`
- Added documentation using Doxygen. Modified all docstrings in according to Doxygen documentation
- Added crafting functions to the Sender for sending TCP Packets, `Sender_craftTcp` and `Sender_fillTcpHeader`
- Added `PseudoHeader` struct to `ip.h` and functions to compute Checksum for ICMP, UDP and TCP

##### Small Changes

- Changed `snprintf` into `sprintf` into `ControlBits_toBin` function of `ip.c`
- Added TCP Option printing informations to the `TcpHeader_printInfo` function

##### Fixed Bugs

- When wrapping a packet into an IP Packet it causes the address of the payload being copied.
  This can cause a double free error when trying to call the `IpPacket_delete` function. To
  avoid this behaviour, first the Header and then the payload are copied into new memory
  locations. Before it was `memcpy(dstpacket, srcpacket, size)` now is
  `memcpy(dstpacket->hdr, srcpacket->hdr, hdr_size)` and `memcpy(dstpacket->payload, srcpacket->payload, psize)`

- When computing the checksum it was jumping two bytes, now it does not. Notice that it computes the checksum
  correctly both for ICMP and UDP Packets, but it does not for TCP Packets and complete IP Packets.

- Correct the bug when creating the TCP Option and then encoding flipping the bytes in an incorrect way.