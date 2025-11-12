#pragma once
#include "client/renderer.hpp"
#include "client/audio_engine.hpp"
#include "client/input_system.hpp"
#include "client/net_client.hpp"
#include "common/clock.hpp"
#include "common/ecs/registry.hpp"
#include "common/game/systems.hpp"
#include <string>

namespace client {

struct ClientConfig {
    std::string name{"Player"};
    std::string host{"127.0.0.1"};
    std::uint16_t port{27015};
};

class ClientApp {
public:
    explicit ClientApp(const ClientConfig& config);
    ~ClientApp();

    bool start();
    void run();
    void stop();

private:
    void update(float deltaTime);
    void render();

    ClientConfig config_{};
    Renderer renderer_{};
    AudioEngine audio_{};
    InputSystem input_{};
    NetClient netClient_{};
    common::ecs::Registry registry_{};
    common::game::MovementSystem movementSystem_{};
    common::game::NetPredictionSystem predictionSystem_{};
    common::game::SnapshotInterpolationSystem interpolationSystem_{};
    common::Clock clock_{};
    bool running_{false};
};

} // namespace client
