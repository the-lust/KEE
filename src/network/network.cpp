#include "network.h"

#include <array>

using namespace PortableAPI;

namespace {
    constexpr Network::next_packet_size_t KEE_MAX_PACKET = 4 * 1024 * 1024;

    // Drains any pending data into the buffer. Returns false when the
    // connection was closed or reset by the peer.
    bool read_available(Network::tcp_buffer_t& tcp_buffer)
    {
        bool ok = true;
        while (tcp_buffer.socket.has_data(0))
        {
            size_t tail = tcp_buffer.buffer.size();
            tcp_buffer.buffer.resize(tail + 4096);
            int r = tcp_buffer.socket.recv(&tcp_buffer.buffer[tail], 4096);
            if (r <= 0)
            {
                tcp_buffer.buffer.resize(tail);
                ok = false;
                break;
            }
            tcp_buffer.buffer.resize(tail + static_cast<size_t>(r));
        }
        return ok;
    }
}

Network::Network() :
    m_tcp_self_recv{}
{
    m_tcp_self_recv.buffer.reserve(1024 * 10);
    Socket::InitSocket();
    m_network_task = std::thread(&Network::network_thread, this);
}

Network::~Network()
{
    APP_LOG(Log::LogLevel::DEBUG, "Shutting down Network");
    m_want_stop = true;
    m_udp_socket.close();
    m_tcp_socket.close();
    m_tcp_self_send.close();
    m_tcp_self_recv.socket.close();
    if (m_network_task.joinable())
        m_network_task.join();
    stop_network();
}

void Network::start_network()
{
    try
    {
        m_udp_socket.open();
    }
    catch (...)
    {
        return;
    }

    uint16_t port = 0;
    for (port = network_port; port < max_network_port; ++port)
    {
        try
        {
            m_udp_socket.bind(port);
            break;
        }
        catch (...)
        {
        }
    }
    if (port == max_network_port)
    {
        APP_LOG(Log::LogLevel::ERR, "Failed to start UDP socket");
        m_want_stop = true;
        return;
    }

    APP_LOG(Log::LogLevel::INFO, "UDP socket started on port: %hu", port);

    std::mt19937_64& gen = get_gen();
    std::uniform_int_distribution<int64_t> dis;

    int x;
    for (x = 0, port = static_cast<uint16_t>(dis(gen) % 30000 + 30000); x < 100; ++x, port = static_cast<uint16_t>(dis(gen) % 30000 + 30000))
    {
        try
        {
            m_tcp_socket.open();
            m_tcp_socket.bind(port);
            m_tcp_socket.listen(32);

            ipv4m_addr self = ipv4m_addr::loopback_addr();
            self.set_port(port);
            m_tcp_self_send.open();
            if (!m_tcp_self_send.connect(self))
                throw socket_exception("self connect failed");

            for (int i = 0; i < 50 && !m_tcp_socket.has_data(0); ++i)
                std::this_thread::sleep_for(std::chrono::milliseconds(5));

            m_tcp_self_recv.socket = std::move(m_tcp_socket.accept());
            if (!m_tcp_self_recv.socket.is_valid())
                throw socket_exception("self accept failed");
            break;
        }
        catch (...)
        {
            m_tcp_socket.close();
            m_tcp_self_send.close();
            m_tcp_self_recv.socket.close();
            APP_LOG(Log::LogLevel::WARN, "Failed to start tcp socket on port %hu", port);
        }
    }
    if (x == 100)
    {
        APP_LOG(Log::LogLevel::ERR, "Failed to start tcp socket");
        m_udp_socket.close();
        m_want_stop = true;
    }
    else
    {
        m_tcp_port = port;
        APP_LOG(Log::LogLevel::INFO, "TCP socket started after %hu tries on port: %hu", x, port);
    }
}

void Network::stop_network()
{
    m_advertise = false;
    m_udp_socket.close();
    m_tcp_socket.close();
    m_tcp_self_send.close();
    m_tcp_self_recv.socket.close();
    m_tcp_clients.clear();
    m_waiting_in_tcp_clients.clear();
    m_waiting_out_tcp_clients.clear();
    m_waiting_connect_tcp_clients.clear();
    m_network_msgs.clear();
    m_pending_network_msgs.clear();
    m_udp_addrs.clear();
    m_tcp_peers.clear();
}

