#ifndef _CRAFTER_H
#define _CRAFTER_H

#include <sys/cdefs.h>
#include "neto.h"

__BEGIN_DECLS

extern IpPacket* craftIpPacket(
    u_int8_t _version, int       _dscp,     int   _ecn,     u_int16_t _tlen,   u_int16_t _id,
    int      _xf,      int       _df,       int   _mf,      int       _offset, u_int8_t  _ttl,
    u_int8_t _proto,   u_int16_t _checksum, char* _srcaddr, char*     _dstaddr
);

extern IpPacket* craftIpPacket_withData(
    u_int8_t _version, int       _dscp,     int   _ecn,     u_int16_t _tlen,   u_int16_t _id,
    int      _xf,      int       _df,       int   _mf,      int       _offset, u_int8_t  _ttl,
    u_int8_t _proto,   u_int16_t _checksum, char* _srcaddr, char*     _dstaddr,
    char*    _payload
);

extern IcmpPacket* craftIcmpPacket_Unused(
    u_int8_t _type, u_int8_t _code, u_int16_t _checksum
);

extern IcmpPacket* craftIcmpPacket_Echo(
    u_int8_t _type, u_int8_t _code, u_int16_t _checksum, u_int16_t _id, u_int16_t _seqnum
);

extern IcmpPacket* craftIcmpPacket_Redirect(
    u_int8_t _type, u_int8_t _code, u_int16_t _checksum, char* _gateway
);

__END_DECLS

#endif