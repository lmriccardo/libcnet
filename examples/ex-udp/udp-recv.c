#include <ip.h>
#include <receiver.h>

int main(void)
{
    char lo[3] = "lo";
    Receiver *recv = Receiver_new(lo, 1234, "udp", false);
    Receiver_start(recv);

    printf("Starting Receiving ... \n\n");
    
    while (true)
    {
        if (MessageQueue_isEmpty(recv->_queue)) continue;

        struct Node* node = MessageQueue_pop(recv->_queue);
        struct Response* resp = (struct Response*)node->_value;

        ByteBuffer* buff = ByteBuffer_new_b(resp->_buffer, resp->_size);
        IpPacket* pckt = IpPacket_decodeUdp(buff);

        size_t psize = UdpPacket_getPayloadSize(pckt->_payload._udp) + 1;
        char* payload = (char*)malloc(psize * sizeof(char));
        memcpy(payload, pckt->_payload._udp->_payload, psize);

        if (strncmp(payload, "quit", 5) == 0) break;

        printf("Received Message (len %ld): %s\n", strlen(payload), payload);
        
        IpPacket_delete(pckt);
        Node_delete(node);
        ByteBuffer_delete(buff);

        free(payload);
    }

    printf("Exiting ...\n");
    Receiver_stop(recv);
    Receiver_delete(recv);

    return 0;
}