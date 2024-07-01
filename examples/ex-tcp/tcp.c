#include <ip.h>
#include <sender.h>

int main(void)
{
    char interface[] = "eth0";
    char remote[] = "google.com";
    char addr[INET_ADDRSTRLEN];
    getHostnameIP(remote, addr);

    Sender* sender = Sender_new(interface, addr, NULL, 80, "tcp", true);

    convertIntToControlBits(TCP_SYN_SET, &sender->_tcpp._cbits);
    sender->_tcpp._sn = 0;

    IpPacket* pckt = Sender_craftTcp(sender, 12345, 5, 64240, 0, NULL, 0);
    TcpHeader_printInfo(&pckt->_payload._tcp->_hdr);
    Sender_send(sender, pckt, 0.0);

    Sender_delete(sender);
    IpPacket_delete(pckt);

    return 0;
}