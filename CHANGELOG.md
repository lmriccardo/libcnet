# CHANGELOG

## libcnet v01.01d -- 29/06/2024 (06/29/2024) - ?

##### Addition

- Added TCP Header and Packets structure and functions to `ip.h` and `ip.c`
- Added functions for IP Packets to include TCP Packets into `ip.h` and `ip.c`
- Added a version header files containing the current version of the library `utils/version.h`
- Added documentation using Doxygen. Modified all docstrings in according to Doxygen documentation
- Added crafting functions to the Sender for sending TCP Packets, `Sender_craftTcp` and `Sender_fillTcpHeader`
- Added `PseudoHeader` struct to `ip.h` and functions to compute Checksum for ICMP, UDP and TCP

##### Small Changes

- Changed `snprintf` into `sprintf` into `ControlBits_toBin` function of `ip.c`

##### Fixed Bugs

- When wrapping a packet into an IP Packet it causes the address of the payload being copied.
  This can cause a double free error when trying to call the `IpPacket_delete` function. To
  avoid this behaviour, first the Header and then the payload are copied into new memory
  locations. Before it was `memcpy(dstpacket, srcpacket, size)` now is
  `memcpy(dstpacket->hdr, srcpacket->hdr, hdr_size)` and `memcpy(dstpacket->payload, srcpacket->payload, psize)`