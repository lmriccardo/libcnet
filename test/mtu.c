#include <receiver.h>
#include <sender.h>
#include <utils/net.h>

int processResponse(struct Response* resp)
{
    int next_hop_mtu;
    
    ByteBuffer* bbuffer = ByteBuffer_new_v2(resp->_buffer, resp->_size);
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

    ByteBuffer_delete(bbuffer);
    IpPacket_delete(pckt);
    IcmpPacket_delete(icmppckt);

    return next_hop_mtu;
}

int main(int argc, char** argv)
{
    if (argc < 2)
    {
        fprintf(stderr, "Usage: ./mtu [interface] [hostname]\n");
    }

    char remote[INET_ADDRSTRLEN];
    getHostnameIP(argv[2], remote);

    Receiver *recv = Receiver_new(argv[1], INADDR_ANY, "icmp", true);
    Sender *sender = Sender_new(argv[1], remote, NULL, INADDR_ANY, "icmp", true);

    Receiver_start(recv);

    int buffer_size = getInterfaceMTU(argv[1]) - IP_HEADER_SIZE - ICMP_HEADER_MAX_SIZE;
    char payload[buffer_size];

    while (true)
    {
        memset(payload, 0, buffer_size);
        generateRandomData(payload, buffer_size);

        Sender_sendIcmp(sender, ICMP_ECHO_TYPE, ICMP_ECHO_CODE, 1, 0.2, payload, buffer_size);
        
        if (!MessageQueue_isEmpty(recv->_queue))
        {
            struct Node* node = MessageQueue_pop(recv->_queue);
            int mtu_next_hop = processResponse((struct Response*)node->_value);
            if (mtu_next_hop == -1) break;

            buffer_size = mtu_next_hop;
            printf("New MTU %d\n", buffer_size);
        }
    }

    Receiver_stop(recv);
    Receiver_delete(recv);
    Sender_delete(sender);

    return 0;
    
}