#include "neto.h"

/* --------------------------------------------- ICMP HEADER --------------------------------------------- */

IcmpHeader* IcmpHeader_new(u_int8_t _type)
{
    IcmpHeader* hdr = (IcmpHeader*)malloc(sizeof(IcmpHeader));
    union h_data_t* rest = (union h_data_t*)malloc(sizeof(union h_data_t));
    hdr->_rest = rest;

    switch (_type)
    {
        case ICMP_DESTINATION_UNREACHABLE_TYPE:
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
            fprintf(stderr, "ICMP Header Type %d do not exists!", _type);
            exit(EXIT_FAILURE);
    }

    IcmpHeader_setType(hdr, _type);

    return hdr;
}

void IcmpHeader_delete(IcmpHeader* _self)
{
    free(_self->_rest);
    free(_self);
}

void __IcmpHeader_createHeader_v1(IcmpHeader* _self)
{
    // The first version consists on the header with Type, code
    // checksum and the next 32 bit left unused.
    _self->_rest->_unused = 0;
}

void __IcmpHeader_createHeader_v2(IcmpHeader* _self)
{
    // The second version of the header consists of Type, code
    // checksum and 32 bit reserved to the gateway address.
    _self->_rest->_gateway = 0;
}

void __IcmpHeader_createHeader_v3(IcmpHeader* _self)
{
    // The second version of the header consists of Type, code
    // checksum, identification and sequence number.
    struct h_echo_t echo_s;
    echo_s._id = 0;
    echo_s._seqnum = 0;

    _self->_rest->_echo = echo_s;
}

void IcmpHeader_setType(IcmpHeader* _self, u_int8_t _type)
{
    _self->_type = _type;
}

void IcmpHeader_setCode(IcmpHeader* _self, u_int8_t _code)
{
    _self->_code = _code;
}

void IcmpHeader_setChecksum(IcmpHeader* _self, u_int16_t _checksum)
{
    _self->_checksum = _checksum;
}

void IcmpHeader_setGateway(IcmpHeader* _self, u_int32_t _gateway)
{
    if (_self->_type != ICMP_REDIRECT_TYPE)
    {
        fprintf(stderr, "(Invalid Operation) To set gateway address ");
        fprintf(stderr, "an ICMP message of Type %d is required. ", ICMP_REDIRECT_TYPE);
        fprintf(stderr, "Current Type is: %d.\n", _self->_type);
        exit(EXIT_FAILURE);
    }

    _self->_rest->_gateway = _gateway;
}

void IcmpHeader_setIdentifier(IcmpHeader* _self, u_int16_t _id)
{
    if (
        (
            _self->_type != ICMP_ECHO_REPLY_TYPE          &&
            _self->_type != ICMP_ECHO_TYPE                &&
            _self->_type != ICMP_INFORMATION_REQUEST_TYPE &&
            _self->_type != ICMP_INFORMATION_REPLY_TYPE
        )
    ) {
        fprintf(stderr, "(Invalid Operation) To set Identifier Header field ");

        fprintf(
            stderr, "an ICMP message of Type %d/%d/%d/%d is required. ", 
            ICMP_ECHO_REPLY_TYPE, ICMP_ECHO_TYPE, ICMP_INFORMATION_REQUEST_TYPE,
            ICMP_INFORMATION_REPLY_TYPE
        );

        fprintf(stderr, "Current Type is: %d.\n", _self->_type);
        exit(EXIT_FAILURE);
    }

    _self->_rest->_echo._id = _id;
}

void IcmpHeader_setSequenceNumber(IcmpHeader* _self, u_int16_t _seqnum)
{
    if (
        (
            _self->_type != ICMP_ECHO_REPLY_TYPE          &&
            _self->_type != ICMP_ECHO_TYPE                &&
            _self->_type != ICMP_INFORMATION_REQUEST_TYPE &&
            _self->_type != ICMP_INFORMATION_REPLY_TYPE
        )
    ) {
        fprintf(stderr, "(Invalid Operation) To set Sequence Number Header field ");

        fprintf(
            stderr, "an ICMP message of Type %d/%d/%d/%d is required. ", 
            ICMP_ECHO_REPLY_TYPE, ICMP_ECHO_TYPE, ICMP_INFORMATION_REQUEST_TYPE,
            ICMP_INFORMATION_REPLY_TYPE
        );

        fprintf(stderr, "Current Type is: %d.\n", _self->_type);
        exit(EXIT_FAILURE);
    }

    _self->_rest->_echo._seqnum = _seqnum;
}

