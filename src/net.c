#include "utils/net.h"
#include "sender.h"
#include "receiver.h"

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

void generateRandomData(const size_t _size, char* _dst)
{
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