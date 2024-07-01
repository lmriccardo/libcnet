#include <ip.h>
#include <sender.h>


int main(void)
{
    char lo[3] = "lo";
    char addr[INET_ADDRSTRLEN];
    getInterfaceIp(lo, addr);

    // Initialize the sender
    Sender* sender = Sender_new(lo, addr, NULL, 1234, "udp", false);

    // Get the payload from user input
    char payload[100];
    char exit[5] = "quit";
    
    while (true)
    {

        // Continously send the message until the user do not enter "quit"
        printf("Write a message (max 100 chars): ");
        char *result = fgets(payload, 100, stdin);

        if (result == NULL || strlen(payload) < 0)
        {
            fprintf(stderr, "Error when reading from input\n");
            break;
        }
        
        payload[strlen(payload) - 1] = '\0';

        // Craft the UDP Packet and send it
        size_t size = (size_t)strlen(payload);
        IpPacket* pckt = Sender_craftUdp(sender, INADDR_ANY, payload, size + 1);
        Sender_send(sender, pckt, 0.0);

        if (strncmp(payload, exit, 5) == 0) break;

    }

    printf("Exiting ...\n");
    Sender_delete(sender);

    return 0;
}