void IcmpHeader_printInfo(IcmpHeader* _self)
{
    printf("[*] Printing Information of the ICMP Header\n");
    printf("ICMP Message Type: %d\n", _self->_type);
    printf("ICMP Message Code: %d\n", _self->_code);
    printf("ICMP Header Checksum: %d\n", _self->_checksum);

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

    printf("\n");
}

void IcmpHeader_printInfo_v2(IcmpHeader* _self)
{
    printf("ICMP Header Gateway: %d\n", _self->_rest->_gateway);
}

void IcmpHeader_printInfo_v3(IcmpHeader* _self)
{
    printf("ICMP Header Identifier: %d\n", _self->_rest->_echo._id);
    printf("ICMP Header Sequence Number: %d\n", _self->_rest->_echo._seqnum);
}

u_int16_t computeIcmpChecksum(char* _buff, size_t _size)
{
    u_int16_t x1, x2, x3, checksum, sum;
    x1 = (*(_buff) << 8) + *(_buff + 1);

    memcpy(&x2, _buff + 4, 2);
    memcpy(&x3, _buff + 6, 2);

    sum = x1 + htons(x2) + htons(x3);
    checksum = 0xFFFF - sum;

    return checksum;
}

void IcmpHeader_encode(IcmpHeader *_self, ByteBuffer* _buffer)
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
        ByteBuffer_putInt(_buffer, htonl(_self->_rest->_unused));
    }

    if (_self->_type == ICMP_REDIRECT_TYPE)
    {
        ByteBuffer_putInt(_buffer, htonl(_self->_rest->_gateway));
    }

    if (
        (
            _self->_type == ICMP_ECHO_REPLY_TYPE          ||
            _self->_type == ICMP_ECHO_TYPE                ||
            _self->_type == ICMP_INFORMATION_REQUEST_TYPE ||
            _self->_type == ICMP_INFORMATION_REPLY_TYPE
        )
    ) {
        ByteBuffer_putShort(_buffer, htons(_self->_rest->_echo._id));
        ByteBuffer_putShort(_buffer, htons(_self->_rest->_echo._seqnum));
    }
}

ByteBuffer* IcmpHeader_encode_v2(IcmpHeader *_self)
{
    ByteBuffer *buff = ByteBuffer_new(ICMP_HEADER_MAX_SIZE);
    IcmpHeader_encode(_self, buff);
    return buff;
}

