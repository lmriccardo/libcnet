/****************************************************************************************
 ****************************************************************************************
 *****                                                                              *****
 *****   @details Header file                                                       *****
 *****   @name crafter.h                                                            *****
 *****   @author Riccardo La Marca (riccardo.lamarca98@gmail.com)                   *****
 *****   @date 1 June 2024 (Mon Jul 01 2024 Last Modification)                      *****
 *****                                                                              *****
 *****   @brief This file contains the declarations of functions to craft packets   *****
 *****                                                                              *****
 ****************************************************************************************
 ****************************************************************************************
*/


#ifndef _CRAFTER_H
#define _CRAFTER_H

#include <sys/cdefs.h>
#include "ip.h"
#include "utils/version.h"

__BEGIN_DECLS

/**
 * Craft an IP Packet given the fields of the IP header
 */
extern IpPacket* craftIpPacket(
    const u_int8_t  _version, const int       _dscp, const int      _ecn,   const u_int16_t _tlen,
    const u_int16_t _id,      const int       _xf,   const int      _df,    const int       _mf, 
    const int       _offset,  const u_int8_t  _ttl,  const u_int8_t _proto, const u_int16_t _checksum, 
    const u_int32_t _srcaddr, const u_int32_t _dstaddr
) __attribute__((returns_nonnull));

/**
 * Craft an ICMP Packet with Unused fields
 */
extern IcmpPacket* craftIcmpPacket_Unused(
    const u_int8_t _type, const u_int8_t _code, const u_int16_t _checksum, const char* _payload, const size_t _size
) __attribute__((returns_nonnull));

/**
 * Craft an ICMP Packet with ID and Sequence Number fields
 */
extern IcmpPacket* craftIcmpPacket_Echo(
    const u_int8_t  _type, const u_int8_t  _code,   const u_int16_t _checksum, 
    const u_int16_t _id,   const u_int16_t _seqnum, const char*     _payload, 
    const size_t    _size
) __attribute__((returns_nonnull));

/**
 * Craft an ICMP Packet with the Gateway field
 */
extern IcmpPacket* craftIcmpPacket_Redirect(
    const u_int8_t _type,    const u_int8_t _code, const u_int16_t _checksum, const char* _gateway,
    const char*    _payload, const size_t   _size
) __attribute__((returns_nonnull)) __attribute__((nonnull(4)));

/**
 * Craft an ICMP Packet with the MTU Field
 */
extern IcmpPacket* craftIcmpPacket_Mtu(
    const u_int8_t _type,    const u_int8_t _code, const u_int16_t _checksum, const u_int16_t _mtu,
    const char*    _payload, const size_t   _size
)  __attribute__((returns_nonnull));

/**
 * Craft an UDP packet given the fields of the UDP header and the payload of the UDP Packet
 */
extern UdpPacket* craftUdpPacket(
    const u_int16_t _srcport, const u_int16_t _dstport, const u_int16_t _length, const u_int16_t _checksum,
    const char*     _payload, const size_t    _size
) __attribute__((returns_nonnull));


/**
 * Create a TCP Packet given the fields of the TCP header and the payload of the TCP Packet
 */
extern TcpPacket* craftTcpPacket(
    const u_int16_t _srcport, const u_int16_t _dstport,  const u_int32_t          _seqnum, 
    const u_int32_t _acknum,  const u_int8_t  _offset,   const struct ControlBits _cbits,
    const u_int16_t _window,  const u_int16_t _checksum, const u_int16_t          _urgpntr,
    const char*     _payload, const size_t    _size
) __attribute__((returns_nonnull));

__END_DECLS

#endif