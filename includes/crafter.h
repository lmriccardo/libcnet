#ifndef _CRAFTER_H
#define _CRAFTER_H

#include <sys/cdefs.h>
#include "neto.h"

__BEGIN_DECLS

/* Craft an IP Packet given the fields of the IP header */
extern IpPacket* craftIpPacket(
    u_int8_t _version, int       _dscp,     int       _ecn,     u_int16_t _tlen,   u_int16_t _id,
    int      _xf,      int       _df,       int       _mf,      int       _offset, u_int8_t  _ttl,
    u_int8_t _proto,   u_int16_t _checksum, u_int32_t _srcaddr, u_int32_t _dstaddr
) __returns_nonnull;

/* Craft an IP Packet given the fields of the IP header and the payload with size */
extern IpPacket* craftIpPacket_withData(
    u_int8_t _version, int       _dscp,     int       _ecn,     u_int16_t _tlen,   u_int16_t _id,
    int      _xf,      int       _df,       int       _mf,      int       _offset, u_int8_t  _ttl,
    u_int8_t _proto,   u_int16_t _checksum, u_int32_t _srcaddr, u_int32_t _dstaddr,
    char*    _payload
) __returns_nonnull __nonnull((15));

/* Craft an ICMP Packet with Unused fields */
extern IcmpPacket* craftIcmpPacket_Unused(u_int8_t _type, u_int8_t _code, u_int16_t _checksum) __returns_nonnull;

/* Craft an ICMP Packet with ID and Sequence Number fields */
extern IcmpPacket* craftIcmpPacket_Echo(
    u_int8_t _type, u_int8_t _code, u_int16_t _checksum, u_int16_t _id, u_int16_t _seqnum
) __returns_nonnull;

/* Craft an ICMP Packet with the Gateway field */
extern IcmpPacket* craftIcmpPacket_Redirect(
    u_int8_t _type, u_int8_t _code, u_int16_t _checksum, char* _gateway) __returns_nonnull __nonnull ((4));

/* Craft an UDP packet given the fields of the UDP header and the payload of the UDP Packet */
extern UdpPacket* craftUdpPacket(
    u_int16_t _srcport, u_int16_t _dstport, u_int16_t _length, u_int16_t _checksum,
    char*     _payload, size_t    _size
) __returns_nonnull __nonnull ((5));

__END_DECLS

#endif