IcmpHeader* IcmpHeader_decode(ByteBuffer* _buffer)
{
    u_int8_t type      = ByteBuffer_get(_buffer);
    u_int8_t code      = ByteBuffer_get(_buffer);
    u_int16_t checksum = ByteBuffer_getShort(_buffer);

    IcmpHeader* hdr = IcmpHeader_new(type);
    IcmpHeader_setCode(hdr, code);
    IcmpHeader_setChecksum(hdr, ntohs(checksum));

    if (
        (
            hdr->_type == ICMP_DESTINATION_UNREACHABLE_TYPE ||
            hdr->_type == ICMP_SOURCE_QUENCH_TYPE           ||
            hdr->_type == ICMP_TIME_EXCEEEDED_TYPE
        )
    ) {
        ByteBuffer_position(_buffer, _buffer->_position + INT_SIZE);
        hdr->_rest->_unused = 0x0;
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

    return hdr;
}

/* --------------------------------------------- ICMP PACKET --------------------------------------------- */

IcmpPacket* IcmpPacket_new(u_int8_t _type)
{
    return IcmpPacket_new_v2(_type, ICMP_PAYLOAD_MAXIMUM_SIZE);
}

IcmpPacket* IcmpPacket_new_v2(u_int8_t _type, size_t _size)
{
    // Check if the given size is out of bound
    if (_size > ICMP_PAYLOAD_MAXIMUM_SIZE)
    {
        fprintf(stderr, "Given ICMP Payload size is OOB\n");
        exit(EXIT_FAILURE);
    }

    IcmpPacket* pckt = (IcmpPacket*)malloc(sizeof(IcmpPacket));
    IcmpHeader* hdr = IcmpHeader_new(_type);
    
    pckt->_icmphdr = hdr;
    pckt->_payload = (char *)malloc(_size * sizeof(char));
    pckt->__size = _size;
}

void IcmpPacket_delete(IcmpPacket* _self)
{
    IcmpHeader_delete(_self->_icmphdr);
    free(_self->_payload);
    free(_self);
}

void IcmpPacket_fillHeader_v1(IcmpPacket* _self, u_int8_t _code)
{
    IcmpHeader_setCode(_self->_icmphdr, _code);
    IcmpHeader_setChecksum(_self->_icmphdr, (u_int16_t)0);
}

void IcmpPacket_fillHeader_v2(IcmpPacket* _self, u_int8_t _code, u_int32_t _gateway)
{
    IcmpHeader_setCode(_self->_icmphdr, _code);
    IcmpHeader_setChecksum(_self->_icmphdr, (u_int16_t)0);
    IcmpHeader_setGateway(_self->_icmphdr, _gateway);
}

void IcmpPacket_fillHeader_v3(
    IcmpPacket* _self, u_int8_t _code, u_int16_t _id, u_int16_t _seqnum
) {
    IcmpHeader_setCode(_self->_icmphdr, _code);
    IcmpHeader_setChecksum(_self->_icmphdr, (u_int16_t)0);
    IcmpHeader_setIdentifier(_self->_icmphdr, _id);
    IcmpHeader_setSequenceNumber(_self->_icmphdr, _seqnum);
}

void IcmpPacket_fillPayload(IcmpPacket* _self, char* _data, size_t _size)
{
    if (_size > ICMP_PAYLOAD_MAXIMUM_SIZE)
    {
        fprintf(stderr, "Given ICMP Payload size is OOB\n");
        exit(EXIT_FAILURE);
    }

    memcpy(_self->_payload, _data, _size);
    _self->__size = _size;
}

size_t IcmpPacket_getPacketSize(IcmpPacket* _self)
{
    return _self->__size + ICMP_HEADER_MAX_SIZE;
}

ByteBuffer* IcmpPacket_encode(IcmpPacket *_self)
{
    ByteBuffer* buffer = ByteBuffer_new(IcmpPacket_getPacketSize(_self));
    IcmpHeader_encode(_self->_icmphdr, buffer);
    ByteBuffer_putBuffer(buffer, _self->_payload, _self->__size);
    return buffer;
}

void IcmpPacket_setHeader(IcmpPacket* _self, IcmpHeader* _hdr)
{
    // We cannot just assign the _icmphdr field of the IcmpPacket structure
    // to the new value, otherwise the memory of the previous value will
    // never be freed. Hence, we need to copy the memory content of 
    // the input IcmpHeader and then free the input one.

    // First we can copy first 4 bytes for Type, Code and Checksum
    memcpy(_self->_icmphdr, _hdr, 4);
    
    // The remaining 4 bytes depends on the ICMP Header Type Value
    if (_self->_icmphdr->_type == ICMP_REDIRECT_TYPE)
    {
        memcpy(&_self->_icmphdr->_rest->_gateway, &_hdr->_rest->_gateway, 4);
    }

    if (
        (
            _self->_icmphdr->_type == ICMP_ECHO_REPLY_TYPE          ||
            _self->_icmphdr->_type == ICMP_ECHO_TYPE                ||
            _self->_icmphdr->_type == ICMP_INFORMATION_REQUEST_TYPE ||
            _self->_icmphdr->_type == ICMP_INFORMATION_REPLY_TYPE
        )
    ) {
        memcpy(&_self->_icmphdr->_rest->_echo, &_hdr->_rest->_echo, 4);
    }

    IcmpHeader_delete(_hdr);
}

IcmpPacket* IcmpPacket_decode(ByteBuffer *_buffer)
{
    IcmpHeader* hdr  = IcmpHeader_decode(_buffer);
    IcmpPacket* pckt = IcmpPacket_new(hdr->_type);
    
    IcmpPacket_setHeader(pckt, hdr);

    size_t payload_size = _buffer->_size - _buffer->_position;

    // Check that the final payload size is grater than 0
    if (payload_size < 0) payload_size = 0;

    char *payload = ByteBuffer_getBuffer(_buffer, payload_size);
    IcmpPacket_fillPayload(pckt, payload, payload_size);

    // Since fillPayload performs memcpy, we can free this memory
    free(payload);

    return pckt;
}

/* --------------------------------------------- UDP HEADER --------------------------------------------- */

UdpHeader* UdpHeader_new()
{
    UdpHeader* hdr = (UdpHeader*)malloc(sizeof(UdpHeader));
    return hdr;
}

void UdpHeader_delete(UdpHeader* _self)
{
    free(_self);
}

void UdpHeader_setSourcePort(UdpHeader* _self, u_int16_t _srcport)
{
    _self->_srcport = _srcport;
}

void UdpHeader_setDestinationPort(UdpHeader* _self, u_int16_t _dstport)
{
    _self->_dstport = _dstport;
}

void UdpHeader_setLength(UdpHeader* _self, u_int16_t _length )
{
    _self->_length = _length;
}

void UdpHeader_setChecksum(UdpHeader* _self, u_int16_t _checksum)
{
    _self->_checksum = _checksum;
}

void UdpHeader_encode(UdpHeader* _self, ByteBuffer* _buffer)
{
    ByteBuffer_putShort(_buffer, htons(_self->_srcport));
    ByteBuffer_putShort(_buffer, htons(_self->_dstport));
    ByteBuffer_putShort(_buffer, htons(_self->_length));
    ByteBuffer_putShort(_buffer, htons(_self->_checksum));
}

ByteBuffer* UdpHeader_encode_v2(UdpHeader* _self)
{
    ByteBuffer* buff = ByteBuffer_new(UDP_HEADER_SIZE);
    UdpHeader_encode(_self, buff);
    return buff;
}

/* --------------------------------------------- UDP PACKET --------------------------------------------- */

UdpPacket* UdpPacket_new()
{
    size_t payload_size = sizeof(char) * UDP_PAYLOAD_MAX_SIZE;
    char* payload = (char*)malloc(payload_size);

    UdpHeader* hdr = UdpHeader_new();
    UdpHeader_setLength(hdr, UDP_PAYLOAD_MAX_SIZE + UDP_HEADER_SIZE);

    UdpPacket* pckt = (UdpPacket*)malloc(sizeof(UdpPacket));
    pckt->_hdr = hdr;
    pckt->_payload = payload;

    return pckt;
}

void UdpPacket_delete(UdpPacket* _self)
{
    UdpHeader_delete(_self->_hdr);
    free(_self->_payload);
    free(_self);
}

void UdpPacket_fillHeader(
    UdpPacket* _self,   u_int16_t _srcport, u_int16_t _dstport,
    u_int16_t  _length, u_int16_t _checksum
) {
    UdpHeader_setSourcePort(_self->_hdr, _srcport);
    UdpHeader_setDestinationPort(_self->_hdr, _dstport);
    UdpHeader_setLength(_self->_hdr, _length);
    UdpHeader_setChecksum(_self->_hdr, _checksum);
}

void UdpPacket_fillPayload(UdpPacket* _self, char* _data, size_t _size)
{
    if (_size > UDP_PAYLOAD_MAX_SIZE)
    {
        fprintf(stderr, "Given UDP Payload size %ld > %d", _size, UDP_PAYLOAD_MAX_SIZE);
        exit(EXIT_FAILURE);
    }

    if (_self->_hdr->_length - UDP_HEADER_SIZE > _size)
    {
        _self->_payload = (char*)realloc(_self->_payload, _size * sizeof(char));
    }

    memcpy(_self->_payload, _data, _size);
    UdpHeader_setLength(_self->_hdr, _size + UDP_HEADER_SIZE);
}

size_t UdpPacket_getPayloadSize(UdpPacket* _self)
{
    return _self->_hdr->_length - UDP_HEADER_SIZE;
}

ByteBuffer* UdpPacket_encode(UdpPacket* _self)
{
    ByteBuffer* buff = UdpHeader_encode_v2(_self->_hdr);
    ByteBuffer_putBuffer(buff, _self->_payload, (size_t)UdpPacket_getPayloadSize(_self));
    return buff;
}

/* --------------------------------------------- IP HEADER --------------------------------------------- */

IpHeader* IpHeader_new()
{
    IpHeader *_iphdr = malloc(sizeof(IpHeader));
    return _iphdr;
}

void IpHeader_delete(IpHeader* _self)
{
    free(_self);
}

void IpHeader_setVersion(IpHeader* _self, u_int8_t _version)
{
    // Add 5 for IHL (this ensure the IP header always be 20 bytes)
    _self->_version = (_version << 4) + 5;
}

void IpHeader_setDifferentiatedServiceField(IpHeader* _self, u_int8_t _dsf)
{
    _self->_dsf = _dsf;
}

void IpHeader_setTotalLength(IpHeader* _self, u_int16_t _total_length)
{
    _self->_tlength = _total_length;
}

void IpHeader_setIdentfication(IpHeader* _self, u_int16_t _identification)
{
    _self->_id = _identification;
}

void IpHeader_setFlagOffField(IpHeader* _self, u_int16_t _flagoff)
{
    _self->_flag_off = _flagoff;
}

void IpHeader_setTimeToLive(IpHeader* _self, u_int8_t _time_to_live)
{
    _self->_ttl = _time_to_live;
}

void IpHeader_setProtocol(IpHeader* _self, u_int8_t _protocol)
{
    _self->_protocol = _protocol;
}

void IpHeader_setHeaderChecksum(IpHeader* _self, u_int16_t _checksum)
{
    _self->_hdr_chksum = _checksum;
}

void IpHeader_setSourceAddress(IpHeader* _self, u_int32_t _srcaddress)
{
    _self->_srcaddr = _srcaddress;
}

void IpHeader_setDestinationAddress(IpHeader* _self, u_int32_t _dstaddress)
{
    _self->_dstaddr = _dstaddress;
}

u_int16_t computeFlagOff(int _x, int _d, int _m, int _offset)
{
    return (IP_HEADER_FLAG(_x, _d, _m) << 13) + _offset;
}

u_int8_t computeDifferentiatedServiceField(int _dscp, int _ecn)
{
    return (_dscp << 2) + _ecn;
}

u_int8_t IpHeader_getVersion(IpHeader* _self)
{
    return (_self->_version >> 4);
}

u_int8_t IpHeader_getInternetHeaderLength(IpHeader* _self)
{
    return _self->_version & ~(IpHeader_getVersion(_self) << 4);
}

u_int8_t IpHeader_getDSCP(IpHeader* _self)
{
    return (_self->_dsf >> 2);
}

u_int8_t IpHeader_getECN(IpHeader* _self)
{
    return _self->_dsf & ~(IpHeader_getDSCP(_self) << 2);
}

u_int8_t IpHeader_getFlags(IpHeader* _self)
{
    return (_self->_flag_off >> 13);
}

u_int8_t IpHeader_getFragmentOffset(IpHeader* _self)
{
    return _self->_flag_off & ~(IpHeader_getFlags(_self) << 13);
}

char* convertFlagToBin(u_int8_t _flags)
{
    int _x = ((_flags >> 2) & 1);
    int _d = ((_flags >> 1) & 1);
    int _m = (_flags & 1);

    char* flags = (char *)malloc(3 * sizeof(char) + 1);
    sprintf(flags, "%d%d%d", _x, _d, _m);

    return flags;
}

char* addressNumberToString(u_int32_t _addr, bool _be)
{
    // If it is in Little-Endian, then convert it to Big-Endian
    if (!_be) _addr = htonl(_addr);

    char *addr_str = (char *)malloc(INET_ADDRSTRLEN * sizeof(char));
    inet_ntop(AF_INET, &_addr, addr_str, INET_ADDRSTRLEN);

    return addr_str;
}

void IpHeader_printInfo(IpHeader* _self)
{
    char *flag = convertFlagToBin(IpHeader_getFlags(_self));
    char *srcaddr = addressNumberToString(_self->_srcaddr, false);
    char *dstaddr = addressNumberToString(_self->_dstaddr, false);

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

    free(flag);
    free(srcaddr);
    free(dstaddr);
}

void IpHeader_encode(IpHeader* _self, ByteBuffer* _buffer)
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

ByteBuffer* IpHeader_encode_v2(IpHeader* _self)
{
    ByteBuffer* buff = ByteBuffer_new((size_t) _self->_tlength);
    IpHeader_encode(_self, buff);
    return buff;
}

IpHeader* IpHeader_decode(ByteBuffer* _buffer)
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

    IpHeader* hdr = IpHeader_new();

    IpHeader_setVersion(hdr, version);
    IpHeader_setDifferentiatedServiceField(hdr, dsf);
    IpHeader_setTotalLength(hdr, ntohs(tlen));
    IpHeader_setIdentfication(hdr, ntohs(id));
    IpHeader_setFlagOffField(hdr, ntohs(flagoff));
    IpHeader_setTimeToLive(hdr, ttl);
    IpHeader_setProtocol(hdr, protocol);
    IpHeader_setHeaderChecksum(hdr, ntohs(checksum));
    IpHeader_setSourceAddress(hdr, ntohl(srcaddr));
    IpHeader_setDestinationAddress(hdr, ntohl(dstaddr));

    return hdr;
}

