/****************************************************************************************
 ****************************************************************************************
 *****                                                                              *****
 *****   @details Header file                                                       *****
 *****   @name buffer.h                                                             *****
 *****   @author Riccardo La Marca (riccardo.lamarca98@gmail.com)                   *****
 *****   @date 1 June 2024 (Mon Jul 01 2024 Last Modification)                      *****
 *****                                                                              *****
 *****   @brief This file contains the declaration of the ByteBuffer struct and     *****
 *****   a number of functions to interact with it. This byte buffer object is      *****
 *****   used principally when encoding and decoding IP Packets etc.                *****
 *****                                                                              *****
 ****************************************************************************************
 ****************************************************************************************
*/


#ifndef _ENCODER_H
#define _ENCODER_H

#include <sys/cdefs.h>
#include <sys/types.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <stdbool.h>
#include "utils/version.h"

#define BYTE_SIZE  1
#define SHORT_SIZE 2
#define INT_SIZE   4

__BEGIN_DECLS

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

    char*  _buffer;   //!< The actual buffer
    int    _position; //!< The current position into the buffer
    size_t _size;     //!< The size of the buffer

} ByteBuffer;

/**
 * Create a new ByteBuffer with initial input size
 */
extern ByteBuffer* ByteBuffer_new(const size_t _size);

/**
 * Create a new ByteBuffer already filled with input buffer of input size
 */
extern ByteBuffer* ByteBuffer_new_b(const char *_buffer, const size_t _size) __attribute__((nonnull));

/**
 * Free the memory allocated for the input ByteBuffer (like a deconstructor in C++)
 */
extern void ByteBuffer_delete(ByteBuffer* _self) __attribute__((nonnull));

/**
 * Put a single inpute byte into the buffer and increase the position of 1
 */
extern void ByteBuffer_put(ByteBuffer* _self, const char _data) __attribute__((nonnull));

/**
 * Put an unsigned Short into the buffer and increase the position of 2
 */
extern void ByteBuffer_putShort(ByteBuffer* _self, const u_int16_t _data) __attribute__((nonnull));

/**
 * Put an unsigned Int into the buffer and increase the position of 4
 */
extern void ByteBuffer_putInt(ByteBuffer* _self, const u_int32_t _data) __attribute__((nonnull));

/**
 * Set a new position into the buffer
 */
extern void ByteBuffer_position(ByteBuffer* _self, const int _newpos) __attribute__((nonnull));

/**
 * Put an input buffer into the buffer starting from the current position
 */
extern void ByteBuffer_putBuffer(ByteBuffer* _self, const char* _src, const size_t _size) __attribute__((nonnull));

/**
 * Put an input buffer into the buffer starting from a given input position
 */
extern void ByteBuffer_putBufferFrom(ByteBuffer* _self, const char* _src, const int _start, const size_t _size) __attribute__((nonnull));

/**
 * Return a single byte from the buffer and increase the position of 1
 */
extern u_int8_t  ByteBuffer_get(ByteBuffer* _self) __attribute__((nonnull));

/**
 * Return an unsigned short from the buffer and increase the position of 2
 */
extern u_int16_t ByteBuffer_getShort(ByteBuffer* _self) __attribute__((nonnull));

/**
 * Return an unsigned int from the buffer and increase the position of 4
 */
extern u_int32_t ByteBuffer_getInt(ByteBuffer* _self) __attribute__((nonnull));

/**
 * Return a buffer of input size and increase the position of given size
 */
extern void ByteBuffer_getBuffer(ByteBuffer* _self, char* _out, const size_t _size) __attribute__((nonnull));

/**
 * Return a buffer of input size from a given starting position
 * and set the current position to the starting position plus the input size
 */
extern void ByteBuffer_getBufferFrom(ByteBuffer* _self, char* _out, const size_t _start, const size_t _size) __attribute__((nonnull));

/**
 * Reset the current position to 0
 */
extern void ByteBuffer_resetPosition(ByteBuffer* _self) __attribute__((nonnull));

/**
 * Write the content of the buffer into a file
 */
extern void ByteBuffer_writeToFile(const ByteBuffer* _self, const char *_file) __attribute__((nonnull));

/**
 * Check if the byte buffer is empty or not
 */
extern bool ByteBuffer_isEmpty(const ByteBuffer* _self) __attribute__((nonnull));

/**
 * Check if the buffer position is at the end of the buffer
 */
extern bool ByteBuffer_isEndOfBuffer(const ByteBuffer* _self) __attribute__((nonnull));

/**
 * Check if the input size + position is less than the maximum buffer dimension
 */
extern void checkForOOB(const int _position, const size_t _size, const size_t _max, const char* _func) __attribute__((nonnull));

/**
 * Raise an error if any operation requiring a non-empty buffer operates on an empty one
 */
extern void errorIfEmpty(const ByteBuffer* _self, const char* _func) __attribute__((nonnull));

__END_DECLS

#endif