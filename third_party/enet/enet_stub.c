#include "enet/enet.h"
#include <stdlib.h>

struct _ENetHost { int dummy; };
struct _ENetPeer { int dummy; };

int enet_initialize(void) { return 0; }
void enet_deinitialize(void) {}
const char* enet_linked_version(void) { return "stub"; }
ENetHost* enet_host_create(const ENetAddress*, size_t, size_t, enet_uint32, enet_uint32) { return (ENetHost*)malloc(sizeof(ENetHost)); }
void enet_host_destroy(ENetHost* host) { free(host); }
int enet_host_service(ENetHost*, ENetEvent* event, enet_uint32) {
    if (event) event->type = ENET_EVENT_TYPE_NONE;
    return 0;
}
void enet_host_broadcast(ENetHost*, enet_uint8, ENetPacket* packet) {
    enet_packet_destroy(packet);
}
ENetPeer* enet_host_connect(ENetHost*, const ENetAddress*, size_t, enet_uint32) { return (ENetPeer*)malloc(sizeof(ENetPeer)); }
int enet_peer_send(ENetPeer*, enet_uint8, ENetPacket*) { return 0; }
void enet_peer_disconnect(ENetPeer* peer, enet_uint32) { free(peer); }
ENetPacket* enet_packet_create(const void* data, size_t dataLength, enet_uint32) {
    ENetPacket* packet = (ENetPacket*)malloc(sizeof(ENetPacket));
    if (!packet) return NULL;
    packet->dataLength = dataLength;
    packet->data = malloc(dataLength);
    if (packet->data && data) {
        for (size_t i = 0; i < dataLength; ++i) {
            ((unsigned char*)packet->data)[i] = ((const unsigned char*)data)[i];
        }
    }
    return packet;
}
void enet_packet_destroy(ENetPacket* packet) {
    if (!packet) return;
    free(packet->data);
    free(packet);
}
