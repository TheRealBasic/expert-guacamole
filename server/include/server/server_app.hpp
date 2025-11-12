#pragma once
#include "common/clock.hpp"
#include "common/ecs/registry.hpp"
#include "common/game/systems.hpp"
#include "server/network_server.hpp"
#include <string>

namespace server {

struct ServerConfig {
    std::string host{"0.0.0.0"};
    std::uint16_t port{27015};
    std::size_t maxPlayers{8};
};

class ServerApp {
public:
    explicit ServerApp(const ServerConfig& config);
    bool start();
    void run();
    void stop();

private:
    void tick(float deltaTime);

    ServerConfig config_{};
    common::Clock clock_{};
    common::ecs::Registry registry_{};
    common::game::MovementSystem movementSystem_{};
    common::game::MonsterAISystem monsterSystem_{};
    NetworkServer network_{};
    bool running_{false};
};

} // namespace server
