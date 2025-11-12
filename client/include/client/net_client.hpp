#pragma once
#include "common/network/protocol.hpp"
#include "common/network/serialization.hpp"
#include <deque>
#include <string>
#include <vector>

#include <enet/enet.h>

namespace client {

class NetClient {
public:
    NetClient();
    ~NetClient();

    bool connect(const std::string& host, std::uint16_t port, const std::string& name);
    void disconnect();
    void send_input(const common::net::InputPacket& input);
    std::vector<common::net::StatePacket> poll();

private:
    ENetHost* host_{nullptr};
    ENetPeer* peer_{nullptr};
    std::deque<common::net::StatePacket> stateBuffer_{};
};

} // namespace client
