#ifndef _SENDER_H
#define _SENDER_H

#include <sys/cdefs.h>
#include <sys/socket.h>
#include <stdlib.h>
#include <stdio.h>
#include <netdb.h>
#include <unistd.h>
#include <string.h>

#include "ip.h"
#include "crafter.h"
#include "utils/timer.h"
#include "utils/net.h"
#include "utils/synch.h"

#define handle_error(msg, fd) do { perror(msg); shutdown(fd, 2); exit(EXIT_FAILURE); } while (0)
#define LOOPBACK "127.0.0.1"

__BEGIN_DECLS

/* Struct representing a Sender. It is called Raw since it uses the SOCK_RAW
   option when creating a new socket. This structure has the following fields:
   - `_srcaddress` The IP address sending the packets (type `char*`)
   - `_dstaddress` The host receiving the packet (type `struct sockaddr_in`)
   - `_gateway` The gateway address of the current Source address (type `char*`)
   - `_socket` The file descriptor of the created socket (type `int`)
   - `_msgcnt` The current Message number being sent (type `int`)
   - `_proto` The protocol used to send the packets (type `struct protoent*`)
   - `_lastid` The last used IP Packet Identifier (type `u_int16_t`)
   - `_lasticmpid` The last used ICMP Packet Identifier (type `u_int16_t`)
   - `_icmpsn` The last ICMP Message Sequence Number (type `u_int16_t`)
   - `_timer` A Timer shared with the Receiver
   - `_mtu` The Maximum Transmission Unit
   - `_mutex` A simple Mutex to syncrhonize with the receiver
   - `_synch` A boolean flag to indicate the syncrhonization
   - `_sent` A boolean flag to help avoid deadlock when synchronizing
*/
typedef struct 
{

    char*              _srcaddress;  /* Source Address */
    struct sockaddr_in _dstaddress;  /* The Host receiving the packet */
    char*              _gateway;     /* The gateway address of the current Source Address */
    int                _socket;      /* File descriptor for the created socket */
    int                _msgcnt;      /* Current Message Number */
    struct protoent*   _proto;       /* The protocol used */
    u_int16_t          _lastid;      /* Last used IP Packet identifier */
    u_int16_t          _lsticmpid;   /* Last used ICMP Packet identifier */
    u_int16_t          _icmpsn;      /* ICMP Message Sequence Number */
    bool               _verbose;     /* Enable verbosity */
    struct Timer*      _timer;       /* A timer synchronized with the receiver */
    int                _mtu;         /* The maximum transmission unit */
    sem_t              _mutex;       /* A semaphore to synchronize with the Receiver */
    bool               _synch;       /* Set to true when it is synchronized with the Receiver */
    bool               _sent;        /* Another variable to syncrhonize with the Receiver */

} Sender;

/* Creates and returns a new Sender. */
extern Sender* Sender_new(
    const char *_interface, const char* _dstaddr, char* _gateway, const u_int16_t _dstport, 
    const char* _proto, const bool _verbose
) __attribute__((returns_nonnull)) __attribute__((nonnull(1, 2, 5)));

/* Free the memory allocated for the input Sender */
extern void Sender_delete(Sender* _self) __attribute__((nonnull));

/* Set a timer into the sender */
extern void Sender_setTimer(Sender* _self, struct Timer* _timer) __attribute__((nonnull));

/* Set the Maximum Transmission Unit into the Sender */
extern void Sender_setMtu(Sender* _self, const int _mtu) __attribute__((nonnull));

/* Send the input IP Packet */
extern void  Sender_sendto(Sender* _self, const IpPacket* _pckt) __attribute__((nonnull));
extern void __Sender_sendto_v2(Sender* _self, const char* _buffer, const size_t _size) __attribute__((nonnull));

/* Continuously send the input IP Packet */
extern void  Sender_sendc(Sender* _self, const IpPacket* _pckt) __attribute__((nonnull));

/* Print some informations about the input Sender */
extern void  Sender_printInfo(const Sender* _self) __attribute__((nonnull));

/* Returns the string containing the destination IP */
extern char* Sender_getDestinationIP(const Sender* _self) __attribute__((nonnull)) __attribute__((returns_nonnull));

/* Create an IP Packet with some informations from the current Sender */
extern IpPacket* Sender_createIpPacket(Sender *_self, const u_int16_t _id) __attribute__((nonnull)) __attribute__((returns_nonnull));

/* Create an ICMP Packet with some informations from the current sender */
extern IcmpPacket* Sender_createIcmpPacket(
    Sender* _self, const u_int8_t _type, const u_int8_t _code, const char* _payload, const size_t _size
) __attribute__((nonnull(1))) __attribute__((returns_nonnull));

/* Create an UDP Packet with some informations from the current sender */
extern UdpPacket* Sender_createUdpPacket(
    Sender* _self, const u_int16_t _srcport, const char* _payload, const size_t _size
) __attribute__((nonnull)) __attribute__((returns_nonnull));

/* Craft and send an ICMP Packet */
extern void Sender_sendIcmp(
    Sender* _self, const u_int8_t _type, const u_int8_t _code, const int _n, const double _delay, 
    const char* _payload, const size_t _size ) __attribute__((nonnull(1)));

/* Craft and send an UDP Packet */
extern void Sender_sendUdp(
    Sender* _self, const u_int16_t _srcport, const char* _payload, const size_t _size) __attribute__((nonnull));

__END_DECLS

#endif