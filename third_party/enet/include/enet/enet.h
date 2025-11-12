#ifndef ENET_ENET_H
#define ENET_ENET_H
#include <stdint.h>
#include <stddef.h>
#ifdef __cplusplus
extern "C" {
#endif

typedef uint32_t enet_uint32;
typedef uint16_t enet_uint16;
typedef uint8_t enet_uint8;

typedef struct _ENetHost ENetHost;
typedef struct _ENetPeer ENetPeer;
typedef struct _ENetEvent {
    int type;
    ENetPeer* peer;
    enet_uint8 channelID;
    enet_uint32 data;
    struct {
        void* data;
        size_t dataLength;
    } packet;
} ENetEvent;

typedef struct _ENetAddress {
    enet_uint32 host;
    enet_uint16 port;
} ENetAddress;

typedef struct _ENetPacket {
    void* data;
    size_t dataLength;
} ENetPacket;

enum {
    ENET_EVENT_TYPE_NONE,
    ENET_EVENT_TYPE_CONNECT,
    ENET_EVENT_TYPE_DISCONNECT,
    ENET_EVENT_TYPE_RECEIVE
};

int enet_initialize(void);
void enet_deinitialize(void);
const char* enet_linked_version(void);
ENetHost* enet_host_create(const ENetAddress* address, size_t peerCount, size_t channelLimit, enet_uint32 inBandwidth, enet_uint32 outBandwidth);
void enet_host_destroy(ENetHost* host);
int enet_host_service(ENetHost* host, ENetEvent* event, enet_uint32 timeout);
void enet_host_broadcast(ENetHost* host, enet_uint8 channelID, ENetPacket* packet);
ENetPeer* enet_host_connect(ENetHost* host, const ENetAddress* address, size_t channelCount, enet_uint32 data);
int enet_peer_send(ENetPeer* peer, enet_uint8 channelID, ENetPacket* packet);
void enet_peer_disconnect(ENetPeer* peer, enet_uint32 data);
ENetPacket* enet_packet_create(const void* data, size_t dataLength, enet_uint32 flags);
void enet_packet_destroy(ENetPacket* packet);

#ifdef __cplusplus
}
#endif
#endif
