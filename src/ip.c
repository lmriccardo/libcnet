#include "ip.h"

/* --------------------------------------------- ICMP HEADER --------------------------------------------- */

// IcmpHeader* IcmpHeader_new(const u_int8_t _type, const u_int8_t _code)
void IcmpHeader_new(IcmpHeader* hdr, const u_int8_t _type, const u_int8_t _code)
{
    switch (_type)
    {
        case ICMP_DESTINATION_UNREACHABLE_TYPE:
            if ( _code == ICMP_FRAGMENTATION_NEEDED_CODE )
            {
                __IcmpHeader_createHeader_v4(hdr);
                break;
            }
        case ICMP_SOURCE_QUENCH_TYPE:
        case ICMP_TIME_EXCEEEDED_TYPE:
            __IcmpHeader_createHeader_v1(hdr);
            break;

        case ICMP_REDIRECT_TYPE:
            __IcmpHeader_createHeader_v2(hdr);
            break;

        case ICMP_ECHO_REPLY_TYPE:
        case ICMP_ECHO_TYPE:
        case ICMP_INFORMATION_REQUEST_TYPE:
        case ICMP_INFORMATION_REPLY_TYPE:
            __IcmpHeader_createHeader_v3(hdr);
            break;
        
        default:
            fprintf(stderr, "[IcmpHeader_new] ICMP Header Type %d do not exists!\n", _type);
            exit(EXIT_FAILURE);
    }

    IcmpHeader_setType(hdr, _type);
    IcmpHeader_setCode(hdr, _code);
    IcmpHeader_setChecksum(hdr, 0);
}

void __IcmpHeader_createHeader_v1(IcmpHeader* _self)
{
    // The first version consists on the header with Type, code
    // checksum and the next 32 bit left unused.
    _self->_rest._unused = 0x0;
}

void __IcmpHeader_createHeader_v2(IcmpHeader* _self)
{
    // The second version of the header consists of Type, code
    // checksum and 32 bit reserved to the gateway address.
    _self->_rest._gateway = 0;
}

void __IcmpHeader_createHeader_v3(IcmpHeader* _self)
{
    // The third version of the header consists of Type, code
    // checksum, identification and sequence number.
    _self->_rest._echo._id = 0;
    _self->_rest._echo._seqnum = 0;
}

void __IcmpHeader_createHeader_v4(IcmpHeader* _self)
{
    // The fourth version of the header is used for MTU
    // Path Discovery. It has 16 bits set to zero, and
    // remaining 16 bits contains the MTU of the next hop
    _self->_rest._mtu._unused = 0x0;
    _self->_rest._mtu._mtu = 0x0;
}

void IcmpHeader_setType(IcmpHeader* _self, const u_int8_t _type)
{
    _self->_type = _type;
}

void IcmpHeader_setCode(IcmpHeader* _self, const u_int8_t _code)
{
    _self->_code = _code;
}

void IcmpHeader_setChecksum(IcmpHeader* _self, const u_int16_t _checksum)
{
    _self->_checksum = _checksum;
}

void IcmpHeader_setGateway(IcmpHeader* _self, const u_int32_t _gateway)
{
    if (_self->_type != ICMP_REDIRECT_TYPE)
    {
        fprintf(stderr, "[IcmpHeader_setGateway] To set gateway address ");
        fprintf(stderr, "an ICMP message of Type %d is required. ", ICMP_REDIRECT_TYPE);
        fprintf(stderr, "Current Type is: %d.\n", _self->_type);
        exit(EXIT_FAILURE);
    }

    _self->_rest._gateway = _gateway;
}

void IcmpHeader_setIdentifier(IcmpHeader* _self, const u_int16_t _id)
{
    if (
        (
            _self->_type != ICMP_ECHO_REPLY_TYPE          &&
            _self->_type != ICMP_ECHO_TYPE                &&
            _self->_type != ICMP_INFORMATION_REQUEST_TYPE &&
            _self->_type != ICMP_INFORMATION_REPLY_TYPE
        )
    ) {
        fprintf(stderr, "[IcmpHeader_setIdentifier] To set Identifier Header field ");

        fprintf(
            stderr, "an ICMP message of Type %d/%d/%d/%d is required. ", 
            ICMP_ECHO_REPLY_TYPE, ICMP_ECHO_TYPE, ICMP_INFORMATION_REQUEST_TYPE,
            ICMP_INFORMATION_REPLY_TYPE
        );

        fprintf(stderr, "Current Type is: %d.\n", _self->_type);
        exit(EXIT_FAILURE);
    }

    _self->_rest._echo._id = _id;
}

