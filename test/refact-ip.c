#include <sender.h>
#include <ip.h>

int main(void)
{
    char *remote = "google.com";
    char raddr[INET_ADDRSTRLEN];
    getHostnameIP(remote, raddr);

    char payload[18];
    generateRandomData(payload, 18);
    
    bool verbose = true;

    Sender *sender = Sender_new("eth0", raddr, NULL, INADDR_ANY, "icmp", verbose);
    IpPacket* pckt = Sender_craftIcmp(sender, ICMP_ECHO_TYPE, ICMP_ECHO_CODE, payload, 18);

    if (verbose)
    {
        IpHeader_printInfo(&pckt->_iphdr);
        IcmpHeader_printInfo(&pckt->_payload._icmp->_icmphdr);
    }

    Sender_send(sender, pckt, 0.0);

    Sender_delete(sender);
    IpPacket_delete(pckt);

    return 0;
}