#ifndef _ENCODER_H
#define _ENCODER_H

#include <sys/cdefs.h>
#include <sys/types.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>

__BEGIN_DECLS

#define BYTE_SIZE  1
#define SHORT_SIZE 2
#define INT_SIZE   4

typedef struct
{

    char*  _buffer;
    int    _position;
    size_t _size;

} ByteBuffer;

extern ByteBuffer* ByteBuffer_new(size_t _size);
extern ByteBuffer* ByteBuffer_new_v2(char *_buffer, size_t _size);
extern void ByteBuffer_delete(ByteBuffer* _self);

extern void ByteBuffer_resetPosition(ByteBuffer* _self);
extern void ByteBuffer_put(ByteBuffer* _self, char _data);
extern void ByteBuffer_putShort(ByteBuffer* _self, u_int16_t _data);
extern void ByteBuffer_putInt(ByteBuffer* _self, u_int32_t _data);
extern void ByteBuffer_position(ByteBuffer* _self, int _newpos);
extern void ByteBuffer_putBuffer(ByteBuffer* _self, char* _src, size_t _size);
extern void ByteBuffer_putBufferFrom(
    ByteBuffer* _self, char* _src, int _start, size_t _size);

extern void ByteBuffer_writeToFile(ByteBuffer* _self, char *_file);

extern void checkForOOB(int _position, size_t _size, size_t _max);

#endif