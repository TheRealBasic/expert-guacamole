#pragma once
#include "../math/vec3.hpp"
#include <cstdint>
#include <string>

namespace common::game {
struct Transform {
    math::Vec3 position{};
    float rotationY{0.f};
};

struct CharacterController {
    math::Vec3 velocity{};
    bool grounded{false};
    float stamina{100.f};
};

struct Flashlight {
    float battery{100.f};
    bool active{true};
};

struct MonsterTag {
    float screamCooldown{0.f};
    float blinkCooldown{0.f};
};

struct Door {
    bool open{false};
    float cooldown{0.f};
};

struct Item {
    enum class Type { Battery, Key } type{Type::Battery};
    bool picked{false};
};

struct AudioEmitter {
    float gain{1.f};
    math::Vec3 lastHeardAt{};
};

struct FogVolume {
    float density{0.02f};
};

struct NetIdentity {
    std::uint32_t id{0};
};

} // namespace common::game
