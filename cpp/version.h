#ifndef VERSION_H
#define VERSION_H

#define LIBCNET_VERSION "v.01.00d"

#define LIBCNET_MAJOR 1
#define LIBCNET_MINOR 0
#define LIBCNET_LEVEL 1 // 1 -> development, 2 -> release

// Hexadecimal representation of the version, e.g. 0x20110
// represents version 02.01d0.
#define LIBCNET_VERSION_HEX (  (LIBCNET_MAJOR << 16) \
                             | (LIBCNET_MINOR << 8)  \
                             | (LIBCNET_LEVEL << 4))

#endif