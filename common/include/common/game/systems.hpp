#pragma once
#include "components.hpp"
#include "../ecs/registry.hpp"
#include "../network/protocol.hpp"
#include <functional>
#include <string>
#include <vector>

namespace common::game {

struct InputCommand {
    float moveX{0.f};
    float moveY{0.f};
    float yaw{0.f};
    float pitch{0.f};
    std::uint32_t actions{0};
    float deltaTime{0.f};
};

class MovementSystem {
public:
    void update(ecs::Registry& registry, float deltaTime);
};

class MonsterAISystem {
public:
    void update(ecs::Registry& registry, float deltaTime);
};

class NetPredictionSystem {
public:
    void apply_local_prediction(ecs::Registry& registry, const InputCommand& command);
};

class SnapshotInterpolationSystem {
public:
    void apply(ecs::Registry& registry, const std::vector<net::StatePacket>& snapshots, float alpha);
};

} // namespace common::game
