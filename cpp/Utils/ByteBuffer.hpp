/****************************************************************************************
 ****************************************************************************************
 *****                                                                              *****
 *****   @details Header file                                                       *****
 *****   @name ByteBuffer.hpp                                                       *****
 *****   @author Riccardo La Marca (riccardo.lamarca98@gmail.com)                   *****
 *****   @date 1 June 2024 (Mon Jul 13 2024 Last Modification)                      *****
 *****                                                                              *****
 *****   @brief This file contains the declaration of the ByteBuffer class and      *****
 *****   a number of functions to interact with it. This byte buffer object is      *****
 *****   used principally when encoding and decoding IP Packets etc.                *****
 *****                                                                              *****
 ****************************************************************************************
 ****************************************************************************************
*/

#ifndef _BYTE_BUFFER_H
#define _BYTE_BUFFER_H

#pragma once

#include <iostream>
#include <vector>
#include <string>
#include <stdexcept>
#include <memory>
#include <algorithm>
#include <cassert>

namespace Utils
{
    /**
     * @class ByteBuffer
     * 
     * This class represents a ByteBuffer. It has a number of methods to fill the buffer
     * and to get bytes from it. It is possible to put and get unsigned char, unsigned short,
     * unsigned int and variable length buffers. 
     */
    class ByteBuffer
    {
        private:
            using buffer_t = std::vector<unsigned char>;

        protected:
            int         _position; //!< The current position into the buffer
            std::size_t _size;     //!< The size of the buffer
            buffer_t    _buffer;   //!< The actual buffer

        private:
            static void checkForOutOfBound(const int _position, const std::size_t _size, const std::size_t _max, std::string _func);
            static void errorIfEmpty(ByteBuffer* _buff, std::string _func);

        public:
            const static std::size_t BYTE_SIZE  = 1;
            const static std::size_t SHORT_SIZE = 2;
            const static std::size_t INT_SIZE   = 4;

            ByteBuffer(const std::size_t _size);
            ByteBuffer(const ByteBuffer& other);
            ~ByteBuffer() = default;

            ByteBuffer& operator=(const ByteBuffer& other);

            /**
             * @brief Set a new position into the buffer
             * 
             * @param _newpos The new position
             */
            void position(const int _newpos);

            /**
             * @brief Returns the current position of the buffer
             * @return An integer value
             */
            int position();

            /**
             * @brief Returns the total size of the buffer
             * @return The size of the buffer
             */
            const std::size_t& getBufferSize();

            /**
             * @brief Returns the total size of the buffer
             * 
             * This function is called only by const instance of ByteBuffer
             * 
             * @return The size of the buffer
             */
            const std::size_t& getBufferSize() const;

            /**
             * @brief "Resize" the current ByteBuffer by changing the actual size
             * 
             * @param _size The new size of the ByteBuffer
             */
            void resize(const std::size_t _size);

            /**
             * @brief Copy the input buffer into the ByteBuffer
             * 
             * @param _buffer The input buffer to be copied
             */
            void copy(const buffer_t& _buffer);

            /**
             * @brief Copy only a portion of the input buffer starting from a given position.
             * @param _buffer The input buffer to be copied
             * @param _start The starting position
             */
            void copy(const buffer_t& _buffer, const int _start);

            /**
             * @brief Copy only a portion of the input buffer given the interval.
             * 
             * @param _buffer The input buffer to be copied
             * @param _start The starting position
             * @param _end The ending position
             */
            void copy(const buffer_t& _buffer, const int _start, const int _end);

            /**
             * @brief Merge the input ByteBuffer into the current one
             * @param _buffer The input ByteBuffer
             */
            void merge(ByteBuffer& _buffer);

            /**
             * @brief Reset the position of the Byte Buffer
             */
            void resetPosition();

            /**
             * @brief Put an unsigned byte into the buffer
             * 
             * @param _data The input data of size 8 bits (1 byte)
             */
            void put(const unsigned char _data);

            /**
             * @brief Put an unisnged short into the buffer
             * 
             * @param _data The input data of size 16 bits (2 bytes)
             */
            void putShort(const unsigned short _data);

            /**
             * @brief Put an unsigned integer into the buffer
             * 
             * @param _data The input data of size 32 bits (4 bytes)
             */
            void putInt(const unsigned int _data);

            /**
             * @brief Put an input buffer into the buffer starting from a given input position
             * 
             * @param _data A buffer of unsinged char
             * @param _start The starting position inside the Byte Buffer
             * @param _size The total length in bytes of the input unsigned char buffer
             */
            void putBuffer(unsigned char* const& _data, const int _start, const std::size_t _size);
            
            /**
             * @brief Put an input buffer into the buffer starting from the current position
             * 
             * @param _data A buffer of unsinged char
             * @param _size The total length in bytes of the input unsigned char buffer
             */
            void putBuffer(unsigned char* const& _data, const std::size_t _size);

            /**
             * @brief Put an input buffer (vector of unsigned char) into the ByteBuffer
             * 
             * @param _buffer The vector of unsigned char
             */
            void putBuffer(const buffer_t& _buffer);

            /**
             * @brief Return the unsigned char at the current buffer position
             */
            unsigned char get();

            /**
             * @brief Return an unsigned short from the buffer and increase the position of 2
             */
            unsigned short getShort();

            /**
             * @brief Return an unsigned int from the buffer and increase the position of 4
             */
            unsigned int getInt();

            /**
             * @brief Return a buffer of input size from a given starting position.
             * 
             * @param _data The input destination buffer
             * @param _start The starting index inside the buffer
             * @param _size The total number of bytes to take from the Byte Buffer
             */
            void getBuffer(unsigned char* _data, const int _start, const std::size_t _size);

            /**
             * @brief Return a buffer of input size from the current Byte Buffer position.
             * 
             * @param _data The input destination buffer
             * @param _size The total number of bytes to take from the Byte Buffer
             */
            void getBuffer(unsigned char* _data, const std::size_t _size);

            /**
             * @brief Returns the entire buffer in vector format
             * 
             * Notice that the returned vector cannot be modified.
             * 
             * @return A std::vector of unsigned char
             */
            const buffer_t& getBuffer();

            /**
             * @brief Returns the entire buffer in vector format
             * 
             * This function can be called only by const reference to ByteBuffer
             * 
             * @return A std::vector of unsigned char
             */
            const buffer_t& getBuffer() const;

            /**
             * @brief Check if the Byte Buffer is Empty or not
             * @return A boolean value
             */
            bool isEmpty();

            /**
             * @brief Check if the current position is at the end of the buffer
             * @return A boolean value
             */
            bool isEndOfBuffer();

            /**
             * @brief Write the content of the byte buffer into a file
             * 
             * @param filename The name of the file
             */
            void writeToFile(const std::string _filename);
    };

    typedef std::shared_ptr<ByteBuffer> ByteBuffer_ptr;
}

#endif