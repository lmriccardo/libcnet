#include "neto.h"
#include "sender.h"
#include "receiver.h"


void *process_recv(char *buff)
{
    printf("Processing ...");
    return NULL;
}


int main(int argc, char **argv)
{

    // Instantiate the Receiver
    Receiver* recv = Receiver_new("127.0.0.1", 8004, "udp");
    Receiver_start(recv, process_recv);

    // Initialize a sender
    RawSender* sender = RawSender_new("172.19.11.1", "127.0.0.1", "172.19.0.1", 8004, "udp");
    // RawSender_sendIcmp(sender, ICMP_ECHO_TYPE, ICMP_ECHO_CODE, 2);
    RawSender_sendUdp(sender, INADDR_ANY, "Ciao", 5);

    sleep(1);
    Receiver_stop(recv);
    sleep(1);

    RawSender_delete(sender);
    Receiver_delete(recv);

    return 0;

}