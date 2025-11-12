#include "miniaudio.h"

ma_result ma_engine_init(void*, ma_engine* engine) { if (engine) engine->dummy = 1; return MA_SUCCESS; }
void ma_engine_uninit(ma_engine*) {}
ma_result ma_engine_start(ma_engine*) { return MA_SUCCESS; }
ma_result ma_engine_stop(ma_engine*) { return MA_SUCCESS; }
ma_result ma_sound_init_from_file(ma_engine*, const char*, unsigned int, void*, void*, ma_sound* sound) { if (sound) sound->dummy = 1; return MA_SUCCESS; }
void ma_sound_uninit(ma_sound*) {}
ma_result ma_sound_start(ma_sound*) { return MA_SUCCESS; }
ma_result ma_sound_stop(ma_sound*) { return MA_SUCCESS; }
