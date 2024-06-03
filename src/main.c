#include "neto.h"
#include "sender.h"


int main(char **argv, int argc)
{

    // Initialize a sender
    RawSender* sender = RawSender_new("172.19.11.1", "8.8.8.8", "172.19.0.1", 0, "icmp");
    RawSender_sendIcmp(sender, ICMP_ECHO_TYPE, ICMP_ECHO_CODE);
    RawSender_delete(sender);
    
    return 0;

}