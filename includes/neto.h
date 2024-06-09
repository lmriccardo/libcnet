#ifndef _NETO_H
#define _NETO_H

#include <sys/cdefs.h>
#include <sys/types.h>
#include <stdbool.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include "buffer.h"

__BEGIN_DECLS

/******************************* ICMP PACKET ******************************/

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


struct h_echo_t 
{
    
    u_int16_t _id; // 16 bit for the identifier of the echo or echo reply
    u_int16_t _seqnum; // 16 bit for the sequence number

};

union h_data_t 
{

    u_int32_t       _unused;  // 32 bit of unused data
    u_int32_t       _gateway; // 32 bit for gateway internet address
    struct h_echo_t _echo;    // Echo and other message headers
    
};

/* Struct representing the ICMP Header of the ICMP Packet. There are the
   classical 32 bits of ICMP Header: Type, Code and Checksum and the also
   the remaining 32 bits of Optional header fields like: Identification,
   Sequence Number, Gateway or Zeros (Unused) according to the given type
   and ICMP Code.
   
   This structure refers to the RFC 792 https://datatracker.ietf.org/doc/html/rfc792 
*/
typedef struct
{

    u_int8_t        _type;      // The type of the ICMP Message
    u_int8_t        _code;      // The code corresponding to the Type
    u_int16_t       _checksum;  // The checksum for packet validation
    union h_data_t *_rest;      // Additional data to the header

} IcmpHeader;

/* Struct representing the ICMP Packet with 8 bytes of Header and Payload */
typedef struct
{

    IcmpHeader *_icmphdr; // The ICMP Header
    char       *_payload; // The payload containing all the ICMP data

    size_t __size;  // The size of the payload

} IcmpPacket;

/* Return a pointer to an IcmpHeader struct initialized given the input type */
extern IcmpHeader* IcmpHeader_new(const u_int8_t _type) __attribute__((returns_nonnull));

/* Free the memory of the previously allocated IcmpHeader */
extern void IcmpHeader_delete(IcmpHeader* _self) __attribute__((nonnull));

/* Set the Type field of the ICMP header with the input Type */
extern void IcmpHeader_setType(IcmpHeader* _self, const u_int8_t _type) __attribute__((nonnull));

/* Set the Code field of the ICMP header with the input Code */
extern void IcmpHeader_setCode(IcmpHeader* _self, const u_int8_t _code) __attribute__((nonnull));

/* Set the Checksum field of the ICMP header with the input checksum */
extern void IcmpHeader_setChecksum(IcmpHeader* _self, const u_int16_t _checksum) __attribute__((nonnull));

/* Set the Gateway field of the ICMP header with the input gateway address
   if the given ICMP Type corresponds to the correct ICMP Header format.
   If it is not, then an error is raise and the process exit with failure
*/
extern void IcmpHeader_setGateway(IcmpHeader* _self, const u_int32_t _gateway) __attribute__((nonnull));

/* Set the Identifier field of the ICMP header with the input identifier
   if the given ICMP Type corresponds to the correct ICMP Header format.
   If it is not, then an error is raise and the process exit with failure
*/
extern void IcmpHeader_setIdentifier(IcmpHeader* _self, const u_int16_t _id) __attribute__((nonnull));

/* Set the Sequence Number field of the ICMP header with the input sequence number
   if the given ICMP Type corresponds to the correct ICMP Header format.
   If it is not, then an error is raise and the process exit with failure
*/
extern void IcmpHeader_setSequenceNumber(IcmpHeader* _self, const u_int16_t _seqnum) __attribute__((nonnull));

/* Print all the Header fields with corresponding values */
extern void IcmpHeader_printInfo(const IcmpHeader* _self) __attribute__((nonnull));
extern void IcmpHeader_printInfo_v2(const IcmpHeader* _self) __attribute__((nonnull));
extern void IcmpHeader_printInfo_v3(const IcmpHeader* _self) __attribute__((nonnull));

