#include <utils/net.h>
#include <sender.h>

#define BUFF_SIZE 64

#define DEBUG

// Assume argv[1] -> interface (like eth0)
// Assume argv[2] -> hostname (like google.com)
int main(int argc, char **argv)
{
    // Create some usefull variables
    char remote[INET_ADDRSTRLEN], payload[BUFF_SIZE];
    u_int16_t srcport = INADDR_ANY, dstport = 1234;

    // Obtain the IP of the remote host
    getHostnameIP(argv[2], remote);

    // Create the sender
    Sender* sender = Sender_new(argv[1], remote, NULL, dstport, "udp", true);

    // Generate a random payload
    generateRandomData(BUFF_SIZE, payload);

    // Send the UDP Message
    Sender_sendUdp(sender, srcport, payload, BUFF_SIZE);

    // Free the allocated memory
    Sender_delete(sender);
    // free(payload);

    return 0;
}