inline Network::next_packet_size_t Network::make_next_packet_size(string const& buff) const
{
    return utils::Endian::net_swap(next_packet_size_t(buff.length() - sizeof(next_packet_size_t)));
}

void Network::build_advertise_msg(Network_Message_pb& msg)
{
    std::lock_guard<std::recursive_mutex> lk(local_mutex);

    Network_Advertise_pb* advertise = new Network_Advertise_pb;
    Network_Peer_pb* peer_pb = new Network_Peer_pb;

    APP_LOG(Log::LogLevel::DEBUG, "Advertising with peer ids: ");
    for (auto& id : m_my_peer_ids)
    {
        APP_LOG(Log::LogLevel::DEBUG, "%s", id.c_str());
        peer_pb->add_peer_ids(id);
    }

    advertise->set_allocated_peer(peer_pb);
    msg.set_allocated_network_advertise(advertise);
    msg.set_source_id(*m_my_peer_ids.begin());
}

std::pair<PortableAPI::tcp_socket*, std::vector<Network::peer_t>> Network::get_new_peer_ids(Network_Peer_pb const& peer_msg)
{
    std::lock_guard<std::recursive_mutex> lk(local_mutex);

    std::pair<tcp_socket*, std::vector<peer_t>> peer_ids_to_add;
    peer_ids_to_add.first = nullptr;
    peer_ids_to_add.second.reserve(peer_msg.peer_ids_size());

    for (auto& peer_id : peer_msg.peer_ids())
    {
        auto it = m_tcp_peers.find(peer_id);
        if (it == m_tcp_peers.end())
        {
            peer_ids_to_add.second.emplace_back(peer_id);
        }
        else if (peer_ids_to_add.first == nullptr)
        {
            peer_ids_to_add.first = it->second;
        }
    }
    return peer_ids_to_add;
}

void Network::do_advertise()
{
    std::lock_guard<std::recursive_mutex> lk(local_mutex);
    if (!m_advertise)
        return;

    auto now = clock::now();
    if ((now - m_last_advertise) < m_advertise_rate)
        return;

    m_last_advertise = now;

    try
    {
        if (!m_my_peer_ids.empty())
        {
            Network_Message_pb msg;
            Network_Advertise_pb* network = new Network_Advertise_pb;
            Network_Port_pb* port = new Network_Port_pb;

            port->set_port(m_tcp_port);
            network->set_allocated_port(port);
            msg.set_allocated_network_advertise(network);
            msg.set_source_id(*m_my_peer_ids.begin());

            SendBroadcast(msg);
        }
    }
    catch (...)
    {
    }
}

void Network::set_advertise_rate(std::chrono::milliseconds rate)
{
    std::lock_guard<std::recursive_mutex> lk(local_mutex);
    m_advertise_rate = rate;
}

std::chrono::milliseconds Network::get_advertise_rate()
{
    std::lock_guard<std::recursive_mutex> lk(local_mutex);
    return m_advertise_rate;
}

void Network::add_new_tcp_client(PortableAPI::tcp_socket* cli, std::vector<peer_t> const& peer_ids, bool advertise_peer)
{
    std::lock_guard<std::recursive_mutex> lk(local_mutex);

    Network_Message_pb msg;
    Network_Advertise_pb adv;
    Network_Peer_Connect_pb conn;

    adv.set_allocated_peer_connect(&conn);
    msg.set_allocated_network_advertise(&adv);

    for (auto& peerid : peer_ids)
    {
        APP_LOG(Log::LogLevel::DEBUG, "Adding peer id %s to client %s", peerid.c_str(), cli->get_addr().to_string(true).c_str());
        m_tcp_peers[peerid] = cli;

        msg.set_source_id(peerid);

        for (auto& channel : m_default_channels)
        {
            m_pending_network_msgs[channel.second].emplace_back(msg);
        }
    }

    adv.release_peer_connect();
    msg.release_network_advertise();

    if (advertise_peer)
    {
        APP_LOG(Log::LogLevel::DEBUG, "New peer: id %s %s", (*peer_ids.begin()).c_str(), cli->get_addr().to_string(true).c_str());

        Network_Message_pb msg;
        Network_Advertise_pb* adv = new Network_Advertise_pb;
        Network_Peer_Accept_pb* accept_peer = new Network_Peer_Accept_pb;

        adv->set_allocated_accept(accept_peer);
        msg.set_allocated_network_advertise(adv);

        string buff(sizeof(next_packet_size_t), 0);

        buff += std::move(msg.SerializeAsString());

        *reinterpret_cast<next_packet_size_t*>(&buff[0]) = make_next_packet_size(buff);

        cli->send(buff.data(), buff.length());
    }
}

