#ifndef _SENDER_H
#define _SENDER_H

#include <sys/cdefs.h>
#include <sys/socket.h>
#include <stdlib.h>
#include <stdio.h>
#include <netdb.h>
#include <unistd.h>
#include <string.h>

#include "neto.h"
#include "crafter.h"
#include "utils.h"

__BEGIN_DECLS

#define handle_error(msg) do { perror(msg); exit(EXIT_FAILURE); } while (0)
#define LOOPBACK "127.0.0.1"

/* Struct representing a RawSender. It is called Raw since it uses the SOCK_RAW
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

} RawSender;

/* Creates and returns a new RawSender. */
extern RawSender* RawSender_new(
    char *_interface, const char* _dstaddr, char* _gateway, const u_int16_t _dstport, 
    const char* _proto, const bool _verbose
) __attribute__((returns_nonnull)) __attribute__((nonnull(1, 2, 5)));

/* Free the memory allocated for the input RawSender */
extern void RawSender_delete(RawSender* _self) __attribute__((nonnull));

/* Send the input IP Packet */
extern void  RawSender_sendto(RawSender* _self, const IpPacket* _pckt) __attribute__((nonnull));
extern void __RawSender_sendto_v2(RawSender* _self, const char* _buffer, const size_t _size) __attribute__((nonnull));

/* Continuously send the input IP Packet */
extern void  RawSender_sendc(RawSender* _self, const IpPacket* _pckt) __attribute__((nonnull));

/* Print some informations about the input Sender */
extern void  RawSender_printInfo(const RawSender* _self) __attribute__((nonnull));

/* Returns the string containing the destination IP */
extern char* RawSender_getDestinationIP(const RawSender* _self) __attribute__((nonnull)) __attribute__((returns_nonnull));

/* Create an IP Packet with some informations from the current Sender */
extern IpPacket* RawSender_createIpPacket(RawSender *_self, const u_int16_t _id) __attribute__((nonnull)) __attribute__((returns_nonnull));

/* Create an ICMP Packet with some informations from the current sender */
extern IcmpPacket* RawSender_createIcmpPacket(
    RawSender* _self, const u_int8_t _type, const u_int8_t _code) __attribute__((nonnull)) __attribute__((returns_nonnull));

/* Create an UDP Packet with some informations from the current sender */
extern UdpPacket* RawSender_createUdpPacket(
    RawSender* _self, const u_int16_t _srcport, const char* _payload, const size_t _size
) __attribute__((nonnull)) __attribute__((returns_nonnull));

/* Craft and send an ICMP Packet */
extern void RawSender_sendIcmp(
    RawSender* _self, const u_int8_t _type, const u_int8_t _code, const int _n, const double _delay
) __attribute__((nonnull));

/* Craft and send an UDP Packet */
extern void RawSender_sendUdp(
    RawSender* _self, const u_int16_t _srcport, const char* _payload, const size_t _size) __attribute__((nonnull));

__END_DECLS

#endif