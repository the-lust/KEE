#pragma once

#include "common_includes.h"
#include "proto/network_proto.pb.h"

#include <deque>

class IRunNetwork
{
public:
    virtual bool RunNetwork(Network_Message_pb const& msg) = 0;
};

class LOCAL_API Network
{
public:
    using channel_t          = int32_t;
    using peer_t             = string;
    using next_packet_size_t = uint32_t;

    struct tcp_buffer_t
    {
        PortableAPI::tcp_socket socket;
        string buffer;
        next_packet_size_t next_packet_size = 0;
    };

    // UDP discovery/broadcast port range
    static constexpr uint16_t network_port    = 40000;
    static constexpr uint16_t max_network_port = 40010;

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

private:
    void start_network();
    void stop_network();
    void network_thread();
    void do_advertise();
    void process_udp();
    void process_tcp_listen();
    void process_tcp_data(tcp_buffer_t& tcp_buffer);
    void process_network_message(Network_Message_pb& msg);
    void process_waiting_in_client();
    void process_waiting_out_clients();
    void connect_to_peer(PortableAPI::ipv4m_addr& addr, peer_t const& peer_id);
    void remove_tcp_peer(tcp_buffer_t& tcp_buffer);
    std::pair<PortableAPI::tcp_socket*, std::vector<peer_t>> get_new_peer_ids(Network_Peer_pb const& peer_msg);
    next_packet_size_t make_next_packet_size(string const& buff) const;
    void build_advertise_msg(Network_Message_pb& msg);

    std::atomic_bool m_want_stop{false};
    std::thread m_network_task;

    bool m_advertise = false;
    uint16_t m_tcp_port = 0;
    PortableAPI::udp_socket m_udp_socket;
    PortableAPI::tcp_socket m_tcp_socket;
    PortableAPI::tcp_socket m_tcp_self_send;
    tcp_buffer_t m_tcp_self_recv;

    std::list<tcp_buffer_t> m_tcp_clients;
    std::map<peer_t, PortableAPI::tcp_socket*> m_tcp_peers;
    std::map<peer_t, PortableAPI::ipv4m_addr> m_udp_addrs;
    std::set<peer_t> m_my_peer_ids;
    std::map<peer_t, channel_t> m_default_channels;

    std::mutex message_mutex;
    std::map<channel_t, std::deque<Network_Message_pb>> m_network_msgs;
    std::map<channel_t, std::deque<Network_Message_pb>> m_pending_network_msgs;

    std::map<Network_Message_pb::MessagesCase, std::map<channel_t, std::vector<IRunNetwork*>>> m_network_listeners;

    std::map<peer_t, tcp_buffer_t> m_waiting_out_tcp_clients;
    std::map<peer_t, PortableAPI::tcp_socket> m_waiting_connect_tcp_clients;
    std::vector<tcp_buffer_t> m_waiting_in_tcp_clients;

    std::chrono::steady_clock::time_point m_last_advertise;
};