void Network::remove_tcp_peer(tcp_buffer_t& tcp_buffer)
{
    std::lock_guard<std::recursive_mutex> lk(local_mutex);

    APP_LOG(Log::LogLevel::DEBUG, "TCP Client %s gone", tcp_buffer.socket.get_addr().to_string().c_str());

    Network_Message_pb msg;
    Network_Advertise_pb adv;
    Network_Peer_Disconnect_pb disc;

    adv.set_allocated_peer_disconnect(&disc);
    msg.set_allocated_network_advertise(&adv);

    for (auto it = m_tcp_peers.begin(); it != m_tcp_peers.end();)
    {
        if (it->second == &(tcp_buffer.socket))
        {
            msg.set_source_id(it->first);
            it = m_tcp_peers.erase(it);

            for (auto& channel : m_default_channels)
            {
                m_pending_network_msgs[channel.second].emplace_back(msg);
            }
        }
        else
            ++it;
    }

    adv.release_peer_disconnect();
    msg.release_network_advertise();
}

void Network::connect_to_peer(ipv4m_addr &addr, peer_t const& peer_id)
{
    std::lock_guard<std::recursive_mutex> lk(local_mutex);

    if (m_waiting_out_tcp_clients.count(peer_id) != 0)
        return;

    try
    {
        tcp_socket sock;
        sock.open();

        APP_LOG(Log::LogLevel::DEBUG, "Connecting to %s : %s", addr.to_string(true).c_str(), peer_id.c_str());

        if (!sock.connect(addr))
        {
            APP_LOG(Log::LogLevel::WARN, "Failed to TCP connect to %s: timeout", addr.to_string(true).c_str());
            return;
        }

        Network_Message_pb msg;
        build_advertise_msg(msg);

        string buff(sizeof(next_packet_size_t), 0);

        buff += std::move(msg.SerializeAsString());

        *reinterpret_cast<next_packet_size_t*>(&buff[0]) = make_next_packet_size(buff);

        if (sock.send(buff.data(), buff.length()) <= 0)
        {
            APP_LOG(Log::LogLevel::WARN, "Failed to TCP send to %s", addr.to_string(true).c_str());
            return;
        }

        APP_LOG(Log::LogLevel::DEBUG, "Connected to %s : %s", sock.get_addr().to_string(true).c_str(), peer_id.c_str());

        tcp_buffer_t tcp_buffer{};
        tcp_buffer.socket = std::move(sock);
        m_waiting_out_tcp_clients.emplace(peer_id, std::move(tcp_buffer));
    }
    catch (std::exception &e)
    {
        APP_LOG(Log::LogLevel::WARN, "Failed to TCP connect to %s: %s", addr.to_string(true).c_str(), e.what());
    }
}

void Network::process_waiting_out_clients()
{
    if (m_waiting_out_tcp_clients.empty())
        return;

    std::lock_guard<std::recursive_mutex> lk(local_mutex);

    Network_Message_pb msg;
    for (auto it = m_waiting_out_tcp_clients.begin(); it != m_waiting_out_tcp_clients.end(); )
    {
        try
        {
            if (!read_available(it->second))
            {
                it = m_waiting_out_tcp_clients.erase(it);
                continue;
            }

            if (it->second.next_packet_size == 0 && it->second.buffer.size() >= sizeof(next_packet_size_t))
            {
                it->second.next_packet_size = *reinterpret_cast<next_packet_size_t*>(&it->second.buffer[0]);
                it->second.next_packet_size = utils::Endian::net_swap(it->second.next_packet_size);
                it->second.buffer.erase(it->second.buffer.begin(), it->second.buffer.begin() + sizeof(next_packet_size_t));
            }

            if (it->second.next_packet_size > KEE_MAX_PACKET)
            {
                APP_LOG(Log::LogLevel::WARN, "Dropping oversized packet (%u bytes) from peer – disconnecting", it->second.next_packet_size);
                it->second.next_packet_size = 0;
                it->second.buffer.clear();
                it = m_waiting_out_tcp_clients.erase(it);
                continue;
            }

            if (it->second.next_packet_size > 0 && it->second.buffer.size() >= it->second.next_packet_size)
            {
                if (msg.ParseFromArray(it->second.buffer.data(), static_cast<int>(it->second.next_packet_size)) &&
                    msg.has_network_advertise() &&
                    msg.network_advertise().has_accept())
                {
                    it->second.next_packet_size = 0;
                    it->second.buffer.clear();

                    m_tcp_clients.emplace_back(std::move(it->second));
                    add_new_tcp_client(&(m_tcp_clients.rbegin()->socket), std::vector<peer_t>{it->first}, false);
                }
                it = m_waiting_out_tcp_clients.erase(it);
                continue;
            }

            ++it;
        }
        catch (std::exception &e)
        {
            APP_LOG(Log::LogLevel::WARN, "Failed peer pair: %s", e.what());
            it = m_waiting_out_tcp_clients.erase(it);
        }
    }
}

