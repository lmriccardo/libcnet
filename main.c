#include "neto.h"
#include "sender.h"
#include "receiver.h"


void *process_recv(char *buff, size_t size)
{
    printf("Processing ...\n");
    return NULL;
}


int main(int argc, char **argv)
{

    // Instantiate the Receiver
    Receiver* recv = Receiver_new("eth0", 0, "icmp");
    Receiver_start(recv, process_recv);

    // Initialize a sender
    RawSender* sender = RawSender_new("eth0", "142.250.180.174", NULL, 0, "icmp", true);
    RawSender_sendIcmp(sender, ICMP_ECHO_TYPE, ICMP_ECHO_CODE, -1);
    // RawSender_sendUdp(sender, INADDR_ANY, "Ciao", 5);

    sleep(1);
    Receiver_stop(recv);
    sleep(1);

    RawSender_delete(sender);
    Receiver_delete(recv);

    return 0;

}