#include "ByteBuffer.hpp"

Utils::ByteBuffer::ByteBuffer(const std::size_t _size)
{
    this->_size = _size;
    this->_position = 0;
    this->_buffer.reserve(_size);
}

Utils::ByteBuffer::ByteBuffer(const ByteBuffer &other) : ByteBuffer(other.getBufferSize()) 
{
    const buffer_t buffer = other.getBuffer();
    std::copy(buffer.begin(), buffer.end(), this->_buffer.begin());
}

Utils::ByteBuffer &Utils::ByteBuffer::operator=(const ByteBuffer &other)
{
    const std::size_t size = other.getBufferSize();
    this->resize(size);
    this->copy(other.getBuffer());

    return *this;
}

void Utils::ByteBuffer::position(const int _newpos)
{
    this->_position = _newpos;
}

int Utils::ByteBuffer::position()
{
    return this->_position;
}

const std::size_t& Utils::ByteBuffer::getBufferSize()
{
    return this->_size;
}

const std::size_t &Utils::ByteBuffer::getBufferSize() const
{
    return this->_size;
}

void Utils::ByteBuffer::resize(const std::size_t _size)
{
    this->_size = _size;
    this->_buffer.resize(_size);
}

void Utils::ByteBuffer::copy(const buffer_t& _buffer)
{
    this->copy(_buffer, 0, _buffer.size());
}

void Utils::ByteBuffer::copy(const buffer_t &_buffer, const int _start)
{
    this->copy(_buffer, _start, _buffer.size());
}

void Utils::ByteBuffer::copy(const buffer_t &_buffer, const int _start, const int _end)
{
    assert((_start > 0 && _end > 0) && "[ByteBuffer::copy] Start and Stop range must be positive");
    
    // Resize if necessary
    if ((std::size_t)(_end - _start) > this->_size) this->resize(_end - _start);
    this->_buffer.clear();

    int _rem = _buffer.size() - _end;
    std::copy(_buffer.begin() + _start, _buffer.end() - _rem, this->_buffer.begin());
}

void Utils::ByteBuffer::merge(ByteBuffer &_buffer)
{
    std::size_t remaining = _buffer.getBufferSize() - (this->getBufferSize() - this->position());
    if (remaining > 0) this->resize(this->getBufferSize() + remaining);
    this->putBuffer(_buffer.getBuffer());
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
        ByteBuffer::checkForOutOfBound(this->_position, _size, this->_size, "ByteBuffer::putBuffer");

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

void Utils::ByteBuffer::putBuffer(const buffer_t&_buffer)
{
    try
    {
        ByteBuffer::checkForOutOfBound(this->_position, _buffer.size(), this->_size, "ByteBuffer::putBuffer");
        std::copy(_buffer.begin(), _buffer.end(), this->_buffer.begin() + this->_position);
        this->position(this->_position + _buffer.size());
    }
    catch(const std::out_of_range& oob)
    {
        std::cerr << oob.what() << '\n';
        exit(EXIT_FAILURE);
    }
    
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

const Utils::ByteBuffer::buffer_t &Utils::ByteBuffer::getBuffer()
{
    return this->_buffer;
}

const Utils::ByteBuffer::buffer_t &Utils::ByteBuffer::getBuffer() const
{
    return this->_buffer;
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
    this->resetPosition();
    
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