void IcmpHeader_setSequenceNumber(IcmpHeader* _self, const u_int16_t _seqnum)
{
    if (
        (
            _self->_type != ICMP_ECHO_REPLY_TYPE          &&
            _self->_type != ICMP_ECHO_TYPE                &&
            _self->_type != ICMP_INFORMATION_REQUEST_TYPE &&
            _self->_type != ICMP_INFORMATION_REPLY_TYPE
        )
    ) {
        fprintf(stderr, "[IcmpHeader_setSequenceNumber] To set Sequence Number Header field ");

        fprintf(
            stderr, "an ICMP message of Type %d/%d/%d/%d is required. ", 
            ICMP_ECHO_REPLY_TYPE, ICMP_ECHO_TYPE, ICMP_INFORMATION_REQUEST_TYPE,
            ICMP_INFORMATION_REPLY_TYPE
        );

        fprintf(stderr, "Current Type is: %d.\n", _self->_type);
        exit(EXIT_FAILURE);
    }

    _self->_rest._echo._seqnum = _seqnum;
}

void IcmpHeader_setNextHopMtu(IcmpHeader* _self, const u_int16_t _mtu)
{
    if (
        (
            _self->_type != ICMP_DESTINATION_UNREACHABLE_TYPE ||
            _self->_code != ICMP_FRAGMENTATION_NEEDED_CODE
        )
    ) {
        fprintf(stderr, "[IcmpHeader_setNextHopMtu] To set Next Hop MTU header field ");

        fprintf(
            stderr, "an ICMP Message of Type %d and Code %d is required. ",
            ICMP_DESTINATION_UNREACHABLE_TYPE, ICMP_FRAGMENTATION_NEEDED_CODE
        );

        fprintf(stderr, "Current Type and Code are: t=%d, c=%d\n", _self->_type, _self->_code);
        exit(EXIT_FAILURE);
    }

    _self->_rest._mtu._mtu = _mtu;
}

void IcmpHeader_printInfo(const IcmpHeader* _self)
{
    printf("[*] Printing Information of the ICMP Header\n");
    printf("ICMP Message Type: %d\n", _self->_type);
    printf("ICMP Message Code: %d\n", _self->_code);
    printf("ICMP Header Checksum: %hu\n", _self->_checksum);

    if (_self->_type == ICMP_REDIRECT_TYPE)
    {
        IcmpHeader_printInfo_v2(_self);
    }

    if (
        (
            _self->_type == ICMP_ECHO_REPLY_TYPE          ||
            _self->_type == ICMP_ECHO_TYPE                ||
            _self->_type == ICMP_INFORMATION_REQUEST_TYPE ||
            _self->_type == ICMP_INFORMATION_REPLY_TYPE
        )
    ) {
        IcmpHeader_printInfo_v3(_self);
    }

    if (
        (
            _self->_type == ICMP_DESTINATION_UNREACHABLE_TYPE &&
            _self->_code == ICMP_FRAGMENTATION_NEEDED_CODE
        )
    ) {
        IcmpHeader_printInfo_v4(_self);
    }

    printf("\n");
}

void IcmpHeader_printInfo_v2(const IcmpHeader* _self)
{
    printf("ICMP Header Gateway: %d\n", _self->_rest._gateway);
}

void IcmpHeader_printInfo_v3(const IcmpHeader* _self)
{
    printf("ICMP Header Identifier: %d\n", _self->_rest._echo._id);
    printf("ICMP Header Sequence Number: %d\n", _self->_rest._echo._seqnum);
}

void IcmpHeader_printInfo_v4(const IcmpHeader* _self)
{
    printf("ICMP Next Hop MTU: %hu\n", _self->_rest._mtu._mtu);
}

