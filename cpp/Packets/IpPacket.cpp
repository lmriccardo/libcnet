#include "IpPacket.hpp"

void Packets::IpHeader::setVersion(const Ip::Version _version)
{
    this->_version = _version;
}

void Packets::IpHeader::setVersion(const unsigned char _version)
{
    this->_version = Enum::cast<unsigned char, Ip::Version>(_version);
}

const Packets::Ip::Version &Packets::IpHeader::getVersion()
{
    return this->_version;
}

void Packets::IpHeader::setDifferentiatedServiceField(const Ip::DSCP _dscp, const Ip::ECN _ecn)
{
    this->_dscp = _dscp;
    this->_ecn  = _ecn;

    unsigned short dscp = Enum::cast<Ip::DSCP, unsigned short>(_dscp);
    unsigned short ecn = Enum::cast<Ip::ECN, unsigned short>(_ecn);

    this->_dsf = (dscp << 4) + ecn;
}

void Packets::IpHeader::setDifferentiatedServiceField(const unsigned short _dscp, const unsigned short _ecn)
{
    this->_dscp = Enum::cast<unsigned short, Ip::DSCP>(_dscp);
    this->_ecn = Enum::cast<unsigned short, Ip::ECN>(_ecn);
    this->_dsf = (_dscp << 2) + _ecn;
}

void Packets::IpHeader::setDifferentiatedServiceField(const unsigned char _dsf)
{
    this->_dsf = _dsf;
    
    unsigned short dscp = _dsf >> 2;
    unsigned short ecn = _dsf & ~(dscp << 2);
    
    this->_dscp = Enum::cast<unsigned short, Ip::DSCP>(dscp);
    this->_ecn = Enum::cast<unsigned short, Ip::ECN>(ecn);
}

unsigned char Packets::IpHeader::getDifferentiatedServiceField()
{
    return this->_dsf;
}

const Packets::Ip::DSCP &Packets::IpHeader::getDSCP()
{
    return this->_dscp;
}

const Packets::Ip::ECN &Packets::IpHeader::getECN()
{
    return this->_ecn;
}

void Packets::IpHeader::setTotalLength(const unsigned short _tlength)
{
    this->_tlength = _tlength;
}

unsigned short Packets::IpHeader::getTotalLength()
{
    return this->_tlength;
}

void Packets::IpHeader::setIdentifier(const unsigned short _id)
{
    this->_id = _id;
}

unsigned short Packets::IpHeader::getIdentifier()
{
    return this->_id;
}

void Packets::IpHeader::setFragmentationFlags(const Ip::Flag _d, const Ip::Flag _m)
{
    unsigned char d = Enum::cast<Ip::Flag, unsigned char>(_d);
    unsigned char m = Enum::cast<Ip::Flag, unsigned char>(_m);

    this->setFragmentationFlags(d, m);
}

void Packets::IpHeader::setFragmentationFlags(const unsigned char _d, const unsigned char _m)
{
    if (
        (
               _d != Enum::cast<Ip::Flag, unsigned char>(Ip::Flag::D_FLAG_NOT_SET)
            && _d != Enum::cast<Ip::Flag, unsigned char>(Ip::Flag::D_FLAG_SET)
        )
        ||
        (
               _m != Enum::cast<Ip::Flag, unsigned char>(Ip::Flag::M_FLAG_NOT_SET)
            && _m != Enum::cast<Ip::Flag, unsigned char>(Ip::Flag::M_FLAG_SET)
        )
    ) {
        throw std::runtime_error(
            "[IpHeader::setFragmentationFlags] Invalid values for D and/or M flag(s)");
    }

    this->_flags = _d + _m;
}

unsigned char Packets::IpHeader::getFragmentationFlags()
{
    return this->_flags;
}

std::string Packets::IpHeader::convertFlagsToBinaryString()
{
    int _d = ((this->_flags >> 1) & 1);
    int _m = (this->_flags & 1);

    char flags[4];
    snprintf(flags, 4, "0%d%d", _d, _m);

    std::string flag_str(flags);
    return flag_str;
}

void Packets::IpHeader::setFragmentOffset(unsigned short _offset)
{
    this->_offset = _offset;
}

unsigned short Packets::IpHeader::getFragmentOffset()
{
    return this->_offset;
}

void Packets::IpHeader::setTimeToLive(unsigned char _ttl)
{
    this->_ttl = _ttl;
}

unsigned char Packets::IpHeader::getTimeToLive()
{
    return this->_ttl;
}

void Packets::IpHeader::setProtocol(const Ip::Protocol _proto)
{
    this->_proto = _proto;
}

void Packets::IpHeader::setProtocol(const unsigned char _proto)
{
    this->_proto = Enum::cast<unsigned char, Ip::Protocol>(_proto);
}

const Packets::Ip::Protocol &Packets::IpHeader::getProtocol()
{
    return this->_proto;
}

void Packets::IpHeader::setChecksum(const unsigned short _checksum)
{
    this->_checksum = _checksum;
}

unsigned short Packets::IpHeader::getChecksum()
{
    return this->_checksum;
}

void Packets::IpHeader::setSourceAddress(const unsigned int _srcaddr)
{
    this->_srcaddr = _srcaddr;
}

unsigned int Packets::IpHeader::getSourceAddress()
{
    return this->_srcaddr;
}

void Packets::IpHeader::setDestinationAddress(const unsigned int _dstaddr)
{
    this->_dstaddr = _dstaddr;
}

unsigned int Packets::IpHeader::getDestinationAddress()
{
    return this->_dstaddr;
}

void Packets::IpHeader::printInfo()
{
    std::string flag = this->convertFlagsToBinaryString();
    std::string srcaddr = Utils::NetUtil::addressNumberToString(this->_srcaddr, false);
    std::string dstaddr = Utils::NetUtil::addressNumberToString(this->_dstaddr, false);

    std::cout << "[*] Print IP Header Field Values" << std::endl;
    std::cout << "Ip Version: " << Enum::cast<Ip::Version, unsigned char>(this->_version) << std::endl;
    std::cout << "Internet Header Length: 5" << std::endl;
    std::cout << "Distributed Service Code Point: " << Enum::cast<Ip::DSCP, unsigned int>(this->_dscp) << std::endl;
    std::cout << "Explicit Congestion Network: " << Enum::cast<Ip::ECN, unsigned int>(this->_ecn) << std::endl;
}
