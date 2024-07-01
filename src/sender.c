#include "sender.h"


Sender* Sender_new(
    const char *_interface, const char* _dstaddr, char* _gateway, const u_int16_t _dstport, 
    const char* _proto, const bool _verbose
) {
    struct sockaddr_in dst;
    int socketfd;

    struct protoent *proto = getprotobyname(_proto);
    socketfd = socket(AF_INET, SOCK_RAW, proto->p_proto);
    if (socketfd == -1) handle_error("socket", socketfd);

    int value = 1;
    setsockopt(socketfd, IPPROTO_IP, IP_HDRINCL, &value, sizeof(value));

    /* Connect to the destination socket */
    memset(&dst, 0, sizeof(dst));
    dst.sin_family = AF_INET;
    dst.sin_port = htons(_dstport);
    inet_pton(AF_INET, _dstaddr, &dst.sin_addr);

    Sender *sender = (Sender*)malloc(sizeof(Sender));

    sender->_srcaddress = (char*)malloc(INET_ADDRSTRLEN * sizeof(char));
    getInterfaceIp(_interface, sender->_srcaddress);

    sender->_dstaddress = dst;
    sender->_gateway = _gateway;
    sender->_socket = socketfd;
    sender->_msgcnt = 0;
    sender->_proto = proto;
    sender->_verbose = _verbose;
    sender->_timer = NULL;
    sender->_mtu = getInterfaceMTU(_interface);
    sender->_synch = false;
    sender->_sent = false;

    // Set default values for Parameters structure
    sender->_ipp._df = D_FLAG_SET;
    sender->_ipp._mf = M_FLAG_NOT_SET;
    sender->_ipp._xf = X_FLAG_NOT_SET;
    sender->_ipp._id = (unsigned short)getpid();
    sender->_icmpp._sn = 0;
    sender->_icmpp._id = 1;
    sender->_tcpp._sn = 1;
    sender->_tcpp._an = 0;

    convertIntToControlBits(0, &sender->_tcpp._cbits);

    return sender;
}

void Sender_delete(Sender* _self)
{
    if (_self->_synch) sem_destroy(&_self->_mutex);

    shutdown(_self->_socket, 2);
    free(_self->_srcaddress);
    free(_self);
}

void Sender_setTimer(Sender* _self, struct Timer* _timer)
{
    _self->_timer = _timer;
}

void Sender_setMtu(Sender* _self, const int _mtu)
{
    _self->_mtu	= _mtu;
}

void Sender_setIpFlags(Sender* _self, int _d, int _m)
{
    if (_d == D_FLAG_SET && _m == M_FLAG_SET)
    {
        fprintf(stderr, "[Sender_setIpFlags] Cannot set both D and M flags to 1. ");
        fprintf(stderr, "Program will not terminate but flags will not be set.\n");
    }

    if (
        (_d != D_FLAG_NOT_SET && _d != D_FLAG_SET) &&
        (_m != M_FLAG_NOT_SET && _m != M_FLAG_SET)
    ) {
        fprintf(stderr, "[Sender_setIpFlags] M and D flags must have correct values\n");
        fprintf(stderr, "Program will not terminate but flags will not be set.\n");
    }

    _self->_ipp._xf = X_FLAG_NOT_SET;
    _self->_ipp._df = _d;
    _self->_ipp._mf = _m;
}

void Sender_bsendto(Sender* _self, const char* _buffer, const size_t _size)
{
    socklen_t dstlen;
    dstlen = sizeof(struct sockaddr_in);
    
    if (
        sendto(
            _self->_socket, _buffer, _size, 0, 
            (struct sockaddr *)&_self->_dstaddress, dstlen
        ) < 0
    ) {
        handle_error("sendto", _self->_socket);
    }

    if (_self->_verbose) printf("[*] Sent %ld bytes of IP Packet\n", _size);
}

void Sender_sendto(Sender* _self, const IpPacket* _pckt)
{
    _self->_sent = false;

    ByteBuffer* buffer = IpPacket_encode(_pckt);

    if (_self->_synch) semaphore_wait(&_self->_mutex, "Sender_sendto");
    
    Sender_bsendto(_self, buffer->_buffer, buffer->_size);

    _self->_msgcnt += 1;

    if (_self->_verbose)
    {
        char filename[100];

        snprintf(
            filename, 60, "sent_%hu_%d_%s.bin", _self->_ipp._id, 
            _self->_msgcnt, _self->_proto->p_name
        );
        
        ByteBuffer_writeToFile(buffer, filename);
    }
    
    ByteBuffer_delete(buffer);

    _self->_sent = true;

    if (_self->_synch) semaphore_post(&_self->_mutex, "Sender_sendto");
}

