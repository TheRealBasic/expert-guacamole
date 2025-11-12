#include "common/game/systems.hpp"
#include "common/config.hpp"
#include <algorithm>

namespace common::game {

void MovementSystem::update(ecs::Registry& registry, float deltaTime) {
    auto& transforms = registry.storage<Transform>();
    auto& controllers = registry.storage<CharacterController>();
    controllers.for_each([&](ecs::Entity entity, CharacterController& controller) {
        if (auto* transform = transforms.get(entity)) {
            transform->position += controller.velocity * deltaTime;
            controller.velocity.y += common::kWorldGravity * deltaTime;
        }
    });
}

void MonsterAISystem::update(ecs::Registry& registry, float deltaTime) {
    auto& monsters = registry.storage<MonsterTag>();
    monsters.for_each([&](ecs::Entity, MonsterTag& monster) {
        monster.screamCooldown = std::max(0.0f, monster.screamCooldown - deltaTime);
        monster.blinkCooldown = std::max(0.0f, monster.blinkCooldown - deltaTime);
    });
}

void NetPredictionSystem::apply_local_prediction(ecs::Registry& registry, const InputCommand& command) {
    auto& controllers = registry.storage<CharacterController>();
    controllers.for_each([&](ecs::Entity, CharacterController& controller) {
        controller.velocity.x = command.moveX;
        controller.velocity.z = command.moveY;
    });
}

void SnapshotInterpolationSystem::apply(ecs::Registry& registry, const std::vector<net::StatePacket>& snapshots, float alpha) {
    if (snapshots.size() < 2) {
        return;
    }
    const auto& a = snapshots[snapshots.size() - 2];
    const auto& b = snapshots[snapshots.size() - 1];
    auto& transforms = registry.storage<Transform>();
    for (std::size_t i = 0; i < b.entityCount && i < b.entities.size(); ++i) {
        const auto& stateA = a.entities[i];
        const auto& stateB = b.entities[i];
        ecs::Entity entity = stateB.entityId;
        auto& transform = transforms.emplace(entity);
        transform.position.x = stateA.position[0] * (1 - alpha) + stateB.position[0] * alpha;
        transform.position.y = stateA.position[1] * (1 - alpha) + stateB.position[1] * alpha;
        transform.position.z = stateA.position[2] * (1 - alpha) + stateB.position[2] * alpha;
        transform.rotationY = stateB.rotationY;
    }
}

} // namespace common::game
