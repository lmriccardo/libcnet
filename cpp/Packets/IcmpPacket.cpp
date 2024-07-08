#include "IcmpPacket.hpp"

void Ip::IcmpHeader::createHeaderMtu()
{
    // The fourth version of the header is used for MTU
    // Path Discovery. It has 16 bits set to zero, and
    // remaining 16 bits contains the MTU of the next hop
    this->_rest._mtu._unused = 0x0;
    this->_rest._mtu._mtu = 0x0;
}

void Ip::IcmpHeader::createHeaderEcho()
{
    // The third version of the header consists of Type, code
    // checksum, identification and sequence number.
    this->_rest._echo._id = 0;
    this->_rest._echo._seqnum = 0;
}

void Ip::IcmpHeader::createHeaderUnused()
{
    // The first version consists on the header with Type, code
    // checksum and the next 32 bit left unused.
    this->_rest._unused = 0x0;
}

void Ip::IcmpHeader::createHeaderRedirect()
{
    // The second version of the header consists of Type, code
    // checksum and 32 bit reserved to the gateway address.
    this->_rest._gateway = 0;
}

Ip::IcmpHeader::IcmpHeader(const unsigned char _type, const unsigned char _code)
{
    switch (_type)
    {
        case ICMP_DESTINATION_UNREACHABLE_TYPE:
            if ( _code == ICMP_FRAGMENTATION_NEEDED_CODE )
            {
                this->createHeaderMtu();
                break;
            }

        case ICMP_SOURCE_QUENCH_TYPE:
        case ICMP_TIME_EXCEEEDED_TYPE:
            this->createHeaderUnused();
            break;

        case ICMP_REDIRECT_TYPE:
            this->createHeaderRedirect();
            break;

        case ICMP_ECHO_REPLY_TYPE:
        case ICMP_ECHO_TYPE:
        case ICMP_INFORMATION_REQUEST_TYPE:
        case ICMP_INFORMATION_REPLY_TYPE:
            this->createHeaderEcho();
            break;
        
        default:
            char buff[200];
            snprintf(
                buff, sizeof(buff), 
                "[IcmpHeader_new] ICMP Header Type %d does not exists!\n", 
                _type
            );

            std::string message = buff;
            throw std::runtime_error(message);
    }
}

void Ip::IcmpHeader::setType(unsigned char _type)
{
    this->_type = _type;
}

unsigned char Ip::IcmpHeader::getType()
{
    return this->_type;
}

void Ip::IcmpHeader::setCode(unsigned char _code)
{
    this->_code = _code;
}

unsigned char Ip::IcmpHeader::getCode()
{
    return this->_code;
}
