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
    sender->_lastid = (unsigned short)getpid();
    sender->_icmpsn = 0;
    sender->_lsticmpid = 1;
    sender->_verbose = _verbose;
    sender->_timer = NULL;
    sender->_mtu = getInterfaceMTU(_interface);
    sender->_synch = false;
    sender->_sent = false;

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

void __Sender_sendto_v2(Sender* _self, const char* _buffer, const size_t _size)
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

    if (_self->_synch)
    {
        semaphore_wait(&_self->_mutex, "Sender_sendto");
    }

    ByteBuffer* buffer = IpPacket_encode(_pckt);
    
    __Sender_sendto_v2(_self, buffer->_buffer, buffer->_size);
    _self->_msgcnt += 1;

    if (_self->_verbose)
    {
        char *filename = (char*)malloc(100 * sizeof(char));

        snprintf(
            filename, 60, "sent_%hu_%d_%s.bin", _self->_lastid, 
            _self->_msgcnt, _self->_proto->p_name
        );
        
        ByteBuffer_writeToFile(buffer, filename);
        free(filename);
    }
    
    ByteBuffer_delete(buffer);

    _self->_sent = true;

    if (_self->_synch)
    {
        semaphore_post(&_self->_mutex, "Sender_sendto");
    }
}

void Sender_printInfo(const Sender* _self)
{
    u_int16_t dstport = ntohs(_self->_dstaddress.sin_port);
    char *dstaddr = Sender_getDestinationIP(_self);

    printf("[*] Sending To %s:%d\n", dstaddr, dstport);

    free(dstaddr);
}

char* Sender_getDestinationIP(const Sender* _self)
{
    char *dstaddr = addressNumberToString(_self->_dstaddress.sin_addr.s_addr, true);
    return dstaddr;
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
    u_int8_t proto = _self->_proto->p_proto;
    u_int32_t dstaddr = ntohl(_self->_dstaddress.sin_addr.s_addr);
    u_int32_t srcaddr = inet_network(_self->_srcaddress);

    IpPacket* ippckt = craftIpPacket(
        IPv4, 0x0, 0x0, IP_HEADER_SIZE, _id, X_FLAG_NOT_SET, D_FLAG_SET,
        M_FLAG_NOT_SET, 0x0, TTL_DEFAULT_VALUE, proto, 0x0,
        srcaddr, dstaddr
    );

    return ippckt;
}

IcmpPacket* Sender_createIcmpPacket(Sender* _self, const u_int8_t _type, const u_int8_t _code)
{
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
            return craftIcmpPacket_Mtu(_type, _code, 0x0, _self->_mtu);
        }

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
        return craftIcmpPacket_Echo(_type, _code, 0x0, _self->_lsticmpid++, _self->_icmpsn++);
    }

    fprintf(stderr, "[Sender_createIcmpPacket] Undefined ICMP type %c\n", _type);
    exit(EXIT_FAILURE);
}

UdpPacket* Sender_createUdpPacket(Sender* _self, const u_int16_t _srcport, const char* _payload, const size_t _size)
{
    u_int16_t dstport = ntohs(_self->_dstaddress.sin_port);
    u_int16_t length = (_size > UDP_PAYLOAD_MAX_SIZE ? UDP_PAYLOAD_MAX_SIZE : _size) + UDP_HEADER_SIZE;
    UdpPacket* pckt = craftUdpPacket(_srcport, dstport, length, 0x0, _payload, _size);
    return pckt;
}

void Sender_sendIcmp(
    Sender* _self, const u_int8_t _type, const u_int8_t _code, const int _n, const double _delay
) {
    IpPacket* ippckt = Sender_createIpPacket(_self, _self->_lastid++);
    IcmpPacket* icmppckt = Sender_createIcmpPacket(_self, _type, _code);
    int counter = _n;

    struct Timer* timer = Timer_new();
    Timer_start(timer);

    while (counter > 0 || _n == -1)
    {
        IcmpHeader_setSequenceNumber(icmppckt->_icmphdr, _self->_icmpsn++);

        // Compute the checksum of the ICMP header
        ByteBuffer *icmphdrbuff = IcmpHeader_encode_v2(icmppckt->_icmphdr);
        u_int16_t chksum = computeIcmpChecksum(icmphdrbuff->_buffer);
        IcmpHeader_setChecksum(icmppckt->_icmphdr, chksum);
        ByteBuffer_delete(icmphdrbuff);

        // Wrap the ICMP packet inside the IP packet
        IpPacket_wrapIcmp(ippckt, icmppckt);

        if (_self->_verbose)
        {
            IpHeader_printInfo(ippckt->_iphdr);
            IcmpHeader_printInfo(icmppckt->_icmphdr);
        }

        if (_self->_timer != NULL) Timer_resetPrevious(_self->_timer);

        // Then send the packet
        Sender_sendto(_self, ippckt);
        
        // Sleep using the created timer
        double eta;
        do 
        {
            eta = Timer_getElapsedTime(timer);
            // printf("%.3f -- \n", eta);
            
        } while (eta < _delay * 1e9);

        // Once the sleep is finished we need to reset the Timer
        Timer_reset(timer);

        counter--;
    }

    Timer_stop(timer);
    Timer_delete(timer);
    IcmpPacket_delete(icmppckt);
    IpPacket_delete(ippckt);
}

void Sender_sendUdp(Sender* _self, const u_int16_t _srcport, const char* _payload, const size_t _size)
{
    IpPacket* ippckt = Sender_createIpPacket(_self, _self->_lastid++);
    UdpPacket* udppckt = Sender_createUdpPacket(_self, _srcport, _payload, _size);

    // Compute the checksum of the UDP header
    ByteBuffer* buff = ByteBuffer_new(UDP_HEADER_PLUS_PSEUDO_SIZE);
    UdpHeader_encode(udppckt->_hdr, buff);

    // Put the pseudo header inside the buffer
    ByteBuffer_putInt(buff, htonl(inet_network(_self->_srcaddress)));
    ByteBuffer_putInt(buff, _self->_dstaddress.sin_addr.s_addr);
    ByteBuffer_put(buff, 0x0);
    ByteBuffer_put(buff, (u_int8_t)_self->_proto->p_proto);
    ByteBuffer_putShort(buff, htons(udppckt->_hdr->_length));

    u_int16_t checksum = computeUDPChecksum(buff->_buffer);
    UdpHeader_setChecksum(udppckt->_hdr, checksum);
    ByteBuffer_delete(buff);

    // Wrap the UDP packet into the IP Packet
    IpPacket_wrapUdp(ippckt, udppckt);
    IpHeader_printInfo(ippckt->_iphdr);
    UdpHeader_printInfo(udppckt->_hdr);

    // Finally, send the packet
    Sender_sendto(_self, ippckt);

    UdpPacket_delete(udppckt);
    IpPacket_delete(ippckt);
}