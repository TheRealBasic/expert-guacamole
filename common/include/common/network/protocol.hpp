#pragma once
#include <cstdint>
#include <vector>
#include <array>

namespace common::net {

enum class PacketType : std::uint8_t {
    Hello = 0,
    Welcome,
    Input,
    State,
    Event,
    Ping,
    Pong
};

struct HelloPacket {
    std::uint32_t version{1};
    char name[32]{};
};

struct WelcomePacket {
    std::uint32_t clientId{0};
    std::uint32_t seed{0};
    std::uint32_t mapHash{0};
};

struct InputPacket {
    std::uint32_t clientId{0};
    std::uint32_t sequence{0};
    float deltaTime{0.f};
    float move[2]{};
    float lookYaw{0.f};
    float lookPitch{0.f};
    std::uint32_t actions{0};
};

struct EntityState {
    std::uint32_t entityId{0};
    std::uint8_t type{0};
    float position[3]{};
    float rotationY{0.f};
    float velocity[3]{};
    std::uint32_t flags{0};
};

struct StatePacket {
    std::uint32_t tick{0};
    std::uint32_t entityCount{0};
    std::array<EntityState, 32> entities{};
};

struct EventPacket {
    std::uint8_t kind{0};
    std::array<std::uint8_t, 64> payload{};
    std::uint32_t payloadSize{0};
};

struct PingPacket {
    std::uint64_t time{0};
};

using PongPacket = PingPacket;

} // namespace common::net
