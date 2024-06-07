#include "sender.h"


RawSender* RawSender_new(char *_srcaddr, char* _dstaddr, char* _gateway, u_int16_t _dstport, char* _proto)
{
    struct sockaddr_in dst;
    int socketfd;

    struct protoent *proto = getprotobyname(_proto);
    socketfd = socket(AF_INET, SOCK_RAW, proto->p_proto);
    if (socketfd == -1) handle_error("socket");

    int value = 1;
    setsockopt(socketfd, IPPROTO_IP, IP_HDRINCL, &value, sizeof(value));

    /* Connect to the destination socket */
    memset(&dst, 0, sizeof(dst));
    dst.sin_family = AF_INET;
    dst.sin_port = htons(_dstport);
    inet_pton(AF_INET, _dstaddr, &dst.sin_addr);

    RawSender *sender = (RawSender*)malloc(sizeof(RawSender));
    sender->_srcaddress = _srcaddr;
    sender->_dstaddress = dst;
    sender->_gateway = _gateway;
    sender->_socket = socketfd;
    sender->_msgcnt = 0;
    sender->_proto = proto;
    sender->_lastid = (unsigned short)getpid();
    sender->_icmpsn = 0;
    sender->_lsticmpid = 1;

    return sender;
}

void RawSender_delete(RawSender* _self)
{
    shutdown(_self->_socket, 2);
    free(_self);
}

void __RawSender_sendto_v2(RawSender* _self, char* _buffer, size_t _size)
{
    socklen_t dstlen;
    dstlen = sizeof(struct sockaddr_in);
    
    if (
        sendto(
            _self->_socket, _buffer, _size, 0, 
            (struct sockaddr *)&_self->_dstaddress, dstlen
        ) < 0
    ) {
        handle_error("sendto");
    }

    printf("Sent %ld bytes of IP Packet\n", _size);
}

void RawSender_sendto(RawSender* _self, IpPacket* _pckt)
{
    ByteBuffer* buffer = IpPacket_encode(_pckt);
    ByteBuffer_writeToFile(buffer, "sent.bin");
    
    __RawSender_sendto_v2(_self, buffer->_buffer, buffer->_size);
    _self->_msgcnt += 1;
    
    ByteBuffer_delete(buffer);
}

void RawSender_printInfo(RawSender* _self)
{
    u_int16_t dstport = ntohs(_self->_dstaddress.sin_port);
    char *dstaddr = RawSender_getDestinationIP(_self);

    printf("[*] Sending To %s:%d\n", dstaddr, dstport);

    free(dstaddr);
}

char* RawSender_getDestinationIP(RawSender* _self)
{
    char *dstaddr = addressNumberToString(_self->_dstaddress.sin_addr.s_addr, true);
    return dstaddr;
}

void RawSender_sendc(RawSender* _self, IpPacket* _pckt)
{
    RawSender_printInfo(_self);

    while (1)
    {
        RawSender_sendto(_self, _pckt);
    }
}

IpPacket* RawSender_createIpPacket(RawSender *_self, u_int16_t _id)
{
    u_int8_t proto = _self->_proto->p_proto;
    u_int32_t dstaddr = _self->_dstaddress.sin_addr.s_addr;
    u_int32_t srcaddr = inet_network(_self->_srcaddress);

    IpPacket* ippckt = craftIpPacket(
        IPv4, 0x0, 0x0, IP_HEADER_SIZE, _id, X_FLAG_NOT_SET, D_FLAG_SET,
        M_FLAG_NOT_SET, 0x0, TTL_DEFAULT_VALUE, proto, 0x0,
        srcaddr, dstaddr
    );

    return ippckt;
}

IcmpPacket* RawSender_createIcmpPacket(RawSender* _self, u_int8_t _type, u_int8_t _code)
{
    if (
        (
            _type == ICMP_DESTINATION_UNREACHABLE_TYPE ||
            _type == ICMP_SOURCE_QUENCH_TYPE           ||
            _type == ICMP_TIME_EXCEEEDED_TYPE  
        )
    ) {
        return craftIcmpPacket_Unused(_type, _code, 0x0);
    }

    if (_type == ICMP_REDIRECT_TYPE)
    {
        return craftIcmpPacket_Redirect(_type, _code, 0x0, _self->_gateway);
    }

    if (
        (
            _type == ICMP_ECHO_REPLY_TYPE          ||
            _type == ICMP_ECHO_TYPE                ||
            _type == ICMP_INFORMATION_REQUEST_TYPE ||
            _type == ICMP_INFORMATION_REPLY_TYPE
        )
    ) {
        return craftIcmpPacket_Echo(_type, _code, 0x0, _self->_lsticmpid++, ++_self->_icmpsn);
    }

    fprintf(stderr, "[RawSender_createIcmpPacket] Undefined ICMP type %c\n", _type);
    exit(EXIT_FAILURE);
}

void RawSender_sendIcmp(RawSender* _self, u_int8_t _type, u_int8_t _code, int _n)
{
    for (int j = 0; j < _n; j++)
    {
        IpPacket* ippckt = RawSender_createIpPacket(_self, _self->_lastid);
        IcmpPacket* icmppckt = RawSender_createIcmpPacket(_self, _type, _code);

        // Compute the checksum of the ICMP header
        ByteBuffer *icmphdrbuff = IcmpHeader_encode_v2(icmppckt->_icmphdr);
        u_int16_t chksum = computeIcmpChecksum(icmphdrbuff->_buffer);

        IcmpHeader_setChecksum(icmppckt->_icmphdr, chksum);
        ByteBuffer_delete(icmphdrbuff);

        // Wrap the ICMP packet inside the IP packet
        IpPacket_wrapIcmp(ippckt, icmppckt);

        IpHeader_printInfo(ippckt->_iphdr);
        IcmpHeader_printInfo(icmppckt->_icmphdr);

        // Then send the packet
        RawSender_sendto(_self, ippckt);

        IcmpPacket_delete(icmppckt);
        IpPacket_delete(ippckt);
    }
}