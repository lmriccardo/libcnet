#include "neto.h"
#include "sender.h"


int main(char **argv, int argc)
{

    // Initialize a sender
    RawSender* sender = RawSender_new("172.19.11.1", "8.8.8.8", "172.19.0.1", 0, "icmp");
    RawSender_sendIcmp(sender, ICMP_ECHO_TYPE, ICMP_ECHO_CODE);

    // Let's try to decode the packet we have sent
    ByteBuffer_resetPosition(sender->_buff);
    
    IpPacket* pckt = IpPacket_decodeIcmp(sender->_buff);
    IpHeader_printInfo(pckt->_iphdr);

    IcmpPacket* icmppckt = IpPacket_getIcmpPacket(pckt);
    IcmpHeader_printInfo(icmppckt->_icmphdr);

    ByteBuffer* recvbuff = IpPacket_encode(pckt);
    ByteBuffer_writeToFile(recvbuff, "recv.bin");
    
    // Delete all the stuff to free memory
    ByteBuffer_delete(recvbuff);
    IcmpPacket_delete(icmppckt);
    IpPacket_delete(pckt);
    RawSender_delete(sender);
    return 0;

}