/****************************************************************************************
 ****************************************************************************************
 *****                                                                              *****
 *****   @details Header file                                                       *****
 *****   @name Base.hpp                                                             *****
 *****   @author Riccardo La Marca (riccardo.lamarca98@gmail.com)                   *****
 *****   @date 08 July 2024 (Mon Jul 14 2024 Last Modification)                     *****
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
#include <type_traits>
#include <Utils/ByteBuffer.hpp>

namespace Packets
{
    class Enum
    {
        public:
            template <typename U, typename T, typename = typename std::enable_if<std::is_enum<U>::value>::type>
            static T cast(const U& __obj)
            {
                return static_cast<T>(__obj);
            }

            template <typename T, typename U, typename = typename std::enable_if<std::is_enum<U>::value>::type>
            static U cast(const T __val)
            {
                return static_cast<U>(__val);
            }
    };

    /**
     * @class PacketHeader
     * 
     * The base class for all Packet header. All class inheriting from this class
     * must implement all methods.
     */
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

    /**
     * @class Packet
     * 
     * The base class for actual packets. All class inheriting from this class
     * must implements all methods.
     */
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

    /**
     * @class PseudoHeader
     * 
     * This class is used when computing the Checksum for TCP and UDP Packet.
     * In both cases, some informations must be taken from the IP Packet and
     * used to compute the checksum.
     */
    class PseudoHeader
    {
        protected:
            unsigned int   _srcaddr;   //!< The source address from the IP Packet
            unsigned int   _dstaddr;   //!< The Destination address from the IP Packet
            unsigned char  _protocol;  //!< The protocol number from the IP Packet
            unsigned short _size;      //!< The total size of the underlying packet

        public:
            PseudoHeader(
                unsigned int _src, unsigned int _dst, 
                unsigned char _proto, unsigned short _size
            ) : _srcaddr(_src), _dstaddr(_dst), _protocol(_proto), _size(_size) {};

            ~PseudoHeader() = default;

            /**
             * @brief Add to the ByteBuffer the values from the Pseudo Header
             * @param _buffer The input ByteBuffer
             */
            void encode(Utils::ByteBuffer& _buffer)
            {
                _buffer.putInt(htonl(this->_srcaddr));
                _buffer.putInt(htonl(this->_dstaddr));
                _buffer.put(this->_protocol);
                _buffer.putShort(htons(this->_size));
            }
    };
};

#endif