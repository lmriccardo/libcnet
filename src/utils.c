#include "utils.h"

char* getHostnameIP(const char* _hostname)
{
    struct hostent *hostentry = gethostbyname(_hostname);
    if (hostentry == NULL)
    {
        fprintf(stderr, "[getHostName_s] GetHostByName call failed.\n");
        exit(EXIT_FAILURE);
    }

    char *ip = inet_ntoa(*((struct in_addr*) hostentry->h_addr_list[0]));

    return ip;
}

char* getInterfaceIp(const char* _interface)
{
    int sockfd = socket(AF_INET, SOCK_DGRAM, 0);
    struct ifreq ifr;

    ifr.ifr_addr.sa_family = AF_INET;
    strncpy(ifr.ifr_name , _interface , IFNAMSIZ - 1);
    ioctl(sockfd, SIOCGIFADDR, &ifr);
    shutdown(sockfd, 2);

    char* _address = inet_ntoa(( (struct sockaddr_in *)&ifr.ifr_addr )->sin_addr);
    return _address;
}