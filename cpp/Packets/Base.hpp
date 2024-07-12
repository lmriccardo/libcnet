/****************************************************************************************
 ****************************************************************************************
 *****                                                                              *****
 *****   @details Header file                                                       *****
 *****   @name Base.hpp                                                             *****
 *****   @author Riccardo La Marca (riccardo.lamarca98@gmail.com)                   *****
 *****   @date 08 July 2024 (Mon Jul 12 2024 Last Modification)                     *****
 *****                                                                              *****
 *****   @brief This file contains the creation of bases classes for constructing   *****
 *****   different IP Packets: ICMP, TCP, UDP and IP itself. All the packet header  *****
 *****   must inherit from the PacketHeader abstract class, and all the Packet      *****
 *****   must inherit from the Packet abstract class.                               *****
 *****                                                                              *****
 ****************************************************************************************
 ****************************************************************************************
*/


#ifndef _BASE_H
#define _BASE_H

#include <iostream>
#include <arpa/inet.h>
#include <Utils/ByteBuffer.hpp>

namespace Packets
{
    class PacketHeader
    {
        public:
            virtual void printInfo() = 0;
            virtual void encode(Utils::ByteBuffer& _buffer) = 0;
            virtual Utils::ByteBuffer_ptr encode() = 0;
            virtual void decode(Utils::ByteBuffer& _buffer) = 0;
    };

    class Packet
    {
        private:
            unsigned char* _payload;

        public:
            virtual void fillHeader(PacketHeader& _hdr) = 0;
            virtual void fillPayload(unsigned char* _payload) = 0;
            virtual std::size_t getPacketSize() = 0;
            virtual std::size_t getPayloadSize() = 0;
            virtual void encode(Utils::ByteBuffer& _buffer) = 0;
            virtual Utils::ByteBuffer_ptr encode() = 0;
            virtual void decode(Utils::ByteBuffer& _buffer) = 0;
    };
};

#endif