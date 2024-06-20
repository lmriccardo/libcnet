#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define IP_HEADER_SIZE 20

unsigned short compute_checksum(const unsigned char* buffer, long size)
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

    return (unsigned short)((~checksum) & 0xFFFF);
}

int main(int argc, char** argv)
{
    char filename[22] = "sent_60627_1_icmp.bin";
    
    FILE* fd;
    char* buffer;
    long  size;

    fd = fopen(filename, "rb");
    fseek(fd, 0, SEEK_END);
    size = ftell(fd);
    printf("Size: %ld\n", size);
    rewind(fd);

    buffer = (char*)malloc(size * sizeof(char));
    fread(buffer, size, 1, fd);
    fclose(fd);

    printf("Size ICMP Packet: %ld\n", size - IP_HEADER_SIZE);
    int checksum = compute_checksum(buffer + IP_HEADER_SIZE, size - IP_HEADER_SIZE);
    printf("Checksum: %#04x\n", checksum);

    return 0;
}