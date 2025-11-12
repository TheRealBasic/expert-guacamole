#pragma once
#include <string>
#include <vector>

struct ma_engine;
struct ma_sound;

namespace client {

class AudioEngine {
public:
    AudioEngine();
    ~AudioEngine();

    bool initialize();
    void shutdown();
    void update();
    void play_one_shot(const std::string& path);

private:
    ma_engine* engine_{nullptr};
    std::vector<ma_sound*> sounds_{};
};

} // namespace client