void IcmpHeader_encode__(const IcmpHeader *_self, ByteBuffer* _buffer)
{
    ByteBuffer_put(_buffer, _self->_type);
    ByteBuffer_put(_buffer, _self->_code);
    ByteBuffer_putShort(_buffer, htons(_self->_checksum));

    if (
        (
            _self->_type == ICMP_DESTINATION_UNREACHABLE_TYPE ||
            _self->_type == ICMP_SOURCE_QUENCH_TYPE           ||
            _self->_type == ICMP_TIME_EXCEEEDED_TYPE
        )
    ) {
        // Check for Code in case of Destination Unreachable type
        if (
            (
                _self->_type == ICMP_DESTINATION_UNREACHABLE_TYPE &&
                _self->_code == ICMP_FRAGMENTATION_NEEDED_CODE
            )
        ) {
            ByteBuffer_putShort(_buffer, htons(_self->_rest._mtu._unused));
            ByteBuffer_putShort(_buffer, htons(_self->_rest._mtu._mtu));
        }
        else
        {
            ByteBuffer_putInt(_buffer, htonl(_self->_rest._unused));
        }
    }

    if (_self->_type == ICMP_REDIRECT_TYPE)
    {
        ByteBuffer_putInt(_buffer, htonl(_self->_rest._gateway));
    }

    if (
        (
            _self->_type == ICMP_ECHO_REPLY_TYPE          ||
            _self->_type == ICMP_ECHO_TYPE                ||
            _self->_type == ICMP_INFORMATION_REQUEST_TYPE ||
            _self->_type == ICMP_INFORMATION_REPLY_TYPE
        )
    ) {
        ByteBuffer_putShort(_buffer, htons(_self->_rest._echo._id));
        ByteBuffer_putShort(_buffer, htons(_self->_rest._echo._seqnum));
    }
}

ByteBuffer* IcmpHeader_encode(const IcmpHeader *_self)
{
    ByteBuffer *buff = ByteBuffer_new(ICMP_HEADER_MAX_SIZE);
    IcmpHeader_encode__(_self, buff);
    return buff;
}

void IcmpHeader_decode(IcmpHeader* hdr, ByteBuffer* _buffer)
{
    u_int8_t type      = ByteBuffer_get(_buffer);
    u_int8_t code      = ByteBuffer_get(_buffer);
    u_int16_t checksum = ByteBuffer_getShort(_buffer);

    IcmpHeader_setType(hdr, type);
    IcmpHeader_setCode(hdr, code);
    IcmpHeader_setChecksum(hdr, ntohs(checksum));

    if (
        (
            hdr->_type == ICMP_DESTINATION_UNREACHABLE_TYPE ||
            hdr->_type == ICMP_SOURCE_QUENCH_TYPE           ||
            hdr->_type == ICMP_TIME_EXCEEEDED_TYPE
        )
    ) {
        // Check for Code in case of Destination Unreachable type
        if (
            (
                hdr->_type == ICMP_DESTINATION_UNREACHABLE_TYPE &&
                hdr->_code == ICMP_FRAGMENTATION_NEEDED_CODE
            )
        ) {
            ByteBuffer_position(_buffer, _buffer->_position + SHORT_SIZE);
            u_int16_t mtu = ByteBuffer_getShort(_buffer);

            hdr->_rest._mtu._unused = 0x0;
            IcmpHeader_setNextHopMtu(hdr, ntohs(mtu));
        }
        else
        {
            ByteBuffer_position(_buffer, _buffer->_position + INT_SIZE);
            hdr->_rest._unused = 0x0;
        }
    }

    if (hdr->_type == ICMP_REDIRECT_TYPE)
    {
        u_int32_t gateway = ByteBuffer_getInt(_buffer);
        IcmpHeader_setGateway(hdr, ntohl(gateway));
    }

    if (
        (
            hdr->_type == ICMP_ECHO_REPLY_TYPE          ||
            hdr->_type == ICMP_ECHO_TYPE                ||
            hdr->_type == ICMP_INFORMATION_REQUEST_TYPE ||
            hdr->_type == ICMP_INFORMATION_REPLY_TYPE
        )
    ) {
        u_int16_t id = ByteBuffer_getShort(_buffer);
        u_int16_t seqnum = ByteBuffer_getShort(_buffer);

        IcmpHeader_setIdentifier(hdr, ntohs(id));
        IcmpHeader_setSequenceNumber(hdr, ntohs(seqnum));
    }
}

/* --------------------------------------------- ICMP PACKET --------------------------------------------- */

IcmpPacket* IcmpPacket_new__(const u_int8_t _type, const u_int8_t _code)
{
    return IcmpPacket_new(_type, _code, 0x0);
}

IcmpPacket* IcmpPacket_new(const u_int8_t _type, const u_int8_t _code, const size_t _size)
{
    IcmpPacket* pckt = (IcmpPacket*)malloc(sizeof(IcmpPacket));
    IcmpHeader_new(&pckt->_icmphdr, _type, _code);

    pckt->_payload = (char *)malloc(_size * sizeof(char));
    pckt->__size = _size;

    return pckt;
}

void IcmpPacket_delete(IcmpPacket* _self)
{
    free(_self->_payload);
    free(_self);
}

