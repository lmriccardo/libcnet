#include "IcmpPacket.hpp"

void Packets::IcmpHeader::createHeaderMtu()
{
    // The fourth version of the header is used for MTU
    // Path Discovery. It has 16 bits set to zero, and
    // remaining 16 bits contains the MTU of the next hop
    this->_rest._mtu._unused = 0x0;
    this->_rest._mtu._mtu = 0x0;
}

void Packets::IcmpHeader::createHeaderEcho()
{
    // The third version of the header consists of Type, code
    // checksum, identification and sequence number.
    this->_rest._echo._id = 0;
    this->_rest._echo._seqnum = 0;
}

void Packets::IcmpHeader::createHeaderUnused()
{
    // The first version consists on the header with Type, code
    // checksum and the next 32 bit left unused.
    this->_rest._unused = 0x0;
}

void Packets::IcmpHeader::createHeaderRedirect()
{
    // The second version of the header consists of Type, code
    // checksum and 32 bit reserved to the gateway address.
    this->_rest._gateway = 0;
}

void Packets::IcmpHeader::printInfoRedirect()
{
    std::cout << "ICMP Gateway: " << this->_rest._gateway << std::endl;
}

void Packets::IcmpHeader::printInfoEcho()
{
    std::cout << "ICMP Identifier: "      << this->_rest._echo._id     << std::endl;
    std::cout << "ICMP Sequence Number: " << this->_rest._echo._seqnum << std::endl;
}

void Packets::IcmpHeader::printInfoMtu()
{
    std::cout << "ICMP Next Hop Mtu: " << this->_rest._mtu._mtu << std::endl;
}

Packets::IcmpHeader::IcmpHeader(const unsigned char _type, const unsigned char _code)
{
    bool correctType = false;

    if (_type == ICMP_REDIRECT_TYPE )
    {
        this->createHeaderRedirect();
        correctType = true;
    }

    if (
           _type == ICMP_DESTINATION_UNREACHABLE_TYPE
        || _type == ICMP_SOURCE_QUENCH_TYPE
        || _type == ICMP_TIME_EXCEEEDED_TYPE
    ) {
        if (
               _type == ICMP_DESTINATION_UNREACHABLE_TYPE
            && _code == ICMP_FRAGMENTATION_NEEDED_CODE
        ) {
            this->createHeaderMtu();
        }
        else
        {
            this->createHeaderUnused();
        }

        correctType = true;
    }

    if (
        _type == ICMP_ECHO_REPLY_TYPE          ||
        _type == ICMP_ECHO_TYPE                ||
        _type == ICMP_INFORMATION_REQUEST_TYPE ||
        _type == ICMP_INFORMATION_REPLY_TYPE
    ) {
        this->createHeaderEcho();
        correctType = true;
    }

    if (!correctType)
    {
        fprintf(
            stderr, "[IcmpHeader::new] ICMP Header Type %d do not exists!\n", 
            _type
        );

        throw std::runtime_error("[IcmpHeader::new] Error!\n");
    }

    this->setType(_type);
    this->setCode(_code);
    this->setChecksum(0);
}

void Packets::IcmpHeader::setType(const unsigned char _type)
{
    this->_type = _type;
}

unsigned char Packets::IcmpHeader::getType()
{
    return this->_type;
}

void Packets::IcmpHeader::setCode(const unsigned char _code)
{
    this->_code = _code;
}

unsigned char Packets::IcmpHeader::getCode()
{
    return this->_code;
}

void Packets::IcmpHeader::setChecksum(const unsigned short _checksum)
{
    this->_checksum = _checksum;
}

unsigned short Packets::IcmpHeader::getChecksum()
{
    return this->_checksum;
}

void Packets::IcmpHeader::setGateway(const unsigned int _gateway)
{
    if (this->_type != ICMP_REDIRECT_TYPE)
    {
        fprintf(stderr, "[IcmpHeader::setGateway] To set gateway address ");
        fprintf(stderr, "an ICMP message of Type %d is required. ", ICMP_REDIRECT_TYPE);
        fprintf(stderr, "Current Type is: %d.\n", this->_type);

        throw std::runtime_error("[IcmpHeader::setGateway] Error!\n");
    }

    this->_rest._gateway = _gateway;
}

