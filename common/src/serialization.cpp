#include "common/network/serialization.hpp"
#include <cstring>

namespace common::net {
namespace {
constexpr std::size_t kHeaderSize = sizeof(PacketType) + sizeof(std::uint16_t);
}

ByteBuffer Serialize(PacketType type, const void* payload, std::size_t payloadSize) {
    ByteBuffer buffer;
    const std::uint16_t size = static_cast<std::uint16_t>(payloadSize);
    buffer.write(type);
    buffer.write(size);
    if (payload && payloadSize > 0) {
        const auto* bytes = static_cast<const std::uint8_t*>(payload);
        buffer.data.insert(buffer.data.end(), bytes, bytes + payloadSize);
    }
    return buffer;
}

bool Deserialize(const std::uint8_t* data, std::size_t size, PacketType& outType, const void** payload, std::size_t& payloadSize) {
    if (!data || size < kHeaderSize) {
        return false;
    }
    outType = *reinterpret_cast<const PacketType*>(data);
    const auto payloadLen = *reinterpret_cast<const std::uint16_t*>(data + sizeof(PacketType));
    if (size < kHeaderSize + payloadLen) {
        return false;
    }
    *payload = data + kHeaderSize;
    payloadSize = payloadLen;
    return true;
}

} // namespace common::net