extern void __IcmpHeader_createHeader_v1(IcmpHeader* _self) __attribute__((nonnull));
extern void __IcmpHeader_createHeader_v2(IcmpHeader* _self) __attribute__((nonnull));
extern void __IcmpHeader_createHeader_v3(IcmpHeader* _self) __attribute__((nonnull));

/* Compute the ICMP Checksum as defined in the Corresponding RFC 792.
   The input buffer parameter is the header encoded into a buffer of bytes.
*/
extern u_int16_t computeIcmpChecksum(const char* _buff) __attribute__((nonnull));

/* Encode the Icmp Header into a buffer of bytes filling the input ByteBuffer.
   Notice that all the elements with size grater than 1 are converted
   from little-endian into big-endian (network byte order)
*/
extern void IcmpHeader_encode(const IcmpHeader *_self, ByteBuffer* _buffer) __attribute__((nonnull));

/* Perform the encoding as the `IcmpHeader_encode` function and also returns the ByteBuffer */
extern ByteBuffer* IcmpHeader_encode_v2(const IcmpHeader *_self) __attribute__((nonnull)) __attribute__((returns_nonnull));

/* Decode the input bytes into an ICMP Header */
extern IcmpHeader* IcmpHeader_decode(ByteBuffer* _buffer) __attribute__((nonnull)) __attribute__((returns_nonnull));

/* Create a new ICMP Packet initialized according to input Type */
extern IcmpPacket* IcmpPacket_new(const u_int8_t _type) __attribute__((returns_nonnull));

/* Create a new ICMP Packet initialized according to input Type and Payload Size */
extern IcmpPacket* IcmpPacket_new_v2(const u_int8_t _type, const size_t _size) __attribute__((returns_nonnull));

/* Free the memory allocated for the ICMP Packet */
extern void IcmpPacket_delete(IcmpPacket* _self) __attribute__((nonnull));

/* Fill the Header of the ICMP Packet with the Code. This version of the
   `fillHeader` function is reserved for ICMP Header with 32 bits unused
*/
extern void IcmpPacket_fillHeader_v1(IcmpPacket* _self, const u_int8_t _code) __attribute__((nonnull));

/* Fill the Header of the ICMP Packet with the Code and the Gateway. This version of the
   `fillHeader` function is reserved for ICMP Header with 32 bits for the gateway address
*/
extern void IcmpPacket_fillHeader_v2(IcmpPacket* _self, const u_int8_t _code, const u_int32_t _gateway) __attribute__((nonnull));

/* Fill the Header of the ICMP Packet with the Code, Identifier and Sequence Number. 
   This version of the `fillHeader` function is reserved for ICMP Header with 32 bits 
   divided into Identifier (16 bits) and Sequence Number (16 bits).
*/
extern void IcmpPacket_fillHeader_v3(
    IcmpPacket* _self, const u_int8_t _code, const u_int16_t _id, const u_int16_t _seqnum) __attribute__((nonnull));

/* Set a new Header for the ICMP Packet. The input headers will be copied into
   a new header of the ICMP Packet, and the memory for the input header will
   be freed. This means, that it will be unusable after calling this function */
extern void IcmpPacket_setHeader(IcmpPacket* _self, IcmpHeader* _hdr) __attribute__((nonnull));

/* Fill the ICMP Packet payload with input buffer and set a new size */
extern void IcmpPacket_fillPayload(IcmpPacket* _self, const char* _data, const size_t _size) __attribute__((nonnull));

/* Returns the entire size of the ICMP Packet (Header + Payload) */
extern size_t IcmpPacket_getPacketSize(const IcmpPacket* _self) __attribute__((nonnull));

/* Encodes the entire ICMP Packet into a ByteBuffer */
extern ByteBuffer* IcmpPacket_encode(const IcmpPacket *_self) __attribute__((nonnull)) __attribute__((returns_nonnull));

/* Decode the input ByteBuffer into a ICMP Packet */
extern IcmpPacket* IcmpPacket_decode(ByteBuffer *_buffer) __attribute__((nonnull)) __attribute__((returns_nonnull));

/******************************* UDP PACKET ******************************/