void IcmpPacket_fillHeader_v1(IcmpPacket* _self, const u_int16_t _checksum)
{
    IcmpHeader_setChecksum(&_self->_icmphdr, _checksum);
}

void IcmpPacket_fillHeader_v2(IcmpPacket* _self, const u_int16_t _checksum, const u_int32_t _gateway)
{
    IcmpHeader_setChecksum(&_self->_icmphdr, _checksum);
    IcmpHeader_setGateway(&_self->_icmphdr, _gateway);
}

void IcmpPacket_fillHeader_v3(IcmpPacket* _self, const u_int16_t _checksum, const u_int16_t _id, const u_int16_t _seqnum) 
{
    IcmpHeader_setChecksum(&_self->_icmphdr, _checksum);
    IcmpHeader_setIdentifier(&_self->_icmphdr, _id);
    IcmpHeader_setSequenceNumber(&_self->_icmphdr, _seqnum);
}

void IcmpPacket_fillHeader_v4(IcmpPacket *_self, const u_int16_t _checksum, const u_int16_t _data)
{
    IcmpHeader_setChecksum(&_self->_icmphdr, _checksum);
    IcmpHeader_setNextHopMtu(&_self->_icmphdr, _data);
}

void IcmpPacket_fillPayload(IcmpPacket* _self, const char* _data, const size_t _size)
{
    if (_size > ICMP_PAYLOAD_MAXIMUM_SIZE)
    {
        fprintf(stderr, "[IcmpPacket_fillPayload] Given ICMP Payload size is OOB\n");
        exit(EXIT_FAILURE);
    }

    if (_self->__size - ICMP_HEADER_MAX_SIZE > _size)
    {
        _self->_payload = (char*)realloc(_self->_payload, _size * sizeof(char));
    }

    memcpy(_self->_payload, _data, _size);
    _self->__size = _size;
}

size_t IcmpPacket_getPacketSize(const IcmpPacket* _self)
{
    return _self->__size + ICMP_HEADER_MAX_SIZE;
}

void IcmpPacket_encode__(const IcmpPacket *_self, ByteBuffer* _buffer)
{
    IcmpHeader_encode__(&_self->_icmphdr, _buffer);
    ByteBuffer_putBuffer(_buffer, _self->_payload, _self->__size);
}

ByteBuffer* IcmpPacket_encode(const IcmpPacket *_self)
{
    ByteBuffer* buffer = ByteBuffer_new(IcmpPacket_getPacketSize(_self));
    IcmpPacket_encode__(_self, buffer);
    return buffer;
}

void IcmpPacket_setHeader(IcmpPacket* _self, IcmpHeader* _hdr)
{
    // We cannot just assign the _icmphdr field of the IcmpPacket structure
    // to the new value, otherwise the memory of the previous value will
    // never be freed. Hence, we need to copy the memory content of 
    // the input IcmpHeader and then free the input one.

    // First we can copy first 4 bytes for Type, Code and Checksum
    memcpy(&_self->_icmphdr, _hdr, 4);
    
    // The remaining 4 bytes depends on the ICMP Header Type Value
    if (_self->_icmphdr._type == ICMP_REDIRECT_TYPE)
    {
        memcpy(&_self->_icmphdr._rest._gateway, &_hdr->_rest._gateway, 4);
    }

    if (
        (
            _self->_icmphdr._type == ICMP_ECHO_REPLY_TYPE          ||
            _self->_icmphdr._type == ICMP_ECHO_TYPE                ||
            _self->_icmphdr._type == ICMP_INFORMATION_REQUEST_TYPE ||
            _self->_icmphdr._type == ICMP_INFORMATION_REPLY_TYPE
        )
    ) {
        memcpy(&_self->_icmphdr._rest._echo, &_hdr->_rest._echo, 4);
    }

    if (
        (
            _hdr->_type == ICMP_DESTINATION_UNREACHABLE_TYPE &&
            _hdr->_code == ICMP_FRAGMENTATION_NEEDED_CODE
        )
    ) {
        memcpy(&_self->_icmphdr._rest._mtu, &_hdr->_rest._mtu, 4);
    }
}

IcmpPacket* IcmpPacket_decode(ByteBuffer *_buffer)
{
    IcmpHeader hdr;
    IcmpHeader_decode(&hdr, _buffer);

    size_t payload_size = _buffer->_size - _buffer->_position;

    // Check that the final payload size is grater than 0
    if (payload_size < 0) payload_size = 0;

    IcmpPacket* pckt = IcmpPacket_new(hdr._type, hdr._code, payload_size);
    IcmpPacket_setHeader(pckt, &hdr);

    char payload[payload_size];
    ByteBuffer_getBuffer(_buffer, payload, payload_size);
    IcmpPacket_fillPayload(pckt, payload, payload_size);

    return pckt;
}

