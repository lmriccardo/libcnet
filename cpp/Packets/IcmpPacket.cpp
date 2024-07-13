#include "IcmpPacket.hpp"

using namespace Packets::Icmp;

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

Packets::IcmpHeader::IcmpHeader(const Icmp::Type _type, const Icmp::Code _code)
{
    bool correctType = false;

    if (_type == Type::REDIRECT )
    {
        this->createHeaderRedirect();
        correctType = true;
    }

    if (
           _type == Type::DESTINATION_UNREACHABLE
        || _type == Type::SOURCE_QUENCH
        || _type == Type::TIME_EXCEEEDED
    ) {
        if (
               _type == Type::DESTINATION_UNREACHABLE
            && _code == Code::FRAGMENTATION_NEEDED
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
        _type == Type::ECHO_REPLY          ||
        _type == Type::ECHO                ||
        _type == Type::INFORMATION_REQUEST ||
        _type == Type::INFORMATION_REPLY
    ) {
        this->createHeaderEcho();
        correctType = true;
    }

    if (!correctType)
    {
        fprintf(
            stderr, "[IcmpHeader::new] ICMP Header Type %d do not exists!\n", 
            IcmpHeader::cast<int>(_type)
        );

        throw std::runtime_error("[IcmpHeader::new] Error!\n");
    }

    this->setType(_type);
    this->setCode(_code);
    this->setChecksum(0);
}

void Packets::IcmpHeader::setType(const Type _type)
{
    this->_type = _type;
}

Type Packets::IcmpHeader::getType()
{
    return this->_type;
}

void Packets::IcmpHeader::setCode(const Code _code)
{
    this->_code = _code;
}

Code Packets::IcmpHeader::getCode()
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
    if (this->_type != Type::REDIRECT)
    {
        fprintf(stderr, "[IcmpHeader::setGateway] To set gateway address ");
        fprintf(stderr, "an ICMP message of Type %d is required. ", IcmpHeader::cast<int>(Type::REDIRECT));
        fprintf(stderr, "Current Type is: %d.\n", IcmpHeader::cast<int>(this->_type));

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
            this->_type != Type::ECHO_REPLY          &&
            this->_type != Type::ECHO                &&
            this->_type != Type::INFORMATION_REQUEST &&
            this->_type != Type::INFORMATION_REPLY
        )
    ) {
        fprintf(stderr, "[IcmpHeader::setIdentifier] To set Identifier Header field ");

        fprintf(
            stderr, "an ICMP message of Type %d/%d/%d/%d is required. ", 
            IcmpHeader::cast<int>(Type::ECHO_REPLY), IcmpHeader::cast<int>(Type::ECHO), 
            IcmpHeader::cast<int>(Type::INFORMATION_REQUEST), 
            IcmpHeader::cast<int>(Type::INFORMATION_REPLY)
        );

        fprintf(stderr, "Current Type is: %d.\n", IcmpHeader::cast<int>(this->_type));

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
            this->_type != Type::ECHO_REPLY          &&
            this->_type != Type::ECHO                &&
            this->_type != Type::INFORMATION_REQUEST &&
            this->_type != Type::INFORMATION_REPLY
        )
    ) {
        fprintf(stderr, "[IcmpHeader::setIdentifier] To set Sequence Number Header field ");

        fprintf(
            stderr, "an ICMP message of Type %d/%d/%d/%d is required. ", 
            IcmpHeader::cast<int>(Type::ECHO_REPLY), IcmpHeader::cast<int>(Type::ECHO), 
            IcmpHeader::cast<int>(Type::INFORMATION_REQUEST), 
            IcmpHeader::cast<int>(Type::INFORMATION_REPLY)
        );

        fprintf(stderr, "Current Type is: %d.\n", IcmpHeader::cast<int>(this->_type));

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
            this->_type != Type::DESTINATION_UNREACHABLE ||
            this->_code != Code::FRAGMENTATION_NEEDED
        )
    ) {
        fprintf(stderr, "[IcmpHeader::setNextHopMtu] To set Next Hop MTU header field ");

        fprintf(
            stderr, "an ICMP Message of Type %d and Code %d is required. ",
            IcmpHeader::cast<int>(Type::DESTINATION_UNREACHABLE),
            IcmpHeader::cast<int>(Code::FRAGMENTATION_NEEDED)
        );

        fprintf(
            stderr, "Current Type and Code are: t=%d, c=%d\n", 
            IcmpHeader::cast<int>(this->_type),
            IcmpHeader::cast<int>(this->_code)
        );
        
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
    std::cout << "ICMP Message Type: " << IcmpHeader::cast<unsigned char>(this->_type) << std::endl;
    std::cout << "ICMP Message Code: " << IcmpHeader::cast<unsigned char>(this->_code) << std::endl;
    std::cout << "ICMP Checksum : "    << this->_checksum                              << std::endl;

    if (this->_type == Type::REDIRECT)
    {
        this->printInfoRedirect();
    }

    if (
        this->_type == Type::ECHO_REPLY          ||
        this->_type == Type::ECHO                ||
        this->_type == Type::INFORMATION_REQUEST ||
        this->_type == Type::INFORMATION_REPLY
    ) {
        this->printInfoEcho();
    }

    if (
        this->_type == Type::DESTINATION_UNREACHABLE &&
        this->_code == Code::FRAGMENTATION_NEEDED
    ) {
        this->printInfoMtu();
    }

    std::cout << std::endl;
}