void Network::process_waiting_in_client()
{
    if (m_waiting_in_tcp_clients.empty())
        return;

    std::lock_guard<std::recursive_mutex> lk(local_mutex);

    Network_Message_pb msg;
    for (auto it = m_waiting_in_tcp_clients.begin(); it != m_waiting_in_tcp_clients.end(); )
    {
        try
        {
            if (!read_available(*it))
            {
                it = m_waiting_in_tcp_clients.erase(it);
                continue;
            }

            if (it->next_packet_size == 0 && it->buffer.size() >= sizeof(next_packet_size_t))
            {
                it->next_packet_size = *reinterpret_cast<next_packet_size_t*>(&it->buffer[0]);
                it->next_packet_size = utils::Endian::net_swap(it->next_packet_size);
                it->buffer.erase(it->buffer.begin(), it->buffer.begin() + sizeof(next_packet_size_t));
            }

            if (it->next_packet_size > KEE_MAX_PACKET)
            {
                APP_LOG(Log::LogLevel::WARN, "Dropping oversized inbound packet (%u bytes) – closing connection", it->next_packet_size);
                it->next_packet_size = 0;
                it->buffer.clear();
                it = m_waiting_in_tcp_clients.erase(it);
                continue;
            }

            if (it->next_packet_size > 0 && it->buffer.size() >= it->next_packet_size)
            {
                if (msg.ParseFromArray(it->buffer.data(), static_cast<int>(it->next_packet_size)) &&
                    msg.has_network_advertise() &&
                    msg.network_advertise().has_peer())
                {
                    it->next_packet_size = 0;
                    it->buffer.clear();

                    auto const& peer_msg = msg.network_advertise().peer();
                    std::pair<tcp_socket*, std::vector<peer_t>> peer_ids_to_add = std::move(get_new_peer_ids(peer_msg));

                    if (!peer_ids_to_add.second.empty())
                    {
                        if (peer_ids_to_add.first == nullptr)
                        {
                            m_tcp_clients.emplace_back(std::move(*it));
                            it = m_waiting_in_tcp_clients.erase(it);
                            peer_ids_to_add.first = &(m_tcp_clients.rbegin()->socket);
                        }
                        add_new_tcp_client(peer_ids_to_add.first, peer_ids_to_add.second, true);
                        continue;
                    }
                }
                it = m_waiting_in_tcp_clients.erase(it);
                continue;
            }

            ++it;
        }
        catch (std::exception &e)
        {
            APP_LOG(Log::LogLevel::WARN, "Failed peer pair: %s", e.what());
            it = m_waiting_in_tcp_clients.erase(it);
        }
    }
}

void Network::process_network_message(Network_Message_pb &msg)
{
    std::lock_guard<std::mutex> lk(message_mutex);

    if (msg.dest_id() == peer_t())
    {
        for (auto& channel : m_default_channels)
            m_pending_network_msgs[channel.second].emplace_back(msg);
    }
    else
    {
        assert(m_default_channels.find(msg.dest_id()) != m_default_channels.end());
        m_pending_network_msgs[m_default_channels[msg.dest_id()]].emplace_back(std::move(msg));
    }
}

