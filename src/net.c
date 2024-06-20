#include "utils/net.h"
#include "sender.h"
#include "receiver.h"
#include "utils/timer.h"

void getHostnameIP(const char* _hostname, char* _out)
{
    struct hostent *hostentry = gethostbyname(_hostname);
    if (hostentry == NULL)
    {
        fprintf(stderr, "[getHostName_s] GetHostByName call failed.\n");
        exit(EXIT_FAILURE);
    }

    addressNumberToString_s((*((struct in_addr*) hostentry->h_addr_list[0])).s_addr, _out, true);
}

char* addressNumberToString(u_int32_t _addr, const bool _be)
{
    // If it is in Little-Endian, then convert it to Big-Endian
    if (!_be) _addr = htonl(_addr);

    char *addr_str = (char *)malloc(INET_ADDRSTRLEN * sizeof(char));
    inet_ntop(AF_INET, &_addr, addr_str, INET_ADDRSTRLEN);

    return addr_str;
}

void addressNumberToString_s(u_int32_t _addr, char *_out, const bool _be)
{
    // If it is in Little-Endian, then convert it to Big-Endian
    if (!_be) _addr = htonl(_addr);
    inet_ntop(AF_INET, &_addr, _out, INET_ADDRSTRLEN);
}

void getInterfaceIp(const char* _interface, char* _addr)
{
    int sockfd = socket(AF_INET, SOCK_DGRAM, 0);
    struct ifreq ifr;

    ifr.ifr_addr.sa_family = AF_INET;
    strncpy(ifr.ifr_name , _interface , IFNAMSIZ - 1);
    ioctl(sockfd, SIOCGIFADDR, &ifr);
    close(sockfd);

    u_int32_t _addr_i = ( (struct sockaddr_in *)&ifr.ifr_addr )->sin_addr.s_addr;
    inet_ntop(AF_INET, &_addr_i, _addr, INET_ADDRSTRLEN);
}

void generateRandomData(char* _dst, const size_t _size)
{
    srand( time(NULL) );

    for (int i = 0; i < _size; i++)
    {
        *(_dst + i) = rand();
    }
}

int getInterfaceMTU(const char* _interface)
{
    int sockfd = socket(AF_INET, SOCK_DGRAM, 0);
    struct ifreq ifr;

    ifr.ifr_addr.sa_family = AF_INET;
    strncpy(ifr.ifr_name , _interface ,IFNAMSIZ - 1);
    if (ioctl(sockfd, SIOCGIFMTU, &ifr) < 0)
    {
        fprintf(stderr, "[getInterfaceMTU] Error setting MTU for given interface\n");
        exit(EXIT_FAILURE);
    }

    close(sockfd);

    return ifr.ifr_mtu;
}

u_int16_t computeChecksum(const unsigned char* buffer, const size_t size)
{
    int checksum = 0;

    for (int i = 0; i < size; i +=2 )
    {
        if (i == 2) continue;
        if (i == size - 1)
        {
            checksum += (*(buffer + i) << 8);
            continue;
        }

        checksum += (*(buffer + i) << 8) + *(buffer + i + 1);
    }

    checksum = (checksum >> 16) + (checksum & 0xFFFF);
    checksum += checksum >> 16;

    return (u_int16_t)((~checksum) & 0xFFFF);
}

int pathMtuDiscovery(const char* _interface, const char* _hostname)
{
    // Get the IP of the remote hostname given as input
    char remote[INET_ADDRSTRLEN];
    getHostnameIP(_hostname, remote);

    // Create the Receiver and Sender ICMP
    Receiver *recv = Receiver_new(_interface, INADDR_ANY, "icmp", true);
    Sender *sender = Sender_new(_interface, remote, NULL, INADDR_ANY, "icmp", true);

    // Start the receiver thread
    Receiver_start(recv);

    // The buffer size must be: initial MTU (1500) - Headers of IP and ICMP (28)
    int mtu = getInterfaceMTU(_interface);
    int buffer_size = mtu - IP_HEADER_SIZE - ICMP_HEADER_MAX_SIZE;

    char payload[buffer_size];
    int next_hop_mtu;
    ByteBuffer* bbuffer = ByteBuffer_new(mtu);

    while (true)
    {
        memset(payload, 0, buffer_size);
        generateRandomData(payload, buffer_size);

        // Send the ICMP Message with the random generated payload
        Sender_sendIcmp(sender, ICMP_ECHO_TYPE, ICMP_ECHO_CODE, 1, 0.2, payload, buffer_size);
        
        // There must be at least one response inside the message queue
        if (MessageQueue_isEmpty(recv->_queue)) continue;

        // Get the last message in the queue
        struct Node* node = MessageQueue_pop(recv->_queue);
        struct Response* resp = (struct Response*)node->_value;

        // Decode the buffer into an IP Packet, get the corresponding ICMP and the MTU field
        ByteBuffer_resetPosition(bbuffer);
        ByteBuffer_putBuffer(bbuffer, resp->_buffer, resp->_size);
        ByteBuffer_resetPosition(bbuffer);
        
        IpPacket* pckt = IpPacket_decodeIcmp(bbuffer);
        IcmpPacket* icmppckt = IpPacket_getIcmpPacket(pckt);

        u_int8_t type = icmppckt->_icmphdr->_type;
        u_int8_t code = icmppckt->_icmphdr->_code;

        if (
            (
                type == ICMP_DESTINATION_UNREACHABLE_TYPE &&
                code == ICMP_FRAGMENTATION_NEEDED_CODE
            )
        ) {
            next_hop_mtu = icmppckt->_icmphdr->_rest->_mtu._mtu;
        } else {
            next_hop_mtu = -1;
        }

        IpPacket_delete(pckt);
        IcmpPacket_delete(icmppckt);
        Node_delete(node);

        if (next_hop_mtu == -1) break;

        buffer_size = next_hop_mtu;
    }

    ByteBuffer_delete(bbuffer);
    Receiver_stop(recv);
    Receiver_delete(recv);
    Sender_delete(sender);

    return buffer_size;
}