#define UDP_HEADER_SIZE             0x08
#define UDP_PAYLOAD_MAX_SIZE        0xffe3
#define UDP_HEADER_PLUS_PSEUDO_SIZE 0x14

/* Struct representing the UDP Header of the UDP Packet. It consists of the
   classical 8 bytes divided into: (optional) Source port (16 bits),
   Destination port (16 bits), Total length (16 bits) and Checksum (16 bits)
    
   This structure refers to the RFC 768 https://datatracker.ietf.org/doc/html/rfc768
*/
typedef struct
{

    u_int16_t _srcport;   // The optional Source Port 
    u_int16_t _dstport;   // The Desination Port
    u_int16_t _length;    // The total length of the UDP Packet
    u_int16_t _checksum;  // The checksum

} UdpHeader;

/* This struct represents the entire UDP Packet (Header + Payload)
   as according to the RFC 768 https://datatracker.ietf.org/doc/html/rfc768
*/
typedef struct
{

    UdpHeader* _hdr;      // The UDP Header
    char*      _payload;  // The payload

} UdpPacket;

/* Creates and returns a new UDP Header instance */
extern UdpHeader* UdpHeader_new() __attribute__((returns_nonnull));

/* Free the memory allocated for the input UDP Header */
extern void UdpHeader_delete(UdpHeader* _self) __attribute__((nonnull));

/* Set the Source Port field into the UDP Header */
extern void UdpHeader_setSourcePort(UdpHeader* _self, const u_int16_t _srcport) __attribute__((nonnull));

/* Set the Destination Port field into the UDP Header */
extern void UdpHeader_setDestinationPort(UdpHeader* _self, const u_int16_t _dstport) __attribute__((nonnull));

/* Set the Length field into the UDP Header */
extern void UdpHeader_setLength(UdpHeader* _self, const u_int16_t _length ) __attribute__((nonnull));

/* Set the Checksum field into the UDP Header */
extern void UdpHeader_setChecksum(UdpHeader* _self, const u_int16_t _checksum) __attribute__((nonnull));

/* Print all the Header fields with current values */
extern void UdpHeader_printInfo(const UdpHeader* _self) __attribute__((nonnull));

/* Compute the UDP Checksum according to its RFC */
extern u_int16_t computeUDPChecksum(const char* _buff) __attribute__((nonnull));

/* Encode the UDP Header into a ByteBuffer */
extern void UdpHeader_encode(const UdpHeader* _self, ByteBuffer* _buffer) __attribute__((nonnull));

/* Encode and returns the ByteBuffer containing the UDP header */
extern ByteBuffer* UdpHeader_encode_v2(const UdpHeader* _self) __attribute__((nonnull)) __attribute__((returns_nonnull));

/* Decode a ByteBuffer into an UDP Header */
extern UdpHeader* UdpHeader_decode(ByteBuffer* _buffer) __attribute__((nonnull)) __attribute__((returns_nonnull));

/* Creates and returns a new UDP Packet */
extern UdpPacket* UdpPacket_new() __attribute__((returns_nonnull));

/* Creates and returns a new UDP Packet with the payload initialized with input size */
extern UdpPacket* UdpPacket_new_v2(const size_t _size) __attribute__((returns_nonnull));

/* Free the memory allocated for the input UDP Packet */
extern void UdpPacket_delete(UdpPacket* _self) __attribute__((nonnull));

/* Fill the header of the input UDP Packet given all its fields */
extern void UdpPacket_fillHeader(
    UdpPacket*       _self,   const u_int16_t _srcport, const u_int16_t _dstport,
    const u_int16_t  _length, const u_int16_t _checksum
) __attribute__((nonnull));

/* Set the header of the udp packet. Notice that the entire header is
   copied into the header of the input UDP Packet. Finally, the memory
   location of the input Header is freed. This means that, once the function
   finished it will not be reachable anymore. 
*/
extern void UdpPacket_setHeader(UdpPacket* _self, UdpHeader* _hdr) __attribute__((nonnull));

