#include "GLFW/glfw3.h"
#include <cstdlib>
#include <chrono>

struct GLFWwindow {
    int shouldClose{0};
};

int glfwInit(void) { return 1; }
void glfwTerminate(void) {}
void glfwPollEvents(void) {}
void glfwSwapBuffers(GLFWwindow*) {}
GLFWwindow* glfwCreateWindow(int, int, const char*, void*, void*) { return new GLFWwindow(); }
void glfwDestroyWindow(GLFWwindow* window) { delete window; }
void glfwMakeContextCurrent(GLFWwindow*) {}
void glfwSetKeyCallback(GLFWwindow*, GLFWkeyfun) {}
void glfwSetCursorPosCallback(GLFWwindow*, GLFWcursorposfun) {}
void glfwSetFramebufferSizeCallback(GLFWwindow*, GLFWframebuffersizefun) {}
void glfwSwapInterval(int) {}
int glfwWindowShouldClose(GLFWwindow* window) { return window->shouldClose; }
void glfwSetWindowShouldClose(GLFWwindow* window, int value) { window->shouldClose = value; }
double glfwGetTime(void) {
    using namespace std::chrono;
    static auto start = high_resolution_clock::now();
    auto now = high_resolution_clock::now();
    return duration_cast<duration<double>>(now - start).count();
}
void glfwGetFramebufferSize(GLFWwindow*, int* width, int* height) {
    if (width) *width = 1280;
    if (height) *height = 720;
}
