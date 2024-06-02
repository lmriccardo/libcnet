#ifndef _SENDER_H
#define _SENDER_H

#include <sys/cdefs.h>
#include <sys/socket.h>
#include <stdlib.h>
#include <stdio.h>
#include <netdb.h>
#include <unistd.h>
#include "neto.h"

__BEGIN_DECLS

#define handle_error(msg) do { perror(msg); exit(EXIT_FAILURE); } while (0)
#define LOOPBACK "127.0.0.1"

typedef struct 
{

    char*              _srcaddress;  /* Source Address */
    struct sockaddr_in _dstaddress;  /* The Host receiving the packet */
    int                _socket;      /* File descriptor for the created socket */
    int                _msgcnt;      /* Current Message Number */
    ByteBuffer*        _buff;        /* Last buffer being sent */
    struct protoent*   _proto;       /* The protocol used */
    u_int8_t           _lastid;      /* Last used identifier */
    u_int16_t          _icmpsn;      /* ICMP Message Sequence Number */

} RawSender;

extern RawSender* RawSender_new(char *_srcaddr, char* _dstaddr, u_int16_t _dstport, char* _proto);
extern void RawSender_delete(RawSender* _self);

extern void  RawSender_sendto(RawSender* _self, IpPacket* _pckt);
extern void  RawSender_sendc(RawSender* _self, IpPacket* _pckt);
extern void  RawSender_printInfo(RawSender* _self);
extern char* RawSender_getDestinationIP(RawSender* _self);

extern void __RawSender_sendto_v2(RawSender* _self, char* _buffer, size_t _size);

extern IpPacket*   RawSender_craftIpPacket(RawSender *_self, u_int16_t _id);
extern IcmpPacket* RawSender_craftIcmpPacket(RawSender* _self, u_int8_t _type, u_int8_t _code);

extern void RawSender_sendIcmp(RawSender* _self, u_int8_t _type, u_int8_t _code);
extern void RawSender_sendIcmp_Echo_v2(RawSender* _self, u_int8_t _type, u_int8_t _code, u_int16_t _id);
extern void RawSender_sendIcmp_Echo_v3(RawSender* _self, u_int8_t _type, u_int8_t _code, u_int16_t _seqnum);
extern void RawSender_sendIcmp_Echo_v4(
    RawSender* _self, u_int8_t _type, u_int8_t _code, u_int16_t _id, u_int16_t _seqnum);

extern IpPacket* craftIpPacket(
    u_int8_t _version, int       _dscp,     int   _ecn,     u_int16_t _tlen,   u_int16_t _id,
    int      _xf,      int       _df,       int   _mf,      int       _offset, u_int8_t  _ttl,
    u_int8_t _proto,   u_int16_t _checksum, char* _srcaddr, char*     _dstaddr
);

extern IcmpPacket* craftIcmpPacket(
    
);

#endif