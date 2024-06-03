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

__BEGIN_DECLS

#define handle_error(msg) do { perror(msg); exit(EXIT_FAILURE); } while (0)
#define LOOPBACK "127.0.0.1"

typedef struct 
{

    char*              _srcaddress;  /* Source Address */
    struct sockaddr_in _dstaddress;  /* The Host receiving the packet */
    char*              _gateway;     /* The gateway address of the current Source Address */
    int                _socket;      /* File descriptor for the created socket */
    int                _msgcnt;      /* Current Message Number */
    ByteBuffer*        _buff;        /* Last buffer being sent */
    struct protoent*   _proto;       /* The protocol used */
    u_int16_t          _lastid;      /* Last used IP Packet identifier */
    u_int16_t          _lsticmpid;   /* Last used ICMP Packet identifier */
    u_int16_t          _icmpsn;      /* ICMP Message Sequence Number */

} RawSender;

extern RawSender* RawSender_new(
    char *_srcaddr, char* _dstaddr, char* _gateway, u_int16_t _dstport, char* _proto
);

extern void RawSender_delete(RawSender* _self);

extern void  RawSender_sendto(RawSender* _self, IpPacket* _pckt);
extern void  RawSender_sendc(RawSender* _self, IpPacket* _pckt);
extern void  RawSender_printInfo(RawSender* _self);
extern char* RawSender_getDestinationIP(RawSender* _self);

extern void __RawSender_sendto_v2(RawSender* _self, char* _buffer, size_t _size);

extern IpPacket*   RawSender_createIpPacket(RawSender *_self, u_int16_t _id);
extern IcmpPacket* RawSender_createIcmpPacket(RawSender* _self, u_int8_t _type, u_int8_t _code);

extern void RawSender_sendIcmp(RawSender* _self, u_int8_t _type, u_int8_t _code);

__END_DECLS

#endif