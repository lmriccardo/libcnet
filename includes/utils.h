#ifndef _UTILS_H
#define _UTILS_H

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
*/
extern char* addressNumberToString(u_int32_t _addr, const bool _be) 
    __attribute__((returns_nonnull));

__END_DECLS

#endif