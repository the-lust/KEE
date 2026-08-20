#include "network.h"

using namespace PortableAPI;

Network::Network():
    m_advertise(false),
    m_advertise_rate(2000),
    m_tcp_port(0)
{
    
#if defined(NETWORK_COMPRESS)
    max_message_size = 0;
    max_compressed_message_size = 0;
    m_zstd_ccontext = ZSTD_createCCtx();
    m_zstd_dstream = ZSTD_createDStream();
#endif

    m_network_task.run(&Network::network_thread, this);
}

Network::~Network()
{
#if defined(NETWORK_COMPRESS)
    APP_LOG(Log::LogLevel::DEBUG, "Shutting down Network, biggest message size was %llu, biggest compressed message size was %llu", max_message_size, max_compressed_message_size);
#else
    APP_LOG(Log::LogLevel::DEBUG, "Shutting down Network");
#endif

    m_network_task.stop();
    m_network_task.join();

#if defined(NETWORK_COMPRESS)
    ZSTD_freeCCtx(m_zstd_ccontext);
    ZSTD_freeDStream(m_zstd_dstream);
#endif

}

#if defined(NETWORK_COMPRESS)

string Network::compress(void const* data, size_t len)
{
    string res(ZSTD_compressBound(len), '\0');
    res.resize(ZSTD_compressCCtx(m_zstd_ccontext, &res[0], res.length(), data, len, ZSTD_CLEVEL_DEFAULT));
    return res;
}

string Network::decompress(void const* data, size_t len)
{
    static size_t decompress_block_size = ZSTD_DStreamOutSize();
    static string res;

    res.resize(decompress_block_size);
    ZSTD_inBuffer inbuff{ data, len, 0 };
    ZSTD_outBuffer outbuff{ const_cast<char*>(res.data()), res.length(), 0 };

    while (inbuff.pos < inbuff.size)
    {
        size_t x = 0;
        x = ZSTD_decompressStream(m_zstd_dstream, &outbuff, &inbuff);
        if (ZSTD_isError(x))
        {
            if (x == size_t(-70))
            {
                res.resize(res.length() + decompress_block_size);
                outbuff.size = res.length();
                outbuff.dst = const_cast<char*>(res.data());
            }
            else
            {
                auto str_error = ZSTD_getErrorName(x);
                APP_LOG(Log::LogLevel::WARN, "Decompression error: %s", str_error);
                return string((char*)data, ((char*)data) + len);
            }
        }
    }

    ZSTD_initDStream(m_zstd_dstream);
    res.resize(outbuff.pos);
    return res;
}

#endif

void Network::start_network()
{
    ipv4m_addr addr;
    uint16_t port;
    addr.set_addr(ipv4m_addr::any_addr);

    for (port = network_port; port < max_network_port; ++port)
    {
        addr.set_port(port);
        try
        {
            m_udp_socket.bind(addr);
            break;
        }
        catch (...)
        {
        }
    }
    if (port == max_network_port)
    {
        
        m_network_task.stop();
    }
    else
    {
        APP_LOG(Log::LogLevel::INFO, "UDP socket started on port: %hu", port);
        std::uniform_int_distribution<int64_t> dis;
        std::mt19937m_64& gen = get_gen();
        int x;
        for (x = 0, port = (dis(gen) % 30000 + 30000); x < 100; ++x, port = (dis(gen) % 30000 + 30000))
        {
            addr.set_port(port);
            try
            {
                m_tcp_socket.bind(addr);
                m_tcp_socket.listen(32);
                addr.set_addr(ipv4m_addr::loopback_addr);
                m_tcp_self_send.connect(addr);
                m_tcp_self_recv.socket = std::move(m_tcp_socket.accept());
                m_tcp_self_recv.buffer.reserve(1024 * 10);
                break;
            }
            catch (...)
            {
                APP_LOG(Log::LogLevel::WARN, "Failed to start tcp socket on port %hu", x);
            }
        }
        if (x == 100)
        {
            APP_LOG(Log::LogLevel::ERR, "Failed to start tcp socket");
            m_udp_socket.close();
            m_network_task.stop();
        }
        else
        {
            m_tcp_port = port;
            APP_LOG(Log::LogLevel::INFO, "TCP socket started after %hu tries on port: %hu", x, port);
        }
    }
}

