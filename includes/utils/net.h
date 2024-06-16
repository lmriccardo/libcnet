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
#include <semaphore.h>

__BEGIN_DECLS

/* Returns the IP address of the input host */
extern void getHostnameIP(const char* _hostname, char* _out) __attribute__((nonnull));

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

/* Fill the input buffer with a given number of random bytes */
extern void generateRandomData(const size_t _size, char* _dst) __attribute__((nonnull));

/* Returns the MTU (Maximum Transmission Unit) for the given input interface */
extern int getInterfaceMTU(const char* _interface) __attribute__((nonnull));

/* Synchronize a sender and a receiver with the same timer */
extern void synchronizeRTT(void* _sender, void* _recv, void* _timer)
    __attribute__((nonnull));

/* Synchronize the sender and the receiver setting a semaphore */
extern void syncrhonizeSendReceive(void* _sender, void* _recv) __attribute__((nonnull));

extern void __semaphore_init(sem_t* _sem, int _phsared, unsigned int _value, const char* _fname) __attribute__((nonnull));
extern void __semaphore_wait(sem_t* _sem, const char* _fname) __attribute__((nonnull));
extern void __semaphore_post(sem_t* _sem, const char* _fname) __attribute__((nonnull));


__END_DECLS

#endif