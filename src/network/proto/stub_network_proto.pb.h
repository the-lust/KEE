#pragma once
// Stub protobuf header for non-protobuf builds.
// Provides minimal type definitions so network.h compiles.

#include <cstdint>
#include <string>

// Minimal forward declarations for types used by network.h
class Network_Message_pb {
public:
    enum MessagesCase {
        MESSAGES_NOT_SET = 0,
    };
    MessagesCase messages_case() const { return MESSAGES_NOT_SET; }
};

class Network_Peer_Connect_pb {};
class Network_Peer_Disconnect_pb {};
