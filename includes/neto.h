#ifndef _NETO_H
#define _NETO_H

#include <sys/cdefs.h>
#include <sys/types.h>
#include <stdbool.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include "buffer.h"

__BEGIN_DECLS

/******************************* ICMP PACKET ******************************/

#define ICMP_HEADER_MAX_SIZE 0x08

// Summary of ICMP Message Types
#define ICMP_ECHO_REPLY_TYPE             0x00
#define ICMP_DESTINATIO_UNREACHABLE_TYPE 0x03
#define ICMP_SOURCE_QUENCH_TYPE          0x04
#define ICMP_REDIRECT_TYPE               0x05
#define ICMP_ECHO_TYPE                   0x08
#define ICMP_TIME_EXCEEEDED_TYPE         0x0b
#define ICMP_PARAMETER_PROBLEM_TYPE      0x0c
#define ICMP_INFORMATION_REQUEST_TYPE    0x0f
#define ICMP_INFORMATION_REPLY_TYPE      0x10

// Summary of Destination Unreachable Codes
#define ICMP_NET_UNREACHABLE_CODE      0x00
#define ICMP_HOST_UNREACHABLE_CODE     0x01
#define ICMP_PROTOCOL_UNREACHABLE_CODE 0x02
#define ICMP_PORT_UNREACHABLE_CODE     0x03
#define ICMP_FRAGMENTATION_NEEDED_CODE 0x04
#define ICMP_SOURCE_ROUTE_FAILED_CODE  0x05

struct h_echo_t {
    
    u_int16_t _id; // 16 bit for the identifier of the echo or echo reply
    u_int16_t _seqnum; // 16 bit for the sequence number

};

union h_data_t {

    u_int32_t       _unused;  // 32 bit of unused data
    u_int32_t       _gateway; // 32 bit for gateway internet address
    struct h_echo_t _echo;    // Echo and other message headers
    
};

typedef struct
{

    u_int8_t        _type;      // The type of the ICMP Message
    u_int8_t        _code;      // The code corresponding to the Type
    u_int16_t       _checksum;  // The checksum for packet validation
    union h_data_t *_rest;      // Additional data to the header

} IcmpHeader;

typedef struct
{

    IcmpHeader *_icmphdr; // The ICMP Header
    char       *_payload; // The payload containing all the ICMP data

    size_t __size;  // The size of the payload

} IcmpPacket;


/* ICMP Header Functions */
extern IcmpHeader* IcmpHeader_new(u_int8_t _type);
extern void        IcmpHeader_delete(IcmpHeader* _self);

extern void IcmpHeader_setType(IcmpHeader* _self, u_int8_t _type);
extern void IcmpHeader_setCode(IcmpHeader* _self, u_int8_t _code);
extern void IcmpHeader_setChecksum(IcmpHeader* _self, u_int16_t _checksum);
extern void IcmpHeader_setGateway(IcmpHeader* _self, u_int32_t _gateway);
extern void IcmpHeader_setIdentifier(IcmpHeader* _self, u_int16_t _id);
extern void IcmpHeader_setSequenceNumber(IcmpHeader* _self, u_int16_t _seqnum);
extern void IcmpHeader_printInfo(IcmpHeader* _self);
extern void IcmpHeader_printInfo_v2(IcmpHeader* _self);
extern void IcmpHeader_printInfo_v3(IcmpHeader* _self);

extern void __IcmpHeader_createHeader_v1(IcmpHeader* _self);
extern void __IcmpHeader_createHeader_v2(IcmpHeader* _self);
extern void __IcmpHeader_createHeader_v3(IcmpHeader* _self);

extern u_int16_t computeIcmpChecksum(char* _buff, size_t _size);

extern void IcmpHeader_encode(IcmpHeader *_self, ByteBuffer* _buffer);
extern ByteBuffer* IcmpHeader_encode_v2(IcmpHeader *_self);

/* ICMP Packet Functions */
#define ICMP_PAYLOAD_MAXIMUM_SIZE 0xffe3

extern IcmpPacket* IcmpPacket_new(u_int8_t _type);
extern IcmpPacket* IcmpPacket_new_v2(u_int8_t _type, size_t _size);
extern void        IcmpPacket_delete(IcmpPacket* _self);

