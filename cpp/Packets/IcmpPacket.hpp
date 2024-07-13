/****************************************************************************************
 ****************************************************************************************
 *****                                                                              *****
 *****   @details Header file                                                       *****
 *****   @name IcmpPacket.hpp                                                       *****
 *****   @author Riccardo La Marca (riccardo.lamarca98@gmail.com)                   *****
 *****   @date 08 July 2024 (Mon Jul 12 2024 Last Modification)                     *****
 *****                                                                              *****
 *****   @brief This file contains the creation of Icmp classes for ICMP Packets    ***** 
 *****                                                                              *****
 ****************************************************************************************
 ****************************************************************************************
*/

#ifndef _ICMP_PACKET_H
#define _ICMP_PACKET_H

#include "Base.hpp"

namespace Packets
{
    namespace Icmp
    {
        /**
         * @enum Type
         * 
         * Enumerates all possible types for an ICMP Packet
         */
        enum class Type : unsigned char
        {
            ECHO_REPLY              = 0x00,
            DESTINATION_UNREACHABLE = 0x03,
            SOURCE_QUENCH           = 0x04,
            REDIRECT                = 0x05,
            ECHO                    = 0x08,
            TIME_EXCEEEDED          = 0x0b,
            PARAMETER_PROBLEM       = 0x0c,
            INFORMATION_REQUEST     = 0x0f,
            INFORMATION_REPLY       = 0x10
        };

        /**
         * @enum Code
         * 
         * Enumerates all possible codes for an ICMP Packet
         */
        enum class Code : unsigned char
        {
            ECHO                 = 0x00,
            NET_UNREACHABLE      = 0x00,
            HOST_UNREACHABLE     = 0x01,
            PROTOCOL_UNREACHABLE = 0x02,
            PORT_UNREACHABLE     = 0x03,
            FRAGMENTATION_NEEDED = 0x04,
            SOURCE_ROUTE_FAILED  = 0x05
        };
    };

    /**
     * Part of the ICMP Header reserved for Echo-like requests and responses
     */
    struct h_echo_t 
    {
        unsigned short _id;     //!< 16 bit for the identifier of the echo or echo reply
        unsigned short _seqnum; //!< 16 bit for the sequence number
    };

    /**
     * Part of the ICMP Header reserved for Maximum Transmission Unit
     */
    struct h_mtu_t
    {
        unsigned short _unused; //!< Set to zeros
        unsigned short _mtu;    //!< Next hop MTU
    };

    /**
     * Part of the ICMP Header containing different options for ICMP Header
     * last 32 bits. They could remain unused, entirely containing a
     * gateway address, used for MTU purpouses or ECHO requests/reply.
     */
    union h_data_t 
    {
        unsigned int    _unused;  //!< 32 bit of unused data
        unsigned int    _gateway; //!< 32 bit for gateway internet address
        struct h_mtu_t  _mtu;     //!< Used for MTU Path Discovery
        struct h_echo_t _echo;    //!< Echo and other message headers
    };

    /**
     * @class IcmpHeader
     * 
     * Class representing the ICMP Header of the ICMP Packet. There are the
     * classical 32 bits of ICMP Header: Type, Code and Checksum and the also
     * the remaining 32 bits of Optional header fields like: Identification,
     * Sequence Number, Gateway or Zeros (Unused) according to the given type
     * and ICMP Code.
     * 
     * This class refers to the RFC 792 https://datatracker.ietf.org/doc/html/rfc792 
     */
    class IcmpHeader : public PacketHeader
    {
        public:
            static const std::size_t size = 0x08; //!< The size of the ICMP Header

            template<typename T>
            static T cast(const Icmp::Type& __t)
            {
                return static_cast<T>(__t);
            }

            template<typename T>
            static T cast(const Icmp::Code& __c)
            {
                return static_cast<T>(__c);
            }

        private:
            Icmp::Type     _type;     //!< The type of the ICMP Message
            Icmp::Code     _code;     //!< The code corresponding to the Type
            unsigned short _checksum; //!< The checksum for packet validation
            union h_data_t _rest;     //!< Remaining data of the ICMP Header

            void createHeaderMtu();
            void createHeaderEcho();
            void createHeaderUnused();
            void createHeaderRedirect();

            void printInfoRedirect();
            void printInfoEcho();
            void printInfoMtu();

        public:
            IcmpHeader(const Icmp::Type _type, const Icmp::Code _code);
            ~IcmpHeader() = default;

            /**
             * @brief Set the type for the current ICMP Header
             */
            void setType(const Icmp::Type _type);

            /**
             * @brief Get the type field for the current ICMP Header
             * @return An unsigned char (8 bits)
             */
            Icmp::Type getType();

            /**
             * @brief Set the code for the current ICMP Header
             */
            void setCode(const Icmp::Code _code);

            /**
             * @brief Get the code field for the current ICMP Header
             * @return An unsigned char (8 bits)
             */
            Icmp::Code getCode();

            /**
             * @brief Set the checksum field for the current ICMP Header
             */
            void setChecksum(const unsigned short _checksum);