/* Fill the payload of the UDP Packet with the input buffer of input size */
extern void UdpPacket_fillPayload(UdpPacket* _self, const char* _data, const size_t _size) __attribute__((nonnull));

/* Returns the size of the payload inside the input UDP Packet */
extern size_t UdpPacket_getPayloadSize(const UdpPacket* _self) __attribute__((nonnull));

/* Encode the UDP Packet into a Byte Buffer */
extern ByteBuffer* UdpPacket_encode(const UdpPacket* _self) __attribute__((nonnull)) __attribute__((returns_nonnull));

/* Decode a ByteBuffer into a UDP Packet */
extern UdpPacket* UdpPacket_decode(ByteBuffer* _buffer) __attribute__((nonnull)) __attribute__((returns_nonnull));

/******************************* IP PACKET ******************************/

#define IPv4 0x4
#define IPv6 0x6
#define IP_HEADER_SIZE 0x14

/* IP Header Flags */
#define X_FLAG_NOT_SET 0
#define D_FLAG_NOT_SET 0
#define M_FLAG_NOT_SET 0
#define D_FLAG_SET     0x02
#define M_FLAG_SET     0x01

#define IP_HEADER_FLAG(_x, _d, _m) (_x + _d + _m)

/* IP Differentiated Service Code Points */
#define IP_HEADER_DSCP_CS0         0x00
#define IP_HEADER_DSCP_CS1         0x08
#define IP_HEADER_DSCP_CS2         0x10
#define IP_HEADER_DSCP_CS3         0x18
#define IP_HEADER_DSCP_CS4         0x20
#define IP_HEADER_DSCP_CS5         0x28
#define IP_HEADER_DSCP_CS6         0x30
#define IP_HEADER_DSCP_CS7         0x38
#define IP_HEADER_DSCP_AF11        0x0a
#define IP_HEADER_DSCP_AF12        0x0c
#define IP_HEADER_DSCP_AF13        0x0e
#define IP_HEADER_DSCP_AF21        0x12
#define IP_HEADER_DSCP_AF22        0x14
#define IP_HEADER_DSCP_AF23        0x16
#define IP_HEADER_DSCP_AF31        0x1a
#define IP_HEADER_DSCP_AF32        0x1c
#define IP_HEADER_DSCP_AF33        0x1e
#define IP_HEADER_DSCP_AF41        0x22
#define IP_HEADER_DSCP_AF42        0x24
#define IP_HEADER_DSCP_AF43        0x26
#define IP_HEADER_DSCP_EF          0x2e
#define IP_HEADER_DSCP_VOICE_ADMIT 0x2c

/* IP Explicit Congestion Notification Codes */
#define IP_HEADER_ECN_NECT 0x00 // Not-ECT (Not-ECN Capable Transport)
#define IP_HEADER_ECN_ECT1 0x01 // ECN-Capable Transport (1) - Experimental Only
#define IP_HEADER_ECN_ECT0 0x02 // ECN-Capable Transport (0)
#define IP_HEADER_ECN_CE   0x03 // Congestion Experienced

/* Some IP Header Protocol Codes */
#define IP_HEADER_ICMP_PROTOCOL_CODE 0x01
#define IP_HEADER_UDP_PROTOCOL_CODE  0x11
#define IP_HEADER_TCP_PROTOCOL_CODE  0x06

/* TTL values */
#ifndef _WIN32
    #define TTL_DEFAULT_VALUE 0x80
#elif __linux__
    #define TTL_DEFAULT_VALUE 0x40
#endif

#define IP_PAYLOAD_MAX_SIZE 65515 // Max dimension of the payload of an IP Packet