void Sender_printInfo(const Sender* _self)
{
    u_int16_t dstport = ntohs(_self->_dstaddress.sin_port);

    char dstaddr[INET_ADDRSTRLEN];
    Sender_getDestinationIP(_self, dstaddr);

    printf("[*] Sending To %s:%d\n", dstaddr, dstport);
}

void Sender_getDestinationIP(const Sender* _self, char *_out)
{
    addressNumberToString(_self->_dstaddress.sin_addr.s_addr, _out, true);
}

void Sender_sendc(Sender* _self, const IpPacket* _pckt)
{
    Sender_printInfo(_self);

    while (1)
    {
        Sender_sendto(_self, _pckt);
    }
}

IpPacket* Sender_createIpPacket(Sender *_self, const u_int16_t _id)
{
    u_int8_t  proto   = _self->_proto->p_proto;
    u_int32_t dstaddr = ntohl(_self->_dstaddress.sin_addr.s_addr);
    u_int32_t srcaddr = inet_network(_self->_srcaddress);

    int _m = _self->_ipp._mf;
    int _d = _self->_ipp._df;
    int _x = _self->_ipp._xf;

    IpPacket* ippckt = craftIpPacket(
        IPv4, 0x0, 0x0, IP_HEADER_SIZE, _id, _x, _d, _m, 
        0x0, TTL_DEFAULT_VALUE, proto, 0x0, srcaddr, dstaddr
    );

    return ippckt;
}

void Sender_fillIpHeader(Sender* _self, IpPacket* _pckt)
{
    u_int8_t  proto   = _self->_proto->p_proto;
    u_int32_t dstaddr = ntohl(_self->_dstaddress.sin_addr.s_addr);
    u_int32_t srcaddr = inet_network(_self->_srcaddress);

    int _mf = _self->_ipp._mf;
    int _df = _self->_ipp._df;
    int _xf = _self->_ipp._xf;

    u_int16_t flagoff = computeFlagOff(_xf, _df, _mf, 0);
    u_int16_t dsf = computeDifferentiatedServiceField(0, 0);

    IpPacket_fillHeader(
        _pckt, IPv4, dsf, _pckt->_iphdr._tlength, _self->_ipp._id++, flagoff, 
        TTL_DEFAULT_VALUE, proto, 0, srcaddr, dstaddr
    );
}

void Sender_fillIcmpHeader(
    Sender* _self, IcmpPacket* _pckt, const u_int8_t _type, const u_int8_t _code
) {
    if (
        (
            _type == ICMP_DESTINATION_UNREACHABLE_TYPE ||
            _type == ICMP_SOURCE_QUENCH_TYPE           ||
            _type == ICMP_TIME_EXCEEEDED_TYPE  
        )
    ) {
        if (
            (
                _type == ICMP_DESTINATION_UNREACHABLE_TYPE &&
                _code == ICMP_FRAGMENTATION_NEEDED_CODE
            )
        ) {
            return IcmpPacket_fillHeader_Mtu(_pckt, 0x0, _self->_mtu);
        }

        return IcmpPacket_fillHeader_Unused(_pckt, 0x0);
    }

    if (_type == ICMP_REDIRECT_TYPE)
    {
        return IcmpPacket_fillHeader_Redirect(_pckt, 0x0, inet_network(_self->_gateway));
    }

    if (
        (
            _type == ICMP_ECHO_REPLY_TYPE          ||
            _type == ICMP_ECHO_TYPE                ||
            _type == ICMP_INFORMATION_REQUEST_TYPE ||
            _type == ICMP_INFORMATION_REPLY_TYPE
        )
    ) {
        return IcmpPacket_fillHeader_Echo(_pckt, 0x0, _self->_icmpp._id++, _self->_icmpp._sn++);
    }

    fprintf(stderr, "[Sender_createIcmpPacket] Undefined ICMP type %c\n", _type);
    exit(EXIT_FAILURE);
}

void Sender_fillUdpHeader(Sender* _self, UdpPacket* _pckt, const u_int16_t _srcport) 
{
    u_int16_t dstport = ntohs(_self->_dstaddress.sin_port);
    u_int16_t size = UdpPacket_getPacketSize(_pckt);

    UdpPacket_fillHeader(_pckt, _srcport, dstport, size, 0);
}

