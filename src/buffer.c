#include "buffer.h"

ByteBuffer* ByteBuffer_new(size_t _size)
{
    char *data = (char *)malloc(_size * sizeof(char));
    ByteBuffer* buffer = (ByteBuffer *)malloc(sizeof(ByteBuffer));
    buffer->_buffer = data;
    buffer->_position = 0;
    buffer->_size = _size;
    return buffer;
}

ByteBuffer* ByteBuffer_new_v2(char *_buffer, size_t _size)
{
    ByteBuffer* buffer = (ByteBuffer*)malloc(sizeof(ByteBuffer));
    buffer->_buffer = _buffer;
    buffer->_position = 0;
    buffer->_size = _size;
    return buffer;
}

void ByteBuffer_delete(ByteBuffer* _self)
{
    free(_self->_buffer);
    free(_self);
}

void ByteBuffer_resetPosition(ByteBuffer* _self)
{
    _self->_position = 0;
}

void ByteBuffer_position(ByteBuffer* _self, int _newpos)
{
    _self->_position = _newpos;
}

void ByteBuffer_put(ByteBuffer* _self, char _data)
{
    checkForOOB(_self->_position, BYTE_SIZE, _self->_size);
    _self->_buffer[_self->_position] = _data;
    ByteBuffer_position(_self, _self->_position + BYTE_SIZE);
}

void ByteBuffer_putShort(ByteBuffer* _self, u_int16_t _data)
{
    checkForOOB(_self->_position, SHORT_SIZE, _self->_size);
    memcpy(_self->_buffer + _self->_position, &_data, SHORT_SIZE * sizeof(char));
    ByteBuffer_position(_self, _self->_position + SHORT_SIZE);
}

void ByteBuffer_putInt(ByteBuffer* _self, u_int32_t _data)
{
    checkForOOB(_self->_position, INT_SIZE, _self->_size);
    memcpy(_self->_buffer + _self->_position, &_data, INT_SIZE * sizeof(char));
    ByteBuffer_position(_self, _self->_position + INT_SIZE);
}

void ByteBuffer_putBufferFrom(ByteBuffer* _self, char* _src, int _start, size_t _size)
{
    checkForOOB(_start, _size, _self->_size);
    memcpy(_self->_buffer + _start, _src, _size);
    ByteBuffer_position(_self, _start + _size);
}

void ByteBuffer_putBuffer(ByteBuffer* _self, char* _src, size_t _size)
{
    ByteBuffer_putBufferFrom(_self, _src, _self->_position, _size);
}

void ByteBuffer_writeToFile(ByteBuffer* _self, char *_file)
{
    FILE *fptr = fopen(_file, "wb");
    fwrite(_self->_buffer, _self->_size, 1, fptr);
}

void checkForOOB(int _position, size_t _size, size_t _max)
{
    if (_position + _size > _max)
    {
        fprintf(stderr, "Index Out Of Bound: %ld > %ld", _position + _size, _max);
        exit(EXIT_FAILURE);
    }
}