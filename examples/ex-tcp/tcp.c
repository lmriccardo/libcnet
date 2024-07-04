#include <ip.h>
#include <sender.h>
#include <receiver.h>


int main(void)
{
    char interface[] = "eth0";
    char remote[] = "google.com";
    char addr[INET_ADDRSTRLEN];
    getHostnameIP(remote, addr);

    Sender* sender = Sender_new(interface, addr, NULL, 80, "tcp", true);
    Receiver* recv = Receiver_new(interface, 42604, "tcp", true);
    Receiver_start(recv);

    ControlBits_fromValue(TCP_SYN_SET, &sender->_tcpp._cbits);
    sender->_tcpp._sn = 833673060;

    struct TcpOption* opts[TCP_OPTIONS_MAX_SIZE];
    opts[0] = TcpOption_newMss(1460);
    opts[1] = TcpOption_newSackPermitted();
    opts[2] = TcpOption_newTimestamps(3438707201, 0);
    opts[3] = TcpOption_newNoOperation();
    opts[4] = TcpOption_newWindowScale(7);

    Sender_setTcpOptions(sender, opts, 5);

    IpPacket* pckt = Sender_craftTcp(sender, 42604, 10, 64240, 0, NULL, 0);
    Sender_send(sender, pckt, 0.2);

    TcpHeader_printInfo(&pckt->_payload._tcp->_hdr);

    Receiver_stop(recv);
    Receiver_delete(recv);
    TcpOptions_delete(opts, 5);
    Sender_delete(sender);
    IpPacket_delete(pckt);

    return 0;
}