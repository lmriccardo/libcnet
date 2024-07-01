# CHANGELOG

## libcnet v01.02d -- 29/06/2024 (06/29/2024) - ?

- Added TCP Header and Packets structure and functions to `ip.h` and `ip.c`
- Added functions for IP Packets to include TCP Packets into `ip.h` and `ip.c`
- Added a version header files containing the current version of the library `utils/version.h`
- Changed `snprintf` into `sprintf` into `convertControlBitsToBin` function of `ip.c`
- Added documentation using Doxygen. Modified all docstrings in according to Doxygen documentation
- Added crafting functions to the Sender for sending TCP Packets, `Sender_craftTcp` and `Sender_fillTcpHeader`
- Added `PseudoHeader` struct to `ip.h` and functions to compute Checksum for ICMP, UDP and TCP