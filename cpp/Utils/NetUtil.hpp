/****************************************************************************************
 ****************************************************************************************
 *****                                                                              *****
 *****   @details Header file                                                       *****
 *****   @name NetUtil.hpp                                                          *****
 *****   @author Riccardo La Marca (riccardo.lamarca98@gmail.com)                   *****
 *****   @date 12 July 2024 (Mon Jul 12 2024 Last Modification)                     *****
 *****                                                                              *****
 *****   @brief This file contains the declaration of some utility functions.       ***** 
 *****                                                                              *****
 ****************************************************************************************
 ****************************************************************************************
*/

#ifndef _NETUTIL_H
#define _NETUTIL_H

#pragma once

#include <iostream>
#include <unistd.h>
#include <netdb.h>
#include <linux/if.h>
#include <arpa/inet.h>
#include <netinet/in.h>
#include <sys/socket.h>
#include <sys/ioctl.h>
#include <string.h>
#include <vector>

#include "ByteBuffer.hpp"

namespace Utils
{
    /**
     * @class NetUtil
     * 
     * This class contains only static methods used for some networking utilities.
     */
    class NetUtil
    {
        public:
            /**
             * @brief Returns the corresponding string given the input IP address in 32 bit number.
             * 
             * @param _addr The 32-bit address
             * @param _be   If the input address is in Big/Little-Endian
             * @return A string representing the IP address
             */
            static std::string addressNumberToString(unsigned int _addr, const bool _be);
            
            /**
             * @brief Given an hostname returns the corresponding IP address
             * 
             * @param _hostname The string representing the hostname
             * @return A string with the IP address
             * @throws std::runtime_error If the GetHostByName function has failed.
             */
            static std::string getHostnameIp(const std::string& _hostname);

            /**
             * @brief Get the IP address of the given interface
             * 
             * @param _interface A string with the interface (e.g., eth0, wlan0)
             * @return A string with the IP address
             */
            static std::string getInterfaceIp(const std::string& _interface);

            /**
             * @brief Get the corresponding MTU of a given input interface 
             * 
             * @param _interface The input interface
             * @return An integer representing the MTU
             * @throws std::runtime_error If getting the MTU results in an error
             */
            static int getInterfaceMtu(const std::string& _interface);
            
            /**
             * @brief Compute the checksum of an IP Packet as described in the corresponding RFC
             * 
             * @param _buffer The input byte buffer
             * @return The computed checksum
             */
            static unsigned short computeChecksum(ByteBuffer& _buffer);
    };
};

#endif