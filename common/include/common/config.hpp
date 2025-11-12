#pragma once
#include <cstdint>

namespace common {
constexpr std::uint32_t kProtocolVersion = 1;
constexpr float kServerTickRate = 30.0f;
constexpr float kClientSendRate = 30.0f;
constexpr float kMaxPredictionLag = 0.2f; // seconds
constexpr std::size_t kMaxPlayers = 8;
constexpr float kWorldGravity = -9.8f;
constexpr float kStepHeight = 0.4f;
constexpr float kMaxStamina = 100.0f;
} // namespace common
