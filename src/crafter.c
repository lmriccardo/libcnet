#include "crafter.h"


IpPacket* craftIpPacket(
    u_int8_t _version, int       _dscp,     int       _ecn,     u_int16_t _tlen,   u_int16_t _id,
    int      _xf,      int       _df,       int       _mf,      int       _offset, u_int8_t  _ttl,
    u_int8_t _proto,   u_int16_t _checksum, u_int32_t _srcaddr, u_int32_t _dstaddr
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
    u_int8_t _version, int       _dscp,     int       _ecn,     u_int16_t _tlen,   u_int16_t _id,
    int      _xf,      int       _df,       int       _mf,      int       _offset, u_int8_t  _ttl,
    u_int8_t _proto,   u_int16_t _checksum, u_int32_t _srcaddr, u_int32_t _dstaddr,
    char*    _payload
){
    IpPacket* ippckt = craftIpPacket(
        _version, _dscp, _ecn, _tlen, _id, _xf, _df, _mf, _offset,
        _ttl, _proto, _checksum, _srcaddr, _dstaddr
    );

    IpPacket_fillPayload(ippckt, _payload, IpPacket_getPayloadSize(ippckt));

    return ippckt;
}

IcmpPacket* craftIcmpPacket_Unused(u_int8_t _type, u_int8_t _code, u_int16_t _checksum)
{
    IcmpPacket* icmppckt = IcmpPacket_new_v2(_type, 0x0);
    IcmpHeader_setCode(icmppckt->_icmphdr, _code);
    IcmpHeader_setChecksum(icmppckt->_icmphdr, _checksum);
    return icmppckt;
}

IcmpPacket* craftIcmpPacket_Echo(
    u_int8_t _type, u_int8_t _code, u_int16_t _checksum, u_int16_t _id, u_int16_t _seqnum
) {
    IcmpPacket* icmppckt = craftIcmpPacket_Unused(_type, _code, _checksum);
    IcmpHeader_setIdentifier(icmppckt->_icmphdr, _id);
    IcmpHeader_setSequenceNumber(icmppckt->_icmphdr, _seqnum);
    return icmppckt;
}

IcmpPacket* craftIcmpPacket_Redirect(
    u_int8_t _type, u_int8_t _code, u_int16_t _checksum, char* _gateway
) {
    IcmpPacket* icmppckt = craftIcmpPacket_Unused(_type, _code, _checksum);
    IcmpHeader_setGateway(icmppckt->_icmphdr, inet_network(_gateway));
    return icmppckt;
}

UdpPacket* craftUdpPacket(
    u_int16_t _srcport, u_int16_t _dstport, u_int16_t _length, u_int16_t _checksum,
    char*     _payload, size_t    _size
) {
    UdpPacket* pckt = UdpPacket_new_v2(_size);
    UdpPacket_fillHeader(pckt, _srcport, _dstport, _length, _checksum);
    UdpPacket_fillPayload(pckt, _payload, _size);
    return pckt;
}