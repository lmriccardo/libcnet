/****************************************************************************************
 ****************************************************************************************
 *****                                                                              *****
 *****   @details Header file                                                       *****
 *****   @name Sender.hpp                                                           *****
 *****   @author Riccardo La Marca (riccardo.lamarca98@gmail.com)                   *****
 *****   @date 12 July 2024 (Mon Jul 12 2024 Last Modification)                     *****
 *****                                                                              *****
 *****   @brief This header file contains the declaration of the Sender object      *****
 *****   and a number of functions used to interact with it. The Sender class       *****
 *****   contains some useful information in order to be able to craft an ICMP,     *****
 *****   UDP or TCP IP Packets from scratch. The sender can also be syncrhonized    *****
 *****   with the Receiver, so that RTT and other things are easily computed.       *****
 *****                                                                              *****
 ****************************************************************************************
 ****************************************************************************************
*/

#ifndef _SENDER_H
#define _SENDER_H

#include <string.h>
#include <iostream>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <netdb.h>
#include <unistd.h>
#include <memory>
#include <mutex>
#include <condition_variable>

#include <Utils/ByteBuffer.hpp>
#include <Utils/Timer.hpp>
#include <Utils/NetUtil.hpp>
#include <Packets/IpPacket.hpp>

#define LOOKBACK "127.0.0.1"

namespace Communication
{
    /**
     * Struct containing some parameters not configurable when sending Packets
     * Notice that D flag and M flag are mutually exclusive.
     */
    struct IpParameters
    {
        int            _xf; //!< The X flag (not used so it must be always 0)
        int            _df; //!< Don't Fragment Flag
        int            _mf; //!< More Fragment Flag
        unsigned short _id; //!< The identification number to put into the IP Packet
    };

    /**
     * Struct containing some parameters, like identificaton and sequence number
     * useful when constructing ICMP Packets from the sender
     */
    struct IcmpParameters
    {
        unsigned short _id; //!< The Identification number to put into the ICMP Packet
        unsigned short _sn; //!< The Sequence number to put into the ICMP Packet
    };
    
    /**
     * @class Sender
     * 
     * This class is the one of the three main classes in this project, along
     * with IpPacket and Receiver. It has the job to craft IP packets and send them.
     * Moreover, it can also be synchronized with the Receiver.
     */
    class Sender
    {
        private:
            std::string                  _srcaddress; //!< Source Address
            struct sockaddr_in           _dstaddress; //!< The host receiving the packet
            char*                        _gateway;    //!< The gateway address of the current Source Address
            int                          _socketfd;   //!< File descriptor for the created socket
            int                          _msgcnt;     //!< Current Message Counter
            struct protoent*             _proto;      //!< The used protocol
            bool                         _verbose;    //!< Enable verbosity
            Utils::Timer_ptr             _timer;      //!< A timer synchronizing with the Receiver
            int                          _mtu;        //!< The maximum transmission unit
            std::mutex                   _mutex;      //!< A mutex to syncrhonize with the Receiver
            std::condition_variable      _cond;       //!< A condition variable to synchronize over a mutex
            std::unique_lock<std::mutex> _mlock;      //!< To lock to be used when synchronizing
            bool                         _synch;      //!< Set to true when it is synchronized with the Receiver
            bool                         _sent;       //!< Another variable to synchrnoize with the Receiver
            struct IpParameters          _ipp;        //!< Some additional parameters for IP Packets
            struct IcmpParameters        _icmpp;      //!< Some parameters used for ICMP Packets

        public:
            Sender(
                const std::string _interface, const std::string _dstaddr,
                char* _gateway, const unsigned short _dstport,
                const std::string _proto, const bool _verbose
            );

            ~Sender() 
            {
                shutdown(this->_socketfd, 2);
            };

            /**
             * @brief Set a synchronization timer into the Sender
             */
            void setTimer(Utils::Timer_ptr& _timer);

            /**
             * @brief Set the current Maximum Transmission Unit
             */
            void setMtu(const int _mtu);

            /**
             * @brief Set the flags to the Ip Parameter Structure
             */
            void setIpFlags(const int _d, const int _m);

            /**
             * @brief Sends the input IP Packet
             */
            void sendTo(Packets::IpPacket& _pckt);

            /**
             * @brief Sends the input buffer
             * 
             * @param _buffer The buffer containing the packet to be sent
             * @param _size   The total dimension of the input buffer
             */
            void sendTo(const char* _buffer, const std::size_t _size);
    };
};

#endif