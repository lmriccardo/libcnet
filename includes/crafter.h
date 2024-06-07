#ifndef _CRAFTER_H
#define _CRAFTER_H

#include <sys/cdefs.h>
#include "neto.h"

__BEGIN_DECLS

/* Craft an IP Packet given the fields of the IP header */
extern IpPacket* craftIpPacket(
    const u_int8_t  _version, const int       _dscp, const int      _ecn,   const u_int16_t _tlen,
    const u_int16_t _id,      const int       _xf,   const int      _df,    const int       _mf, 
    const int       _offset,  const u_int8_t  _ttl,  const u_int8_t _proto, const u_int16_t _checksum, 
    const u_int32_t _srcaddr, const u_int32_t _dstaddr
) __returns_nonnull;

/* Craft an IP Packet given the fields of the IP header and the payload with size */
extern IpPacket* craftIpPacket_withData(
    const u_int8_t  _version, const int       _dscp,    const int      _ecn,   const u_int16_t _tlen,
    const u_int16_t _id,      const int       _xf,      const int      _df,    const int       _mf, 
    const int       _offset,  const u_int8_t  _ttl,     const u_int8_t _proto, const u_int16_t _checksum, 
    const u_int32_t _srcaddr, const u_int32_t _dstaddr, char*          _payload
) __returns_nonnull __attribute__((nonnull));

/* Craft an ICMP Packet with Unused fields */
extern IcmpPacket* craftIcmpPacket_Unused(
    const u_int8_t _type, const u_int8_t _code, const u_int16_t _checksum) __returns_nonnull;

/* Craft an ICMP Packet with ID and Sequence Number fields */
extern IcmpPacket* craftIcmpPacket_Echo(
    const u_int8_t  _type, const u_int8_t  _code, const u_int16_t _checksum, 
    const u_int16_t _id,   const u_int16_t _seqnum
) __returns_nonnull;

/* Craft an ICMP Packet with the Gateway field */
extern IcmpPacket* craftIcmpPacket_Redirect(
    const u_int8_t _type, const u_int8_t _code, const u_int16_t _checksum, const char* _gateway
) __returns_nonnull __attribute__((nonnull));

/* Craft an UDP packet given the fields of the UDP header and the payload of the UDP Packet */
extern UdpPacket* craftUdpPacket(
    const u_int16_t _srcport, const u_int16_t _dstport, const u_int16_t _length, const u_int16_t _checksum,
    const char*     _payload, const size_t    _size
) __returns_nonnull __attribute__((nonnull));

__END_DECLS

#endif