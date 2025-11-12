#pragma once
#include <chrono>

namespace common {
class Clock {
public:
    using clock = std::chrono::steady_clock;

    void reset() { start_ = clock::now(); }

    float elapsed() const {
        return std::chrono::duration_cast<std::chrono::duration<float>>(clock::now() - start_).count();
    }

private:
    clock::time_point start_{clock::now()};
};
} // namespace common
