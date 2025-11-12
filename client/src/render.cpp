#include "client/renderer.hpp"
#include "common/game/components.hpp"
#include <GLFW/glfw3.h>
#include <glad/glad.h>

namespace client {

namespace {
const char* kVertexShader = R"(#version 410 core
layout(location = 0) in vec3 aPos;
uniform mat4 uViewProj;
void main() {
    gl_Position = uViewProj * vec4(aPos, 1.0);
})";

const char* kFragmentShader = R"(#version 410 core
out vec4 FragColor;
void main() {
    FragColor = vec4(0.1, 0.2, 0.3, 1.0);
})";

unsigned int CompileShader(unsigned int type, const char* source) {
    (void)type;
    (void)source;
    return 1;
}

unsigned int CreateProgram() {
    return 1;
}

} // namespace

Renderer::Renderer() = default;
Renderer::~Renderer() { shutdown(); }

bool Renderer::initialize() {
    if (!init_window()) {
        return false;
    }
    if (!init_gl()) {
        return false;
    }
    return true;
}

void Renderer::shutdown() {
    if (window_) {
        glfwDestroyWindow(window_);
        window_ = nullptr;
    }
    glfwTerminate();
}

bool Renderer::should_close() const {
    return window_ ? glfwWindowShouldClose(window_) != 0 : true;
}

void Renderer::begin_frame() {
    if (!window_) return;
    glClearColor(0.f, 0.f, 0.f, 1.f);
    glClear(GL_COLOR_BUFFER_BIT);
}

void Renderer::draw_world(common::ecs::Registry& registry) {
    (void)registry;
}

void Renderer::end_frame() {
    if (!window_) return;
    glfwSwapBuffers(window_);
    glfwPollEvents();
}

bool Renderer::init_window() {
    if (!glfwInit()) {
        return false;
    }
    window_ = glfwCreateWindow(1280, 720, "Shadow Lantern", nullptr, nullptr);
    if (!window_) {
        return false;
    }
    glfwMakeContextCurrent(window_);
    glfwSwapInterval(1);
    return true;
}

bool Renderer::init_gl() {
    if (!gladLoadGL(nullptr)) {
        return false;
    }
    shader_ = CreateProgram();
    return shader_ != 0;
}

} // namespace client
