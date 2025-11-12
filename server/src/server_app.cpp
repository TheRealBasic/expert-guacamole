#include "server/server_app.hpp"
#include "common/config.hpp"
#include <iostream>

namespace server {

ServerApp::ServerApp(const ServerConfig& config) : config_(config) {}

bool ServerApp::start() {
    if (!network_.start(config_.port, config_.maxPlayers)) {
        return false;
    }
    running_ = true;
    clock_.reset();
    return true;
}

void ServerApp::run() {
    const float tickInterval = 1.0f / common::kServerTickRate;
    float accumulator = 0.0f;
    while (running_) {
        float dt = clock_.elapsed();
        clock_.reset();
        accumulator += dt;
        network_.poll(
            [&](const common::net::InputPacket& input) {
                // placeholder: apply to registry
                (void)input;
            },
            [&](std::uint32_t id) {
                std::cout << "Client connected: " << id << "\n";
            },
            [&](std::uint32_t id) {
                std::cout << "Client disconnected: " << id << "\n";
            });

        while (accumulator >= tickInterval) {
            tick(tickInterval);
            accumulator -= tickInterval;
        }
    }
}

void ServerApp::stop() {
    running_ = false;
    network_.stop();
}

void ServerApp::tick(float deltaTime) {
    movementSystem_.update(registry_, deltaTime);
    monsterSystem_.update(registry_, deltaTime);

    common::net::StatePacket state;
    state.tick++;
    network_.broadcast_state(state);
}

} // namespace server
