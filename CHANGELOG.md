# CHANGELOG

## libcnet v02.01d -- 29/06/2024 (06/29/2024) - ?

- Added TCP Header and Packets structure and functions to `ip.h` and `ip.c`
- Added functions for IP Packets to include TCP Packets into `ip.h` and `ip.c`
- Added a version header files containing the current version of the library `utils/version.h`
- Changed `snprintf` into `sprintf` into `convertControlBitsToBin` function of `ip.c`
- Added documentation using Doxygen. Modified all docstrings in according to Doxygen documentation