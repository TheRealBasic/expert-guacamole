#ifndef IMGUI_H
#define IMGUI_H
struct ImVec2 { float x, y; ImVec2(float _x = 0, float _y = 0) : x(_x), y(_y) {} };
namespace ImGui {
void CreateContext();
void DestroyContext();
void NewFrame();
void Render();
void Begin(const char* name, bool* p_open = nullptr);
void End();
bool CollapsingHeader(const char* label);
void Text(const char* fmt, ...);
bool SliderFloat(const char* label, float* v, float v_min, float v_max);
bool Checkbox(const char* label, bool* v);
}
#endif
