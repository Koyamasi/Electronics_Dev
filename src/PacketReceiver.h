#ifndef PACKETRECEIVER_H
#define PACKETRECEIVER_H

#include <Arduino.h>
#include <string>

class PacketReceiver {
public:
    struct Packet {
        std::string name;
        char value;
    };

    static PacketReceiver& instance();

    // Read the next packet from Serial into outPacket.
    // Returns true if a complete packet was read.
    bool readPacket(Packet& outPacket);

private:
    PacketReceiver() = default;
    PacketReceiver(const PacketReceiver&) = delete;
    PacketReceiver& operator=(const PacketReceiver&) = delete;

    std::string buffer; // accumulates incoming data until a packet is complete
};

#endif // PACKETRECEIVER_H
