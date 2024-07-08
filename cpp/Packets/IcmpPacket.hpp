#ifndef _ICMP_PACKET_H
#define _ICMP_PACKET_H

#include "Base.hpp"

#define ICMP_HEADER_MAX_SIZE 0x08

// Summary of ICMP Message Types
#define ICMP_ECHO_REPLY_TYPE              0x00
#define ICMP_DESTINATION_UNREACHABLE_TYPE 0x03
#define ICMP_SOURCE_QUENCH_TYPE           0x04
#define ICMP_REDIRECT_TYPE                0x05
#define ICMP_ECHO_TYPE                    0x08
#define ICMP_TIME_EXCEEEDED_TYPE          0x0b
#define ICMP_PARAMETER_PROBLEM_TYPE       0x0c
#define ICMP_INFORMATION_REQUEST_TYPE     0x0f
#define ICMP_INFORMATION_REPLY_TYPE       0x10

// Summary of Destination Unreachable Codes
#define ICMP_NET_UNREACHABLE_CODE      0x00
#define ICMP_HOST_UNREACHABLE_CODE     0x01
#define ICMP_PROTOCOL_UNREACHABLE_CODE 0x02
#define ICMP_PORT_UNREACHABLE_CODE     0x03
#define ICMP_FRAGMENTATION_NEEDED_CODE 0x04
#define ICMP_SOURCE_ROUTE_FAILED_CODE  0x05

#define ICMP_ECHO_CODE 0x0
#define ICMP_PAYLOAD_MAXIMUM_SIZE 0xffe3

namespace Ip
{

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
        private:
            unsigned char  _type;     //!< The type of the ICMP Message
            unsigned char  _code;     //!< The code corresponding to the Type
            unsigned short _checksum; //!< The checksum for packet validation
            union h_data_t _rest;

            void createHeaderMtu();
            void createHeaderEcho();
            void createHeaderUnused();
            void createHeaderRedirect();

        public:
            IcmpHeader(const unsigned char _type, const unsigned char _code);
            ~IcmpHeader() = default;

            /**
             * @brief Set the type for the current ICMP Header
             */
            void setType(unsigned char _type);

            /**
             * @brief Get the type field for the current ICMP Header
             * @return An unsigned char (8 bits)
             */
            unsigned char getType();

            /**
             * @brief Set the code for the current ICMP Header
             */
            void setCode(unsigned char _code);

            /**
             * @brief Get the code field for the current ICMP Header
             * @return An unsigned char (8 bits)
             */
            unsigned char getCode();
    };
};

#endif