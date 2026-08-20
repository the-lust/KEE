#include "eossdk_p2p.h"
#include "eossdk_platform.h"
#include "eos_client_api.h"
#include "settings.h"

namespace sdk
{

decltype(EOSSDK_P2P::connecting_timeout) EOSSDK_P2P::connecting_timeout;
decltype(EOSSDK_P2P::connection_timeout) EOSSDK_P2P::connection_timeout;

EOSSDK_P2P::EOSSDK_P2P():
    next_requested_channel(-1),
    m_relay_control(EOS_ERelayControl::EOS_RC_AllowRelays),
    m_p2p_port(7777),
    m_max_additional_ports_to_try(99)
{
    GetCB_Manager().register_frame(this);
    GetCB_Manager().register_callbacks(this);

    GetNetwork().register_listener(this, 0, Network_Message_pb::MessagesCase::kP2P);
}

EOSSDK_P2P::~EOSSDK_P2P()
{
    GetNetwork().unregister_listener(this, 0, Network_Message_pb::MessagesCase::kP2P);

    GetCB_Manager().unregister_callbacks(this);
    GetCB_Manager().unregister_frame(this);

    GetCB_Manager().remove_all_notifications(this);
}

void EOSSDK_P2P::set_p2p_state_connected(EOS_ProductUserId remote_id, p2p_state_t& state)
{
    state.status = p2p_state_t::status_e::connected;
    for (auto& out_msgs : state.p2p_out_messages)
    {
        send_p2p_data(remote_id->to_string(), &out_msgs);
    }
    state.p2p_out_messages.clear();
}

EOS_EResult EOSSDK_P2P::SendPacket(const EOS_P2P_SendPacketOptions* Options)
{
    TRACE_FUNC();
    lock _(GLOBAL_MUTEX);
    
    if (Options == nullptr || Options->RemoteUserId == nullptr || Options->Data == nullptr)
        return EOS_EResult::EOS_InvalidParameters;

    p2p_state_t& p2p_state = m_p2p_connections[Options->RemoteUserId];
    P2P_Data_Message_pb data;
    data.set_data(reinterpret_cast<const char*>(Options->Data), Options->DataLengthBytes);
    data.set_channel(Options->Channel);
    data.set_socket_name(Options->SocketId->SocketName);
    data.set_user_id(Options->LocalUserId->to_string());

    switch(p2p_state.status)
    {
        case p2p_state_t::status_e::requesting:
        {
            APP_LOG(Log::LogLevel::INFO, "Implicit P2P acceptation on send");
            
            set_p2p_state_connected(Options->RemoteUserId, p2p_state);
        }

        case p2p_state_t::status_e::connected:
        {
            send_p2p_data(Options->RemoteUserId->to_string(), &data);
        }
        break;

        case p2p_state_t::status_e::connection_loss:
        case p2p_state_t::status_e::connecting:
        {
            
            p2p_state.p2p_out_messages.emplace_back(std::move(data));
        }
        break;

        case p2p_state_t::status_e::closed:
        {
            
            p2p_state.p2p_out_messages.emplace_back(std::move(data));

            p2p_state.status = p2p_state_t::status_e::connecting;
            p2p_state.socket_name = Options->SocketId->SocketName;
            p2p_state.connection_loss_start = clock::now();

            P2P_Connect_Request_pb* req = new P2P_Connect_Request_pb;
            req->set_socket_name(p2p_state.socket_name);
            send_p2p_connection_request(Options->RemoteUserId->to_string(), req);
        }
    }

    return EOS_EResult::EOS_Success;
}

EOS_EResult EOSSDK_P2P::GetNextReceivedPacketSize(const EOS_P2P_GetNextReceivedPacketSizeOptions* Options, uint32_t* OutPacketSizeBytes)
{
    
    std::lock_guard<std::recursive_mutex> lk(local_mutex);

    if (Options == nullptr || OutPacketSizeBytes == nullptr)
        return EOS_EResult::EOS_InvalidParameters;

    bool has_packet = false;
    if (Options->RequestedChannel == nullptr)
    {
        for (auto& in_msgs : m_p2p_in_messages)
        {
            if (!in_msgs.second.empty())
            {
                auto& front = in_msgs.second.front();
                *OutPacketSizeBytes = static_cast<uint32_t>(front.data().length());
                next_requested_channel = front.channel();
                has_packet = true;
            }
        }
    }
    else
    {
        next_requested_channel = *Options->RequestedChannel;
        auto& in_msgs = m_p2p_in_messages[next_requested_channel];
        if (!in_msgs.empty())
        {
            *OutPacketSizeBytes = static_cast<uint32_t>(in_msgs.front().data().length());
            has_packet = true;
        }
    }

    if (has_packet)
    {
        return EOS_EResult::EOS_Success;
    }
    
    *OutPacketSizeBytes = 0;
    return EOS_EResult::EOS_NotFound;
}

EOS_EResult EOSSDK_P2P::ReceivePacket(const EOS_P2P_ReceivePacketOptions* Options, EOS_ProductUserId* OutPeerId, EOS_P2P_SocketId* OutSocketId, uint8_t* OutChannel, void* OutData, uint32_t* OutBytesWritten)
{
    std::lock_guard<std::recursive_mutex> lk(local_mutex);

    if (Options == nullptr || OutPeerId == nullptr || OutSocketId == nullptr ||
        OutChannel == nullptr || OutData == nullptr || OutBytesWritten == nullptr)
    {
        return EOS_EResult::EOS_InvalidParameters;
    }

    if (Options->RequestedChannel != nullptr)
        next_requested_channel = *Options->RequestedChannel;

    std::list<P2P_Data_Message_pb> *queue = nullptr;
    if (next_requested_channel == -1)
    {
        auto it = std::find_if(m_p2p_in_messages.begin(), m_p2p_in_messages.end(), []( std::pair<uint8_t const, std::list<P2P_Data_Message_pb>>& messages_queue)
        {
            return !messages_queue.second.empty();
        });
        if (it != m_p2p_in_messages.end())
            queue = &it->second;
    }
    else
    {
        queue = &m_p2p_in_messages[next_requested_channel];
        if (queue->empty())
        {
            queue = nullptr;
        }
    }
    if (queue == nullptr)
    {
        return EOS_EResult::EOS_NotFound;
    }

    auto& msg = queue->front();

    *OutPeerId = GetProductUserId(msg.user_id());
    *OutBytesWritten = static_cast<uint32_t>(msg.data().copy(reinterpret_cast<char*>(OutData), Options->MaxDataSizeBytes));
    msg.socket_name().copy(OutSocketId->SocketName, sizeof(EOS_P2P_SocketId::SocketName));
    OutSocketId->SocketName[32] = 0;
    *OutChannel = msg.channel();
    next_requested_channel = -1;

    queue->pop_front();

    return EOS_EResult::EOS_Success;
}

EOS_NotificationId EOSSDK_P2P::AddNotifyPeerConnectionRequest(const EOS_P2P_AddNotifyPeerConnectionRequestOptions* Options, void* ClientData, EOS_P2P_OnIncomingConnectionRequestCallback ConnectionRequestHandler)
{
    TRACE_FUNC();
    lock _(GLOBAL_MUTEX);

    if (ConnectionRequestHandler == nullptr)
        return EOS_INVALID_NOTIFICATIONID;

    pFrameResult_t res(new FrameResult);
    
    EOS_P2P_OnIncomingConnectionRequestInfo& oicri = res->CreateCallback<EOS_P2P_OnIncomingConnectionRequestInfo>((CallbackFunc)ConnectionRequestHandler);
    oicri.ClientData = ClientData;
    oicri.LocalUserId = Settings::Inst().productuserid;
    oicri.RemoteUserId = GetProductUserId(sdk::NULL_USER_ID);
    oicri.SocketId = new EOS_P2P_SocketId;

    return GetCB_Manager().add_notification(this, res);
}

void EOSSDK_P2P::RemoveNotifyPeerConnectionRequest(EOS_NotificationId NotificationId)
{
    TRACE_FUNC();
    lock _(GLOBAL_MUTEX);

    GetCB_Manager().remove_notification(this, NotificationId);
}

EOS_NotificationId EOSSDK_P2P::AddNotifyPeerConnectionClosed(const EOS_P2P_AddNotifyPeerConnectionClosedOptions* Options, void* ClientData, EOS_P2P_OnRemoteConnectionClosedCallback ConnectionClosedHandler)
{
    TRACE_FUNC();
    lock _(GLOBAL_MUTEX);

    if (ConnectionClosedHandler == nullptr)
        return EOS_INVALID_NOTIFICATIONID;

    pFrameResult_t res(new FrameResult);

    EOS_P2P_OnRemoteConnectionClosedInfo& orcci = res->CreateCallback<EOS_P2P_OnRemoteConnectionClosedInfo>((CallbackFunc)ConnectionClosedHandler);
    orcci.ClientData = ClientData;
    orcci.LocalUserId = Settings::Inst().productuserid;
    orcci.RemoteUserId = GetProductUserId(sdk::NULL_USER_ID);
    orcci.SocketId = new EOS_P2P_SocketId;
    orcci.Reason = EOS_EConnectionClosedReason::EOS_CCR_Unknown;

    return GetCB_Manager().add_notification(this, res);
}

void EOSSDK_P2P::RemoveNotifyPeerConnectionClosed(EOS_NotificationId NotificationId)
{
    TRACE_FUNC();
    lock _(GLOBAL_MUTEX);

    GetCB_Manager().remove_notification(this, NotificationId);
}

EOS_EResult EOSSDK_P2P::AcceptConnection(const EOS_P2P_AcceptConnectionOptions* Options)
{
    TRACE_FUNC();
    lock _(GLOBAL_MUTEX);

    if (Options == nullptr || Options->RemoteUserId == nullptr || Options->SocketId == nullptr)
        return EOS_EResult::EOS_InvalidParameters;
    
    auto& conn = m_p2p_connections[Options->RemoteUserId];

    if (conn.status == p2p_state_t::status_e::requesting)
    {
        P2P_Connect_Response_pb* resp = new P2P_Connect_Response_pb;
        resp->set_accepted(true);
        send_p2p_connection_response(Options->RemoteUserId->to_string(), resp);
    }
    
    conn.status = p2p_state_t::status_e::connected;
    conn.connection_loss_start = {};
    return EOS_EResult::EOS_Success;
}

EOS_EResult EOSSDK_P2P::CloseConnection(const EOS_P2P_CloseConnectionOptions* Options)
{
    TRACE_FUNC();
    APP_LOG(Log::LogLevel::DEBUG, "TODO");
    lock _(GLOBAL_MUTEX);
    
    if (Options == nullptr || Options->RemoteUserId == nullptr)
        return EOS_EResult::EOS_InvalidParameters;

    if (Options->SocketId == nullptr)
    {
        auto& conn = m_p2p_connections[Options->RemoteUserId];
        conn.p2p_out_messages.clear();
        for (auto& in_msgs : m_p2p_in_messages)
        {
            in_msgs.second.erase(std::remove_if(in_msgs.second.begin(), in_msgs.second.end(), [&Options](P2P_Data_Message_pb& msg)
            {
                return msg.user_id() == Options->RemoteUserId->to_string();
            }), in_msgs.second.end());
        }

        if (conn.status != p2p_state_t::status_e::closed)
        {
            conn.status = p2p_state_t::status_e::closed;

            P2P_Connection_Close_pb* close = new P2P_Connection_Close_pb;
            send_p2p_connetion_close(Options->RemoteUserId->to_string(), close);
        }
    }
    else
    {
        string target_sock_name = Options->SocketId->SocketName;
        auto& conn = m_p2p_connections[Options->RemoteUserId];
        conn.p2p_out_messages.clear();
        for (auto& in_msgs : m_p2p_in_messages)
        {
            in_msgs.second.erase(std::remove_if(in_msgs.second.begin(), in_msgs.second.end(), [&Options](P2P_Data_Message_pb& msg)
            {
                return msg.user_id() == Options->RemoteUserId->to_string();
            }), in_msgs.second.end());
        }

        if (conn.status != p2p_state_t::status_e::closed)
        {
            if (conn.socket_name == target_sock_name)
            {
                conn.status = p2p_state_t::status_e::closed;

                P2P_Connection_Close_pb* close = new P2P_Connection_Close_pb;
                send_p2p_connetion_close(Options->RemoteUserId->to_string(), close);
            }
        }
    }
    
    return EOS_EResult::EOS_Success;
}

EOS_EResult EOSSDK_P2P::CloseConnections(const EOS_P2P_CloseConnectionsOptions* Options)
{
    TRACE_FUNC();
    lock _(GLOBAL_MUTEX);

    if (Options == nullptr || Options->SocketId == nullptr)
        return EOS_EResult::EOS_InvalidParameters;

    string target_sock_name = Options->SocketId->SocketName;
    for (auto& conn : m_p2p_connections)
    {
        if (conn.second.socket_name == target_sock_name)
        {
            conn.second.status = p2p_state_t::status_e::closed;

            P2P_Connection_Close_pb* close = new P2P_Connection_Close_pb;
            send_p2p_connetion_close(conn.first->to_string(), close);
        }
    }
    
    return EOS_EResult::EOS_Success;
}

void EOSSDK_P2P::QueryNATType(const EOS_P2P_QueryNATTypeOptions* Options, void* ClientData, const EOS_P2P_OnQueryNATTypeCompleteCallback NATTypeQueriedHandler)
{
    TRACE_FUNC();
    APP_LOG(Log::LogLevel::DEBUG, "TODO");
    lock _(GLOBAL_MUTEX);

    if (NATTypeQueriedHandler == nullptr)
        return;

    pFrameResult_t res(new FrameResult);
    EOS_P2P_OnQueryNATTypeCompleteInfo& pqntci = res->CreateCallback<EOS_P2P_OnQueryNATTypeCompleteInfo>((CallbackFunc)NATTypeQueriedHandler, std::chrono::milliseconds(5000));
    pqntci.ClientData = ClientData;

    if (Options == nullptr)
    {
        pqntci.NATType = EOS_ENATType::EOS_NAT_Unknown;
        pqntci.ResultCode = EOS_EResult::EOS_InvalidParameters;
    }
    else
    {
        pqntci.NATType = EOS_ENATType::EOS_NAT_Open;
        pqntci.ResultCode = EOS_EResult::EOS_Success;
    }
    
    SE_CB_END(this);
}

EOS_EResult EOSSDK_P2P::GetNATType(const EOS_P2P_GetNATTypeOptions* Options, EOS_ENATType* OutNATType)
{
    TRACE_FUNC();
    APP_LOG(Log::LogLevel::DEBUG, "TODO");
    lock _(GLOBAL_MUTEX);

    *OutNATType = EOS_ENATType::EOS_NAT_Moderate;
    return EOS_EResult::EOS_Success;
}

EOS_EResult EOSSDK_P2P::SetRelayControl(const EOS_P2P_SetRelayControlOptions* Options)
{
    TRACE_FUNC();
    lock _(GLOBAL_MUTEX);

    if(Options == nullptr)
        return EOS_EResult::EOS_InvalidParameters;
    
    m_relay_control = Options->RelayControl;
    return EOS_EResult::EOS_Success;
}

EOS_EResult EOSSDK_P2P::GetRelayControl(const EOS_P2P_GetRelayControlOptions* Options, EOS_ERelayControl* OutRelayControl)
{
    TRACE_FUNC();
    lock _(GLOBAL_MUTEX);

    if (Options == nullptr || OutRelayControl == nullptr)
        return EOS_EResult::EOS_InvalidParameters;

    *OutRelayControl = m_relay_control;
    return EOS_EResult::EOS_Success;
}

EOS_EResult EOSSDK_P2P::SetPortRange(const EOS_P2P_SetPortRangeOptions* Options)
{
    TRACE_FUNC();
    lock _(GLOBAL_MUTEX);

    if (Options == nullptr || Options->Port <= 1024)
        return EOS_EResult::EOS_InvalidParameters;

    m_p2p_port = Options->Port;
    m_max_additional_ports_to_try = Options->MaxAdditionalPortsToTry;
    return EOS_EResult::EOS_Success;
}

EOS_EResult EOSSDK_P2P::GetPortRange(const EOS_P2P_GetPortRangeOptions* Options, uint16_t* OutPort, uint16_t* OutNumAdditionalPortsToTry)
{
    TRACE_FUNC();
    lock _(GLOBAL_MUTEX);

    if (Options == nullptr || OutPort == nullptr || OutNumAdditionalPortsToTry == nullptr)
        return EOS_EResult::EOS_InvalidParameters;

    *OutPort = m_p2p_port;
    *OutNumAdditionalPortsToTry = m_max_additional_ports_to_try;
    return EOS_EResult::EOS_Success;
}

bool EOSSDK_P2P::send_p2p_connection_request(Network::peer_t const& peerid, P2P_Connect_Request_pb* req) const
{
    TRACE_FUNC();
    string const& user_id = Settings::Inst().productuserid->to_string();

    Network_Message_pb msg;
    P2P_Message_pb* p2p = new P2P_Message_pb;

    p2p->set_allocated_connect_request(req);

    msg.set_source_id(user_id);
    msg.set_dest_id(peerid);
    msg.set_game_id(Settings::Inst().appid);

    msg.set_allocated_p2p(p2p);

    return GetNetwork().TCPSendTo(msg);
}

bool EOSSDK_P2P::send_p2p_connection_response(Network::peer_t const& peerid, P2P_Connect_Response_pb* resp) const
{
    TRACE_FUNC();
    string const& user_id = Settings::Inst().productuserid->to_string();

    Network_Message_pb msg;
    P2P_Message_pb* p2p = new P2P_Message_pb;

    p2p->set_allocated_connect_response(resp);

    msg.set_source_id(user_id);
    msg.set_dest_id(peerid);
    msg.set_game_id(Settings::Inst().appid);

    msg.set_allocated_p2p(p2p);

    return GetNetwork().TCPSendTo(msg);
}

bool EOSSDK_P2P::send_p2p_data(Network::peer_t const& peerid, P2P_Data_Message_pb* data) const
{
    TRACE_FUNC();
    string const& user_id = Settings::Inst().productuserid->to_string();

    Network_Message_pb msg;
    P2P_Message_pb* p2p = new P2P_Message_pb;

    p2p->set_allocated_data_message(data);

    msg.set_source_id(user_id);
    msg.set_dest_id(peerid);
    msg.set_game_id(Settings::Inst().appid);

    msg.set_allocated_p2p(p2p);
    auto res = GetNetwork().UDPSendTo(msg);

    (void)p2p->release_data_message();

    return res;
}

bool EOSSDK_P2P::send_p2p_data_ack(Network::peer_t const& peerid, P2P_Data_Acknowledge_pb* ack) const
{
    TRACE_FUNC();
    string const& user_id = Settings::Inst().productuserid->to_string();

    Network_Message_pb msg;
    P2P_Message_pb* p2p = new P2P_Message_pb;

    p2p->set_allocated_data_acknowledge(ack);

    msg.set_source_id(user_id);
    msg.set_dest_id(peerid);
    msg.set_game_id(Settings::Inst().appid);

    msg.set_allocated_p2p(p2p);

    return GetNetwork().UDPSendTo(msg);
}

bool EOSSDK_P2P::send_p2p_connetion_close(Network::peer_t const& peerid, P2P_Connection_Close_pb* close) const
{
    TRACE_FUNC();
    string const& user_id = Settings::Inst().productuserid->to_string();

    Network_Message_pb msg;
    P2P_Message_pb* p2p = new P2P_Message_pb;

    p2p->set_allocated_connection_close(close);

    msg.set_source_id(user_id);
    msg.set_dest_id(peerid);
    msg.set_game_id(Settings::Inst().appid);

    msg.set_allocated_p2p(p2p);

    return GetNetwork().TCPSendTo(msg);
}

bool EOSSDK_P2P::on_peer_connect(Network_Message_pb const& msg, Network_Peer_Connect_pb const& peer)
{
    TRACE_FUNC();
    lock _(GLOBAL_MUTEX);

    auto peer_id = GetProductUserId(msg.source_id());
    auto it = m_p2p_connections.find(peer_id);
    if (it != m_p2p_connections.end() && it->second.status == p2p_state_t::status_e::connection_loss)
    {
        it->second.status = p2p_state_t::status_e::connected;

        for (auto& msg : it->second.p2p_out_messages)
        {
            send_p2p_data(it->first->to_string(), &msg);
        }
        it->second.p2p_out_messages.clear();
    }

    return true;
}

bool EOSSDK_P2P::on_peer_disconnect(Network_Message_pb const& msg, Network_Peer_Disconnect_pb const& peer)
{
    TRACE_FUNC();
    lock _(GLOBAL_MUTEX);

    auto peer_id = GetProductUserId(msg.source_id());
    auto it = m_p2p_connections.find(peer_id);
    if (it != m_p2p_connections.end())
    {
        it->second.connection_loss_start = clock::now();
    }

    return true;
}

bool EOSSDK_P2P::on_p2p_connection_request(Network_Message_pb const& msg, P2P_Connect_Request_pb const& req)
{
    TRACE_FUNC();
    lock _(GLOBAL_MUTEX);

    auto peer_id = GetProductUserId(msg.source_id());
    auto& conn = m_p2p_connections[peer_id];
    if (conn.status != p2p_state_t::status_e::connected)
    {
        conn.status = p2p_state_t::status_e::requesting;
        conn.connection_loss_start = clock::now();
        conn.socket_name = req.socket_name();
        std::vector<pFrameResult_t> notifs = std::move(GetCB_Manager().get_notifications(this, EOS_P2P_OnIncomingConnectionRequestInfo::k_iCallback));
        for (auto& notif : notifs)
        {
            EOS_P2P_OnIncomingConnectionRequestInfo& oicrc = notif->GetCallback<EOS_P2P_OnIncomingConnectionRequestInfo>();
            oicrc.RemoteUserId = peer_id;
            strncpy(const_cast<char*>(oicrc.SocketId->SocketName), req.socket_name().c_str(), sizeof(EOS_P2P_SocketId::SocketName));

            notif->GetFunc()(notif->GetFuncParam());
        }
    }
    else
    {
        P2P_Connect_Response_pb* resp = new P2P_Connect_Response_pb;
        resp->set_accepted(true);
        send_p2p_connection_response(msg.source_id(), resp);
    }

    return true;
}

bool EOSSDK_P2P::on_p2p_connection_response(Network_Message_pb const& msg, P2P_Connect_Response_pb const& resp)
{
    TRACE_FUNC();
    lock _(GLOBAL_MUTEX);
    
    EOS_ProductUserId remote_id = GetProductUserId(msg.source_id());
    if (resp.accepted())
    {
        set_p2p_state_connected(remote_id, m_p2p_connections[remote_id]);
    }
    else
    {
        std::vector<pFrameResult_t> notifs = std::move(GetCB_Manager().get_notifications(this, EOS_P2P_OnRemoteConnectionClosedInfo::k_iCallback));
        for (auto& notif : notifs)
        {
            EOS_P2P_OnRemoteConnectionClosedInfo& orcci = notif->GetCallback<EOS_P2P_OnRemoteConnectionClosedInfo>();
            orcci.Reason = EOS_EConnectionClosedReason::EOS_CCR_ClosedByPeer;
            orcci.RemoteUserId = remote_id;

            notif->GetFunc()(notif->GetFuncParam());
        }
    }

    return true;
}

bool EOSSDK_P2P::on_p2p_data(Network_Message_pb const& msg, P2P_Data_Message_pb const& data)
{
    TRACE_FUNC();
    std::lock_guard<std::recursive_mutex> lk(local_mutex);

    EOS_ProductUserId remote_id = GetProductUserId(msg.source_id());
    auto& p2p_state = m_p2p_connections[remote_id];

    P2P_Data_Acknowledge_pb* ack = new P2P_Data_Acknowledge_pb;

    switch (p2p_state.status)
    {
        case p2p_state_t::status_e::connecting:
        {
            APP_LOG(Log::LogLevel::INFO, "Implicit P2P acceptation on receive");
            set_p2p_state_connected(remote_id, p2p_state);
        }

        case p2p_state_t::status_e::connected:
        {
            ack->set_channel(data.channel());
            ack->set_accepted(true);
            m_p2p_in_messages[data.channel()].emplace_back(data);
        }
        break;

        default:
            ack->set_accepted(false);
    }

    return send_p2p_data_ack(msg.source_id(), ack);
}

bool EOSSDK_P2P::on_p2p_data_ack(Network_Message_pb const& msg, P2P_Data_Acknowledge_pb const& ack)
{
    TRACE_FUNC();
    lock _(GLOBAL_MUTEX);

    return true;
}

bool EOSSDK_P2P::on_p2p_connection_close(Network_Message_pb const& msg, P2P_Connection_Close_pb const& close)
{
    TRACE_FUNC();
    lock _(GLOBAL_MUTEX);

    std::vector<pFrameResult_t> notifs = std::move(GetCB_Manager().get_notifications(this, EOS_P2P_OnRemoteConnectionClosedInfo::k_iCallback));
    for (auto& notif : notifs)
    {
        EOS_P2P_OnRemoteConnectionClosedInfo& orcci = notif->GetCallback<EOS_P2P_OnRemoteConnectionClosedInfo>();
        orcci.Reason = EOS_EConnectionClosedReason::EOS_CCR_ClosedByPeer;
        orcci.RemoteUserId = GetProductUserId(msg.source_id());

        notif->GetFunc()(notif->GetFuncParam());
    }

    m_p2p_connections[GetProductUserId(msg.source_id())].status = p2p_state_t::status_e::closed;

    return true;
}

bool EOSSDK_P2P::CBRunFrame()
{
    lock _(GLOBAL_MUTEX);

    for (auto it = m_p2p_connections.begin(); it != m_p2p_connections.end(); ++it)
    {
        switch(it->second.status)
        {
            case p2p_state_t::status_e::requesting:
            {
                auto now = clock::now();
                if ((now - it->second.connection_loss_start) > connecting_timeout)
                {
                    it->second.status = p2p_state_t::status_e::closed;
                    it->second.p2p_out_messages.clear();
                }
            }
            break;

            case p2p_state_t::status_e::connecting:
            {
                auto now = clock::now();
                if ((now - it->second.connection_loss_start) > connecting_timeout)
                {
                    it->second.status = p2p_state_t::status_e::closed;
                    it->second.p2p_out_messages.clear();

                    std::vector<pFrameResult_t> notifs(std::move(GetCB_Manager().get_notifications(this, EOS_P2P_OnRemoteConnectionClosedInfo::k_iCallback)));
                    for (auto& notif : notifs)
                    {
                        EOS_P2P_OnRemoteConnectionClosedInfo& orcci = notif->GetCallback<EOS_P2P_OnRemoteConnectionClosedInfo>();
                        orcci.RemoteUserId = it->first;
                        strncpy(const_cast<char*>(orcci.SocketId->SocketName), it->second.socket_name.c_str(), sizeof(orcci.SocketId->SocketName));
                        const_cast<char*>(orcci.SocketId->SocketName)[sizeof(orcci.SocketId->SocketName) - 1] = '\0';
                        orcci.Reason = EOS_EConnectionClosedReason::EOS_CCR_ConnectionFailed;
                    }
                }
            }
            break;

            case p2p_state_t::status_e::connection_loss:
            {
                auto now = clock::now();
                if ((now - it->second.connection_loss_start) > connection_timeout)
                {
                    it->second.status = p2p_state_t::status_e::closed;
                    it->second.p2p_out_messages.clear();

                    std::vector<pFrameResult_t> notifs(std::move(GetCB_Manager().get_notifications(this, EOS_P2P_OnRemoteConnectionClosedInfo::k_iCallback)));
                    for (auto& notif : notifs)
                    {
                        EOS_P2P_OnRemoteConnectionClosedInfo& orcci = notif->GetCallback<EOS_P2P_OnRemoteConnectionClosedInfo>();
                        orcci.RemoteUserId = it->first;
                        strncpy(const_cast<char*>(orcci.SocketId->SocketName), it->second.socket_name.c_str(), sizeof(orcci.SocketId->SocketName));
                        const_cast<char*>(orcci.SocketId->SocketName)[sizeof(orcci.SocketId->SocketName) - 1] = '\0';
                        orcci.Reason = EOS_EConnectionClosedReason::EOS_CCR_TimedOut;
                    }
                }
            }
            break;
        }
    }

    return true;
}

bool EOSSDK_P2P::RunNetwork(Network_Message_pb const& msg)
{
    switch (msg.messages_case())
    {
        case Network_Message_pb::MessagesCase::kP2P:
        {
            P2P_Message_pb const& p2p = msg.p2p();
            switch (p2p.message_case())
            {
                case P2P_Message_pb::MessageCase::kConnectRequest : return on_p2p_connection_request(msg, p2p.connect_request());
                case P2P_Message_pb::MessageCase::kConnectResponse: return on_p2p_connection_response(msg, p2p.connect_response());
                case P2P_Message_pb::MessageCase::kDataMessage    : return on_p2p_data(msg, p2p.data_message());
                case P2P_Message_pb::MessageCase::kDataAcknowledge: return on_p2p_data_ack(msg, p2p.data_acknowledge());
                case P2P_Message_pb::MessageCase::kConnectionClose: return on_p2p_connection_close(msg, p2p.connection_close());
                default: APP_LOG(Log::LogLevel::WARN, "Unhandled network message %d", p2p.message_case());
            }
        }
    }

    return true;
}

bool EOSSDK_P2P::RunCallbacks(pFrameResult_t res)
{
    lock _(GLOBAL_MUTEX);

    return res->done;
}

void EOSSDK_P2P::FreeCallback(pFrameResult_t res)
{
    lock _(GLOBAL_MUTEX);

    switch (res->ICallback())
    {

        case EOS_P2P_OnIncomingConnectionRequestInfo::k_iCallback:
        {
            EOS_P2P_OnIncomingConnectionRequestInfo& callback = res->GetCallback<EOS_P2P_OnIncomingConnectionRequestInfo>();
            delete callback.SocketId;
        }
        break;
        case EOS_P2P_OnRemoteConnectionClosedInfo::k_iCallback:
        {
            EOS_P2P_OnRemoteConnectionClosedInfo& callback = res->GetCallback<EOS_P2P_OnRemoteConnectionClosedInfo>();
            delete callback.SocketId;
        }
        break;
    }
}

}