#include "neto.h"
#include "sender.h"


int main(char **argv, int argc)
{
    IpPacket* ippckt = IpPacket_new();  
    u_int16_t flagoff = computeFlagOff(
        IP_HEADER_X_FLAG_NOT_SET, IP_HEADER_D_FLAG_SET, IP_HEADER_M_FLAG_NOT_SET, 0x0
    );

    u_int8_t dsf = computeDifferentiatedServiceField(IP_HEADER_DSCP_CS0, IP_HEADER_ECN_NECT);

    IpPacket_fillHeader(
        ippckt, IPv4, dsf, IP_HEADER_MIN_SIZE, 0xabcd, flagoff  ,
        0x40, IP_HEADER_ICMP_PROTOCOL_CODE, 0x0, "192.168.1.113", "8.8.8.8"
    );

    IcmpPacket* icmppckt = IcmpPacket_new_v2(ICMP_ECHO_TYPE, 0);

    IcmpPacket_fillHeader_v3(icmppckt, 0x0, 0x3412, 0x0100);
    IcmpHeader_printInfo(icmppckt->_icmphdr);

    IpPacket_wrapIcmp(ippckt, icmppckt);
    IpHeader_printInfo(ippckt->_iphdr);

    // Initialize a sender
    RawSender* sender = RawSender_new("8.8.8.8", 0, "tcp");

    // Send the packet
    RawSender_sendto(sender, ippckt);

    // ByteBuffer *icmpbuff = ByteBuffer_new_v2(sender->_buff->_buffer + 20, 8);
    // ByteBuffer_writeToFile(icmpbuff, "test.bin");

    RawSender_delete(sender);
    IcmpPacket_delete(icmppckt);
    IpPacket_delete(ippckt);

    return 0;
}