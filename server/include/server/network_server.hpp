#pragma once
#include "common/network/protocol.hpp"
#include <functional>
#include <string>
#include <vector>

#include <enet/enet.h>

namespace server {

class NetworkServer {
public:
    NetworkServer();
    ~NetworkServer();

    bool start(std::uint16_t port, std::size_t maxClients);
    void stop();
    void poll(std::function<void(const common::net::InputPacket&)> onInput,
              std::function<void(std::uint32_t)> onConnect,
              std::function<void(std::uint32_t)> onDisconnect);
    void broadcast_state(const common::net::StatePacket& state);

private:
    ENetHost* host_{nullptr};
    std::vector<ENetPeer*> peers_{};
    std::uint32_t nextClientId_{1};
};

} // namespace server
