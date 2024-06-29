#ifndef _NET_H
#define _NET_H

#include <sys/cdefs.h>
#include <sys/types.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <netdb.h>
#include <arpa/inet.h>
#include <netinet/in.h>
#include <linux/if.h>
#include <sys/socket.h>
#include <sys/ioctl.h>
#include <string.h>
#include <stdbool.h>
#include "utils/version.h"

__BEGIN_DECLS

/* Returns the IP address of the input host */
extern void getHostnameIP(const char* _hostname, char* _out) __attribute__((nonnull));

/* Returns the IP address of the given input interface */
extern void getInterfaceIp(const char* _interface, char *_addr)
    __attribute__((nonnull));

/* Converts the input address number into a string. The third argument is used
   to specify whether the input address number is in LE or BE format. The result
   of the operation will be saved into the _out input buffer.
*/
extern void addressNumberToString(u_int32_t _addr, char *_out, const bool _be) 
    __attribute__((nonnull));

/* Fill the input buffer with a given number of random bytes */
extern void generateRandomData(char* _dst, const size_t _size) __attribute__((nonnull));

/* Returns the MTU (Maximum Transmission Unit) for the given input interface */
extern int getInterfaceMTU(const char* _interface) __attribute__((nonnull));

/* Compute the checksum of an IP Packet as described in the corresponding RFC */
extern u_int16_t computeChecksum(const unsigned char* buffer, const size_t size) 
    __attribute__((nonnull));

/* Performs Path MTU Discovery Search */
extern int pathMtuDiscovery(const char* _interface, const char* _hostname) 
    __attribute__((nonnull));


__END_DECLS

#endif