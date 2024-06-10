#include "neto.h"
#include "sender.h"
#include "receiver.h"
#include "utils.h"

void *process(char *response, size_t len)
{
    ByteBuffer* buffer = ByteBuffer_new_v2(response, len);
    IpPacket* ippckt = IpPacket_decodeIcmp(buffer);
    
    IcmpPacket* icmppckt = IpPacket_getIcmpPacket(ippckt);
    IcmpHeader* icmphdr = icmppckt->_icmphdr;

    char* addr = addressNumberToString(ippckt->_iphdr->_srcaddr, false);

    printf("%hu bytes from %s: icmp_seq=%hu ttl=%hu\n", 
           ippckt->_iphdr->_tlength, addr, icmphdr->_rest->_echo._seqnum,
           ippckt->_iphdr->_ttl);

    free(addr);
    IcmpPacket_delete(icmppckt);
    ByteBuffer_delete(buffer);
    IpPacket_delete(ippckt);
    return NULL;
}

int ping(const char* address)
{
    char *remote = getHostnameIP(address);

    printf("PING to %s (%s)\n", address, remote);

    Receiver* recv = Receiver_new("eth0", 0, "icmp", false);
    Receiver_start(recv, process);

    RawSender* pinger = RawSender_new("eth0", remote, NULL, 0, "icmp", false);
    RawSender_sendIcmp(pinger, ICMP_ECHO_TYPE, ICMP_ECHO_CODE, 4, 1.0);

    Receiver_stop(recv);
    RawSender_delete(pinger);
    Receiver_delete(recv);

    return 0;
}

int main(int argc, char **argv)
{
    if (argc < 2)
    {
        fprintf(stderr, "Usage: ping [addres/hostname]\n");
        exit(EXIT_FAILURE);
    }

    ping(argv[1]);
    return 0;
}