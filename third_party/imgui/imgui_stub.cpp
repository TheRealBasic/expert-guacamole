#include "imgui.h"
#include <cstdarg>
namespace ImGui {
void CreateContext() {}
void DestroyContext() {}
void NewFrame() {}
void Render() {}
void Begin(const char*, bool*) {}
void End() {}
bool CollapsingHeader(const char*) { return false; }
void Text(const char*, ...) {}
bool SliderFloat(const char*, float*, float, float) { return false; }
bool Checkbox(const char*, bool*) { return false; }
}
