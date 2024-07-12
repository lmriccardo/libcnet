#include "Sender.hpp"

Communication::Sender::Sender(
    const std::string _interface, const std::string _dstaddr, char *_gateway, 
    const unsigned short _dstport, const std::string _proto, const bool _verbose
) : _mlock(_mutex, std::defer_lock) 
{
    struct sockaddr_in dst;
    int socketfd;

    struct protoent *proto = getprotobyname(_proto.c_str());
    socketfd = socket(AF_INET, SOCK_RAW, proto->p_proto);
    if (socketfd == -1)
    {
        throw std::runtime_error("[Sender] Impossible to create a socket");
    }

    int value = 1;
    setsockopt(socketfd, IPPROTO_IP, IP_HDRINCL, &value, sizeof(value));

    /* Connect to the destination socket */
    memset(&dst, 0, sizeof(dst));
    dst.sin_family = AF_INET;
    dst.sin_port = htons(_dstport);
    inet_pton(AF_INET, _dstaddr.c_str(), &dst.sin_addr);

    this->_srcaddress = Utils::NetUtil::getInterfaceIp(_interface);
    this->_dstaddress = dst;
    this->_gateway = _gateway;
    this->_socketfd = socketfd;
    this->_msgcnt = 0;
    this->_proto = proto;
    this->_verbose = _verbose;
    this->_timer = nullptr;
    this->_mtu = Utils::NetUtil::getInterfaceMtu(_interface);
    this->_synch = false;
    this->_sent = false;

    // Set default values for Parameters structure
    this->_icmpp._sn = 0;
    this->_icmpp._id = 1;
}

void Communication::Sender::setTimer(Utils::Timer_ptr &_timer)
{
    this->_timer = _timer;
}

void Communication::Sender::setMtu(const int _mtu)
{
    this->_mtu = _mtu;
}

void Communication::Sender::setIpFlags(const int _d, const int _m)
{
    if (_d == D_FLAG_SET && _m == M_FLAG_SET)
    {
        std::cerr << "[Sender::setIpFlags] Cannot set both D and M flags to 1. " << std::endl;
        std::cerr << "Program will not terminate but flags will not be set." << std::endl;
    }

    if (
        (_d != D_FLAG_NOT_SET && _d != D_FLAG_SET) &&
        (_m != M_FLAG_NOT_SET && _m != M_FLAG_SET)
    ) {
        std::cerr << "[Sender::setIpFlags] M and D flags must have correct values\n" << std::endl;
        std::cerr << "Program will not terminate but flags will not be set.\n" << std::endl;
    }
    
    this->_ipp._xf = X_FLAG_NOT_SET;
    this->_ipp._id = _d;
    this->_ipp._mf = _m;
}

void Communication::Sender::sendTo(Packets::IpPacket &_pckt)
{
    this->_sent = false;

    Utils::ByteBuffer_ptr buff = _pckt.encode();

    // Synchronize Sender and receiver if necessarily
    if (this->_synch)
    {
        // Aquire the lock and wait to proceed
        this->_mlock.lock();
        this->_cond.wait(this->_mlock);
    }
    
    // Create the char buffer to be sent using the overloaded method
    buff->position(0);

    const std::size_t buffSize = buff->getBufferSize();

    char* buffer = new char[buffSize];
    buff->getBuffer((unsigned char*)buffer, buffSize);
    this->sendTo(buffer, buffSize);
    delete(buffer);

    // Increase the message counter
    this->_msgcnt++;

    // If verbosity is set, save the sent buffer into a file
    if (this->_verbose)
    {
        char filename[100];

        snprintf(
            filename, 60, "sent_%hu_%d_%s.bin", this->_ipp._id, 
            this->_msgcnt, this->_proto->p_name
        );
        
        buff->writeToFile(filename);
    }

    // Release the lock and notify the receiver
    this->_sent = true;

    if (this->_synch)
    {
        this->_mlock.unlock();
        this->_cond.notify_all();
    }
}

void Communication::Sender::sendTo(const char *_buffer, const std::size_t _size)
{
    socklen_t dstlen;
    dstlen = sizeof(struct sockaddr_in);
    
    if (
        sendto(
            this->_socketfd, _buffer, _size, 0, 
            (struct sockaddr *)&this->_dstaddress, dstlen
        ) < 0
    ) {
        throw std::runtime_error("[Sender::sendTo] Impossible to send the input buffer.");
    }

    if (this->_verbose) printf("[*] Sent %ld bytes of IP Packet\n", _size);
}
