#include "client/client_app.hpp"
#include <iostream>

namespace client {

ClientApp::ClientApp(const ClientConfig& config) : config_(config) {}

ClientApp::~ClientApp() {
    stop();
}

bool ClientApp::start() {
    if (!renderer_.initialize()) {
        return false;
    }
    input_.initialize(renderer_.window());
    if (!audio_.initialize()) {
        return false;
    }
    if (!netClient_.connect(config_.host, config_.port, config_.name)) {
        std::cerr << "Unable to connect to server\n";
    }
    running_ = true;
    clock_.reset();
    return true;
}

void ClientApp::run() {
    while (running_ && !renderer_.should_close()) {
        float dt = clock_.elapsed();
        clock_.reset();
        update(dt);
        render();
    }
}

void ClientApp::stop() {
    if (!running_) {
        return;
    }
    running_ = false;
    netClient_.disconnect();
    audio_.shutdown();
    renderer_.shutdown();
}

void ClientApp::update(float deltaTime) {
    auto command = input_.poll(deltaTime);
    predictionSystem_.apply_local_prediction(registry_, command);
    movementSystem_.update(registry_, deltaTime);
    auto snapshots = netClient_.poll();
    interpolationSystem_.apply(registry_, snapshots, 0.5f);
    netClient_.send_input({});
    audio_.update();
}

void ClientApp::render() {
    renderer_.begin_frame();
    renderer_.draw_world(registry_);
    renderer_.end_frame();
}

} // namespace client
