#pragma once
#include "common/game/systems.hpp"
#include <unordered_map>

struct GLFWwindow;

namespace client {

class InputSystem {
public:
    bool initialize(GLFWwindow* window);
    common::game::InputCommand poll(float deltaTime);
    void set_key_state(int key, bool pressed);

private:
    std::unordered_map<int, bool> keys_{};
};

} // namespace client