/* --------------------------------------------- UDP HEADER --------------------------------------------- */

void UdpHeader_setSourcePort(UdpHeader* _self, const u_int16_t _srcport)
{
    _self->_srcport = _srcport;
}

void UdpHeader_setDestinationPort(UdpHeader* _self, const u_int16_t _dstport)
{
    _self->_dstport = _dstport;
}

void UdpHeader_setLength(UdpHeader* _self, const u_int16_t _length )
{
    _self->_length = _length;
}

void UdpHeader_setChecksum(UdpHeader* _self, const u_int16_t _checksum)
{
    _self->_checksum = _checksum;
}

void UdpHeader_printInfo(const UdpHeader* _self)
{
    printf("[*] Printing Header Information Fields\n");
    printf("Source Port: %hu\n", _self->_srcport);
    printf("Destination Port: %hu\n", _self->_dstport);
    printf("Length: %hu\n", _self->_length);
    printf("Checksum: %hu\n", _self->_checksum);
    printf("\n");
}

void UdpHeader_encode(const UdpHeader* _self, ByteBuffer* _buffer)
{
    ByteBuffer_putShort(_buffer, htons(_self->_srcport));
    ByteBuffer_putShort(_buffer, htons(_self->_dstport));
    ByteBuffer_putShort(_buffer, htons(_self->_length));
    ByteBuffer_putShort(_buffer, htons(_self->_checksum));
}

ByteBuffer* UdpHeader_encode__(const UdpHeader* _self)
{
    ByteBuffer* buff = ByteBuffer_new(_self->_length);
    UdpHeader_encode(_self, buff);
    return buff;
}

void UdpHeader_decode(UdpHeader *_self, ByteBuffer* _buffer)
{
    u_int16_t srcport  = ByteBuffer_getShort(_buffer);
    u_int16_t dstport  = ByteBuffer_getShort(_buffer);
    u_int16_t length   = ByteBuffer_getShort(_buffer);
    u_int16_t checksum = ByteBuffer_getShort(_buffer);

    UdpHeader_setSourcePort(_self, ntohs(srcport));
    UdpHeader_setDestinationPort(_self, ntohs(dstport));
    UdpHeader_setLength(_self, ntohs(length));
    UdpHeader_setChecksum(_self, ntohs(checksum));
}

/* --------------------------------------------- UDP PACKET --------------------------------------------- */

UdpPacket* UdpPacket_new()
{
    return UdpPacket_new_s(UDP_PAYLOAD_MAX_SIZE);
}

UdpPacket* UdpPacket_new_s(const size_t _size)
{
    char* payload = (char*)malloc(_size * sizeof(char));

    UdpPacket* pckt = (UdpPacket*)malloc(sizeof(UdpPacket));
    pckt->_payload = payload;

    UdpHeader_setLength(&pckt->_hdr, _size + UDP_HEADER_SIZE);

    return pckt;
}

void UdpPacket_delete(UdpPacket* _self)
{
    free(_self->_payload);
    free(_self);
}

void UdpPacket_fillHeader(
    UdpPacket*       _self,   const u_int16_t _srcport, const u_int16_t _dstport,
    const u_int16_t  _length, const u_int16_t _checksum
) {
    UdpHeader_setSourcePort(&_self->_hdr, _srcport);
    UdpHeader_setDestinationPort(&_self->_hdr, _dstport);
    UdpHeader_setLength(&_self->_hdr, _length);
    UdpHeader_setChecksum(&_self->_hdr, _checksum);
}

void UdpPacket_setHeader(UdpPacket* _self, UdpHeader* _hdr)
{
    memcpy(&_self->_hdr, _hdr, UDP_HEADER_SIZE);
}

void UdpPacket_fillPayload(UdpPacket* _self, const char* _data, const size_t _size)
{
    if (_self->_hdr._length - UDP_HEADER_SIZE > _size)
    {
        _self->_payload = (char*)realloc(_self->_payload, _size * sizeof(char));
    }

    memcpy(_self->_payload, _data, _size);
    UdpHeader_setLength(&_self->_hdr, _size + UDP_HEADER_SIZE);
}

size_t UdpPacket_getPayloadSize(const UdpPacket* _self)
{
    return _self->_hdr._length - UDP_HEADER_SIZE;
}

