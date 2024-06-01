#ifndef _SENDER_H
#define _SENDER_H

#include <sys/cdefs.h>
#include <sys/socket.h>
#include <stdlib.h>
#include <stdio.h>
#include <netdb.h>
#include "neto.h"

__BEGIN_DECLS

#define handle_error(msg) do { perror(msg); exit(EXIT_FAILURE); } while (0)
#define LOOPBACK "127.0.0.1"

typedef struct 
{

    struct sockaddr_in _dstaddress;  /* The Host receiving the packet */
    int                _socket;      /* File descriptor for the created socket */
    int                _msgcnt;      /* Current Message Number */
    ByteBuffer*        _buff;        /* Last buffer being sent */

} RawSender;

extern RawSender* RawSender_new(char* _dstaddr, u_int16_t _dstport, char* _proto);
extern void RawSender_delete(RawSender* _self);

extern void RawSender_sendto(RawSender* _self, IpPacket* _pckt);
extern void RawSender_sendc(RawSender* _self, IpPacket* _pckt);
extern void RawSender_printInfo(RawSender* _self);

extern void __RawSender_sendto_v2(RawSender* _self, char* _buffer, size_t _size);

#endif