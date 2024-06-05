#include "neto.h"
#include "sender.h"
#include "receiver.h"


int main(char **argv, int argc)
{

    // Instantiate the Receiver
    Receiver* recv = Receiver_new("172.19.11.1", INADDR_ANY, "icmp");
    Receiver_start(recv);

    // Initialize a sender
    RawSender* sender = RawSender_new("172.19.11.1", "8.8.8.8", "172.19.0.1", 0, "icmp");
    RawSender_sendIcmp(sender, ICMP_ECHO_TYPE, ICMP_ECHO_CODE);

    sleep(1);
    Receiver_stop(recv);
    sleep(1);

    RawSender_delete(sender);
    Receiver_delete(recv);

    return 0;

}