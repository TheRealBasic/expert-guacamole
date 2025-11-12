#pragma once
#include <cstdint>

namespace common::random {
class XorShift32 {
public:
    explicit XorShift32(std::uint32_t seed = 1u) : state_(seed ? seed : 1u) {}

    std::uint32_t next() {
        std::uint32_t x = state_;
        x ^= x << 13;
        x ^= x >> 17;
        x ^= x << 5;
        state_ = x;
        return x;
    }

    float next_float() {
        return static_cast<float>(next()) / static_cast<float>(UINT32_MAX);
    }

private:
    std::uint32_t state_;
};
} // namespace common::random
