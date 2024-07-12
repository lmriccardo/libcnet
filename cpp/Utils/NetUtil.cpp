#include "NetUtil.hpp"

std::string Utils::NetUtil::addressNumberToString(unsigned int _addr, const bool _be)
{
    // If it is in Little-Endian, then convert it to Big-Endian
    if (!_be) _addr = htonl(_addr);
    
    char addr[INET_ADDRSTRLEN];
    inet_ntop(AF_INET, &_addr, addr, INET_ADDRSTRLEN);
    return std::string(addr);
}

std::string Utils::NetUtil::getHostnameIp(const std::string& _hostname)
{
    struct hostent *hostentry = gethostbyname(_hostname.c_str());
    if (hostentry == NULL)
    {
        throw std::runtime_error("[NetUtil::getHostnameIp] GetHostByName call failed.");
    }

    return NetUtil::addressNumberToString(
        (*((struct in_addr*) hostentry->h_addr_list[0])).s_addr, true);
}

std::string Utils::NetUtil::getInterfaceIp(const std::string& _interface)
{
    int sockfd = socket(AF_INET, SOCK_DGRAM, 0);
    struct ifreq ifr;

    ifr.ifr_addr.sa_family = AF_INET;
    strncpy(ifr.ifr_name , _interface.c_str(), IFNAMSIZ - 1);
    ioctl(sockfd, SIOCGIFADDR, &ifr);
    close(sockfd);

    u_int32_t _addr_i = ( (struct sockaddr_in *)&ifr.ifr_addr )->sin_addr.s_addr;
    return NetUtil::addressNumberToString(_addr_i, false);
}

int Utils::NetUtil::getInterfaceMtu(const std::string& _interface)
{
    int sockfd = socket(AF_INET, SOCK_DGRAM, 0);
    struct ifreq ifr;

    ifr.ifr_addr.sa_family = AF_INET;
    strncpy(ifr.ifr_name , _interface.c_str(), IFNAMSIZ - 1);
    if (ioctl(sockfd, SIOCGIFMTU, &ifr) < 0)
    {
        throw std::runtime_error("[NetUtil::getInterfaceMtu] Error getting MTU for given interface.");
    }

    close(sockfd);

    return ifr.ifr_mtu;
}

unsigned short Utils::NetUtil::computeChecksum(ByteBuffer &_buffer)
{
    int checksum = 0;

    if (_buffer.isEmpty()) return static_cast<unsigned short>(checksum);

    while (!_buffer.isEndOfBuffer())
    {
        if (_buffer.getBufferSize() - _buffer.position() == 1)
        {
            checksum += (_buffer.get() << 8);
            continue;
        }

        checksum += (_buffer.get() << 8) + _buffer.get();
    }

    checksum = (checksum >> 16) + (checksum & 0xFFFF);
    checksum += checksum >> 16;

    return static_cast<unsigned short>((~checksum) & 0xFFFF);
}