unsigned int Packets::IcmpHeader::getGateway()
{
    return this->_rest._gateway;
}

void Packets::IcmpHeader::setIdentifier(const unsigned short _id)
{
    if (
        (
            this->_type != ICMP_ECHO_REPLY_TYPE          &&
            this->_type != ICMP_ECHO_TYPE                &&
            this->_type != ICMP_INFORMATION_REQUEST_TYPE &&
            this->_type != ICMP_INFORMATION_REPLY_TYPE
        )
    ) {
        fprintf(stderr, "[IcmpHeader::setIdentifier] To set Identifier Header field ");

        fprintf(
            stderr, "an ICMP message of Type %d/%d/%d/%d is required. ", 
            ICMP_ECHO_REPLY_TYPE, ICMP_ECHO_TYPE, ICMP_INFORMATION_REQUEST_TYPE,
            ICMP_INFORMATION_REPLY_TYPE
        );

        fprintf(stderr, "Current Type is: %d.\n", this->_type);

        throw std::runtime_error("[IcmpHeader::setIdentifier] Error!\n");
    }

    this->_rest._echo._id = _id;
}

unsigned short Packets::IcmpHeader::getIdentifier()
{
    return this->_rest._echo._id;
}

void Packets::IcmpHeader::setSequenceNumber(const unsigned short _sn)
{
    if (
        (
            this->_type != ICMP_ECHO_REPLY_TYPE          &&
            this->_type != ICMP_ECHO_TYPE                &&
            this->_type != ICMP_INFORMATION_REQUEST_TYPE &&
            this->_type != ICMP_INFORMATION_REPLY_TYPE
        )
    ) {
        fprintf(stderr, "[IcmpHeader::setIdentifier] To set Sequence Number Header field ");

        fprintf(
            stderr, "an ICMP message of Type %d/%d/%d/%d is required. ", 
            ICMP_ECHO_REPLY_TYPE, ICMP_ECHO_TYPE, ICMP_INFORMATION_REQUEST_TYPE,
            ICMP_INFORMATION_REPLY_TYPE
        );

        fprintf(stderr, "Current Type is: %d.\n", this->_type);

        throw std::runtime_error("[IcmpHeader::setIdentifier] Error!\n");
    }

    this->_rest._echo._seqnum = _sn;
}

unsigned short Packets::IcmpHeader::getSequenceNumber()
{
    return this->_rest._echo._seqnum;
}

void Packets::IcmpHeader::setNextHopMtu(const unsigned short _mtu)
{
    if (
        (
            this->_type != ICMP_DESTINATION_UNREACHABLE_TYPE ||
            this->_code != ICMP_FRAGMENTATION_NEEDED_CODE
        )
    ) {
        fprintf(stderr, "[IcmpHeader::setNextHopMtu] To set Next Hop MTU header field ");

        fprintf(
            stderr, "an ICMP Message of Type %d and Code %d is required. ",
            ICMP_DESTINATION_UNREACHABLE_TYPE, ICMP_FRAGMENTATION_NEEDED_CODE
        );

        fprintf(stderr, "Current Type and Code are: t=%d, c=%d\n", this->_type, this->_code);
        
        throw std::runtime_error("[IcmpHeader::setNextHopMtu] Error!\n");
    }

    this->_rest._mtu._mtu = _mtu;
}

unsigned short Packets::IcmpHeader::getNextHopMtu()
{
    return this->_rest._mtu._mtu;
}

void Packets::IcmpHeader::printInfo()
{
    std::cout << "[*] Printing Information of the ICMP Header" << std::endl;
    std::cout << "ICMP Message Type: " << this->_type          << std::endl;
    std::cout << "ICMP Message Code: " << this->_code          << std::endl;
    std::cout << "ICMP Checksum : "    << this->_checksum      << std::endl;

    if (this->_type == ICMP_REDIRECT_TYPE)
    {
        this->printInfoRedirect();
    }

    if (
        this->_type == ICMP_ECHO_REPLY_TYPE          ||
        this->_type == ICMP_ECHO_TYPE                ||
        this->_type == ICMP_INFORMATION_REQUEST_TYPE ||
        this->_type == ICMP_INFORMATION_REPLY_TYPE
    ) {
        this->printInfoEcho();
    }

    if (
        this->_type == ICMP_DESTINATION_UNREACHABLE_TYPE &&
        this->_code == ICMP_FRAGMENTATION_NEEDED_CODE
    ) {
        this->printInfoMtu();
    }

    std::cout << std::endl;
}

