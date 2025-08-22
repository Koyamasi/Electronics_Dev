#include "PacketReceiver.h"

PacketReceiver& PacketReceiver::instance() {
    static PacketReceiver inst;
    return inst;
}

bool PacketReceiver::readPacket(Packet& outPacket) {
    while (Serial.available()) {
        char c = static_cast<char>(Serial.read());
        if (c == '\n') {
            auto sep = buffer.find(char(0x1F));
            if (sep != std::string::npos && sep + 1 < buffer.size()) {
                outPacket.name = buffer.substr(0, sep);
                outPacket.value = buffer[sep + 1];
                buffer.clear();
                return true;
            }
            buffer.clear();
        } else if (c != '\r') {
            buffer.push_back(c);
        }
    }
    return false;
}
