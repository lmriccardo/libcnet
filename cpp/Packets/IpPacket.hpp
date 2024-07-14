/****************************************************************************************
 ****************************************************************************************
 *****                                                                              *****
 *****   @details Header file                                                       *****
 *****   @name IpPacket.hpp                                                         *****
 *****   @author Riccardo La Marca (riccardo.lamarca98@gmail.com)                   *****
 *****   @date 12 July 2024 (Mon Jul 12 2024 Last Modification)                     *****
 *****                                                                              *****
 *****   @brief This file contains the creation of Ip classes for IP Packets        ***** 
 *****                                                                              *****
 ****************************************************************************************
 ****************************************************************************************
*/

#ifndef _IP_PACKET_H
#define _IP_PACKET_H

#pragma once

#include <Utils/NetUtil.hpp>
#include "Base.hpp"

namespace Packets
{
    namespace Ip
    {
        /**
         * @class Packets::Ip::Version
         * 
         * All possible version of the IP Protocol
         */
        enum class Version : unsigned char
        {
            IPv4 = 0x04,
            IPv6 = 0x06
        };

        /**
         * @class Packets::Ip::Flags
         * 
         * All possible values for D, M and X flags
         */
        enum class Flag : unsigned char
        {
            X_FLAG_NOT_SET = 0,
            D_FLAG_NOT_SET = 0,
            M_FLAG_NOT_SET = 0,
            D_FLAG_SET     = 0x02,
            M_FLAG_SET     = 0x01
        };

        /**
         * @class Packets::Ip::DSCP
         * 
         * All possible values for the Differentiated Service Code Point field
         */
        enum class DSCP : unsigned int
        {
            CS0 = 0x00, CS1 = 0x08, CS2 = 0x10, CS3 = 0x18, CS4 = 0x20, CS5 = 0x28, 
            CS6 = 0x30, CS7 = 0x38, AF11 = 0x0a, AF12 = 0x0c, AF13 = 0x0e, 
            AF21 = 0x12, AF22 = 0x14, AF23 = 0x16, AF31 = 0x1a, AF32 = 0x1c, 
            AF33 = 0x1e, AF41 = 0x22, AF42 = 0x24, AF43 = 0x26, EF = 0x2e,
            VOICE_ADMIT = 0x2c
        };

        /**
         * @class Packets::Ip::ECN
         * 
         * All possible values for the Eplicit Congestion Notification field
         */
        enum class ECN : unsigned int
        {
            ECN_NECT = 0x00, // Not-ECT (Not-ECN Capable Transport)
            ECN_ECT1 = 0x01, // ECN-Capable Transport (1) - Experimental Only
            ECN_ECT0 = 0x02, // ECN-Capable Transport (0)
            ECN_CE   = 0x03  // Congestion Experienced
        };

        /**
         * @class Packets::Ip::Protocols
         * 
         * Some of the possible protocols codes for IP Packet Header. In particular
         * there are values just for TCP, UDP and ICMP.
         */
        enum class Protocol : unsigned char
        {
            ICMP = 0x01,
            UDP  = 0x11,
            TCP  = 0x06
        };
    };

    /**
     * @class Packets::IpHeader
     * 
     * Class representing the IP Header of an IP Packet. It consists of classical 20 bytes.
     * The construction of this class refers to the RFC 791 https://datatracker.ietf.org/doc/html/rfc791
     * The TOS field has been substituted with the Differentiated Service Field in according
     * to the RFC 2474 https://datatracker.ietf.org/doc/html/rfc2474.
     */
    class IpHeader : public PacketHeader
    {
        public:
            static const std::size_t size = 0x14; //!< The Size of the entire Ip Packet header

        private:
            Ip::Version    _version;  //!< The version of the IP Protocol
            Ip::DSCP       _dscp;     //!< The Differentiated Service Code Point Used for the DSF
            Ip::ECN        _ecn;      //!< The Explicit Congestion Notification Used for the DSF

            unsigned char  _dsf;      //!< The Differentiated Service Field value
            unsigned short _tlength;  //!< Total length of the IP datagram or IP fragment
            unsigned short _id;       //!< Unique fragment identifier
            unsigned short _flags;    //!< The IP header flags for fragmentation
            unsigned short _offset;   //!< The fragment offset
            unsigned char  _ttl;      //!< Time-To-Live value
            Ip::Protocol   _proto;    //!< The protocol value (4 / 6)
            unsigned short _checksum; //!< The Header checksum
            unsigned int   _srcaddr;  //!< The source address
            unsigned int   _dstaddr;  //!< The destination address

        public:
            IpHeader() = default;
            ~IpHeader() = default;

            /**
             * @brief Set the version of the IP Packet
             * @param _version The Ip Packet Version
             */
            void setVersion(const Ip::Version _version);

            /**
             * @brief Set the version of the IP Packet
             * @param _version The Ip Packet Version
             */
            void setVersion(const unsigned char _version);

            /**
             * @brief Returns the Ip Packet version
             * @return The Ip Packet Version
             */
            const Ip::Version& getVersion();

