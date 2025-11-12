#pragma once
#include "protocol.hpp"
#include <cstddef>
#include <cstdint>
#include <vector>

namespace common::net {

struct ByteBuffer {
    std::vector<std::uint8_t> data;

    template <typename T>
    void write(const T& value) {
        const auto* bytes = reinterpret_cast<const std::uint8_t*>(&value);
        data.insert(data.end(), bytes, bytes + sizeof(T));
    }

    template <typename T>
    const T* read(std::size_t offset) const {
        if (offset + sizeof(T) > data.size()) {
            return nullptr;
        }
        return reinterpret_cast<const T*>(data.data() + offset);
    }
};

ByteBuffer Serialize(PacketType type, const void* payload, std::size_t payloadSize);
bool Deserialize(const std::uint8_t* data, std::size_t size, PacketType& outType, const void** payload, std::size_t& payloadSize);

} // namespace common::net