/* --------------------------------------------- IP PACKET --------------------------------------------- */

IpPacket* IpPacket_new()
{
    size_t payload_size = sizeof(char) * IP_PAYLOAD_MAX_SIZE;
    
    IpHeader* iphdr = IpHeader_new();
    char* payload = (char *)malloc(payload_size);
    memset(payload, 0, payload_size); // Set data to zeros
    
    IpPacket* ippckt = malloc(sizeof(IpHeader) + payload_size);
    ippckt->_iphdr = iphdr;
    ippckt->_payload = payload;

    return ippckt;
}

void IpPacket_delete(IpPacket* _self)
{
    IpHeader_delete(_self->_iphdr);
    free(_self->_payload);
    free(_self);
}

void IpPacket_setHeader(IpPacket *_self, IpHeader *_iphdr)
{
    memcpy(_self->_iphdr, _iphdr, IP_HEADER_SIZE);
    IpHeader_delete(_iphdr);
}

void IpPacket_fillHeader(
    IpPacket* _self,     u_int8_t  _version, u_int8_t  _dsf, u_int16_t _tlen,
    u_int16_t _id,       u_int16_t _flagoff, u_int8_t  _ttl, u_int8_t  _protocol, 
    u_int16_t _checksum, u_int32_t _srcaddr, u_int32_t _dstaddr
) {
    IpHeader_setVersion(_self->_iphdr, _version);
    IpHeader_setDifferentiatedServiceField(_self->_iphdr, _dsf);
    IpHeader_setTotalLength(_self->_iphdr, _tlen);
    IpHeader_setIdentfication(_self->_iphdr, _id);
    IpHeader_setFlagOffField(_self->_iphdr, _flagoff);
    IpHeader_setTimeToLive(_self->_iphdr, _ttl);
    IpHeader_setProtocol(_self->_iphdr, _protocol);
    IpHeader_setHeaderChecksum(_self->_iphdr, _checksum);
    IpHeader_setSourceAddress(_self->_iphdr, _srcaddr);
    IpHeader_setDestinationAddress(_self->_iphdr, _dstaddr);
}