void Sender_fillTcpHeader(
    Sender*         _self, TcpPacket*        _pckt, const u_int16_t _port, const u_int8_t _offset, 
    const u_int16_t _window, const u_int16_t _urgpntr
) {
    u_int16_t dstport = ntohs(_self->_dstaddress.sin_port);

    TcpPacket_fillHeader(
        _pckt, _port, dstport, _self->_tcpp._sn, _self->_tcpp._an, _offset,
        _self->_tcpp._cbits, _window, 0, _urgpntr
    );
}

IpPacket* Sender_craftIcmp(
    Sender* _self, const u_int8_t _type, const u_int8_t _code, 
    const char* _payload,  const size_t _size
) {
    IpPacket* pckt = IpPacket_newIcmp(_type, _code, _size);
    Sender_fillIpHeader(_self, pckt);
    Sender_fillIcmpHeader(_self, pckt->_payload._icmp, _type, _code);
    
    if (_payload != NULL && _size > 0)
    {
        IcmpPacket_fillPayload(pckt->_payload._icmp, _payload, _size);
    }

    // Now, we need to compute the checksum
    u_int16_t chks = IpPacket_computeIcmpChecksum(pckt);
    IcmpHeader_setChecksum(&pckt->_payload._icmp->_icmphdr, chks);

    return pckt;
}

IpPacket* Sender_craftUdp(
    Sender* _self, const u_int16_t _port, const char* _payload, const size_t _size
) {
    IpPacket* pckt = IpPacket_newUdp(_size);
    Sender_fillIpHeader(_self, pckt);
    Sender_fillUdpHeader(_self, pckt->_payload._udp, _port);

    if (_payload != NULL && _size > 0)
    {
        UdpPacket_fillPayload(pckt->_payload._udp, _payload, _size);
    }

    // Now, we need to compute the checksum
    u_int16_t chks = IpPacket_computeUdpChecksum(pckt);
    UdpHeader_setChecksum(&pckt->_payload._udp->_hdr, chks);
    
    return pckt;
}

IpPacket* Sender_craftTcp(
    Sender*         _self,   const u_int16_t _srcport, const u_int8_t _offset, 
    const u_int16_t _window, const u_int16_t _urgpntr, const char*    _payload, 
    const size_t    _size
) {
    IpPacket* pckt = IpPacket_newTcp(_size);
    Sender_fillIpHeader(_self, pckt);
    Sender_fillTcpHeader(_self, pckt->_payload._tcp, _srcport, _offset, _window, _urgpntr);

    if (_payload != NULL && _size > 0)
    {
        TcpPacket_fillPayload(pckt->_payload._tcp, _payload, _size);
    }

    // Now, we need to compute the checksum
    u_int16_t chks = IpPacket_computeTcpChecksum(pckt);
    TcpHeader_setChecksum(&pckt->_payload._tcp->_hdr, chks);

    return pckt;
}

void Sender_send(Sender* _self, IpPacket* _pckt, const double _delay)
{
    if (_self->_timer != NULL) Timer_resetPrevious(_self->_timer);

    // Then send the packet
    Sender_sendto(_self, _pckt);
    
    // Sleep using the created timer
    Timer_sleep(_delay);
}

void Sender_updateIcmpPacket(Sender* _self, IpPacket* _pckt)
{
    IpHeader_setIdentfication(&_pckt->_iphdr, _self->_ipp._id++);
    
    u_int8_t _type = _pckt->_payload._icmp->_icmphdr._type;

    if (
        (
            _type == ICMP_ECHO_TYPE                ||
            _type == ICMP_INFORMATION_REQUEST_TYPE
        )
    ) {
        IcmpHeader_setIdentifier(&_pckt->_payload._icmp->_icmphdr, _self->_icmpp._id++);
        IcmpHeader_setSequenceNumber(&_pckt->_payload._icmp->_icmphdr, _self->_icmpp._sn++);
    }

    // Now, we need to compute the checksum
    ByteBuffer* bbuff = IcmpPacket_encode(_pckt->_payload._icmp);
    u_int16_t chks = computeChecksum((unsigned char*)bbuff->_buffer, bbuff->_size);
    IcmpHeader_setChecksum(&_pckt->_payload._icmp->_icmphdr, chks);
    ByteBuffer_delete(bbuff);
}

void Sender_updateUdpPacket(Sender* _self, IpPacket* _pckt)
{
    IpHeader_setIdentfication(&_pckt->_iphdr, _self->_ipp._id++);
    
    // Now, we need to compute the checksum
    ByteBuffer* bbuff = UdpPacket_encode(_pckt->_payload._udp);
    u_int16_t chks = computeChecksum((unsigned char*)bbuff->_buffer, bbuff->_size);
    UdpHeader_setChecksum(&_pckt->_payload._udp->_hdr, chks);
    ByteBuffer_delete(bbuff);
}