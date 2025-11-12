#include "client/audio_engine.hpp"
#include <miniaudio.h>

namespace client {

AudioEngine::AudioEngine() = default;
AudioEngine::~AudioEngine() { shutdown(); }

bool AudioEngine::initialize() {
    engine_ = new ma_engine{};
    if (ma_engine_init(nullptr, engine_) != MA_SUCCESS) {
        delete engine_;
        engine_ = nullptr;
        return false;
    }
    ma_engine_start(engine_);
    return true;
}

void AudioEngine::shutdown() {
    if (!engine_) {
        return;
    }
    for (auto* sound : sounds_) {
        ma_sound_uninit(sound);
        delete sound;
    }
    sounds_.clear();
    ma_engine_stop(engine_);
    ma_engine_uninit(engine_);
    delete engine_;
    engine_ = nullptr;
}

void AudioEngine::update() {}

void AudioEngine::play_one_shot(const std::string& path) {
    if (!engine_) return;
    auto* sound = new ma_sound{};
    if (ma_sound_init_from_file(engine_, path.c_str(), 0, nullptr, nullptr, sound) == MA_SUCCESS) {
        ma_sound_start(sound);
        sounds_.push_back(sound);
    } else {
        delete sound;
    }
}

} // namespace client
