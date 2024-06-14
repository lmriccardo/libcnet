#include <ip.h>
#include <utils/fragment.h>
#include <stdlib.h>

#define BUFF_SIZE 65560

int main(int argc, char** argv)
{

    int nfrags = computeNumberOfFragments(BUFF_SIZE);
    size_t psize = BUFF_SIZE - UDP_HEADER_SIZE;

    printf("Number of Fragments: %d\n", nfrags);
    printf("Actual UDP Payload Size: %ld\n", psize);

    UdpPacket* udp = UdpPacket_new_v2(psize);
    UdpPacket_fillHeader(udp, 80, 80, BUFF_SIZE, 0x0);
    
    srand(0);

    unsigned char payload[10];
    for (int i = 0; i < 10; i++)
    {
        *(payload + i) = rand();
    }

    for (int i = 0; i < 10; i++) printf("%d\n", *(payload + i));

    return 0;
}