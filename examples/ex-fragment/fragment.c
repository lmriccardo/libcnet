#include <ip.h>
#include <utils/fragment.h>
#include <stdlib.h>

#define BUFF_SIZE 65560

int main(int argc, char** argv)
{

    int nfrags = computeNumberOfFragments(BUFF_SIZE);
    printf("Number of Fragments: %d\n", nfrags);
    printf("Actual UDP Payload Size: %d\n", BUFF_SIZE - UDP_HEADER_SIZE);

    UdpPacket* udp = UdpPacket_new_v2(BUFF_SIZE - UDP_HEADER_SIZE);
    UdpPacket_fillHeader(udp, 80, 80, BUFF_SIZE, 0x0);

    srand(0);
    

    return 0;
}