void IpPacket_fillPayload(IpPacket *_self, char *_data, size_t _datasize)
{
    memcpy(_self->_payload, _data, _datasize);
    
    /* Update the total size */
    u_int16_t new_tlen = sizeof(IpHeader) + _datasize;
    IpHeader_setTotalLength(_self->_iphdr, new_tlen);
}

u_int16_t IpPacket_getPayloadSize(IpPacket *_self)
{
    return _self->_iphdr->_tlength - IP_HEADER_SIZE;
}

IcmpPacket* IpPacket_getIcmpPacket(IpPacket *_self)
{
    u_int16_t icmp_size = IpPacket_getPayloadSize(_self);
    ByteBuffer *buffer = ByteBuffer_new_v2(_self->_payload, icmp_size);
    IcmpPacket *pckt = IcmpPacket_decode(buffer);
    ByteBuffer_delete(buffer);
    return pckt;
}

ByteBuffer* IpPacket_encode(IpPacket* _self)
{
    ByteBuffer* buff = IpHeader_encode_v2(_self->_iphdr);
    ByteBuffer_putBuffer(buff, _self->_payload, (size_t)IpPacket_getPayloadSize(_self));
    return buff;
}

IpPacket* IpPacket_decodeIcmp(ByteBuffer* _buffer)
{
    IpHeader* hdr = IpHeader_decode(_buffer);
    IpPacket* pckt = IpPacket_new();

    IpPacket_setHeader(pckt, hdr);

    IcmpPacket* icmppckt = IcmpPacket_decode(_buffer);
    IpPacket_wrapIcmp(pckt, icmppckt);

    IcmpPacket_delete(icmppckt);
    return pckt;
}

void IpPacket_wrapIcmp(IpPacket* _self, IcmpPacket* _icmppckt)
{
    ByteBuffer* buff = IcmpPacket_encode(_icmppckt);
    IpPacket_fillPayload(_self, buff->_buffer, buff->_size);
    IpHeader_setProtocol(_self->_iphdr, IP_HEADER_ICMP_PROTOCOL_CODE);
    ByteBuffer_delete(buff);
}