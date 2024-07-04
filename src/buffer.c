#include "utils/buffer.h"

ByteBuffer* ByteBuffer_new(const size_t _size)
{
    ByteBuffer* buffer = (ByteBuffer *)malloc(sizeof(ByteBuffer));
    buffer->_buffer = (char *)malloc(_size * sizeof(char));
    buffer->_position = 0;
    buffer->_size = _size;
    return buffer;
}

ByteBuffer* ByteBuffer_new_b(const char *_buffer, const size_t _size)
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

void ByteBuffer_position(ByteBuffer* _self, const int _newpos)
{
    _self->_position = _newpos;
}

void ByteBuffer_put(ByteBuffer* _self, const char _data)
{
    checkForOOB(_self->_position, BYTE_SIZE, _self->_size, "ByteBuffer_put");
    _self->_buffer[_self->_position] = _data;
    ByteBuffer_position(_self, _self->_position + BYTE_SIZE);
}

void ByteBuffer_putShort(ByteBuffer* _self, const u_int16_t _data)
{
    checkForOOB(_self->_position, SHORT_SIZE, _self->_size, "ByteBuffer_putShort");
    memcpy(_self->_buffer + _self->_position, &_data, SHORT_SIZE);
    ByteBuffer_position(_self, _self->_position + SHORT_SIZE);
}

void ByteBuffer_putInt(ByteBuffer* _self, const u_int32_t _data)
{
    checkForOOB(_self->_position, INT_SIZE, _self->_size, "ByteBuffer_putInt");
    memcpy(_self->_buffer + _self->_position, &_data, INT_SIZE);
    ByteBuffer_position(_self, _self->_position + INT_SIZE);
}

void ByteBuffer_putBufferFrom(ByteBuffer* _self, const char* _src, const int _start, const size_t _size)
{
    checkForOOB(_start, _size, _self->_size, "ByteBuffer_putBufferFrom");

    // ByteBuffer_position(_self, _start);

    // for (int idx = 0; idx < _size; idx += 2)
    // {
    //     if (idx == _size - 1)
    //     {
    //         memcpy(_self->_buffer + _self->_position, _src + idx, 1);
    //         ByteBuffer_position(_self, _self->_position + BYTE_SIZE);
    //         continue;
    //     }

    //     memcpy(_self->_buffer + _self->_position, _src + idx, 2);
    //     ByteBuffer_position(_self, _self->_position + SHORT_SIZE);
    // }

    memcpy(_self->_buffer + _start, _src, _size);
    ByteBuffer_position(_self, _start + _size);
}

void ByteBuffer_putBuffer(ByteBuffer* _self, const char* _src, const size_t _size)
{
    ByteBuffer_putBufferFrom(_self, _src, _self->_position, _size);
}

void ByteBuffer_writeToFile(const ByteBuffer* _self, const char *_file)
{
    FILE *fptr = fopen(_file, "wb");
    fwrite(_self->_buffer, _self->_size, 1, fptr);
    fflush(fptr);
    fclose(fptr);
}

u_int8_t ByteBuffer_get(ByteBuffer* _self)
{
    checkForOOB(_self->_position, BYTE_SIZE, _self->_size, "ByteBuffer_get");
    errorIfEmpty(_self, "ByteBuffer_get");

    u_int8_t data = *(_self->_buffer + _self->_position);
    ByteBuffer_position(_self, _self->_position + BYTE_SIZE);
    return data;
}

u_int16_t ByteBuffer_getShort(ByteBuffer* _self)
{
    checkForOOB(_self->_position, SHORT_SIZE, _self->_size, "ByteBuffer_getShort");
    errorIfEmpty(_self, "ByteBuffer_getShort");

    u_int16_t data;
    memcpy(&data, _self->_buffer + _self->_position, SHORT_SIZE);
    ByteBuffer_position(_self, _self->_position + SHORT_SIZE);
    return data;
}

u_int32_t ByteBuffer_getInt(ByteBuffer* _self)
{
    checkForOOB(_self->_position, INT_SIZE, _self->_size, "ByteBuffer_getInt");
    errorIfEmpty(_self, "ByteBuffer_getInt");

    u_int32_t data;
    memcpy(&data, _self->_buffer + _self->_position, INT_SIZE);
    ByteBuffer_position(_self, _self->_position + INT_SIZE);
    return data;
}

void ByteBuffer_getBuffer(ByteBuffer* _self, char* _out, const size_t _size)
{
    return ByteBuffer_getBufferFrom(_self, _out, _self->_position, _size);
}

void ByteBuffer_getBufferFrom(ByteBuffer* _self, char* _out, const size_t _start, const size_t _size)
{
    checkForOOB(_start, _size, _self->_size, "ByteBuffer_getBufferFrom");
    errorIfEmpty(_self, "ByteBuffer_getBufferFrom");

    memcpy(_out, _self->_buffer + _start, _size);
    ByteBuffer_position(_self, _start + _size);
}

bool ByteBuffer_isEmpty(const ByteBuffer* _self)
{
    return _self->_size == 0;
}

bool ByteBuffer_isEndOfBuffer(const ByteBuffer* _self)
{
    return _self->_size - _self->_position <= 0;
}

void checkForOOB(const int _position, const size_t _size, const size_t _max, const char* _func)
{
    if (_position + _size > _max)
    {
        fprintf(stderr, "[%s] Index Out Of Bound: %ld > %ld\n", _func, _position + _size, _max);
        exit(EXIT_FAILURE);
    }
}

void errorIfEmpty(const ByteBuffer* _self, const char* _func)
{
    if (ByteBuffer_isEmpty(_self))
    {
        fprintf(stderr, "[%s] The Buffer is Empty.\n", _func);
        exit(EXIT_FAILURE);
    }
}