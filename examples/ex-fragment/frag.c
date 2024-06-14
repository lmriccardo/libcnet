#include <ip.h>
#include <utils/fragment.h>
#include <stdlib.h>

#define BUFF_SIZE 2 * 65560

int main(int argc, char** argv)
{
    IpPacket* ippckt = IpPacket_new();
    UdpPacket* udppckt = UdpPacket_new();

    u_int32_t srcaddr = inet_network("127.0.0.1");

    char *payload = (char *)malloc(BUFF_SIZE * sizeof(char));
    generateRandomData(BUFF_SIZE, payload);
    printf("[*] Generated Payload of size: %d\n", BUFF_SIZE);

    struct FragmentList* fragments = createFragments(
        payload, BUFF_SIZE, UDP_PAYLOAD_MAX_SIZE);

    printf("[*] Generated %ld fragments\n", fragments->_size);

    u_int16_t offset = 0;
    for (int nfrag = 0; nfrag < fragments->_size; nfrag++)
    {
        char *fragment = *(fragments->_frags + nfrag);
        u_int16_t size = (nfrag < fragments->_size - 1) ? UDP_PAYLOAD_MAX_SIZE \
            : BUFF_SIZE - nfrag * UDP_PAYLOAD_MAX_SIZE;

        UdpPacket_fillHeader(udppckt, 80, 80, size + UDP_HEADER_SIZE, 0x0);
        UdpPacket_fillPayload(udppckt, fragment, size);

        u_int16_t tlen = IP_HEADER_SIZE + udppckt->_hdr->_length;
        int _m = nfrag < fragments->_size - 1 ? M_FLAG_SET : M_FLAG_NOT_SET;
        int _d = nfrag < fragments->_size - 1 ? D_FLAG_NOT_SET : D_FLAG_SET;
        u_int16_t flagoff = computeFlagOff(X_FLAG_NOT_SET, _d, _m, offset);
        IpPacket_fillHeader(
            ippckt, 4, 0, tlen, 1, flagoff, 128, IP_HEADER_UDP_PROTOCOL_CODE, 
            0x0, srcaddr, srcaddr
        );

        IpPacket_wrapUdp(ippckt, udppckt);

        offset = offset + ((nfrag == 0) ? IP_HEADER_SIZE + UDP_HEADER_SIZE + size : size);
    }

    FragmentList_delete(fragments);
    IpPacket_delete(ippckt);
    UdpPacket_delete(udppckt);

    return 0;
}