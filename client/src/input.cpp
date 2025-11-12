#include "client/input_system.hpp"
#include <GLFW/glfw3.h>

namespace client {

namespace {
InputSystem* g_input = nullptr;

void KeyCallback(GLFWwindow*, int key, int, int action, int) {
    if (!g_input) return;
    if (action == GLFW_PRESS) {
        g_input->set_key_state(key, true);
    } else if (action == GLFW_RELEASE) {
        g_input->set_key_state(key, false);
    }
}
} // namespace

bool InputSystem::initialize(GLFWwindow* window) {
    g_input = this;
    glfwSetKeyCallback(window, KeyCallback);
    return true;
}

common::game::InputCommand InputSystem::poll(float deltaTime) {
    common::game::InputCommand command;
    command.deltaTime = deltaTime;
    command.moveX = keys_[GLFW_KEY_D] ? 1.f : keys_[GLFW_KEY_A] ? -1.f : 0.f;
    command.moveY = keys_[GLFW_KEY_W] ? 1.f : keys_[GLFW_KEY_S] ? -1.f : 0.f;
    return command;
}

void InputSystem::set_key_state(int key, bool pressed) {
    keys_[key] = pressed;
}

} // namespace client
