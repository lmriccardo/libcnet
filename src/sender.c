#include <string.h>
#include "sender.h"

RawSender* RawSender_new( char* _dstaddr, u_int16_t _dstport, char* _proto) 
{
    struct sockaddr_in src, dst;
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
    sender->_dstaddress = dst;
    sender->_socket = socketfd;
    sender->_msgcnt = 0;
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
    __RawSender_sendto_v2(_self, buffer->_buffer, buffer->_size);
    _self->_buff = buffer;
    _self->_msgcnt += 1;
}

void RawSender_printInfo(RawSender* _self)
{
    u_int16_t dstport = ntohs(_self->_dstaddress.sin_port);
    char *dstaddr = addressNumberToString(_self->_dstaddress.sin_addr.s_addr, true);

    printf("[*] Sending To %s:%d\n", dstaddr, dstport);
}

void RawSender_sendc(RawSender* _self, IpPacket* _pckt)
{
    RawSender_printInfo(_self);

    while (1)
    {
        RawSender_sendto(_self, _pckt);
    }
}