#include <ip.h>

int main(void)
{
    IpPacket* pckt = IpPacket_newIcmp(ICMP_ECHO_TYPE, ICMP_ECHO_CODE);

    printf("IpHeader Protocol: %d\n", pckt->_iphdr._protocol);
    
    IcmpHeader_setIdentifier(&pckt->_payload._icmp->_icmphdr, 12);
    IcmpHeader_printInfo(&pckt->_payload._icmp->_icmphdr);

    IcmpHeader hdr;
    hdr._type = 8;
    hdr._code = 0;
    hdr._checksum = 0;
    hdr._rest._echo._id = 10;
    hdr._rest._echo._seqnum = 11;

    IcmpPacket_setHeader(pckt->_payload._icmp, &hdr);
    IcmpHeader_printInfo(&pckt->_payload._icmp->_icmphdr);

    ByteBuffer* bbuff = ByteBuffer_new(9);
    IcmpHeader_encode(&pckt->_payload._icmp->_icmphdr, bbuff);
    // ByteBuffer_writeToFile(bbuff, "icmp.bin");

    ByteBuffer_resetPosition(bbuff);
    IcmpHeader_decode(&pckt->_payload._icmp->_icmphdr, bbuff);
    IcmpHeader_printInfo(&pckt->_payload._icmp->_icmphdr);

    IpPacket_v2_delete(pckt);

    return 0;
}