void Network::process_udp()
{
    try
    {
        ipv4m_addr addr;
        std::array<uint8_t, 4096> buffer;
        Network_Message_pb msg;

        int r = m_udp_socket.recvfrom(buffer.data(), buffer.size(), addr);
        if (r > 0)
        {
            size_t len = static_cast<size_t>(r);
            const void* message = buffer.data();
            int message_size = static_cast<int>(len);

            if (msg.ParseFromArray(message, message_size))
            {
                if (msg.source_id() != peer_t())
                {
                    std::lock_guard<std::recursive_mutex> lk(local_mutex);
                    m_udp_addrs[msg.source_id()] = addr;

                    if (msg.has_network_advertise())
                    {
                        if (m_advertise)
                        {
                            auto const& advertise = msg.network_advertise();
                            if (advertise.has_port())
                            {
                                if (!m_my_peer_ids.empty() &&
                                    m_tcp_peers.count(msg.source_id()) == 0)
                                {
                                    ipv4m_addr peer_addr;
                                    peer_addr.set_ip(addr.get_ip());
                                    peer_addr.set_port(advertise.port().port());
                                    connect_to_peer(peer_addr, msg.source_id());
                                }
                            }
                            else if (advertise.has_peer())
                            {
                                std::pair<tcp_socket*, std::vector<peer_t>> peer_ids_to_add = std::move(get_new_peer_ids(advertise.peer()));

                                if (peer_ids_to_add.first != nullptr && !peer_ids_to_add.second.empty())
                                {
                                    add_new_tcp_client(peer_ids_to_add.first, peer_ids_to_add.second, false);
                                }
                            }
                        }
                    }
                    else
                    {
                        process_network_message(msg);
                    }
                }
                else
                {
                    APP_LOG(Log::LogLevel::DEBUG, "Dropping UDP data: peer_id is null");
                }
            }
            else
            {
                APP_LOG(Log::LogLevel::DEBUG, "Dropping UDP data: failed to parse protobuf");
            }
        }
    }
    catch (socket_exception & e)
    {
    }
}

void Network::process_tcp_listen()
{
    tcp_buffer_t tcp_buff{};
    tcp_buff.socket = std::move(m_tcp_socket.accept());
    if (tcp_buff.socket.is_valid())
    {
        m_waiting_in_tcp_clients.emplace_back(std::move(tcp_buff));
    }
    else
    {
        APP_LOG(Log::LogLevel::WARN, "TCP Listen accept failed");
    }
}

void Network::process_tcp_data(tcp_buffer_t& tcp_buffer)
{
    if (!read_available(tcp_buffer))
    {
        remove_tcp_peer(tcp_buffer);
        throw socket_exception("connection closed");
    }

    Network_Message_pb msg;
    while (tcp_buffer.buffer.size() > 0)
    {
        if (tcp_buffer.next_packet_size == 0 && tcp_buffer.buffer.size() >= sizeof(next_packet_size_t))
        {
            tcp_buffer.next_packet_size = *reinterpret_cast<next_packet_size_t*>(&tcp_buffer.buffer[0]);
            tcp_buffer.next_packet_size = utils::Endian::net_swap(tcp_buffer.next_packet_size);
            tcp_buffer.buffer.erase(tcp_buffer.buffer.begin(), tcp_buffer.buffer.begin() + sizeof(next_packet_size_t));
        }

        if (tcp_buffer.next_packet_size > 0 && tcp_buffer.buffer.size() >= tcp_buffer.next_packet_size)
        {
            if (tcp_buffer.next_packet_size > KEE_MAX_PACKET)
            {
                APP_LOG(Log::LogLevel::WARN, "Dropping oversized packet (%u bytes)", tcp_buffer.next_packet_size);
                tcp_buffer.buffer.clear();
                tcp_buffer.next_packet_size = 0;
                break;
            }

            if (msg.ParseFromArray(tcp_buffer.buffer.data(), static_cast<int>(tcp_buffer.next_packet_size)))
            {
                process_network_message(msg);
            }
            tcp_buffer.buffer.erase(tcp_buffer.buffer.begin(), tcp_buffer.buffer.begin() + tcp_buffer.next_packet_size);
            tcp_buffer.next_packet_size = 0;
        }
        else
        {
            break;
        }
    }
}

