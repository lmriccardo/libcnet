#include "neto.h"
#include "sender.h"
#include "receiver.h"
#include "utils.h"

static int    received_packets = 0;
static int    errors           = 0;
static double tot_time         = 0.0;

void *process(char *response, size_t len, double time)
{
    ByteBuffer* buffer = ByteBuffer_new_v2(response, len);
    IpPacket* ippckt = IpPacket_decodeIcmp(buffer);
    
    IcmpPacket* icmppckt = IpPacket_getIcmpPacket(ippckt);
    IcmpHeader* icmphdr = icmppckt->_icmphdr;

    char addr[INET_ADDRSTRLEN];
    addressNumberToString_s(ippckt->_iphdr->_srcaddr, addr, false);

    // Check the ICMP type of the reply
    if (icmphdr->_type == ICMP_ECHO_REPLY_TYPE)
    {
        printf("%hu bytes from %s: icmp_seq=%hu ttl=%hu rtt=%.3f msec\n", 
           ippckt->_iphdr->_tlength, addr, icmphdr->_rest->_echo._seqnum,
           ippckt->_iphdr->_ttl, time * 1e3);

        received_packets++;
    }
    
    if (icmphdr->_type == ICMP_DESTINATION_UNREACHABLE_TYPE)
    {
        printf(
            "From %s icmp_seq=%hu Destination Host Unreachable\n", 
            addr, icmphdr->_rest->_echo._seqnum
        );

        errors++;
    }

    tot_time = tot_time + time * 1e3;

    IcmpPacket_delete(icmppckt);
    ByteBuffer_delete(buffer);
    IpPacket_delete(ippckt);
    return NULL;
}

int ping(const char* address)
{
    char *remote = getHostnameIP(address);

    printf("PING to %s (%s)\n", address, remote);
    
    struct Timer *timer = Timer_new();

    Receiver* recv = Receiver_new("eth0", 0, "icmp", false);
    RawSender* pinger = RawSender_new("eth0", remote, NULL, 0, "icmp", false);
    synchronizeRTT(pinger, recv, timer);

    Receiver_start(recv, process);
    RawSender_sendIcmp(pinger, ICMP_ECHO_TYPE, ICMP_ECHO_CODE, 2, 1.0);

    double packet_loss = (double)(pinger->_icmpsn - 1 - received_packets) \
        / (double)(pinger->_icmpsn - 1) * 100.0;

    printf("\n--- %s ping statistics ---\n", address);
    printf(
        "%d packets transmitted, %d received, %.1f %% packet loss, tot rtt %.3f msec\n",
        (pinger->_icmpsn - 1), received_packets, packet_loss, tot_time
    );

    Receiver_stop(recv);
    RawSender_delete(pinger);
    Receiver_delete(recv);
    Timer_delete(timer);

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