            /**
             * @brief Get the checksum field value for the current ICMP Header
             * @return An unsigned short (16 bits)
             */
            unsigned short getChecksum();

            /**
             * @brief Set the gateway field for the current ICMP Header
             * @throw runtime_error if the current type and code do not match
             * the header format that includes the gateway.
             */
            void setGateway(const unsigned int _gateway);

            /**
             * @brief Get the gateway field value for the current ICMP Header
             */
            unsigned int getGateway();

            /**
             * @brief Set the Identifier field for the current ICMP Header
             * @throw runtime_error if the current type and code do not match
             * the header format that includes the identifier
             */
            void setIdentifier(const unsigned short _id);

            /**
             * @brief Get the Identifier field for the current ICMP Header
             */
            unsigned short getIdentifier();

            /**
             * @brief Set the Sequence Number field for the current ICMP Header
             * @throw runtime_error if the current type and code do not match
             * the header format that includes the sequence number
             */
            void setSequenceNumber(const unsigned short _sn);

            /**
             * @brief Get the Sequence Number field for the current ICMP Header
             */
            unsigned short getSequenceNumber();

            /**
             * @brief Set the Next Hop MTU field for the current ICMP Header
             * @throw runtime_error if the current type and code do not match
             * the header format that includes the Next Hop MTU
             */
            void setNextHopMtu(const unsigned short _mtu);

            /**
             * @brief Get the Next Hop MTU field for the current ICMP Header
             */
            unsigned short getNextHopMtu();

            /**
             * @overload PacketHeader::printInfo
             * @brief Print all the header fields with corresponding values
             */
            void printInfo();

            /**
             * @overload PacketHeader::encode
             * @brief Encode the ICMP Header into a buffer of bytes filling the input ByteBuffer.
             * Notice that all elements with size grater than 1 are converted from little-endian
             * into big-endian (network byte order).
             * 
             * @param _buffer The ByteBuffer to fill with the Header
             */
            void encode(Utils::ByteBuffer& _buffer);

            /**
             * @overload PacketHeader::encode
             * @brief Encode the ICMP Header into a Byte Buffer
             * @return A pointer to the byte buffer containing the header
             */
            Utils::ByteBuffer_ptr encode();

            /**
             * @overload PacketHeader::decode
             * @brief Decode the input ByteBuffer into an ICMP Header
             */
            void decode(Utils::ByteBuffer& _buffer);
    };

    /**
     * @class Icmp
     * 
     * This class contains the ICMP Packet.
     */
    class IcmpPacket : public Packet
    {
        public:
            static const unsigned short maxSize = 0xffe3; //!< The maximum size of the payload

        private:
            IcmpHeader  _hdr;   //!< The ICMP Packet Header

            void fillHeaderUnused(const unsigned short _checksum);
            void fillHeaderRedirect(const unsigned short _checksum, const unsigned int _gateway);
            void fillHeaderEcho(
                const unsigned short _checksum, const unsigned short _id, 
                const unsigned short _seqnum);

            void fillHeaderMtu(const unsigned short _checksum, const unsigned short _mtu);
        
        public:
            IcmpPacket(const Icmp::Type _type, const Icmp::Code _code) \
                : Packet(IcmpPacket::maxSize), _hdr(_type, _code) {};

            IcmpPacket(const Icmp::Type _type, const Icmp::Code _code, const std::size_t _size) \
                : Packet(_size), _hdr(_type, _code) {};

            ~IcmpPacket() = default;

            /**
             * @brief Returns the pointer to the packet header
             * @return The packet header memory address
             */
            PacketHeader* getPacketHeader();

            /**
             * @brief Returns the total size of the packet header + payload
             * @return The total size of the packet
             */
            std::size_t getPacketSize();

            /**
             * @brief Returns the total size of the payload
             */
            const std::size_t& getPayloadSize();

            /**
             * @brief Set the Packet header with the values of the input header
             * 
             * @param _hdr The input packet header
             */
            void setHeader(PacketHeader* _hdr);
            
            /**
             * @brief Copy the input payload of given size into the ByteBuffer
             * 
             * If the input size of the input payload differs from the size
             * given as input, in particular it is bigger, the ByteBuffer of 
             * the packet will be adapted.
             * 
             * @param _payload The payload to be copied
             * @param _size The size of the input payload 
             */
            void setPayload(unsigned char* _payload, const std::size_t _size);
            
            /**
             * @brief Set the current ByteBuffer with the one in input
             * 
             * @param _buff The input ByteBuffer
             */
            void setPayload(Utils::ByteBuffer& _buff);

            /**
             * @brief Encode the current Icmp Packet into a ByteBuffer
             * @param _buffer The input ByteBuffer
             */
            void encode(Utils::ByteBuffer& _buffer);

            /**
             * @brief Encode the Icmp Packet and returns the ByteBuffer
             * @returns The ByteBuffer with the encoding
             */
            Utils::ByteBuffer_ptr encode();

            /**
             * @brief Decode the input ByteBuffer into an ICMP Packet
             * @param _buffer The ByteBuffer containing the encoded Packet
             */
            void decode(Utils::ByteBuffer& _buffer);
    };
};

#endif