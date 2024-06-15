#include "crafter.h"


IpPacket* craftIpPacket(
    const u_int8_t  _version, const int       _dscp, const int      _ecn,   const u_int16_t _tlen,
    const u_int16_t _id,      const int       _xf,   const int      _df,    const int       _mf, 
    const int       _offset,  const u_int8_t  _ttl,  const u_int8_t _proto, const u_int16_t _checksum, 
    const u_int32_t _srcaddr, const u_int32_t _dstaddr
) {
    IpPacket* ippckt = IpPacket_new();

    u_int16_t flagoff = computeFlagOff(_xf, _df, _mf, _offset);
    u_int16_t dsf = computeDifferentiatedServiceField(_dscp, _ecn);

    IpPacket_fillHeader(
        ippckt, _version, dsf, _tlen, _id, flagoff, _ttl, _proto,
        _checksum, _srcaddr, _dstaddr
    );

    return ippckt;
}

IpPacket* craftIpPacket_withData(
    const u_int8_t  _version, const int       _dscp,    const int      _ecn,   const u_int16_t _tlen,
    const u_int16_t _id,      const int       _xf,      const int      _df,    const int       _mf, 
    const int       _offset,  const u_int8_t  _ttl,     const u_int8_t _proto, const u_int16_t _checksum, 
    const u_int32_t _srcaddr, const u_int32_t _dstaddr, char*          _payload
){
    IpPacket* ippckt = craftIpPacket(
        _version, _dscp, _ecn, _tlen, _id, _xf, _df, _mf, _offset,
        _ttl, _proto, _checksum, _srcaddr, _dstaddr
    );

    IpPacket_fillPayload(ippckt, _payload, IpPacket_getPayloadSize(ippckt));

    return ippckt;
}

IcmpPacket* craftIcmpPacket_Unused(const u_int8_t _type, const u_int8_t _code, const u_int16_t _checksum)
{
    IcmpPacket* icmppckt = IcmpPacket_new_v2(_type, _code, 0x0);
    IcmpPacket_fillHeader_v1(icmppckt->_icmphdr, _checksum);
    return icmppckt;
}

IcmpPacket* craftIcmpPacket_Echo(
    const u_int8_t  _type, const u_int8_t  _code, const u_int16_t _checksum, 
    const u_int16_t _id,   const u_int16_t _seqnum
) {
    IcmpPacket* icmppckt = craftIcmpPacket_Unused(_type, _code, _checksum);
    IcmpPacket_fillHeader_v3(icmppckt->_icmphdr, _checksum, _id, _seqnum);
    return icmppckt;
}

IcmpPacket* craftIcmpPacket_Redirect(
    const u_int8_t _type, const u_int8_t _code, const u_int16_t _checksum, const char* _gateway
) {
    IcmpPacket* icmppckt = craftIcmpPacket_Unused(_type, _code, _checksum);
    IcmpPacket_fillHeader_v2(icmppckt->_icmphdr, _checksum, inet_network(_gateway));
    return icmppckt;
}

IcmpPacket* craftIcmpPacket_Mtu(const u_int8_t _type, const u_int8_t _code, const u_int16_t _checksum, const u_int16_t _mtu)
{
    IcmpPacket* icmppckt = craftIcmpPacket_Unused(_type, _code, _checksum);
    IcmpPacket_fillHeader_v4(icmppckt->_icmphdr, _checksum, _mtu);
    return icmppckt;
}

UdpPacket* craftUdpPacket(
    const u_int16_t _srcport, const u_int16_t _dstport, const u_int16_t _length, const u_int16_t _checksum,
    const char*     _payload, const size_t    _size
) {
    UdpPacket* pckt = UdpPacket_new_v2(_size);
    UdpPacket_fillHeader(pckt, _srcport, _dstport, _length, _checksum);
    UdpPacket_fillPayload(pckt, _payload, _size);
    return pckt;
}