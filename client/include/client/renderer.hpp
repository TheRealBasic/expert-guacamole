#pragma once
#include "common/ecs/registry.hpp"
#include <string>

struct GLFWwindow;

namespace client {

class Renderer {
public:
    Renderer();
    ~Renderer();

    bool initialize();
    void shutdown();
    bool should_close() const;
    void begin_frame();
    void draw_world(common::ecs::Registry& registry);
    void end_frame();
    GLFWwindow* window() const { return window_; }

private:
    bool init_window();
    bool init_gl();

    GLFWwindow* window_{nullptr};
    unsigned int shader_{0};
};

} // namespace client
