#ifndef GLFW3_H
#define GLFW3_H
#ifdef __cplusplus
extern "C" {
#endif

typedef struct GLFWwindow GLFWwindow;

typedef void (*GLFWkeyfun)(GLFWwindow*, int, int, int, int);
typedef void (*GLFWcursorposfun)(GLFWwindow*, double, double);
typedef void (*GLFWframebuffersizefun)(GLFWwindow*, int, int);

enum {
    GLFW_PRESS = 1,
    GLFW_RELEASE = 0,
    GLFW_KEY_ESCAPE = 256,
    GLFW_KEY_W = 87,
    GLFW_KEY_A = 65,
    GLFW_KEY_S = 83,
    GLFW_KEY_D = 68,
    GLFW_KEY_SPACE = 32,
    GLFW_KEY_LEFT_SHIFT = 340,
    GLFW_MOUSE_BUTTON_LEFT = 0,
    GLFW_CONTEXT_VERSION_MAJOR = 0x00022002,
    GLFW_CONTEXT_VERSION_MINOR = 0x00022003,
    GLFW_OPENGL_PROFILE = 0x00022008,
    GLFW_OPENGL_CORE_PROFILE = 0x00032001,
    GLFW_OPENGL_FORWARD_COMPAT = 0x00022006
};

int glfwInit(void);
void glfwTerminate(void);
void glfwPollEvents(void);
void glfwSwapBuffers(GLFWwindow* window);
GLFWwindow* glfwCreateWindow(int width, int height, const char* title, void* monitor, void* share);
void glfwDestroyWindow(GLFWwindow* window);
void glfwMakeContextCurrent(GLFWwindow* window);
void glfwSetKeyCallback(GLFWwindow* window, GLFWkeyfun cbfun);
void glfwSetCursorPosCallback(GLFWwindow* window, GLFWcursorposfun cbfun);
void glfwSetFramebufferSizeCallback(GLFWwindow* window, GLFWframebuffersizefun cbfun);
void glfwSwapInterval(int interval);
int glfwWindowShouldClose(GLFWwindow* window);
void glfwSetWindowShouldClose(GLFWwindow* window, int value);
double glfwGetTime(void);
void glfwGetFramebufferSize(GLFWwindow* window, int* width, int* height);

#ifdef __cplusplus
}
#endif
#endif
