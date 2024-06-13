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

__BEGIN_DECLS

/* Returns the IP address of the input host */
extern char* getHostnameIP(const char* _hostname) 
    __attribute__((nonnull)) __attribute__((returns_nonnull));

/* Returns the IP address of the given input interface */
extern void getInterfaceIp(const char* _interface, char *_addr)
    __attribute__((nonnull));

/* Converts the input address number into a string. The second argument is used
   to specify whether the input address number is in LE or BE format.

   The returns of this function is dynamically allocated, hence it must be freed.
*/
extern char* addressNumberToString(u_int32_t _addr, const bool _be) 
    __attribute__((returns_nonnull));

/* Converts the input address number into a string. The third argument is used
   to specify whether the input address number is in LE or BE format. The result
   of the operation will be saved into the _out input buffer.
*/
extern void addressNumberToString_s(u_int32_t _addr, char *_out, const bool _be) 
    __attribute__((nonnull));

__END_DECLS

#endif