void Packets::IcmpHeader::encode(Utils::ByteBuffer &_buffer)
{
    _buffer.put(static_cast<unsigned char>(this->_type));
    _buffer.put(static_cast<unsigned char>(this->_code));
    _buffer.putShort(htons(this->_checksum));

    if (
        (
            this->_type == Type::DESTINATION_UNREACHABLE ||
            this->_type == Type::SOURCE_QUENCH           ||
            this->_type == Type::TIME_EXCEEEDED
        )
    ) {
        // Check for Code in case of Destination Unreachable type
        if (
            (
                this->_type == Type::DESTINATION_UNREACHABLE &&
                this->_code == Code::FRAGMENTATION_NEEDED
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

    if (this->_type == Type::REDIRECT)
    {
        _buffer.putInt(htonl(this->_rest._gateway));
    }

    if (
        (
            this->_type == Type::ECHO_REPLY          ||
            this->_type == Type::ECHO                ||
            this->_type == Type::INFORMATION_REQUEST ||
            this->_type == Type::INFORMATION_REPLY
        )
    ) {
        _buffer.putShort(htons(this->_rest._echo._id));
        _buffer.putShort(htons(this->_rest._echo._seqnum));
    }
}

Utils::ByteBuffer_ptr Packets::IcmpHeader::encode()
{
    Utils::ByteBuffer buff(IcmpHeader::size);
    this->encode(buff);
    return std::make_shared<Utils::ByteBuffer>(buff);
}

void Packets::IcmpHeader::decode(Utils::ByteBuffer &_buffer)
{
    unsigned char type = _buffer.get();
    unsigned char code = _buffer.get();
    unsigned short checksum = _buffer.getShort();

    this->setType(static_cast<Type>(type));
    this->setCode(static_cast<Code>(code));
    this->setChecksum(checksum);

    if (
        (
            this->_type == Type::DESTINATION_UNREACHABLE ||
            this->_type == Type::SOURCE_QUENCH           ||
            this->_type == Type::TIME_EXCEEEDED
        )
    ) {
        // Check for Code in case of Destination Unreachable type
        if (
            (
                this->_type == Type::DESTINATION_UNREACHABLE &&
                this->_code == Code::FRAGMENTATION_NEEDED
            )
        ) {
            _buffer.position(_buffer.position() + Utils::ByteBuffer::SHORT_SIZE);
            u_int16_t mtu = _buffer.getShort();

            this->_rest._mtu._unused = 0x0;
            this->setNextHopMtu(ntohs(mtu));
        }
        else
        {
            _buffer.position(_buffer.position() + Utils::ByteBuffer::INT_SIZE);
            this->_rest._unused = 0x0;
        }
    }

    if (this->_type == Type::REDIRECT)
    {
        u_int32_t gateway = _buffer.getInt();
        this->setGateway(ntohl(gateway));
    }

    if (
        (
            this->_type == Type::ECHO_REPLY          ||
            this->_type == Type::ECHO                ||
            this->_type == Type::INFORMATION_REQUEST ||
            this->_type == Type::INFORMATION_REPLY
        )
    ) {
        u_int16_t id = _buffer.getShort();
        u_int16_t seqnum = _buffer.getShort();

        this->setIdentifier(ntohs(id));
        this->setSequenceNumber(ntohs(seqnum));
    }
}

void Packets::IcmpPacket::fillHeaderUnused(const unsigned short _checksum)
{
    this->_hdr.setChecksum(_checksum);
}

void Packets::IcmpPacket::fillHeaderRedirect(const unsigned short _checksum, const unsigned int _gateway)
{
    this->fillHeaderUnused(_checksum);
    this->_hdr.setGateway(_gateway);
}

void Packets::IcmpPacket::fillHeaderEcho(
    const unsigned short _checksum, const unsigned short _id, const unsigned short _seqnum
) {
    this->fillHeaderUnused(_checksum);
    this->_hdr.setIdentifier(_id);
    this->_hdr.setSequenceNumber(_seqnum);
}

void Packets::IcmpPacket::fillHeaderMtu(const unsigned short _checksum, const unsigned short _mtu)
{
    this->fillHeaderUnused(_checksum);
    this->_hdr.setNextHopMtu(_mtu);
}

Packets::PacketHeader *Packets::IcmpPacket::getPacketHeader()
{
    return &this->_hdr;
}

std::size_t Packets::IcmpPacket::getPacketSize()
{
    return this->getPayloadSize() + IcmpHeader::size;
}

const std::size_t& Packets::IcmpPacket::getPayloadSize()
{
    return this->_payload->getBufferSize();
}

void Packets::IcmpPacket::setHeader(PacketHeader *_hdr)
{
    IcmpHeader hdr = *PacketHeader::header_cast<IcmpHeader>(_hdr);

    // The first 4 bytes are the same for every possible ICMP Header
    this->_hdr.setType(hdr.getType());
    this->_hdr.setCode(hdr.getCode());

    unsigned short checksum = hdr.getChecksum();

    // The remaining 4 bytes depends on the Header
    if (this->_hdr.getType() == Type::REDIRECT)
    {
        this->fillHeaderRedirect(checksum, hdr.getGateway());
    }

    if (
        (
               this->_hdr.getType() == Type::ECHO_REPLY
            || this->_hdr.getType() == Type::ECHO
            || this->_hdr.getType() == Type::INFORMATION_REQUEST
            || this->_hdr.getType() == Type::INFORMATION_REPLY
        )
    ) {
        this->fillHeaderEcho(checksum, hdr.getIdentifier(), hdr.getSequenceNumber());
    }

    if (
        (
               this->_hdr.getType() == Type::DESTINATION_UNREACHABLE
            && this->_hdr.getCode() == Code::FRAGMENTATION_NEEDED
        )
    ) {
        this->fillHeaderMtu(checksum, hdr.getNextHopMtu());
    }
}

void Packets::IcmpPacket::setPayload(unsigned char *_payload, const std::size_t _size)
{
    std::vector<unsigned char> _buff(_payload, _payload + _size);
    this->_payload->copy(_buff);
}

void Packets::IcmpPacket::setPayload(Utils::ByteBuffer &_buff)
{
    *this->_payload = _buff;
}

void Packets::IcmpPacket::encode(Utils::ByteBuffer &_buffer)
{
    // First we need to encode the header
    this->_hdr.encode(_buffer);
    
    // Then we can encode the payload
    _buffer.merge(*this->_payload);
}

Utils::ByteBuffer_ptr Packets::IcmpPacket::encode()
{
    Utils::ByteBuffer_ptr buff = std::make_shared<Utils::ByteBuffer>(this->getPacketSize());
    this->encode(*buff);
    return buff;
}

void Packets::IcmpPacket::decode(Utils::ByteBuffer &_buffer)
{
    // First decode the header
    this->_hdr.decode(_buffer);

    // Then we need to decode the Payload
    const std::vector<unsigned char> _payload = _buffer.getBuffer();
    this->_payload->copy(_payload, _buffer.position()); 
}