size_t UdpPacket_getPacketSize(const UdpPacket* _self)
{
    return _self->_hdr._length;
}

ByteBuffer* UdpPacket_encode(const UdpPacket* _self)
{
    ByteBuffer* bbuff = ByteBuffer_new(_self->_hdr._length);
    UdpPacket_encode__(_self, bbuff);
    return bbuff;
}

void UdpPacket_encode__(const UdpPacket* _self, ByteBuffer* _buffer)
{
    UdpHeader_encode(&_self->_hdr, _buffer);
    ByteBuffer_putBuffer(_buffer, _self->_payload, (size_t)UdpPacket_getPayloadSize(_self));
}

UdpPacket* UdpPacket_decode(ByteBuffer* _buffer)
{
    UdpHeader hdr;
    UdpHeader_decode(&hdr, _buffer);

    size_t payload_size = hdr._length - UDP_HEADER_SIZE;
    UdpPacket* pckt = UdpPacket_new_v2(payload_size);

    char payload[payload_size];
    ByteBuffer_getBuffer(_buffer, payload, payload_size);

    UdpPacket_setHeader(pckt, &hdr);
    UdpPacket_fillPayload(pckt, payload, payload_size);

    return pckt;
}

/* --------------------------------------------- IP HEADER --------------------------------------------- */

void IpHeader_setVersion(IpHeader* _self, const u_int8_t _version)
{
    // Add 5 for IHL (this ensure the IP header always be 20 bytes)
    _self->_version = (_version << 4) + 5;
}

void IpHeader_setDifferentiatedServiceField(IpHeader* _self, const u_int8_t _dsf)
{
    _self->_dsf = _dsf;
}

void IpHeader_setTotalLength(IpHeader* _self, const u_int16_t _total_length)
{
    _self->_tlength = _total_length;
}

void IpHeader_setIdentfication(IpHeader* _self, const u_int16_t _identification)
{
    _self->_id = _identification;
}

void IpHeader_setFlagOffField(IpHeader* _self, const u_int16_t _flagoff)
{
    _self->_flag_off = _flagoff;
}

void IpHeader_setTimeToLive(IpHeader* _self, const u_int8_t _time_to_live)
{
    _self->_ttl = _time_to_live;
}

void IpHeader_setProtocol(IpHeader* _self, const u_int8_t _protocol)
{
    _self->_protocol = _protocol;
}

void IpHeader_setHeaderChecksum(IpHeader* _self, const u_int16_t _checksum)
{
    _self->_hdr_chksum = _checksum;
}

void IpHeader_setSourceAddress(IpHeader* _self, const u_int32_t _srcaddress)
{
    _self->_srcaddr = _srcaddress;
}

void IpHeader_setDestinationAddress(IpHeader* _self, const u_int32_t _dstaddress)
{
    _self->_dstaddr = _dstaddress;
}

u_int16_t computeFlagOff(int _x, int _d, int _m, int _offset)
{
    return (IP_HEADER_FLAG(_x, _d, _m) << 13) + _offset;
}

u_int8_t computeDifferentiatedServiceField(const int _dscp, const int _ecn)
{
    return (_dscp << 2) + _ecn;
}

u_int8_t IpHeader_getVersion(const IpHeader* _self)
{
    return (_self->_version >> 4);
}

u_int8_t IpHeader_getInternetHeaderLength(const IpHeader* _self)
{
    return _self->_version & ~(IpHeader_getVersion(_self) << 4);
}

u_int8_t IpHeader_getDSCP(const IpHeader* _self)
{
    return (_self->_dsf >> 2);
}

u_int8_t IpHeader_getECN(const IpHeader* _self)
{
    return _self->_dsf & ~(IpHeader_getDSCP(_self) << 2);
}

u_int8_t IpHeader_getFlags(const IpHeader* _self)
{
    return (_self->_flag_off >> 13);
}

u_int8_t IpHeader_getFragmentOffset(const IpHeader* _self)
{
    return _self->_flag_off & ~(IpHeader_getFlags(_self) << 13);
}

void convertFlagToBin(const u_int8_t _flags, char *_out)
{
    int _x = ((_flags >> 2) & 1);
    int _d = ((_flags >> 1) & 1);
    int _m = (_flags & 1);

    sprintf(_out, "%d%d%d", _x, _d, _m);
}

