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

#include "Base.hpp"

/* IP Header Flags */
#define IPv4 0x4
#define IPv6 0x6
#define IP_HEADER_SIZE 0x14

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

namespace Packets
{
    class IpHeader : public PacketHeader
    {

    };

    class IpPacket : public Packet
    {

    };
};

#endif