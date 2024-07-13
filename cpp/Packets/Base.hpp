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
            /**
             * @brief Cast the input header of type PacketHeader to another type.
             * 
             * Notice that the input type should be at least a derived from Packet Header.
             * 
             * @param _hdr A pointer to the packet header to downcast 
             * @return The pointer of a new type
             * @throws std::runtime_error If the case was unsuccessful
             */
            template<class T>
            static T* header_cast(PacketHeader* _hdr)
            {
                T* hdr = dynamic_cast<T*>(_hdr);

                if (hdr == nullptr)
                {
                    throw std::runtime_error("[Packet::cast] Impossible to cast.");
                }

                return std::move(hdr);
            }

            virtual void printInfo() = 0;
            virtual void encode(Utils::ByteBuffer& _buffer) = 0;
            virtual Utils::ByteBuffer_ptr encode() = 0;
            virtual void decode(Utils::ByteBuffer& _buffer) = 0;
    };

    class Packet
    {
        protected:
            Utils::ByteBuffer_ptr _payload;

        public:
            Packet(const std::size_t _size) 
            {
                this->_payload = std::make_shared<Utils::ByteBuffer>(_size);
            };

            virtual PacketHeader* getPacketHeader() = 0;
            virtual void setHeader(PacketHeader& _hdr) = 0;
            virtual void setPayload(unsigned char* _payload) = 0;
            virtual void setPayload(Utils::ByteBuffer& _buff) = 0;
            virtual std::size_t getPacketSize() = 0;
            virtual const std::size_t& getPayloadSize() = 0;
            virtual void encode(Utils::ByteBuffer& _buffer) = 0;
            virtual Utils::ByteBuffer_ptr encode() = 0;
            virtual void decode(Utils::ByteBuffer& _buffer) = 0;
    };
};

#endif