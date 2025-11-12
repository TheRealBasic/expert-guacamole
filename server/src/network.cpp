#include "server/network_server.hpp"
#include "common/network/serialization.hpp"
#include <cstddef>
#include <enet/enet.h>
#include <iostream>

namespace server {

NetworkServer::NetworkServer() {
    enet_initialize();
}

NetworkServer::~NetworkServer() {
    stop();
    enet_deinitialize();
}

bool NetworkServer::start(std::uint16_t port, std::size_t maxClients) {
    ENetAddress address{};
    address.host = 0;
    address.port = port;
    host_ = enet_host_create(&address, maxClients, 2, 0, 0);
    if (!host_) {
        std::cerr << "Failed to create ENet host\n";
        return false;
    }
    peers_.resize(maxClients, nullptr);
    return true;
}

void NetworkServer::stop() {
    if (host_) {
        enet_host_destroy(host_);
        host_ = nullptr;
    }
    peers_.clear();
}

void NetworkServer::poll(std::function<void(const common::net::InputPacket&)> onInput,
                         std::function<void(std::uint32_t)> onConnect,
                         std::function<void(std::uint32_t)> onDisconnect) {
    if (!host_) {
        return;
    }
    ENetEvent event;
    while (enet_host_service(host_, &event, 0) > 0) {
        switch (event.type) {
        case ENET_EVENT_TYPE_CONNECT: {
            std::uint32_t clientId = nextClientId_++;
            if (onConnect) {
                onConnect(clientId);
            }
            break;
        }
        case ENET_EVENT_TYPE_RECEIVE: {
            common::net::PacketType type{};
            const void* payload = nullptr;
            std::size_t size = 0;
            if (common::net::Deserialize(static_cast<const std::uint8_t*>(event.packet.data), event.packet.dataLength, type, &payload, size) &&
                type == common::net::PacketType::Input) {
                if (size == sizeof(common::net::InputPacket) && onInput) {
                    onInput(*static_cast<const common::net::InputPacket*>(payload));
                }
            }
            break;
        }
        case ENET_EVENT_TYPE_DISCONNECT: {
            if (onDisconnect) {
                onDisconnect(0);
            }
            break;
        }
        default:
            break;
        }
    }
}

void NetworkServer::broadcast_state(const common::net::StatePacket& state) {
    if (!host_) {
        return;
    }
    auto buffer = common::net::Serialize(common::net::PacketType::State, &state, sizeof(state));
    ENetPacket* packet = enet_packet_create(buffer.data.data(), buffer.data.size(), 0);
    enet_host_broadcast(host_, 0, packet);
}

} // namespace server
