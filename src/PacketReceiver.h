#ifndef PACKETRECEIVER_H
#define PACKETRECEIVER_H

#include <Arduino.h>
#include <string>

/**
 * @brief Minimal parser for packets arriving over Serial.
 *
 * Packets are expected to be in the form ``<name><US><value>\n`` where ``US``
 * is the ASCII Unit Separator (0x1F). The receiver accumulates incoming bytes
 * until a newline is encountered and then splits the packet into name/value
 * components.
 */
class PacketReceiver {
public:
    struct Packet {
        std::string name; ///< Identifier portion of the packet
        char value;       ///< Single-character value payload
    };

    /// @return Singleton instance of the receiver.
    static PacketReceiver& instance();

    /**
     * @brief Read the next packet from Serial into ``outPacket``.
     * @return true if a complete packet was parsed.
     */
    bool readPacket(Packet& outPacket);

private:
    PacketReceiver() = default;
    PacketReceiver(const PacketReceiver&) = delete;
    PacketReceiver& operator=(const PacketReceiver&) = delete;

    std::string buffer; ///< Accumulates incoming data until a packet is complete
};

#endif // PACKETRECEIVER_H