void IpHeader_printInfo(const IpHeader* _self)
{
    char flag[4];
    convertFlagToBin(IpHeader_getFlags(_self), flag);

    char srcaddr[INET_ADDRSTRLEN], dstaddr[INET_ADDRSTRLEN];
    addressNumberToString(_self->_srcaddr, srcaddr, false);
    addressNumberToString(_self->_dstaddr, dstaddr, false);

    printf("[*] Printing Header Information Fields\n");
    printf("Ip Version: %d\n", IpHeader_getVersion(_self));
    printf("Internet Header Length: %d\n", IpHeader_getInternetHeaderLength(_self));
    printf("Distributed Service Code Point: %d\n", IpHeader_getDSCP(_self));
    printf("Explicit Congestion Network: %d\n", IpHeader_getECN(_self));
    printf("Total Length: %d\n", _self->_tlength);
    printf("Identification: %d\n", _self->_id);
    printf("IP Flags (X D M): %s\n", flag);
    printf("Fragment Offset: %d\n", IpHeader_getFragmentOffset(_self));
    printf("Time To Live: %d\n", _self->_ttl);
    printf("IP Protocol: %d\n", _self->_protocol);
    printf("Header Checksum: %d\n", _self->_hdr_chksum);
    printf("Source Address: %s\n", srcaddr);
    printf("Destination Address: %s\n", dstaddr);
    printf("\n");
}

void IpHeader_encode__(const IpHeader* _self, ByteBuffer* _buffer)
{
    ByteBuffer_put(_buffer, _self->_version);
    ByteBuffer_put(_buffer, _self->_dsf);
    ByteBuffer_putShort(_buffer, htons(_self->_tlength));
    ByteBuffer_putShort(_buffer, htons(_self->_id));
    ByteBuffer_putShort(_buffer, htons(_self->_flag_off));
    ByteBuffer_put(_buffer, _self->_ttl);
    ByteBuffer_put(_buffer, _self->_protocol);
    ByteBuffer_putShort(_buffer, htons(_self->_hdr_chksum));
    ByteBuffer_putInt(_buffer, htonl(_self->_srcaddr));
    ByteBuffer_putInt(_buffer, htonl(_self->_dstaddr));
}

ByteBuffer* IpHeader_encode(const IpHeader* _self)
{
    ByteBuffer* buff = ByteBuffer_new((size_t) _self->_tlength);
    IpHeader_encode__(_self, buff);
    return buff;
}

void IpHeader_decode(IpHeader* _self, ByteBuffer* _buffer)
{
    u_int8_t  version  = ByteBuffer_get(_buffer) >> 4;
    u_int8_t  dsf      = ByteBuffer_get(_buffer);
    u_int16_t tlen     = ByteBuffer_getShort(_buffer);
    u_int16_t id       = ByteBuffer_getShort(_buffer);
    u_int16_t flagoff  = ByteBuffer_getShort(_buffer);
    u_int8_t  ttl      = ByteBuffer_get(_buffer);
    u_int8_t  protocol = ByteBuffer_get(_buffer);
    u_int16_t checksum = ByteBuffer_getShort(_buffer);
    u_int32_t srcaddr  = ByteBuffer_getInt(_buffer);
    u_int32_t dstaddr  = ByteBuffer_getInt(_buffer);

    IpHeader_setVersion(_self, version);
    IpHeader_setDifferentiatedServiceField(_self, dsf);
    IpHeader_setTotalLength(_self, ntohs(tlen));
    IpHeader_setIdentfication(_self, ntohs(id));
    IpHeader_setFlagOffField(_self, ntohs(flagoff));
    IpHeader_setTimeToLive(_self, ttl);
    IpHeader_setProtocol(_self, protocol);
    IpHeader_setHeaderChecksum(_self, ntohs(checksum));
    IpHeader_setSourceAddress(_self, ntohl(srcaddr));
    IpHeader_setDestinationAddress(_self, ntohl(dstaddr));
}

/* --------------------------------------------- IP PACKET --------------------------------------------- */

IpPacket* IpPacket_new()
{
    IpPacket* pckt = (IpPacket *)malloc(sizeof(IpPacket));
    return pckt;
}

IpPacket* IpPacket_newIcmp(const u_int8_t _type, const u_int8_t _code, const size_t _size)
{
    size_t size = _size > 0 ? _size : 0;

    IpPacket* pckt = (IpPacket *)malloc(sizeof(IpPacket));
    pckt->_payload._icmp = IcmpPacket_new(_type, _code, _size);
    IpHeader_setTotalLength(&pckt->_iphdr, IP_HEADER_SIZE + ICMP_HEADER_MAX_SIZE + size);

    return pckt;
}

