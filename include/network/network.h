#pragma once

#include "common_includes.h"
#include "proto/network_proto.pb.h"

class IRunNetwork
{
public:
    virtual bool RunNetwork(Network_Message_pb const& msg) = 0;
};

class LOCAL_API Network
{
public:
    using channel_t = int32_t;
    using peer_t = string;

    std::atomic_bool m_query_started;
    std::recursive_mutex local_mutex;
    std::chrono::milliseconds m_advertise_rate{500};

    Network();
    ~Network();

    void advertise_peer_id(peer_t const& peerid);
    void remove_advertise_peer_id(peer_t const& peerid);
    void advertise(bool doit);
    bool is_advertising();
    void set_advertise_rate(std::chrono::milliseconds rate);
    std::chrono::milliseconds get_advertise_rate();

    void set_default_channel(peer_t peerid, channel_t default_channel);
    void add_new_tcp_client(PortableAPI::tcp_socket* cli, std::vector<peer_t> const& peer_ids, bool advertise_peer);

    void register_listener  (IRunNetwork* listener, channel_t channel, Network_Message_pb::MessagesCase type);
    void unregister_listener(IRunNetwork* listener, channel_t channel, Network_Message_pb::MessagesCase type);

    bool CBRunFrame(channel_t channel, Network_Message_pb::MessagesCase MessageFilter = Network_Message_pb::MessagesCase::MESSAGES_NOT_SET);

    bool SendBroadcast(Network_Message_pb& msg);
    std::set<peer_t> UDPSendToAllPeers(Network_Message_pb& msg);
    bool UDPSendTo(Network_Message_pb& msg);
    std::set<peer_t> TCPSendToAllPeers(Network_Message_pb& msg);
    bool TCPSendTo(Network_Message_pb& msg);
    std::set<peer_t> UDPSendToAllPeers(Network_Message_pb& msg) const;
    bool TCPSendTo(Network_Message_pb& msg) const;
};
