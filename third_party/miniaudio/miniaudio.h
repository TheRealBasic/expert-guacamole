#ifndef MINIAUDIO_H
#define MINIAUDIO_H
#ifdef __cplusplus
extern "C" {
#endif

typedef struct ma_engine { int dummy; } ma_engine;
typedef struct ma_sound { int dummy; } ma_sound;

typedef int ma_result;
#define MA_SUCCESS 0

ma_result ma_engine_init(void*, ma_engine*);
void ma_engine_uninit(ma_engine* engine);
ma_result ma_engine_start(ma_engine* engine);
ma_result ma_engine_stop(ma_engine* engine);
ma_result ma_sound_init_from_file(ma_engine* engine, const char* filename, unsigned int flags, void* pConfig, void* pCallbacks, ma_sound* pSound);
void ma_sound_uninit(ma_sound* sound);
ma_result ma_sound_start(ma_sound* sound);
ma_result ma_sound_stop(ma_sound* sound);

#ifdef __cplusplus
}
#endif
#endif