/* Struct representing the IP Header of the IP Packet. It consists of classical
   28 bytes divided into: IP Protocol Version + IHL (8 bits), Type of service (8 bits),
   Total length (16 bits), Identification (16 bits), Flags and Fragment Offset (16 bits),
   Time to Live (8 bits), IP Protocol (8 bits), Checksum (16 bits), Source Address
   (32 bits) and Destination Address (32 bits). 
   
   This structure refers to the RFC 791 https://datatracker.ietf.org/doc/html/rfc791
   The TOS field has been subtituted with the Differentiated Service Field
   according to the RFC 2474 https://datatracker.ietf.org/doc/html/rfc2474
*/
typedef struct
{

    u_int8_t  _version;    /* Version of IP protocol */
    u_int8_t  _dsf;        /* Differentiated Service Field */
    u_int16_t _tlength;    /* Total len of IP datagram or IP fragment */
    u_int16_t _id;         /* Unique fragment identifier */
    u_int16_t _flag_off;   /* IP Flags and Fragment Offset */
    u_int8_t  _ttl;        /* Time To Live */
    u_int8_t  _protocol;   /* Protocol */
    u_int16_t _hdr_chksum; /* Header Checksum */
    u_int32_t _srcaddr;    /* Source Address */
    u_int32_t _dstaddr;    /* Destination Address */

} IpHeader;

/* This struct refers to the entire IP Packet (Header + Payload) */
typedef struct
{

    IpHeader *_iphdr;
    char     *_payload;
    
} IpPacket;

/* Creates and returns a new IP Header */
extern IpHeader* IpHeader_new() __attribute__((returns_nonnull));

/* Free the memory allocated for the input IP Header */
extern void IpHeader_delete(IpHeader* _self) __attribute__((nonnull));

/* Set the IP Version field and the IHL (always 5) field */
extern void IpHeader_setVersion(IpHeader* _self, const u_int8_t _version) __attribute__((nonnull));

/* Set the differentiated Service field In the IP Header */
extern void IpHeader_setDifferentiatedServiceField(IpHeader* _self, const u_int8_t _dsf) __attribute__((nonnull));

/* Set the Total Length field of the IP Header */
extern void IpHeader_setTotalLength(IpHeader* _self, const u_int16_t _total_length) __attribute__((nonnull));

/* Set the Identification field of the IP Header */
extern void IpHeader_setIdentfication(IpHeader* _self, const u_int16_t _identification) __attribute__((nonnull));

/* Set the Flag and the Fragment Offset field */
extern void IpHeader_setFlagOffField(IpHeader* _self, const u_int16_t _flagoff) __attribute__((nonnull));

/* Set the TTL Field */
extern void IpHeader_setTimeToLive(IpHeader* _self, const u_int8_t _time_to_live) __attribute__((nonnull));

/* Set the Protocol Field */
extern void IpHeader_setProtocol(IpHeader* _self, const u_int8_t _protocol) __attribute__((nonnull));

/* Set the Checksum of the Header */
extern void IpHeader_setHeaderChecksum(IpHeader* _self, const u_int16_t _checksum) __attribute__((nonnull));

/* Set the source Address field of the IP Header */
extern void IpHeader_setSourceAddress(IpHeader* _self, const u_int32_t _srcaddres) __attribute__((nonnull));

/* Set the Destination Address field of the IP Header */
extern void IpHeader_setDestinationAddress(IpHeader* _self, const u_int32_t _dstaddress) __attribute__((nonnull));

/* Returns the version of the IP */
extern u_int8_t IpHeader_getVersion(const IpHeader* _self) __attribute__((nonnull));

/* Returns the IHL Value from the IP Header */
extern u_int8_t IpHeader_getInternetHeaderLength(const IpHeader* _self) __attribute__((nonnull));

/* Returns the Differentiated Service Code Point from the DSF field */
extern u_int8_t IpHeader_getDSCP(const IpHeader* _self) __attribute__((nonnull));

/* Returns the Explicit Congestion Notification value from the DSF field */
extern u_int8_t IpHeader_getECN(const IpHeader* _self) __attribute__((nonnull));

/* Returns the Flags value from the IP Header */
extern u_int8_t IpHeader_getFlags(const IpHeader* _self) __attribute__((nonnull));

/* Returns the Fragment Offset value from the IP Header */
extern u_int8_t IpHeader_getFragmentOffset(const IpHeader* _self) __attribute__((nonnull));

