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

__BEGIN_DECLS

/* Returns the IP address of the input host */
extern char* getHostnameIP(const char* _hostname) 
    __attribute__((nonnull)) __attribute__((returns_nonnull));

/* Returns the IP address of the given input interface */
extern char* getInterfaceIp(const char* _interface)
    __attribute__((nonnull)) __attribute__((returns_nonnull));

__END_DECLS

#endif