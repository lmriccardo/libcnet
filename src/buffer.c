#include "buffer.h"

ByteBuffer* ByteBuffer_new(size_t _size)
{
    ByteBuffer* buffer = (ByteBuffer *)malloc(sizeof(ByteBuffer));
    buffer->_buffer = (char *)malloc(_size * sizeof(char));
    buffer->_position = 0;
    buffer->_size = _size;
    return buffer;
}

ByteBuffer* ByteBuffer_new_v2(char *_buffer, size_t _size)
{
    ByteBuffer* buffer = (ByteBuffer*)malloc(sizeof(ByteBuffer));
    buffer->_buffer = (char *)malloc(_size * sizeof(char));
    buffer->_position = 0;
    buffer->_size = _size;
    
    // As usual it is better to copy the content of the memory instead
    // of directly assigning that allocation inside this variable.
    // This beacause, once this ByteBuffer will be deleted, in the first
    // case the memory from which we copied is still accessible, in the
    // other case it will not.
    memcpy(buffer->_buffer, _buffer, _size);

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
    memcpy(_self->_buffer + _self->_position, &_data, SHORT_SIZE);
    ByteBuffer_position(_self, _self->_position + SHORT_SIZE);
}

void ByteBuffer_putInt(ByteBuffer* _self, u_int32_t _data)
{
    checkForOOB(_self->_position, INT_SIZE, _self->_size);
    memcpy(_self->_buffer + _self->_position, &_data, INT_SIZE);
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

void ByteBuffer_writeToFile(const ByteBuffer* _self, char *_file)
{
    FILE *fptr = fopen(_file, "wb");
    fwrite(_self->_buffer, _self->_size, 1, fptr);
    fclose(fptr);
}

void checkForOOB(int _position, size_t _size, size_t _max)
{
    if (_position + _size > _max)
    {
        fprintf(stderr, "[ByteBuffer_put] Index Out Of Bound: %ld > %ld\n", _position + _size, _max);
        exit(EXIT_FAILURE);
    }
}

u_int8_t ByteBuffer_get(ByteBuffer* _self)
{
    u_int8_t data = *(_self->_buffer + _self->_position);
    ByteBuffer_position(_self, _self->_position + BYTE_SIZE);
    return data;
}

u_int16_t ByteBuffer_getShort(ByteBuffer* _self)
{
    u_int16_t data;
    memcpy(&data, _self->_buffer + _self->_position, SHORT_SIZE);
    ByteBuffer_position(_self, _self->_position + SHORT_SIZE);
    return data;
}

u_int32_t ByteBuffer_getInt(ByteBuffer* _self)
{
    u_int32_t data;
    memcpy(&data, _self->_buffer + _self->_position, INT_SIZE);
    ByteBuffer_position(_self, _self->_position + INT_SIZE);
    return data;
}

char* ByteBuffer_getBuffer(ByteBuffer* _self, size_t _size)
{
    return ByteBuffer_getBufferFrom(_self, _self->_position, _size);
}

char* ByteBuffer_getBufferFrom(ByteBuffer* _self, size_t _start, size_t _size)
{
    char *data = (char*)malloc(_size * sizeof(char));
    memcpy(data, _self->_buffer + _start, _size);
    ByteBuffer_position(_self, _start + _size);
    return data;
}