/* Encode the IP header into a ByteBuffer */
extern void IpHeader_encode(const IpHeader* _self, ByteBuffer* _buffer) __attribute__((nonnull));

/* Encode the IP header into a ByteBuffer and returns the ByteBuffer */
extern ByteBuffer* IpHeader_encode_v2(const IpHeader* _self) __attribute__((nonnull)) __attribute__((returns_nonnull));

/* Decode the input ByteBuffer into an IP Header */
extern IpHeader* IpHeader_decode(ByteBuffer* _buffer) __attribute__((nonnull)) __attribute__((returns_nonnull));

/* Prints the IP Header fields and values */
extern void IpHeader_printInfo(const IpHeader* _self) __attribute__((nonnull));

/* Compute the value for the Flags + Fragment Offset field by combining the
   three input flags into a single value and then combining this single value
   with the input fragment offset into the final value
*/
extern u_int16_t computeFlagOff(const int _x, const int _d, const int _m, const int _offset);

/* Compute the value for the DSF Field by combining the input DSCP and ECN values */
extern u_int8_t computeDifferentiatedServiceField(const int _dscp, const int _ecn);

/* Converts the IP Header flags into a binary string (i.e., 2 -> 010) */
extern char* convertFlagToBin(const u_int8_t _flags) __attribute__((returns_nonnull));

/* Converts the input address number into a string. The second argument is used
   to specify whether the input address number is in LE or BE format.
*/
extern char* addressNumberToString(u_int32_t _addr, const bool _be) __attribute__((returns_nonnull));

/* Create and returns a new instance of IP Packet */
extern IpPacket* IpPacket_new() __attribute__((returns_nonnull));

/* Free the memory allocated for the input IP Packet */
extern void IpPacket_delete(IpPacket* _self) __attribute__((nonnull));

/* Set a new header for the IP Packet. Notice that the conten of the new header is entirely copied 
   inside the header of the input packet. Once all the content has been copied, the memory allocated 
   for the  input header will be freed. This means that, once this function ends the memory location 
   of the input IP Header is no longer accessible.
*/
extern void IpPacket_setHeader(IpPacket * _self, IpHeader * _iphdr) __attribute__((nonnull));

/* Fill the header of the input IP Packet with all the values given as input */
extern void IpPacket_fillHeader(
    IpPacket*       _self,     const u_int8_t  _version, const u_int8_t  _dsf, const u_int16_t _tlen,
    const u_int16_t _id,       const u_int16_t _flagoff, const u_int8_t  _ttl, const u_int8_t  _protocol, 
    const u_int16_t _checksum, const u_int32_t _srcaddr, const u_int32_t _dstaddr
) __attribute__((nonnull));

/* Fill the payload of the input IP Packet with the input data of input size */
extern void IpPacket_fillPayload(IpPacket * _self, const char *_data, const size_t _datasize) __attribute__((nonnull));

/* Returns the payload size of the input IP Packet */
extern u_int16_t IpPacket_getPayloadSize(const IpPacket * _self) __attribute__((nonnull));

/* Returns the ICMP Packet encoded into the IP Packet Payload */
extern IcmpPacket* IpPacket_getIcmpPacket(const IpPacket *_self) __attribute__((nonnull)) __attribute__((returns_nonnull));

/* Encode the IP Packet into a ByteBuffer */
extern ByteBuffer* IpPacket_encode(const IpPacket* _self) __attribute__((nonnull)) __attribute__((returns_nonnull));

/* Decode the input ByteBuffer into an IP packet */
extern IpPacket* IpPacket_decodeIcmp(ByteBuffer* _buffer) __attribute__((nonnull)) __attribute__((returns_nonnull));

/* Wrap the input ICMP Packet into the payload of the input IP Packet */
extern void IpPacket_wrapIcmp(IpPacket* _self, IcmpPacket* _icmppckt) __attribute__((nonnull));

/* Wrap the input UDP Packet into the payload of the input IP Packet */
extern void IpPacket_wrapUdp(IpPacket* _self, UdpPacket* _udppckt) __attribute__((nonnull));

__END_DECLS

#endif