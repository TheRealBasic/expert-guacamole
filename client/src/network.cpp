#include "client/net_client.hpp"
#include <iostream>

namespace client {

NetClient::NetClient() {
    enet_initialize();
}

NetClient::~NetClient() {
    disconnect();
    enet_deinitialize();
}

bool NetClient::connect(const std::string& host, std::uint16_t port, const std::string& name) {
    (void)host;
    (void)name;
    host_ = enet_host_create(nullptr, 1, 2, 0, 0);
    if (!host_) {
        return false;
    }
    ENetAddress address{};
    address.host = 0;
    address.port = port;
    peer_ = enet_host_connect(host_, &address, 2, 0);
    return peer_ != nullptr;
}

void NetClient::disconnect() {
    if (peer_) {
        enet_peer_disconnect(peer_, 0);
        peer_ = nullptr;
    }
    if (host_) {
        enet_host_destroy(host_);
        host_ = nullptr;
    }
    stateBuffer_.clear();
}

void NetClient::send_input(const common::net::InputPacket& input) {
    if (!host_ || !peer_) {
        return;
    }
    auto buffer = common::net::Serialize(common::net::PacketType::Input, &input, sizeof(input));
    ENetPacket* packet = enet_packet_create(buffer.data.data(), buffer.data.size(), 0);
    enet_peer_send(peer_, 0, packet);
}

std::vector<common::net::StatePacket> NetClient::poll() {
    std::vector<common::net::StatePacket> snapshots;
    if (!host_) {
        return snapshots;
    }
    ENetEvent event;
    while (enet_host_service(host_, &event, 0) > 0) {
        if (event.type == ENET_EVENT_TYPE_RECEIVE) {
            common::net::PacketType type{};
            const void* payload = nullptr;
            std::size_t size = 0;
            if (common::net::Deserialize(static_cast<const std::uint8_t*>(event.packet.data), event.packet.dataLength, type, &payload, size) &&
                type == common::net::PacketType::State && size == sizeof(common::net::StatePacket)) {
                snapshots.push_back(*static_cast<const common::net::StatePacket*>(payload));
            }
        }
    }
    return snapshots;
}

} // namespace client
