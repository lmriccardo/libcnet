#include "ByteBuffer.hpp"

Utils::ByteBuffer::ByteBuffer(const std::size_t _size)
{
    this->_size = _size;
    this->_position = 0;
    this->_buffer.reserve(_size);
}

void Utils::ByteBuffer::position(const int _newpos)
{
    this->_position = _newpos;
}

void Utils::ByteBuffer::resetPosition()
{
    this->_position = 0;
}

void Utils::ByteBuffer::put(const unsigned char _data)
{
    try
    {
        ByteBuffer::checkForOutOfBound(this->_position, ByteBuffer::BYTE_SIZE, this->_size, "ByteBuffer::put");

        this->_buffer.push_back(_data);
        this->position(this->_position + ByteBuffer::BYTE_SIZE);
    }
    catch(const std::out_of_range& oob)
    {
        std::cerr << oob.what() << '\n';
        exit(EXIT_FAILURE);
    }   
}

void Utils::ByteBuffer::putShort(const unsigned short _data)
{
    try
    {
        ByteBuffer::checkForOutOfBound(this->_position, ByteBuffer::SHORT_SIZE, this->_size, "ByteBuffer::putShort");

        unsigned char _datau = _data >> 8;
        unsigned char _datal = _data & ~(_datau << 8);
        
        this->put(_datau);
        this->put(_datal);
    }
    catch(const std::out_of_range& oob)
    {
        std::cerr << oob.what() << '\n';
        exit(EXIT_FAILURE);
    }
}

void Utils::ByteBuffer::putInt(const unsigned int _data)
{
    try
    {
        ByteBuffer::checkForOutOfBound(this->_position, ByteBuffer::INT_SIZE, this->_size, "ByteBuffer::putInt");

        unsigned short _datau = _data >> 16;
        unsigned short _datal = _data & ~(_datau << 16);
        
        this->putShort(_datau);
        this->putShort(_datal);
    }
    catch(const std::out_of_range& oob)
    {
        std::cerr << oob.what() << '\n';
        exit(EXIT_FAILURE);
    }
}

void Utils::ByteBuffer::putBuffer(unsigned char* const& _data, const int _start, const std::size_t _size)
{
    try
    {
        this->position(_start);
        ByteBuffer::checkForOutOfBound(this->_position, _size, this->_size, "ByteBuffer::putBufferFrom");

        for (std::size_t pos = 0; pos < _size; pos++)
        {
            this->put(_data[pos]);
        }
    }
    catch(const std::out_of_range& oob)
    {
        std::cerr << oob.what() << '\n';
        exit(EXIT_FAILURE);
    }
    
}

void Utils::ByteBuffer::putBuffer(unsigned char* const& _data, const std::size_t _size)
{
    this->putBuffer(_data, this->_position, _size);
}

unsigned char Utils::ByteBuffer::get()
{
    try
    {
        ByteBuffer::checkForOutOfBound(this->_position, ByteBuffer::BYTE_SIZE, this->_size, "ByteBuffer::get");
        ByteBuffer::errorIfEmpty(this, "ByteBuffer::get");

        unsigned char ret = this->_buffer.at(this->_position);
        this->position(this->_position + ByteBuffer::BYTE_SIZE);

        return ret;
    }
    catch(const std::out_of_range& oob)
    {
        std::cerr << oob.what() << '\n';
        exit(EXIT_FAILURE);
    }
    catch(const std::runtime_error& re)
    {
        std::cerr << re.what() << '\n';
        exit(EXIT_FAILURE);
    }
}

unsigned short Utils::ByteBuffer::getShort()
{
    try
    {
        ByteBuffer::checkForOutOfBound(this->_position, ByteBuffer::SHORT_SIZE, this->_size, "ByteBuffer::getShort");
        ByteBuffer::errorIfEmpty(this, "ByteBuffer::getShort");

        unsigned char ret1 = this->get();
        unsigned char ret2 = this->get();
        unsigned short ret = (ret1 << 8) + ret2;

        return ret;
    }
    catch(const std::out_of_range& oob)
    {
        std::cerr << oob.what() << '\n';
        exit(EXIT_FAILURE);
    }
    catch(const std::runtime_error& re)
    {
        std::cerr << re.what() << '\n';
        exit(EXIT_FAILURE);
    }
}

unsigned int Utils::ByteBuffer::getInt()
{
    try
    {
        ByteBuffer::checkForOutOfBound(this->_position, ByteBuffer::INT_SIZE, this->_size, "ByteBuffer::getInt");
        ByteBuffer::errorIfEmpty(this, "ByteBuffer::getInt");

        unsigned short ret1 = this->getShort();
        unsigned short ret2 = this->getShort();
        unsigned int ret = (ret1 << 16) + ret2;

        return ret;
    }
    catch(const std::out_of_range& oob)
    {
        std::cerr << oob.what() << '\n';
        exit(EXIT_FAILURE);
    }
    catch(const std::runtime_error& re)
    {
        std::cerr << re.what() << '\n';
        exit(EXIT_FAILURE);
    }
}

void Utils::ByteBuffer::getBuffer(unsigned char *_data, const int _start, const std::size_t _size)
{
    try
    {
        this->position(_start);

        ByteBuffer::checkForOutOfBound(this->_position, _size, this->_size, "ByteBuffer::getBufferFrom");
        ByteBuffer::errorIfEmpty(this, "ByteBuffer::getBufferFrom");

        for (std::size_t pos = 0; pos < _size; pos++)
        {
            _data[pos] = this->get();
        }
    }
    catch(const std::out_of_range& oob)
    {
        std::cerr << oob.what() << '\n';
        exit(EXIT_FAILURE);
    }
    catch(const std::runtime_error& re)
    {
        std::cerr << re.what() << '\n';
        exit(EXIT_FAILURE);
    }
}

void Utils::ByteBuffer::getBuffer(unsigned char *_data, const std::size_t _size)
{
    this->getBuffer(_data, this->_position, _size);
}

bool Utils::ByteBuffer::isEmpty()
{
    return this->_size == 0;
}

bool Utils::ByteBuffer::isEndOfBuffer()
{
    return this->_size - this->_position <= 0;
}

void Utils::ByteBuffer::writeToFile(const std::string _filename)
{
    unsigned char* buffer = new unsigned char[this->_size];
    this->getBuffer(buffer, this->_size);

    FILE *fptr = fopen(_filename.c_str(), "wb");
    fwrite(buffer, this->_size, 1, fptr);
    fflush(fptr);
    fclose(fptr);

    delete(buffer);
}

void Utils::ByteBuffer::checkForOutOfBound(
    const int _position, const std::size_t _size, const std::size_t _max, std::string _func
) {
    if (_position + _size > _max)
    {
        char message[200];
        snprintf(
            message, sizeof(message), "[%s] Index Out Of Bound: %ld > %ld\n", 
            _func.c_str(), _position + _size, _max
        );

        std::string msg = message;
        throw std::out_of_range(msg);
    }
}

void Utils::ByteBuffer::errorIfEmpty(ByteBuffer* _buff, std::string _func)
{
    if (_buff->isEmpty())
    {
        char message[200];
        snprintf(
            message, sizeof(message), "[%s] The Buffer is Empty.\n",
            _func.c_str()
        );

        std::string msg = message;
        throw std::runtime_error(msg);
    }
}