void Network::stop_network()
{
    m_advertise = false;
    m_udp_socket.close();
    m_tcp_socket.close();
    m_tcp_clients.clear();
    m_network_msgs.clear();
    m_udp_addrs.clear();
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

    m_poll.add_socket(*cli); 
    m_poll.set_events(*cli, Socket::poll_flags::in);

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
    
    if(advertise_peer)
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
    m_poll.remove_socket(tcp_buffer.socket);

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
    if (m_waiting_out_tcp_clients.count(peer_id) != 0)
        return;

    bool connected = false;
    auto it = m_waiting_connect_tcp_clients.find(peer_id);
    bool wait = false;
    try
    {
        if (it == m_waiting_connect_tcp_clients.end())
        {
            APP_LOG(Log::LogLevel::DEBUG, "Connecting to %s : %s", addr.to_string(true).c_str(), peer_id.c_str());
            
            m_waiting_connect_tcp_clients.emplace(peer_id, tcp_socket());
            it = m_waiting_connect_tcp_clients.find(peer_id);
            it->second.set_nonblocking(true);
        }
        it->second.connect(addr);
        connected = true;
    }
    catch (is_connected &e)
    {
        connected = true;
    }
    catch (would_block &e)
    {
        wait = true;
    }
    catch(in_progress &e)
    {
        wait = true;
    }
    catch (std::exception &e)
    {
        m_waiting_connect_tcp_clients.erase(it);
        APP_LOG(Log::LogLevel::WARN, "Failed to TCP connect to %s: %s", addr.to_string().c_str(), e.what());
    }

    if (wait)
    {
        fd_set set;
        FD_ZERO(&set);
        FD_SET(it->second.native(), &set);
        timeval tv;
        tv.tv_sec = 3;
        tv.tv_usec = 0;
        int res = ::select(0, nullptr, &set, nullptr, &tv);
        if (res > 0)
        {
            connected = true;
        }
        else
        {
            m_waiting_connect_tcp_clients.erase(it);
            APP_LOG(Log::LogLevel::WARN, "TCP connect to %s timed out", addr.to_string().c_str());
        }
    }

    if (connected)
    {
        Network_Message_pb msg;
        build_advertise_msg(msg);

        string buff(sizeof(next_packet_size_t), 0);
        
    #if defined(NETWORK_COMPRESS)
        string data;
        msg.SerializeToString(&data);
        buff += std::move(compress(data.data(), data.length()));

        max_message_size = std::max<uint64_t>(max_message_size, data.length());
        max_compressed_message_size = std::max<uint64_t>(max_compressed_message_size, buff.length());
    #else
        buff += std::move(msg.SerializeAsString());
    #endif
        *reinterpret_cast<next_packet_size_t*>(&buff[0]) = make_next_packet_size(buff);

        it->second.send(buff.data(), buff.length());

        APP_LOG(Log::LogLevel::DEBUG, "Connected to %s : %s", it->second.get_addr().to_string(true).c_str(), peer_id.c_str());

        tcp_buffer_t tcp_buffer{};
        tcp_buffer.socket = std::move(it->second);
        m_waiting_out_tcp_clients.emplace(peer_id, std::move(tcp_buffer));
        m_waiting_connect_tcp_clients.erase(it);
    }
}