void Packets::IcmpHeader::encode(Utils::ByteBuffer &_buffer)
{
    _buffer.put(this->_type);
    _buffer.put(this->_code);
    _buffer.putShort(htons(this->_checksum));

    if (
        (
            this->_type == ICMP_DESTINATION_UNREACHABLE_TYPE ||
            this->_type == ICMP_SOURCE_QUENCH_TYPE           ||
            this->_type == ICMP_TIME_EXCEEEDED_TYPE
        )
    ) {
        // Check for Code in case of Destination Unreachable type
        if (
            (
                this->_type == ICMP_DESTINATION_UNREACHABLE_TYPE &&
                this->_code == ICMP_FRAGMENTATION_NEEDED_CODE
            )
        ) {
            _buffer.putShort(htons(this->_rest._mtu._unused));
            _buffer.putShort(htons(this->_rest._mtu._mtu));
        }
        else
        {
            _buffer.putInt(htonl(this->_rest._unused));
        }
    }

    if (this->_type == ICMP_REDIRECT_TYPE)
    {
        _buffer.putInt(htonl(this->_rest._gateway));
    }

    if (
        (
            this->_type == ICMP_ECHO_REPLY_TYPE          ||
            this->_type == ICMP_ECHO_TYPE                ||
            this->_type == ICMP_INFORMATION_REQUEST_TYPE ||
            this->_type == ICMP_INFORMATION_REPLY_TYPE
        )
    ) {
        _buffer.putShort(htons(this->_rest._echo._id));
        _buffer.putShort(htons(this->_rest._echo._seqnum));
    }
}

Utils::ByteBuffer_ptr Packets::IcmpHeader::encode()
{
    Utils::ByteBuffer buff(ICMP_HEADER_MAX_SIZE);
    this->encode(buff);
    return std::make_shared<Utils::ByteBuffer>(buff);
}

void Packets::IcmpHeader::decode(Utils::ByteBuffer &_buffer)
{
    unsigned char type = _buffer.get();
    unsigned char code = _buffer.get();
    unsigned short checksum = _buffer.getShort();

    this->setType(type);
    this->setCode(code);
    this->setChecksum(checksum);

    if (
        (
            this->_type == ICMP_DESTINATION_UNREACHABLE_TYPE ||
            this->_type == ICMP_SOURCE_QUENCH_TYPE           ||
            this->_type == ICMP_TIME_EXCEEEDED_TYPE
        )
    ) {
        // Check for Code in case of Destination Unreachable type
        if (
            (
                this->_type == ICMP_DESTINATION_UNREACHABLE_TYPE &&
                this->_code == ICMP_FRAGMENTATION_NEEDED_CODE
            )
        ) {
            _buffer.position(_buffer.getCurrentPosition() + Utils::ByteBuffer::SHORT_SIZE);
            u_int16_t mtu = _buffer.getShort();

            this->_rest._mtu._unused = 0x0;
            this->setNextHopMtu(ntohs(mtu));
        }
        else
        {
            _buffer.position(_buffer.getCurrentPosition() + Utils::ByteBuffer::INT_SIZE);
            this->_rest._unused = 0x0;
        }
    }

    if (this->_type == ICMP_REDIRECT_TYPE)
    {
        u_int32_t gateway = _buffer.getInt();
        this->setGateway(ntohl(gateway));
    }

    if (
        (
            this->_type == ICMP_ECHO_REPLY_TYPE          ||
            this->_type == ICMP_ECHO_TYPE                ||
            this->_type == ICMP_INFORMATION_REQUEST_TYPE ||
            this->_type == ICMP_INFORMATION_REPLY_TYPE
        )
    ) {
        u_int16_t id = _buffer.getShort();
        u_int16_t seqnum = _buffer.getShort();

        this->setIdentifier(ntohs(id));
        this->setSequenceNumber(ntohs(seqnum));
    }
}
