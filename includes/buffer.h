#ifndef _ENCODER_H
#define _ENCODER_H

#include <sys/cdefs.h>
#include <sys/types.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <stdbool.h>

__BEGIN_DECLS

#define BYTE_SIZE  1
#define SHORT_SIZE 2
#define INT_SIZE   4

/**
 * ByteBuffer struct created to holds a buffer of bytes. It has been created
 * in order to easily put and get, bytes, unsigned short and unsigned int.
 * It is very similar to the ByteBuffer class in Java.
 * 
 * It has three "member":
 * 
 * - `_buffer` which is a pointer to the buffer of bytes
 * - `_position` the current position inside the buffer
 * - `_size` The total size of the buffer
 */
typedef struct
{

    char*  _buffer;   /* The actual buffer */
    int    _position; /* The current position into the buffer */
    size_t _size;     /* The size of the buffer */

} ByteBuffer;

/* Create a new ByteBuffer with initial input size */
extern ByteBuffer* ByteBuffer_new(size_t _size);

/* Create a new ByteBuffer already filled with input buffer of input size */
extern ByteBuffer* ByteBuffer_new_v2(char *_buffer, size_t _size) __attribute__((nonnull));

/* Free the memory allocated for the input ByteBuffer (like a deconstructor in C++) */
extern void ByteBuffer_delete(ByteBuffer* _self) __attribute__((nonnull));

/* Put a single inpute byte into the buffer and increase the position of 1 */
extern void ByteBuffer_put(ByteBuffer* _self, char _data) __attribute__((nonnull));

/* Put an unsigned Short into the buffer and increase the position of 2 */
extern void ByteBuffer_putShort(ByteBuffer* _self, u_int16_t _data) __attribute__((nonnull));

/* Put an unsigned Int into the buffer and increase the position of 4 */
extern void ByteBuffer_putInt(ByteBuffer* _self, u_int32_t _data) __attribute__((nonnull));

/* Set a new position into the buffer */
extern void ByteBuffer_position(ByteBuffer* _self, int _newpos) __attribute__((nonnull));

/* Put an input buffer into the buffer starting from the current position */
extern void ByteBuffer_putBuffer(ByteBuffer* _self, char* _src, size_t _size) __attribute__((nonnull));

/* Put an input buffer into the buffer starting from a given input position */
extern void ByteBuffer_putBufferFrom(ByteBuffer* _self, char* _src, int _start, size_t _size) __attribute__((nonnull));

/* Return a single byte from the buffer and increase the position of 1 */
extern u_int8_t  ByteBuffer_get(ByteBuffer* _self) __attribute__((nonnull));

/* Return an unsigned short from the buffer and increase the position of 2 */
extern u_int16_t ByteBuffer_getShort(ByteBuffer* _self) __attribute__((nonnull));

/* Return an unsigned int from the buffer and increase the position of 4 */
extern u_int32_t ByteBuffer_getInt(ByteBuffer* _self) __attribute__((nonnull));

/* Return a buffer of input size and increase the position of given size */
extern char* ByteBuffer_getBuffer(ByteBuffer* _self, size_t _size) __attribute__((nonnull));

/* Return a buffer of input size from a given starting position
   and set the current position to the starting position plus the input size */
extern char* ByteBuffer_getBufferFrom(ByteBuffer* _self, size_t _start, size_t _size) __attribute__((nonnull)) __returns_nonnull;

/* Reset the current position to 0 */
extern void ByteBuffer_resetPosition(ByteBuffer* _self) __attribute__((nonnull));

/* Write the content of the buffer into a file */
extern void ByteBuffer_writeToFile(const ByteBuffer* _self, char *_file) __attribute__((nonnull));

/* Check if the input size + position is less than the maximum buffer dimension */
extern void checkForOOB(int _position, size_t _size, size_t _max);

__END_DECLS

#endif