extern void IcmpPacket_fillHeader_v1(IcmpPacket* _self, u_int8_t _code);
extern void IcmpPacket_fillHeader_v2(IcmpPacket* _self, u_int8_t _code, u_int32_t _gateway);
extern void IcmpPacket_fillHeader_v3(
    IcmpPacket* _self, u_int8_t _code, u_int16_t _id, u_int16_t _seqnum);

extern void IcmpPacket_fillPayload(IcmpPacket* _self, char* _data, size_t _size);
extern size_t IcmpPacket_getPacketSize(IcmpPacket* _self);
extern ByteBuffer* IcmpPacket_encode(IcmpPacket *_self);

/******************************* IP PACKET ******************************/

#define IPv4 0x4
#define IPv6 0x6
#define IP_HEADER_SIZE 0x14

/* IP Header Flags */
#define X_FLAG_NOT_SET 0
#define D_FLAG_NOT_SET 0
#define M_FLAG_NOT_SET 0
#define D_FLAG_SET     0x02
#define M_FLAG_SET     0x04

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


#define IP_PAYLOAD_MAX_SIZE 65515 // Max dimension of the payload of an IP Packet


typedef struct
{

    IpHeader *_iphdr;
    char     *_payload;
    
} IpPacket;

/* Constructor, Deconstructor and Methods for IpHeader struct/class */
extern IpHeader* IpHeader_new();

extern void IpHeader_delete(IpHeader* _self);

extern void IpHeader_setVersion(IpHeader* _self, u_int8_t _version);
extern void IpHeader_setDifferentiatedServiceField(IpHeader* _self, u_int8_t _dsf);
extern void IpHeader_setTotalLength(IpHeader* _self, u_int16_t _total_length);
extern void IpHeader_setIdentfication(IpHeader* _self, u_int16_t _identification);
extern void IpHeader_setFlagOffField(IpHeader* _self, u_int16_t _flagoff);
extern void IpHeader_setTimeToLive(IpHeader* _self, u_int8_t _time_to_live);
extern void IpHeader_setProtocol(IpHeader* _self, u_int8_t _protocol);
extern void IpHeader_setHeaderChecksum(IpHeader* _self, u_int16_t _checksum);
extern void IpHeader_setSourceAddress(IpHeader* _self, u_int32_t _srcaddres);
extern void IpHeader_setDestinationAddress(IpHeader* _self, u_int32_t _dstaddress);

extern u_int8_t IpHeader_getVersion(IpHeader* _self);
extern u_int8_t IpHeader_getInternetHeaderLength(IpHeader* _self);
extern u_int8_t IpHeader_getDSCP(IpHeader* _self);
extern u_int8_t IpHeader_getECN(IpHeader* _self);
extern u_int8_t IpHeader_getFlags(IpHeader* _self);
extern u_int8_t IpHeader_getFragmentOffset(IpHeader* _self);
extern void IpHeader_encode(IpHeader* _self, ByteBuffer* _buffer);
extern ByteBuffer* IpHeader_encode_v2(IpHeader* _self);

extern void IpHeader_printInfo(IpHeader* _self);

extern u_int16_t computeFlagOff(int _x, int _d, int _m, int _offset);
extern u_int8_t  computeDifferentiatedServiceField(int _dscp, int _ecn);
extern char*     convertFlagToBin(u_int8_t _flags);
extern char*     addressNumberToString(u_int32_t _addr, bool _be);

/* Constructor, Deconstructor and Methods for IpPacket struct/class */
extern IpPacket* IpPacket_new();

/**
 * @param _ippckt The IpPacket object to delete
*/
extern void IpPacket_delete(IpPacket* _self);

extern void IpPacket_setHeader(IpPacket * _self, IpHeader * _iphdr);
extern void IpPacket_fillHeader(
    IpPacket* _self,     u_int8_t  _version, u_int8_t  _dsf, u_int16_t _tlen,
    u_int16_t _id,       u_int16_t _flagoff, u_int8_t  _ttl, u_int8_t  _protocol, 
    u_int16_t _checksum, char*     _srcaddr, char*     _dstaddr
);

extern void IpPacket_fillPayload(IpPacket * _self, char *_data, size_t _datasize);
extern u_int16_t IpPacket_getPayloadSize(IpPacket * _self);
extern ByteBuffer* IpPacket_encode(IpPacket* _self);

extern void IpPacket_wrapIcmp(IpPacket* _self, IcmpPacket* _icmppckt);

#endif