            /**
             * @brief Constructs and set the DFS field from the given input value
             * 
             * The value is constructed as (_dscp << 4) + _ecn
             * 
             * @param _dscp The Differentiated Service Code Point value
             * @param _ecn The Explicit Congestion Notification value
             */
            void setDifferentiatedServiceField(const Ip::DSCP _dscp, const Ip::ECN _ecn);

            /**
             * @brief Constructs and set the DFS field from the given input value
             * 
             * The value is constructed as (_dscp << 4) + _ecn
             * 
             * @param _dscp The Differentiated Service Code Point value
             * @param _ecn The Explicit Congestion Notification value
             */
            void setDifferentiatedServiceField(const unsigned short _dscp, const unsigned short _ecn);

            /**
             * @brief Set the Input Differentiated Service Field value
             * 
             * This method also set the corresponding ECN and DSCP values
             * 
             * @param _dsf The Differentiated Service Field value
             */
            void setDifferentiatedServiceField(const unsigned char _dsf);

            /**
             * @brief Returns the Differentiated Service Field Value
             * @return An unsigned char
             */
            unsigned char getDifferentiatedServiceField();

            /**
             * @brief Returns the Differentiated Service Code Point value
             * @return DSCP Value
             */
            const Ip::DSCP& getDSCP();

            /**
             * @brief Returns the Explicit Congestion Notification value
             * @return ECN Value
             */
            const Ip::ECN& getECN();

            /**
             * @brief Set the Total Length field with input value
             * @param _tlength The input total length
             */
            void setTotalLength(const unsigned short _tlength);

            /**
             * @brief Get the Total length field value
             * @return An unsigned short
             */
            unsigned short getTotalLength();

            /**
             * @brief Set the Identifier with input value
             * @param _id The input identifier
             */
            void setIdentifier(const unsigned short _id);

            /**
             * @brief Get the Identifier field value
             * @return An unsigned short
             */
            unsigned short getIdentifier();

            /**
             * @brief Set the Ip Fragmentation Flags
             * 
             * The overall flag is computed just summing input flags.
             * Notice that the X flag is reserved and always set to 0.
             * 
             * @param _d The Dont'Fragment Flag
             * @param _m The More Fragment Flag
             */
            void setFragmentationFlags(const Ip::Flag _d, const Ip::Flag _m);

            /**
             * @brief Set the Ip Fragmentation Flags
             * 
             * The overall flag is computed just summing input flags.
             * Notice that the X flag is reserved and always set to 0.
             * 
             * @param _d The Dont'Fragment Flag
             * @param _m The More Fragment Flag
             * @throws std::runtime_error If given flags have wrong values
             */
            void setFragmentationFlags(const unsigned char _d, const unsigned char _m);

            /**
             * @brief Get the Fragmentation Flags of the IP Header
             * @return An unsigned char
             */
            unsigned char getFragmentationFlags();

            /**
             * @brief Converts the Flags into a binary string
             * @returns The binary string
             */
            std::string convertFlagsToBinaryString();

            /**
             * @brief Set the fragment offset to the IP Header
             * @param _offset The fragment offset
             */
            void setFragmentOffset(unsigned short _offset);

            /**
             * @brief Returns the fragment offset of the Ip Header
             * @returns An unsigned short
             */
            unsigned short getFragmentOffset();

            /**
             * @brief Set the Time To Live of the IP Header
             * @param _ttl The time to live
             */
            void setTimeToLive(unsigned char _ttl);

            /**
             * @brief Get the Time To Live Field value
             * @returns An unsigned char
             */
            unsigned char getTimeToLive();

            /**
             * @brief Set the protocol applied to the IP Packet
             * @param _proto The IP Protocol
             */
            void setProtocol(const Ip::Protocol _proto);

            /**
             * @brief Set the protocol applied to the IP Packet
             * @param _proto The IP Protocol
             */
            void setProtocol(const unsigned char _proto);

            /**
             * @brief Returns the IP Protocol (Either ICMP, UDP or TCP)
             * @returns The ip protocol
             */
            const Ip::Protocol& getProtocol();

            /**
             * @brief Set the checksum of the IP Packet
             * @param _checksum The checksum value
             */
            void setChecksum(const unsigned short _checksum);

            /**
             * @brief Returns the checksum of the packet
             * @returns An unsigned short
             */
            unsigned short getChecksum();

            /**
             * @brief Set the source address of the IP Packet
             * @param _srcaddr The source address
             */
            void setSourceAddress(const unsigned int _srcaddr);

            /**
             * @brief Returns the source address of the packet
             * @returns An unsigned int
             */
            unsigned int getSourceAddress();

            /**
             * @brief Set the destination address of the IP Packet
             * @param _dstaddr The destination address
             */
            void setDestinationAddress(const unsigned int _dstaddr);

            /**
             * @brief Returns the destination address of the packet
             * @returns An unsigned int
             */
            unsigned int getDestinationAddress();

            /**
             * @brief Print all the header fields with the corresponding values
             */
            void printInfo();

            void encode(Utils::ByteBuffer& _buffer);
            Utils::ByteBuffer_ptr encode();
            
            void decode(Utils::ByteBuffer& _buffer);
    };

    class IpPacket : public Packet
    {

    };
};

#endif