IpPacket* IpPacket_newUdp(const size_t _size)
{
    size_t size = _size > 0 ? _size : 0;

    IpPacket* pckt = (IpPacket *)malloc(sizeof(IpPacket));
    pckt->_payload._udp = UdpPacket_new_s(_size);
    IpHeader_setTotalLength(&pckt->_iphdr, IP_HEADER_SIZE + UDP_HEADER_SIZE + size);

    return pckt;
}

void IpPacket_delete(IpPacket* _self)
{
    switch (_self->_iphdr._protocol)
    {
        case IP_HEADER_ICMP_PROTOCOL_CODE:
            IcmpPacket_delete(_self->_payload._icmp);
            break;
        
        case IP_HEADER_UDP_PROTOCOL_CODE:
            UdpPacket_delete(_self->_payload._udp);
            break;

        default:
            break;
    }

    free(_self);
}

void IpPacket_setHeader(IpPacket *_self, IpHeader *_iphdr)
{
    memcpy(&_self->_iphdr, _iphdr, IP_HEADER_SIZE);
}

void IpPacket_fillHeader(
    IpPacket*       _self,     const u_int8_t  _version, const u_int8_t  _dsf, const u_int16_t _tlen,
    const u_int16_t _id,       const u_int16_t _flagoff, const u_int8_t  _ttl, const u_int8_t  _protocol, 
    const u_int16_t _checksum, const u_int32_t _srcaddr, const u_int32_t _dstaddr
) {
    IpHeader_setVersion(&_self->_iphdr, _version);
    IpHeader_setDifferentiatedServiceField(&_self->_iphdr, _dsf);
    IpHeader_setTotalLength(&_self->_iphdr, _tlen);
    IpHeader_setIdentfication(&_self->_iphdr, _id);
    IpHeader_setFlagOffField(&_self->_iphdr, _flagoff);
    IpHeader_setTimeToLive(&_self->_iphdr, _ttl);
    IpHeader_setProtocol(&_self->_iphdr, _protocol);
    IpHeader_setHeaderChecksum(&_self->_iphdr, _checksum);
    IpHeader_setSourceAddress(&_self->_iphdr, _srcaddr);
    IpHeader_setDestinationAddress(&_self->_iphdr, _dstaddr);
}

u_int16_t IpPacket_getPayloadSize(const IpPacket *_self)
{
    return _self->_iphdr._tlength - IP_HEADER_SIZE;
}

IcmpPacket* IpPacket_getIcmpPacket(const IpPacket *_self)
{
    return _self->_payload._icmp;
}

ByteBuffer* IpPacket_encode(const IpPacket* _self)
{
    ByteBuffer* buff = ByteBuffer_new((size_t) _self->_iphdr._tlength);
    IpHeader_encode__(&_self->_iphdr, buff);

    switch (_self->_iphdr._protocol)
    {
        case IP_HEADER_ICMP_PROTOCOL_CODE:
            IcmpPacket_encode__(_self->_payload._icmp, buff);
            break;
        
        case IP_HEADER_UDP_PROTOCOL_CODE:
            UdpPacket_encode__(_self->_payload._udp, buff);
            break;

        default:
            break;
    }

    return buff;
}

IpPacket* IpPacket_decodeIcmp(ByteBuffer* _buffer)
{
    IpHeader hdr; IpHeader_decode(&hdr, _buffer);
    IcmpPacket* icmppckt = IcmpPacket_decode(_buffer);

    u_int8_t type = icmppckt->_icmphdr._type;
    u_int8_t code = icmppckt->_icmphdr._code;
    size_t   size = icmppckt->__size;

    IpPacket* pckt = IpPacket_newIcmp(type, code, size);
    IpPacket_setHeader(pckt, &hdr);
    IpPacket_wrapIcmp(pckt, icmppckt);

    // Delete the ICMP Packet, we do not need this
    IcmpPacket_delete(icmppckt);

    return pckt;
}

void IpPacket_wrapIcmp(IpPacket* _self, IcmpPacket* _icmppckt)
{
    memcpy(_self->_payload._icmp, _icmppckt, IcmpPacket_getPacketSize(_icmppckt));
    IpHeader_setTotalLength(&_self->_iphdr, IP_HEADER_SIZE + IcmpPacket_getPacketSize(_icmppckt));
}

void IpPacket_wrapUdp(IpPacket* _self, UdpPacket* _udppckt)
{
   memcpy(_self->_payload._udp, _udppckt, UdpPacket_getPacketSize(_udppckt));
   IpHeader_setTotalLength(&_self->_iphdr, IP_HEADER_SIZE + UdpPacket_getPacketSize(_udppckt));
}