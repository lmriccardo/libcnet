#ifndef _BASE_H
#define _BASE_H

#include <Utils/ByteBuffer.hpp>

namespace Ip
{
    class PacketHeader
    {
        public:
            virtual void printInfo() = 0;
            virtual void encode(Utils::ByteBuffer& _buffer) = 0;
            virtual Utils::ByteBuffer encode() = 0;
            virtual void decode(Utils::ByteBuffer& _buffer) = 0;
    };

    class Packet
    {
        private:
            unsigned char* _payload;

        public:
            virtual void fillHeader(PacketHeader& _hdr) = 0;
            virtual void fillPayload(unsigned char* _payload) = 0;
            virtual std::size_t getPacketSize() = 0;
            virtual std::size_t getPayloadSize() = 0;
            virtual void encode(Utils::ByteBuffer& _buffer) = 0;
            virtual Utils::ByteBuffer encode() = 0;
            virtual void decode(Utils::ByteBuffer& _buffer) = 0;
    };
};

#endif