/****************************************************************************************
 ****************************************************************************************
 *****                                                                              *****
 *****   @details Header file                                                       *****
 *****   @name ByteBuffer.hpp                                                       *****
 *****   @author Riccardo La Marca (riccardo.lamarca98@gmail.com)                   *****
 *****   @date 1 June 2024 (Mon Jul 08 2024 Last Modification)                      *****
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

#include <iostream>
#include <vector>
#include <string>
#include <stdexcept>
#include <memory>

namespace Utils
{
    class ByteBuffer
    {
        protected:
            int                        _position; //!< The current position into the buffer
            std::size_t                _size;     //!< The size of the buffer
            std::vector<unsigned char> _buffer;   //!< The actual buffer

        private:
            static void checkForOutOfBound(const int _position, const std::size_t _size, const std::size_t _max, std::string _func);
            static void errorIfEmpty(ByteBuffer* _buff, std::string _func);

        public:
            const static std::size_t BYTE_SIZE  = 1;
            const static std::size_t SHORT_SIZE = 2;
            const static std::size_t INT_SIZE   = 4;

            ByteBuffer(const std::size_t _size);
            ~ByteBuffer() = default;

            /**
             * Set a new position into the buffer
             * 
             * @param _newpos The new position
             */
            void position(const int _newpos);

            /**
             * Reset the position of the Byte Buffer
             */
            void resetPosition();

            /**
             * Put an unsigned byte into the buffer
             * 
             * @param _data The input data of size 8 bits (1 byte)
             */
            void put(const unsigned char _data);

            /**
             * Put an unisnged short into the buffer
             * 
             * @param _data The input data of size 16 bits (2 bytes)
             */
            void putShort(const unsigned short _data);

            /**
             * Put an unsigned integer into the buffer
             * 
             * @param _data The input data of size 32 bits (4 bytes)
             */
            void putInt(const unsigned int _data);

            /**
             * Put an input buffer into the buffer starting from a given input position
             * 
             * @param _data A buffer of unisnged char
             * @param _start The starting position inside the Byte Buffer
             * @param _size The total length in bytes of the input unsigned char buffer
             */
            void putBuffer(unsigned char* const& _data, const int _start, const std::size_t _size);
            
            /**
             * Put an input buffer into the buffer starting from the current position
             * 
             * @param _data A buffer of unisnged char
             * @param _size The total length in bytes of the input unsigned char buffer
             */
            void putBuffer(unsigned char* const& _data, const std::size_t _size);

            /**
             * Return the unsigned char at the current buffer position
             */
            unsigned char get();

            /**
             * Return an unsigned short from the buffer and increase the position of 2
             */
            unsigned short getShort();

            /**
             * Return an unsigned int from the buffer and increase the position of 4
             */
            unsigned int getInt();

            /**
             * Return a buffer of input size from a given starting position.
             * 
             * @param _data The input destination buffer
             * @param _start The starting index inside the buffer
             * @param _size The total number of bytes to take from the Byte Buffer
             */
            void getBuffer(unsigned char* _data, const int _start, const std::size_t _size);

            /**
             * Return a buffer of input size from the current Byte Buffer position.
             * 
             * @param _data The input destination buffer
             * @param _size The total number of bytes to take from the Byte Buffer
             */
            void getBuffer(unsigned char* _data, const std::size_t _size);

            /**
             * Check if the Byte Buffer is Empty or not
             */
            bool isEmpty();

            /**
             * Check if the current position is at the end of the buffer
             */
            bool isEndOfBuffer();

            /**
             * Write the content of the byte buffer into a file
             * 
             * @param filename The name of the file
             */
            void writeToFile(const std::string _filename);

            /**
             * @brief Get the current position inside the buffer
             * @return An Integer representing the position
             */
            int getCurrentPosition();
    };
}

#endif