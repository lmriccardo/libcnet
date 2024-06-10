#include "neto.h"
#include "sender.h"
#include "receiver.h"
#include "utils.h"

void *process(char *response, size_t len)
{
    return NULL;
}

int ping(const char* address)
{
    char *remote = getHostnameIP(address);

    printf("[*] Requested a ping to %s (%s)\n", address, remote);

    Receiver* recv = Receiver_new("eth0", 0, "icmp", false);
    Receiver_start(recv, process);

    RawSender* pinger = RawSender_new("eth0", remote, NULL, 0, "icmp", false);
    RawSender_sendIcmp(pinger, ICMP_ECHO_TYPE, ICMP_ECHO_CODE, 20, 1.0);

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