void Network::network_thread()
{
    start_network();

    while (!m_want_stop)
    {
        do_advertise();

        if (m_udp_socket.has_data(0))
            process_udp();

        if (m_tcp_socket.has_data(0))
            process_tcp_listen();

        if (m_tcp_self_recv.socket.has_data(0))
        {
            try
            {
                process_tcp_data(m_tcp_self_recv);
            }
            catch (...)
            {
            }
        }

        {
            std::lock_guard<std::recursive_mutex> lk(local_mutex);
            for (auto it = m_tcp_clients.begin(); it != m_tcp_clients.end();)
            {
                if (it->socket.has_data(0))
                {
                    try
                    {
                        process_tcp_data(*it);
                        ++it;
                    }
                    catch (std::exception & e)
                    {
                        APP_LOG(Log::LogLevel::DEBUG, "TCP client error: %s", e.what());
                        remove_tcp_peer(*it);
                        it = m_tcp_clients.erase(it);
                    }
                }
                else
                    ++it;
            }
        }

        process_waiting_in_client();

        process_waiting_out_clients();

        std::this_thread::sleep_for(std::chrono::milliseconds(10));
    }

    stop_network();
}

void Network::advertise_peer_id(peer_t const& peerid)
{
    std::lock_guard<std::recursive_mutex> lk(local_mutex);

    m_my_peer_ids.insert(peerid);
    m_tcp_peers[peerid] = &m_tcp_self_send;
}

void Network::remove_advertise_peer_id(peer_t const& peerid)
{
    std::lock_guard<std::recursive_mutex> lk(local_mutex);

    m_my_peer_ids.erase(peerid);
    m_tcp_peers.erase(peerid);
}

void Network::advertise(bool doit)
{
    std::lock_guard<std::recursive_mutex> lk(local_mutex);
    m_advertise = doit;
}

bool Network::is_advertising()
{
    std::lock_guard<std::recursive_mutex> lk(local_mutex);

    return m_advertise;
}

void Network::set_default_channel(peer_t peerid, channel_t default_channel)
{
    std::lock_guard<std::recursive_mutex> lk(local_mutex);

    m_default_channels[peerid] = default_channel;
}

void Network::register_listener(IRunNetwork* listener, channel_t channel, Network_Message_pb::MessagesCase type)
{
    std::lock_guard<std::recursive_mutex> lk(local_mutex);

    m_network_listeners[type][channel].push_back(listener);
}

void Network::unregister_listener(IRunNetwork* listener, channel_t channel, Network_Message_pb::MessagesCase type)
{
    std::lock_guard<std::recursive_mutex> lk(local_mutex);

    auto& listeners = m_network_listeners[type][channel];
    listeners.erase(
        std::remove(listeners.begin(), listeners.end(), listener),
        listeners.end());
}

bool Network::CBRunFrame(channel_t channel, Network_Message_pb::MessagesCase MessageFilter)
{
    bool rerun = false;
    auto& channel_messages = m_network_msgs[channel];
    {
        std::lock_guard<std::recursive_mutex> lk(local_mutex);
        for (auto it = channel_messages.begin(); it != channel_messages.end(); )
        {
            auto msg_case = it->messages_case();
            if (msg_case != Network_Message_pb::MessagesCase::MESSAGES_NOT_SET)
            {
                if (MessageFilter == Network_Message_pb::MessagesCase::MESSAGES_NOT_SET || MessageFilter == msg_case)
                {
                    auto& listeners = m_network_listeners[msg_case][channel];
                    for (auto& item : listeners)
                        item->RunNetwork(*it);

                    it = channel_messages.erase(it);

                    rerun = true;
                }
                else
                {
                    ++it;
                }
            }
            else
            {
                it = channel_messages.erase(it);
            }
        }
    }

    {
        std::lock_guard<std::mutex> lk(message_mutex);

        auto& pending_channel_messages = m_pending_network_msgs[channel];
        if (!pending_channel_messages.empty())
        {
            std::move(pending_channel_messages.begin(), pending_channel_messages.end(), std::back_inserter(channel_messages));
            pending_channel_messages.clear();
        }
    }

    return rerun;
}

