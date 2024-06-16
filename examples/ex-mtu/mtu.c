#include <utils/net.h>
#include <receiver.h>
#include <sender.h>

void test_get_local_mtu(const char* interface)
{
    int lmtu = getInterfaceMTU(interface);
    printf("%s MTU is %d bytes\n", interface, lmtu);
}

int mtu;

void *processResponse(char *buffer, size_t size, double time)
{
    ByteBuffer* bbuffer = ByteBuffer_new_v2(buffer, size);
    ByteBuffer_writeToFile(bbuffer, "recv.bin");
    
    IpPacket* ippcket = IpPacket_decodeIcmp(bbuffer);
    IcmpPacket* icmppcket = IpPacket_getIcmpPacket(ippcket);
    IcmpHeader* icmphdr = icmppcket->_icmphdr;

    IpHeader_printInfo(ippcket->_iphdr);
    IcmpHeader_printInfo(icmphdr);

    IcmpPacket_delete(icmppcket);
    IpPacket_delete(ippcket);
    ByteBuffer_delete(bbuffer);

    return NULL;
}

void test_pmtud(const char* interface, const char* hostname)
{
    char remote[INET_ADDRSTRLEN];
    getHostnameIP(hostname, remote);

    printf("Remote Address: %s\n", remote);

    // Get the initial MTU
    mtu = getInterfaceMTU(interface) - 28;

    // Let's define a receiver and a sender
    Receiver *recv = Receiver_new(interface, 0, "icmp", false);
    Sender *sender = Sender_new(interface, remote, NULL, 0, "icmp", false);
    syncrhonizeSendReceive(sender, recv);

    // Then we can start the receiver
    Receiver_start(recv, processResponse);

    // Let's create the IP and the ICMP Packets
    IpPacket* ippckt = Sender_createIpPacket(sender, sender->_lastid++);
    IcmpPacket* icmppckt = Sender_createIcmpPacket(sender, ICMP_ECHO_TYPE, ICMP_ECHO_CODE);
    IcmpHeader_setSequenceNumber(icmppckt->_icmphdr, sender->_icmpsn++);

    // Compute the checksum of the ICMP header
    ByteBuffer *icmphdrbuff = IcmpHeader_encode_v2(icmppckt->_icmphdr);
    u_int16_t chksum = computeIcmpChecksum(icmphdrbuff->_buffer);
    IcmpHeader_setChecksum(icmppckt->_icmphdr, chksum);
    ByteBuffer_delete(icmphdrbuff);

    char *payload = (char *)malloc(mtu * sizeof(char));
    generateRandomData(mtu, payload);
    IcmpPacket_fillPayload(icmppckt, payload, mtu);

    // Wrap the ICMP packet inside the IP packet
    IpPacket_wrapIcmp(ippckt, icmppckt);
    IpHeader_printInfo(ippckt->_iphdr);

    // Send the IP Packet
    Sender_sendto(sender, ippckt);
    sleep(1.0);

    Receiver_stop(recv);
    IpPacket_delete(ippckt);
    IcmpPacket_delete(icmppckt);
    Receiver_delete(recv);
    Sender_delete(sender);
}

int main(int argc, char** argv)
{
    if (argc < 3)
    {
        fprintf(stderr, "Usage: ./mtu [option] [arg1, ...]\n");
        exit(EXIT_FAILURE);
    }

    if (strncmp(argv[1], "lmtu", 4) == 0)
    {
        test_get_local_mtu(argv[2]);
        return 0;
    }

    if (strncmp(argv[1], "pmtu", 4) == 0)
    {
        // Check that both values are given
        if (argc < 4)
        {
            fprintf(stderr, "Usage: ./mtu pmtu [interface] [hostname]\n");
            exit(EXIT_FAILURE);
        }

        test_pmtud(argv[2], argv[3]);
        return 0;
    }

    fprintf(stderr, "Usage: [option] must be either: lmtu or pmtu\n");
    exit(EXIT_FAILURE);
}