void Network::process_waiting_out_clients()
{
    if (m_waiting_out_tcp_clients.empty())
        return;

    Network_Message_pb msg;
    for (auto it = m_waiting_out_tcp_clients.begin(); it != m_waiting_out_tcp_clients.end(); )
    {
        try
        {
            unsigned long count = 0;
            it->second.socket.ioctlsocket(Socket::cmd_name::fionread, &count);
            if (count > 0)
            {
                if (it->second.next_packet_size == 0 && count > sizeof(next_packet_size_t))
                {
                    it->second.socket.recv(&it->second.next_packet_size, sizeof(next_packet_size_t));
                    it->second.next_packet_size = utils::Endian::net_swap(it->second.next_packet_size);
                    count -= sizeof(next_packet_size_t);
                }
                
                static constexpr next_packet_size_t KEE_MAX_PACKET = 4 * 1024 * 1024; 
                if (it->second.next_packet_size > KEE_MAX_PACKET)
                {
                    APP_LOG(Log::LogLevel::WARN, "Dropping oversized packet (%u bytes) from peer – disconnecting", it->second.next_packet_size);
                    it->second.next_packet_size = 0;
                    it->second.buffer.clear();
                    it = m_waiting_out_tcp_clients.erase(it);
                    continue;
                }
                if (it->second.next_packet_size > 0 && count >= it->second.next_packet_size)
                {
                    it->second.buffer.resize(it->second.next_packet_size);
                    it->second.socket.recv(it->second.buffer.data(), it->second.next_packet_size);

                    const void* message;
                    int message_size;

                    message = it->second.buffer.data();
                    message_size = it->second.buffer.size();

                    if (msg.ParseFromArray(message, message_size) &&
                        msg.has_network_advertise() && 
                        msg.network_advertise().has_accept())
                    {
                        std::lock_guard<std::recursive_mutex> lk(local_mutex);

                        it->second.next_packet_size = 0;
                        it->second.buffer.clear();
                        it->second.socket.set_nonblocking(false);

                        m_tcp_clients.emplace_back(std::move(it->second));
                        add_new_tcp_client(&(m_tcp_clients.rbegin()->socket), std::vector<peer_t>{it->first}, false);
                    }
                    it = m_waiting_out_tcp_clients.erase(it);
                    continue;
                }
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
    Network_Message_pb msg;
    for (auto it = m_waiting_in_tcp_clients.begin(); it != m_waiting_in_tcp_clients.end(); )
    {
        try
        {
            unsigned long count = 0;
            it->socket.ioctlsocket(Socket::cmd_name::fionread, &count);
            if (count > 0)
            {
                if (it->next_packet_size == 0 && count > sizeof(next_packet_size_t))
                {
                    it->socket.recv(&it->next_packet_size, sizeof(next_packet_size_t));
                    it->next_packet_size = utils::Endian::net_swap(it->next_packet_size);
                    count -= sizeof(next_packet_size_t);
                }
                
                static constexpr next_packet_size_t KEE_MAX_PACKET_IN = 4 * 1024 * 1024;
                if (it->next_packet_size > KEE_MAX_PACKET_IN)
                {
                    APP_LOG(Log::LogLevel::WARN, "Dropping oversized inbound packet (%u bytes) – closing connection", it->next_packet_size);
                    it->next_packet_size = 0;
                    it->buffer.clear();
                    it = m_tcp_clients.erase(it);
                    continue;
                }
                if (it->next_packet_size > 0 && count >= it->next_packet_size)
                {
                    it->buffer.resize(it->next_packet_size);
                    it->socket.recv(it->buffer.data(), it->next_packet_size);

                    const void* message;
                    int message_size;

                #if defined(NETWORK_COMPRESS)
                    string buff = std::move(decompress(it->buffer.data(), it->next_packet_size));
                    message = buff.data();
                    message_size = buff.length();
                #else
                    message = it->buffer.data();
                    message_size = it->buffer.size();
                #endif
                    
                    if (msg.ParseFromArray(message, message_size) &&
                        msg.has_network_advertise() && 
                        msg.network_advertise().has_peer())
                    {
                        std::lock_guard<std::recursive_mutex> lk(local_mutex);

                        it->next_packet_size = 0;
                        it->buffer.clear();
                        it->socket.set_nonblocking(false);

                        auto const& peer_msg = msg.network_advertise().peer();
                        std::pair<tcp_socket*, std::vector<peer_t>> peer_ids_to_add = std::move(get_new_peer_ids(peer_msg));

                        if (!peer_ids_to_add.second.empty())
                        {
                            if (peer_ids_to_add.first == nullptr)
                            {
                                m_tcp_clients.emplace_back(std::move(*it));
                                peer_ids_to_add.first = &(m_tcp_clients.rbegin()->socket);
                            }
                            add_new_tcp_client(peer_ids_to_add.first, peer_ids_to_add.second, true);
                        }
                    }
                    it = m_waiting_in_tcp_clients.erase(it);
                    continue;
                }
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

    std::chrono::system_clock::time_point msg_time(std::chrono::milliseconds(msg.timestamp()));

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
        size_t len;
        
        len = m_udp_socket.recvfrom(addr, buffer.data(), buffer.size());
        if (len > 0)
        {
            const void* message;
            int message_size;

            #if defined(NETWORK_COMPRESS)
                string buff(std::move(decompress(buffer.data(), len)));
                message = buff.data();
                message_size = buff.length();
            #else
                message = buffer.data();
                message_size = (int)len;
            #endif

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
                APP_LOG(Log::LogLevel::DEBUG, "Dropping UDP data: failed to pase protobuf");
            }
        }
    }
    catch (socket_exception & e)
    {
        
    }
}

void Network::process_tcp_listen()
{
    try
    {
        tcp_buffer_t tcp_buff({});
        tcp_buff.socket = std::move(m_tcp_socket.accept());
        tcp_buff.socket.set_nonblocking(true);
        m_waiting_in_tcp_clients.emplace_back(std::move(tcp_buff));
    }
    catch (socket_exception & e)
    {
        APP_LOG(Log::LogLevel::WARN, "TCP Listen exception: %s", e.what());
    }
}

void Network::process_tcp_data(tcp_buffer_t& tcp_buffer)
{
    
    Network_Message_pb msg;
    size_t len;

    unsigned long count = 0;
    tcp_buffer.socket.ioctlsocket(Socket::cmd_name::fionread, &count);
    if (count > 0)
    {
        size_t buff_len = tcp_buffer.buffer.size();
        tcp_buffer.buffer.resize(buff_len + count); 

        len = tcp_buffer.socket.recv(tcp_buffer.buffer.data() + buff_len, count);

        while(tcp_buffer.buffer.size() > 0)
        {
            if (tcp_buffer.next_packet_size == 0 && tcp_buffer.buffer.size() >= sizeof(next_packet_size_t))
            {
                tcp_buffer.next_packet_size = *reinterpret_cast<next_packet_size_t*>(&tcp_buffer.buffer[0]);
                tcp_buffer.next_packet_size = utils::Endian::net_swap(tcp_buffer.next_packet_size);
                tcp_buffer.buffer.erase(tcp_buffer.buffer.begin(), tcp_buffer.buffer.begin() + sizeof(tcp_buffer.next_packet_size));
            }

            if (tcp_buffer.next_packet_size > 0 && tcp_buffer.buffer.size() >= tcp_buffer.next_packet_size)
            {
                const void* message;
                int message_size;
            #if defined(NETWORK_COMPRESS)
                string buff = std::move(decompress(tcp_buffer.buffer.data(), tcp_buffer.next_packet_size));
                message = buff.data();
                message_size = buff.length();
            #else
                message = tcp_buffer.buffer.data();
                message_size = (int)tcp_buffer.next_packet_size;
            #endif

                if (msg.ParseFromArray(message, message_size))
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
}

void Network::network_thread()
{
    int broadcast = 1;

    start_network();

    m_udp_socket.setsockopt(Socket::level::sol_socket, Socket::option_name::so_broadcast, &broadcast, sizeof(broadcast));

    if (!m_network_task.want_stop())
    {
        m_poll.add_socket(m_udp_socket);
        m_poll.add_socket(m_tcp_socket);
        m_poll.add_socket(m_tcp_self_recv.socket);
        for(auto i = 0; i < m_poll.get_num_polls(); ++i)
            m_poll.set_events(i, Socket::poll_flags::in);
    }

    while (!m_network_task.want_stop())
    {
        do_advertise();

        auto res = m_poll.poll(500);
        if (res == 0)
            continue;

        if ((m_poll.get_revents(m_udp_socket) & Socket::poll_flags::in_hup) != Socket::poll_flags::none)
            process_udp(); 

        if ((m_poll.get_revents(m_tcp_socket) & Socket::poll_flags::in_hup) != Socket::poll_flags::none)
            process_tcp_listen(); 
        
        if ((m_poll.get_revents(m_tcp_self_recv.socket) & Socket::poll_flags::in_hup) != Socket::poll_flags::none)
        {
            try
            {
                process_tcp_data(m_tcp_self_recv); 
            }
            catch (...)
            {
                assert(0 == 1 && "The local socket should not fail");
            }
        }
        
        {
            std::lock_guard<std::recursive_mutex> lk(local_mutex);
            for (auto it = m_tcp_clients.begin(); it != m_tcp_clients.end();)
            {
                auto reevents = m_poll.get_revents(it->socket);
                if ((reevents & Socket::poll_flags::hup) != Socket::poll_flags::none)
                {
                    remove_tcp_peer(*it);
                    it = m_tcp_clients.erase(it);
                }
                else if ((reevents & Socket::poll_flags::in_hup) != Socket::poll_flags::none)
                {
                    try
                    {
                        process_tcp_data(*it);
                        ++it;
                    }
                    catch (std::exception & e)
                    {
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
    }
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

    std::vector<ipv4m_addr> broadcasts = std::move(get_broadcasts());

    assert((msg.source_id() != peer_t() && "Source id cannot be null"));
    assert((msg.dest_id() == peer_t() && "Destination id should be null"));

    msg.set_timestamp(std::chrono::duration_cast<std::chrono::milliseconds>(std::chrono::system_clock::now().time_since_epoch()).count());

    string buffer;
    msg.SerializeToString(&buffer);
#if defined(NETWORK_COMPRESS)
    max_message_size = std::max<uint64_t>(max_message_size, buffer.length());

    buffer = std::move(compress(buffer.data(), buffer.length()));
    max_compressed_message_size = std::max<uint64_t>(max_compressed_message_size, buffer.length());
#endif

    for (auto& brd : broadcasts)
    {
        for (uint16_t port = network_port; port < max_network_port; ++port)
        {
            brd.set_port(port);
            try
            {
                m_udp_socket.sendto(brd, buffer.data(), buffer.length());
                
            }
            catch (socket_exception & e)
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

    #if defined(NETWORK_COMPRESS)
        buffer = std::move(compress(buffer.data(), buffer.length()));

        max_message_size = std::max<uint64_t>(max_message_size, buffer.length());
        max_compressed_message_size = std::max<uint64_t>(max_compressed_message_size, buffer.length());
    #endif

        try
        {
            m_udp_socket.sendto(peer_infos.second, buffer.data(), buffer.length());
            peers_sent_to.insert(peer_infos.first);
            
        }
        catch (socket_exception & e)
        {
            
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

#if defined(NETWORK_COMPRESS)
    max_message_size = std::max<uint64_t>(max_message_size, buffer.length());

    buffer = std::move(compress(buffer.data(), buffer.length()));
    max_compressed_message_size = std::max<uint64_t>(max_compressed_message_size, buffer.length());
#endif

    try
    {
        m_udp_socket.sendto(it->second, buffer.data(), buffer.length());
        APP_LOG(Log::LogLevel::DEBUG, "Sent message to peer_id: %s, addr: %s", msg.dest_id().c_str(), it->second.to_string().c_str());
    }
    catch (socket_exception & e)
    {
        
        return false;
    }

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

    #if defined(NETWORK_COMPRESS)
        string data;
        msg.SerializeToString(&data);

        max_message_size = std::max<uint64_t>(max_message_size, data.length());

        buffer += std::move(compress(data.data(), data.length()));
        max_compressed_message_size = std::max<uint64_t>(max_compressed_message_size, buffer.length());
    #else
        buffer += std::move(msg.SerializeAsString());
    #endif

        *reinterpret_cast<next_packet_size_t*>(&buffer[0]) = make_next_packet_size(buffer);

        try
        {
            client.second->send(buffer.data(), buffer.length());
            peers_sent_to.insert(client.first);
            
        }
        catch (socket_exception & e)
        {
            
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

#if defined(NETWORK_COMPRESS)
    string data;
    msg.SerializeToString(&data);

    max_message_size = std::max<uint64_t>(max_message_size, data.length());

    buffer += std::move(compress(data.data(), data.length()));
    max_compressed_message_size = std::max<uint64_t>(max_compressed_message_size, buffer.length());
#else
    buffer += std::move(msg.SerializeAsString());
#endif

    *reinterpret_cast<next_packet_size_t*>(&buffer[0]) = make_next_packet_size(buffer);

    try
    {
        it->second->send(buffer.data(), buffer.length());
        
    }
    catch (socket_exception & e)
    {
        
        return false;
    }

    return true;
}
