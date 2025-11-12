#include "glad/glad.h"

int gladLoadGL(void* (*)(const char*)) { return 1; }
void glClearColor(GLfloat, GLfloat, GLfloat, GLfloat) {}
void glClear(GLbitfield) {}
void glViewport(GLint, GLint, GLsizei, GLsizei) {}
