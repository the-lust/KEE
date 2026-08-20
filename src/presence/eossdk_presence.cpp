#include "eossdk_presence.h"
#include "eossdk_platform.h"
#include "eos_client_api.h"
#include "settings.h"

namespace sdk
{

decltype(EOSSDK_Presence::presence_query_timeout) EOSSDK_Presence::presence_query_timeout;

EOSSDK_Presence::EOSSDK_Presence()
{
    GetCB_Manager().register_callbacks(this);

    GetNetwork().register_listener(this, 0, Network_Message_pb::MessagesCase::kPresence);
}

EOSSDK_Presence::~EOSSDK_Presence()
{
    GetNetwork().unregister_listener(this, 0, Network_Message_pb::MessagesCase::kPresence);

    GetCB_Manager().remove_all_notifications(this);
    GetCB_Manager().unregister_callbacks(this);

    GetCB_Manager().remove_all_notifications(this);
}

void EOSSDK_Presence::setup_myself()
{
    auto& presence = get_myself();
    presence.set_userid(Settings::Inst().userid->to_string());
    presence.set_status(static_cast<int>(EOS_Presence_EStatus::EOS_PS_Online));
    presence.set_productid(GetEOS_Platform().m_product_id);
    presence.set_productversion(EOSSDK_Client::Inst().m_product_version);
    presence.set_productname(EOSSDK_Client::Inst().m_product_name);
#if defined(__WINDOWS__)
    presence.set_platform("WIN");
#elif defined(__LINUX__)
    presence.set_platform("LINUX"); 
#elif defined(__APPLE__)
    presence.set_platform("APPLE"); 
#endif
}

Presence_Info_pb& EOSSDK_Presence::get_myself()
{
    return m_presences[Settings::Inst().userid];
}

Presence_Info_pb* EOSSDK_Presence::get_presence(EOS_EpicAccountId userid)
{
    auto it = m_presences.find(userid);
    if (it == m_presences.end())
        return nullptr;

    return &it->second;
}

void EOSSDK_Presence::trigger_presence_change(EOS_EpicAccountId userid)
{
    auto notifs = std::move(GetCB_Manager().get_notifications(this, EOS_Presence_PresenceChangedCallbackInfo::k_iCallback));
    for (auto& notif : notifs)
    {
        auto& pcci = notif->GetCallback<EOS_Presence_PresenceChangedCallbackInfo>();
        pcci.PresenceUserId = userid;
        notif->GetFunc()(notif->GetFuncParam());
    }
}

void EOSSDK_Presence::set_user_status(EOS_EpicAccountId userid, EOS_Presence_EStatus status)
{
    auto& presence = m_presences[userid];
    if (presence.status() != static_cast<int>(status))
    {
        presence.set_status(static_cast<int>(status));
        trigger_presence_change(userid);
    }
}

void EOSSDK_Presence::QueryPresence( const EOS_Presence_QueryPresenceOptions* Options, void* ClientData, const EOS_Presence_OnQueryPresenceCompleteCallback CompletionDelegate)
{
    TRACE_FUNC();
    lock _(GLOBAL_MUTEX);

    SE_CB_BEGIN(EOS_Presence_QueryPresenceCallbackInfo, CompletionDelegate);
    info.LocalUserId = Settings::Inst().userid;

    if (Options == nullptr || Options->TargetUserId == nullptr)
    {
        info.ResultCode = EOS_EResult::EOS_InvalidParameters;
        info.TargetUserId = GetEpicUserId(sdk::NULL_USER_ID);

        SE_CB_END(this);
        return;
    }

    info.TargetUserId = Options->TargetUserId;
    if (info.TargetUserId == Settings::Inst().userid)
    {
        info.ResultCode = EOS_EResult::EOS_Success;
        res->done = true;
    }
    else
    {
        auto user = GetEOS_Connect().get_user_by_userid(Options->TargetUserId);
        if (user != GetEOS_Connect().get_end_users())
        {
            m_presence_queries[Options->TargetUserId].emplace_back(res);
            Presence_Info_Request_pb* req = new Presence_Info_Request_pb;
            send_presence_info_request(user->first->to_string(), req);
        }
        else
        {
            info.ResultCode = EOS_EResult::EOS_NotFound;
            res->done = true;
        }
    }

    GetCB_Manager().add_callback(this, res);
}

EOS_Bool EOSSDK_Presence::HasPresence( const EOS_Presence_HasPresenceOptions* Options)
{
    TRACE_FUNC();
    lock _(GLOBAL_MUTEX);
    
    if (Options == nullptr || Options->TargetUserId == nullptr || Options->LocalUserId != Settings::Inst().userid)
        return EOS_FALSE;

    return (get_presence(Options->TargetUserId) == nullptr ? EOS_FALSE : EOS_TRUE);
}

EOS_EResult EOSSDK_Presence::CopyPresence( const EOS_Presence_CopyPresenceOptions* Options, EOS_Presence_Info** OutPresence)
{
     
    TRACE_FUNC();
    lock _(GLOBAL_MUTEX);

    if (Options == nullptr || OutPresence == nullptr)
    {
        set_nullptr(OutPresence);
        return EOS_EResult::EOS_InvalidParameters;
    }

    auto presence = get_presence(Options->TargetUserId);
    if (presence == nullptr)
    {
        set_nullptr(OutPresence);
        return EOS_EResult::EOS_NotFound;
    }

    EOS_Presence_Info* presence_info = new EOS_Presence_Info;

    presence_info->ApiVersion = EOS_PRESENCE_COPYPRESENCE_API_LATEST;
    char *str;
    size_t len;

    len = presence->platform().length() + 1;
    str = new char[len];
    strncpy(str, presence->platform().c_str(), len);
    presence_info->Platform = str;

    len = presence->productid().length() + 1;
    str = new char[len];
    strncpy(str, presence->productid().c_str(), len);
    presence_info->ProductId = str;

    len = presence->productname().length() + 1;
    str = new char[len];
    strncpy(str, presence->productname().c_str(), len);
    presence_info->ProductName = str;

    len = presence->productversion().length() + 1;
    str = new char[len];
    strncpy(str, presence->productversion().c_str(), len);
    presence_info->ProductVersion = str;

    len = presence->richtext().length() + 1;
    str = new char[len];
    strncpy(str, presence->richtext().c_str(), len);
    presence_info->RichText = str;

    presence_info->RecordsCount = presence->records_size();
    EOS_Presence_DataRecord* records = nullptr;

    if (presence->records_size())
    {
        records = new EOS_Presence_DataRecord[presence->records_size()];
        auto record_it = presence->records().begin();
        for (int i = 0; i < presence->records_size(); ++i)
        {
            len = record_it->first.length() + 1;
            str = new char[len];
            strncpy(str, record_it->first.c_str(), len);
            records[i].Key = str;

            len = record_it->second.length() + 1;
            str = new char[len];
            strncpy(str, record_it->second.c_str(), len);
            records[i].Value = str;

            ++record_it;
        }
    }

    presence_info->Records = records;
    presence_info->Status = (EOS_Presence_EStatus)presence->status();
    presence_info->UserId = Options->TargetUserId;

    *OutPresence = presence_info;
    return EOS_EResult::EOS_Success;
}

EOS_EResult EOSSDK_Presence::CreatePresenceModification( const EOS_Presence_CreatePresenceModificationOptions* Options, EOS_HPresenceModification* OutPresenceModificationHandle)
{
    
    TRACE_FUNC();
    lock _(GLOBAL_MUTEX);

    if (Options == nullptr || OutPresenceModificationHandle == nullptr)
    {
        set_nullptr(OutPresenceModificationHandle);
        return EOS_EResult::EOS_InvalidParameters;
    }

    if (Options->LocalUserId == Settings::Inst().userid)
    {
        EOSSDK_PresenceModification *modification = new EOSSDK_PresenceModification;
        modification->infos = get_myself();
        *OutPresenceModificationHandle = reinterpret_cast<EOS_HPresenceModification>(modification);
        return EOS_EResult::EOS_Success;
    }

    *OutPresenceModificationHandle = nullptr;
    return EOS_EResult::EOS_UnexpectedError;
}

void EOSSDK_Presence::SetPresence( const EOS_Presence_SetPresenceOptions* Options, void* ClientData, const EOS_Presence_SetPresenceCompleteCallback CompletionDelegate)
{
    TRACE_FUNC();
    lock _(GLOBAL_MUTEX);

    SE_CB_BEGIN(EOS_Presence_SetPresenceCallbackInfo, CompletionDelegate);
    info.LocalUserId = Settings::Inst().userid;
    res->done = true;

    if (Options->PresenceModificationHandle == nullptr)
    {
        info.ResultCode = EOS_EResult::EOS_InvalidParameters;
    }
    else if (Options->LocalUserId != Settings::Inst().userid)
    {
        info.ResultCode = EOS_EResult::EOS_MissingPermissions;
    }
    else
    {
        info.ResultCode = EOS_EResult::EOS_Success;

        EOSSDK_PresenceModification *new_presence = reinterpret_cast<EOSSDK_PresenceModification*>(Options->PresenceModificationHandle);
        get_myself() = new_presence->infos;

        send_my_presence_info_to_all_peers();
    }

    GetCB_Manager().add_callback(this, res);
}

EOS_NotificationId EOSSDK_Presence::AddNotifyOnPresenceChanged( const EOS_Presence_AddNotifyOnPresenceChangedOptions* Options, void* ClientData, const EOS_Presence_OnPresenceChangedCallback NotificationHandler)
{
    TRACE_FUNC();
    lock _(GLOBAL_MUTEX);

    if (NotificationHandler == nullptr)
        return EOS_INVALID_NOTIFICATIONID;

    pFrameResult_t res(new FrameResult);
    EOS_Presence_PresenceChangedCallbackInfo& pcci = res->CreateCallback<EOS_Presence_PresenceChangedCallbackInfo>((CallbackFunc)NotificationHandler);
    pcci.ClientData = ClientData;
    pcci.LocalUserId = Settings::Inst().userid;
    pcci.PresenceUserId = GetEpicUserId(sdk::NULL_USER_ID);

    return GetCB_Manager().add_notification(this, res);
}

void EOSSDK_Presence::RemoveNotifyOnPresenceChanged( EOS_NotificationId NotificationId)
{
    TRACE_FUNC();
    lock _(GLOBAL_MUTEX);

    GetCB_Manager().remove_notification(this, NotificationId);
}

EOS_NotificationId EOSSDK_Presence::AddNotifyJoinGameAccepted( const EOS_Presence_AddNotifyJoinGameAcceptedOptions* Options, void* ClientData, const EOS_Presence_OnJoinGameAcceptedCallback NotificationFn)
{
     TRACE_FUNC();
     lock _(GLOBAL_MUTEX);

     if (NotificationFn == nullptr)
         return EOS_INVALID_NOTIFICATIONID;

     pFrameResult_t res(new FrameResult);
     EOS_Presence_JoinGameAcceptedCallbackInfo& jgaci = res->CreateCallback<EOS_Presence_JoinGameAcceptedCallbackInfo>((CallbackFunc)NotificationFn);
     jgaci.ClientData = ClientData;
     jgaci.LocalUserId = Settings::Inst().userid;
     jgaci.TargetUserId = GetEpicUserId(sdk::NULL_USER_ID);
     jgaci.JoinInfo = new char[EOS_PRESENCEMODIFICATION_JOININFO_MAX_LENGTH + 1];

     return GetCB_Manager().add_notification(this, res);
}

void EOSSDK_Presence::RemoveNotifyJoinGameAccepted( EOS_NotificationId InId)
{
    TRACE_FUNC();
    lock _(GLOBAL_MUTEX);
    
    GetCB_Manager().remove_notification(this, InId);
}

EOS_EResult EOSSDK_Presence::GetJoinInfo( const EOS_Presence_GetJoinInfoOptions* Options, char* OutBuffer, int32_t* InOutBufferLength)
{
    
    TRACE_FUNC();
    lock _(GLOBAL_MUTEX);

    if (Options->TargetUserId == nullptr || InOutBufferLength == nullptr || OutBuffer == nullptr)
        return EOS_EResult::EOS_InvalidParameters;

    auto presence = get_presence(Options->TargetUserId);
    if(presence == nullptr)
        return EOS_EResult::EOS_NotFound;

    auto it = presence->records().find("EOS_JoinInfo");
    if (it == presence->records().end() || it->second.empty())
    {
        *InOutBufferLength = 1;
        *OutBuffer = '\0';
    }
    else
    {
        size_t len = it->second.length() + 1;
        if (*InOutBufferLength < len)
            return EOS_EResult::EOS_LimitExceeded;

        strncpy(OutBuffer, it->second.c_str(), len);
    }

    return EOS_EResult::EOS_Success;
}

bool EOSSDK_Presence::send_presence_info_request(Network::peer_t const& peerid, Presence_Info_Request_pb* req)
{
    TRACE_FUNC();
    string const& user_id = Settings::Inst().productuserid->to_string();

    Network_Message_pb msg;
    Presence_Message_pb* presence = new Presence_Message_pb;

    presence->set_allocated_presence_info_request(req);
    msg.set_allocated_presence(presence);

    msg.set_source_id(user_id);
    msg.set_dest_id(peerid);
    msg.set_game_id(Settings::Inst().appid);

    return GetNetwork().TCPSendTo(msg);
}

bool EOSSDK_Presence::send_my_presence_info(Network::peer_t const& peerid)
{
    TRACE_FUNC();
    string const& user_id = Settings::Inst().productuserid->to_string();

    Network_Message_pb msg;
    Presence_Message_pb* presence = new Presence_Message_pb;

    presence->set_allocated_presence_info(&get_myself());
    msg.set_allocated_presence(presence);

    msg.set_source_id(user_id);
    msg.set_dest_id(peerid);
    msg.set_game_id(Settings::Inst().appid);

    auto res = GetNetwork().TCPSendTo(msg);
    (void)presence->release_presence_info();

    return res;
}

bool EOSSDK_Presence::send_my_presence_info_to_all_peers()
{
    TRACE_FUNC();
    string const& user_id = Settings::Inst().productuserid->to_string();

    Network_Message_pb msg;
    Presence_Message_pb* presence = new Presence_Message_pb;

    presence->set_allocated_presence_info(&get_myself());
    msg.set_allocated_presence(presence);

    msg.set_source_id(user_id);
    msg.set_game_id(Settings::Inst().appid);

    auto& users = GetEOS_Connect().m_users;
    for (auto user_it = ++users.begin(); user_it != users.end(); ++user_it)
    {
        if (user_it->second.authentified)
        {
            msg.set_dest_id(user_it->first->to_string());
            GetNetwork().TCPSendTo(msg);
        }
    }
    
    (void)presence->release_presence_info();
    return true;
}

bool EOSSDK_Presence::on_peer_connect(Network_Message_pb const& msg, Network_Peer_Connect_pb const& peer)
{
    
    lock _(GLOBAL_MUTEX);

    EOS_ProductUserId product_id = GetProductUserId(msg.source_id());
    auto pUser = GetEOS_Connect().get_user_by_productid(product_id);
    if (pUser != GetEOS_Connect().get_end_users() && pUser->second.authentified)
    {
        EOS_EpicAccountId account_id = GetEpicUserId(pUser->second.infos.userid());
        if (account_id->IsValid())
        {
            Presence_Info_Request_pb* req = new Presence_Info_Request_pb;
            send_presence_info_request(pUser->first->to_string(), req);
            
        }
    }

    return true;
}

bool EOSSDK_Presence::on_peer_disconnect(Network_Message_pb const& msg, Network_Peer_Disconnect_pb const& peer)
{
    
    lock _(GLOBAL_MUTEX);

    EOS_ProductUserId product_id = GetProductUserId(msg.source_id());
    auto pUser = GetEOS_Connect().get_user_by_productid(product_id);
    if (pUser != GetEOS_Connect().get_end_users() && pUser->second.authentified)
    {
        EOS_EpicAccountId account_id = GetEpicUserId(pUser->second.infos.userid());
        if (account_id->IsValid())
            set_user_status(account_id, EOS_Presence_EStatus::EOS_PS_Offline);
    }

    return true;
}

bool EOSSDK_Presence::on_presence_request(Network_Message_pb const& msg, Presence_Info_Request_pb const& req)
{
    TRACE_FUNC();
    lock _(GLOBAL_MUTEX);

    return send_my_presence_info(msg.source_id());
}

bool EOSSDK_Presence::on_presence_infos(Network_Message_pb const& msg, Presence_Info_pb const& infos)
{
    string const& user_id = Settings::Inst().productuserid->to_string();
    if (msg.source_id() == user_id)
        return true;

    TRACE_FUNC();
    lock _(GLOBAL_MUTEX);

    if (!msg.source_id().empty())
    {
        bool presence_changed = false;
        auto userid = GetEpicUserId(infos.userid());
        Presence_Info_pb& presence_infos = m_presences[userid];

        if(presence_infos.status()         != infos.status()         ||
           presence_infos.productid()      != infos.productid()      ||
           presence_infos.productversion() != infos.productversion() ||
           presence_infos.platform()       != infos.platform()       ||
           presence_infos.richtext()       != infos.richtext()       ||
           presence_infos.productname()    != infos.productname()    ||
           presence_infos.records_size()   != infos.records_size())
        {
            presence_changed = true;
        }
        else if(presence_infos.records_size())
        {
            for (auto const& record : presence_infos.records())
            {
                auto it = infos.records().find(record.first);
                if (it != infos.records().end())
                {
                    if (record.second != it->second)
                    {
                        presence_changed = true;
                        break;
                    }
                }
                else
                {
                    presence_changed = true;
                    break;
                }
            }
        }

        auto it = m_presence_queries.find(userid);
        if (it != m_presence_queries.end() && !it->second.empty())
        {
            auto presence_query_it = it->second.begin();

            (*presence_query_it)->done = true;
            (*presence_query_it)->GetCallback<EOS_Presence_QueryPresenceCallbackInfo>().ResultCode = EOS_EResult::EOS_Success;

            it->second.erase(presence_query_it);
        }

        if (presence_changed)
        {
            presence_infos = infos;
            trigger_presence_change(userid);
        }
    }

    return true;
}

bool EOSSDK_Presence::CBRunFrame()
{
    
    return true;
}

bool EOSSDK_Presence::RunNetwork(Network_Message_pb const& msg)
{
    if (msg.source_id() == Settings::Inst().userid->to_string())
        return true;

    Presence_Message_pb const& pres = msg.presence();
    switch (pres.message_case())
    {
        case Presence_Message_pb::MessageCase::kPresenceInfoRequest: return on_presence_request(msg, pres.presence_info_request());
        case Presence_Message_pb::MessageCase::kPresenceInfo       : return on_presence_infos(msg, pres.presence_info());
    }

    return false;
}

bool EOSSDK_Presence::RunCallbacks(pFrameResult_t res)
{
    lock _(GLOBAL_MUTEX);

    switch (res->ICallback())
    {
        case EOS_Presence_QueryPresenceCallbackInfo::k_iCallback:
        {
            EOS_Presence_QueryPresenceCallbackInfo& qpci = res->GetCallback<EOS_Presence_QueryPresenceCallbackInfo>();

            if ((clock::now() - res->created_time) > presence_query_timeout)
            {
                res->done = true;
                qpci.ResultCode = EOS_EResult::EOS_TimedOut;
                
                set_user_status(qpci.TargetUserId, EOS_Presence_EStatus::EOS_PS_Offline);
                auto it = m_presence_queries.find(qpci.TargetUserId);
                if (it != m_presence_queries.end())
                    m_presence_queries.erase(it);
            }
        }
        break;
    }

    return res->done;
}

void EOSSDK_Presence::FreeCallback(pFrameResult_t res)
{
    lock _(GLOBAL_MUTEX);

    {

    }
}

}