bool Network::SendBroadcast(Network_Message_pb& msg)
{
    std::lock_guard<std::recursive_mutex> lk(local_mutex);

    std::vector<ipv4m_addr> broadcasts = get_broadcasts();

    assert((msg.source_id() != peer_t() && "Source id cannot be null"));
    assert((msg.dest_id() == peer_t() && "Destination id should be null"));

    msg.set_timestamp(std::chrono::duration_cast<std::chrono::milliseconds>(std::chrono::system_clock::now().time_since_epoch()).count());

    string buffer;
    msg.SerializeToString(&buffer);

    for (auto& brd : broadcasts)
    {
        for (uint16_t port = network_port; port < max_network_port; ++port)
        {
            brd.set_port(port);
            if (m_udp_socket.sendto(buffer.data(), buffer.length(), brd) <= 0)
            {
                return false;
            }
        }
    }

    return true;
}

std::set<Network::peer_t> Network::UDPSendToAllPeers(Network_Message_pb& msg)
{
    std::lock_guard<std::recursive_mutex> lk(local_mutex);

    assert((msg.source_id() != peer_t() && "Source id cannot be null"));

    std::set<peer_t> peers_sent_to;

    std::for_each(m_udp_addrs.begin(), m_udp_addrs.end(), [&](std::pair<peer_t const, PortableAPI::ipv4m_addr>& peer_infos)
    {
        msg.set_dest_id(peer_infos.first);
        msg.set_timestamp(std::chrono::duration_cast<std::chrono::milliseconds>(std::chrono::system_clock::now().time_since_epoch()).count());

        string buffer;
        msg.SerializeToString(&buffer);

        if (m_udp_socket.sendto(buffer.data(), buffer.length(), peer_infos.second) > 0)
        {
            peers_sent_to.insert(peer_infos.first);
        }
    });

    return peers_sent_to;
}

bool Network::UDPSendTo(Network_Message_pb& msg)
{
    std::lock_guard<std::recursive_mutex> lk(local_mutex);

    assert((msg.source_id() != peer_t() && "Source id cannot be null"));

    auto it = m_udp_addrs.find(msg.dest_id());
    if (it == m_udp_addrs.end())
    {
        return false;
    }

    msg.set_timestamp(std::chrono::duration_cast<std::chrono::milliseconds>(std::chrono::system_clock::now().time_since_epoch()).count());

    string buffer;
    msg.SerializeToString(&buffer);

    if (m_udp_socket.sendto(buffer.data(), buffer.length(), it->second) <= 0)
    {
        return false;
    }

    APP_LOG(Log::LogLevel::DEBUG, "Sent message to peer_id: %s, addr: %s", msg.dest_id().c_str(), it->second.to_string().c_str());

    return true;
}

std::set<Network::peer_t> Network::TCPSendToAllPeers(Network_Message_pb& msg)
{
    std::lock_guard<std::recursive_mutex> lk(local_mutex);

    std::set<peer_t> peers_sent_to;

    assert((msg.source_id() != peer_t() && "Source id cannot be null"));

    std::for_each(m_tcp_peers.begin(), m_tcp_peers.end(), [&](std::pair<peer_t const, tcp_socket*>& client)
    {
        msg.set_dest_id(client.first);
        msg.set_timestamp(std::chrono::duration_cast<std::chrono::milliseconds>(std::chrono::system_clock::now().time_since_epoch()).count());

        string buffer(sizeof(next_packet_size_t), 0);

        buffer += std::move(msg.SerializeAsString());

        *reinterpret_cast<next_packet_size_t*>(&buffer[0]) = make_next_packet_size(buffer);

        if (client.second->send(buffer.data(), buffer.length()) > 0)
        {
            peers_sent_to.insert(client.first);
        }
    });

    return peers_sent_to;
}

bool Network::TCPSendTo(Network_Message_pb& msg)
{
    std::lock_guard<std::recursive_mutex> lk(local_mutex);

    assert((msg.source_id() != peer_t() && "Source id cannot be null"));

    auto it = m_tcp_peers.find(msg.dest_id());
    if (it == m_tcp_peers.end())
    {
        return false;
    }

    msg.set_timestamp(std::chrono::duration_cast<std::chrono::milliseconds>(std::chrono::system_clock::now().time_since_epoch()).count());

    string buffer(sizeof(next_packet_size_t), 0);

    buffer += std::move(msg.SerializeAsString());

    *reinterpret_cast<next_packet_size_t*>(&buffer[0]) = make_next_packet_size(buffer);

    if (it->second->send(buffer.data(), buffer.length()) <= 0)
    {
        return false;
    }

    return true;
}