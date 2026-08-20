#include "eossdk_connect.h"
#include "eossdk_platform.h"
#include "eos_client_api.h"
#include "settings.h"
#include "utils/jwt_utils.h"

namespace sdk
{

decltype(EOSSDK_Connect::user_infos_rate)      EOSSDK_Connect::user_infos_rate;

EOSSDK_Connect::EOSSDK_Connect()
{
    auto userProductId = Settings::Inst().productuserid;
    auto& myself = m_users[userProductId];
    myself.connected = false;
    myself.infos.set_userid(Settings::Inst().userid->to_string());
    myself.infos.set_displayname(Settings::Inst().username);

    APP_LOG(Log::LogLevel::DEBUG, "Userid: %s, Productid: %s", Settings::Inst().userid->to_string().c_str(), userProductId->to_string().c_str());
    GetNetwork().set_default_channel(userProductId->to_string(), 0);
    GetNetwork().advertise_peer_id(userProductId->to_string());

    m_device_id_filename = FileManager::join(Settings::Inst().save_path, "device_id.txt");
    {
        std::ifstream f(m_device_id_filename);
        if (f.good()) std::getline(f, m_device_id);
    }
    if (m_device_id.empty())
    {
        
        m_device_id = "kee-device-" + Settings::Inst().userid->to_string();
    }

    GetCB_Manager().register_callbacks(this);
    GetCB_Manager().register_frame(this);

    GetNetwork().register_listener(this, 0, Network_Message_pb::MessagesCase::kNetworkAdvertise);
    GetNetwork().register_listener(this, 0, Network_Message_pb::MessagesCase::kConnect);

    GetNetwork().advertise(true);
}

EOSSDK_Connect::~EOSSDK_Connect()
{
    GetNetwork().advertise(false);
    GetNetwork().remove_advertise_peer_id(get_myself()->first->to_string());

    GetNetwork().unregister_listener(this, 0, Network_Message_pb::MessagesCase::kConnect);
    GetNetwork().unregister_listener(this, 0, Network_Message_pb::MessagesCase::kNetworkAdvertise);

    GetCB_Manager().unregister_frame(this);
    GetCB_Manager().unregister_callbacks(this);

    GetCB_Manager().remove_all_notifications(this);
}

void EOSSDK_Connect::Login(const EOS_Connect_LoginOptions* Options, void* ClientData, const EOS_Connect_OnLoginCallback CompletionDelegate) {
    TRACE_FUNC();
    if (!CompletionDelegate) return;
    
    // just send back a success... it's an emu after all
    dispatch_cb<EOS_Connect_LoginCallbackInfo>(this, CompletionDelegate, ClientData);
    
    get_myself()->second.connected = true;
    
    // tell other systems that we are logged in now
    for (auto& n : GetCB_Manager().get_notifications(this, EOS_Connect_LoginStatusChangedCallbackInfo::k_iCallback)) {
        auto& lscci = n->GetCallback<EOS_Connect_LoginStatusChangedCallbackInfo>();
        lscci.LocalUserId = Settings::Inst().productuserid;
        lscci.PreviousStatus = EOS_ELoginStatus::EOS_LS_NotLoggedIn;
        lscci.CurrentStatus = EOS_ELoginStatus::EOS_LS_LoggedIn;
        n->GetFunc()(n->GetFuncParam());
    }
}

void EOSSDK_Connect::CreateUser(const EOS_Connect_CreateUserOptions* Options, void* ClientData, const EOS_Connect_OnCreateUserCallback CompletionDelegate) {
    TRACE_FUNC();
    if (!CompletionDelegate) return;
    
    // user already exists is fine, it just means we are already setup
    dispatch_cb<EOS_Connect_CreateUserCallbackInfo>(this, CompletionDelegate, ClientData, EOS_EResult::EOS_Connect_UserAlreadyExists);
    
    get_myself()->second.connected = true;
}



void EOSSDK_Connect::LinkAccount(const EOS_Connect_LinkAccountOptions* Options, void* ClientData, const EOS_Connect_OnLinkAccountCallback CompletionDelegate)
{
    TRACE_FUNC();

    SE_CB_BEGIN(EOS_Connect_LinkAccountCallbackInfo, CompletionDelegate);
    info.LocalUserId = get_myself()->first;

    if (Options == nullptr)
    {
        info.ResultCode = EOS_EResult::EOS_InvalidParameters;
    }
    else
    {
        info.ResultCode = EOS_EResult::EOS_Connect_LinkAccountFailed;
    }

    SE_CB_END(this);
}

void EOSSDK_Connect::UnlinkAccount(const EOS_Connect_UnlinkAccountOptions* Options, void* ClientData, const EOS_Connect_OnUnlinkAccountCallback CompletionDelegate)
{
    TRACE_FUNC();

    SE_CB_BEGIN(EOS_Connect_UnlinkAccountCallbackInfo, CompletionDelegate);
    info.LocalUserId = GetProductUserId(Settings::Inst().productuserid->to_string());

    if (Options == nullptr)
    {
        info.ResultCode = EOS_EResult::EOS_InvalidParameters;
    }
    else
    {
        info.ResultCode = EOS_EResult::EOS_Success;
    }

    SE_CB_END(this);
}

void EOSSDK_Connect::CreateDeviceId(const EOS_Connect_CreateDeviceIdOptions* Options, void* ClientData, const EOS_Connect_OnCreateDeviceIdCallback CompletionDelegate)
{
    TRACE_FUNC();

    SE_CB_BEGIN(EOS_Connect_CreateDeviceIdCallbackInfo, CompletionDelegate);

    if (Options == nullptr || Options->DeviceModel == nullptr)
    {
        info.ResultCode = EOS_EResult::EOS_InvalidParameters;
    }
    else
        {
            m_device_id = Options->DeviceModel;
            
            {
                std::ofstream f(m_device_id_filename, std::ios::trunc);
                if (f.good()) f << m_device_id;
            }
            info.ResultCode = EOS_EResult::EOS_Success;
        }

        SE_CB_END(this);
}

void EOSSDK_Connect::DeleteDeviceId(const EOS_Connect_DeleteDeviceIdOptions* Options, void* ClientData, const EOS_Connect_OnDeleteDeviceIdCallback CompletionDelegate)
{
    TRACE_FUNC();

    SE_CB_BEGIN(EOS_Connect_DeleteDeviceIdCallbackInfo, CompletionDelegate);

    if (Options == nullptr || m_device_id.empty())
    {
        info.ResultCode = EOS_EResult::EOS_InvalidParameters;
    }
    else
    {
        m_device_id.clear();
        info.ResultCode = EOS_EResult::EOS_Success;
    }

    SE_CB_END(this);
}

void EOSSDK_Connect::TransferDeviceIdAccount(const EOS_Connect_TransferDeviceIdAccountOptions* Options, void* ClientData, const EOS_Connect_OnTransferDeviceIdAccountCallback CompletionDelegate)
{
    TRACE_FUNC();

    if (CompletionDelegate == nullptr)
        return;

    pFrameResult_t res(new FrameResult);
    EOS_Connect_TransferDeviceIdAccountCallbackInfo& tdiaci = res->CreateCallback<EOS_Connect_TransferDeviceIdAccountCallbackInfo>((CallbackFunc)CompletionDelegate);

    tdiaci.LocalUserId = GetProductUserId(Settings::Inst().productuserid->to_string());
    tdiaci.ClientData = ClientData;

    if (Options == nullptr)
    {
        tdiaci.ResultCode = EOS_EResult::EOS_InvalidParameters;
    }
    else
    {
        tdiaci.ResultCode = EOS_EResult::EOS_Success;
    }

    SE_CB_END(this);
}

void EOSSDK_Connect::QueryExternalAccountMappings(const EOS_Connect_QueryExternalAccountMappingsOptions* Options, void* ClientData, const EOS_Connect_OnQueryExternalAccountMappingsCallback
    CompletionDelegate)
{
    if (CompletionDelegate == nullptr)
        return;

    TRACE_FUNC();
    
    pFrameResult_t res(new FrameResult);
    EOS_Connect_QueryExternalAccountMappingsCallbackInfo& qeamci = res->CreateCallback<EOS_Connect_QueryExternalAccountMappingsCallbackInfo>((CallbackFunc)CompletionDelegate);
    qeamci.ClientData = ClientData;
    qeamci.LocalUserId = Settings::Inst().productuserid;

    if (Options == nullptr || Options->ExternalAccountIds == nullptr)
    {
        qeamci.ResultCode = EOS_EResult::EOS_InvalidParameters;
    }
    else
    {
        switch (Options->AccountIdType)
        {
            case EOS_EExternalAccountType::EOS_EAT_EPIC:
            {
                qeamci.ResultCode = EOS_EResult::EOS_Success;
            }
            break;
            default:
            {
                qeamci.ResultCode = EOS_EResult::EOS_Connect_ExternalServiceUnavailable;
            }
        }
    }

    SE_CB_END(this);
}

void EOSSDK_Connect::QueryProductUserIdMappings(const EOS_Connect_QueryProductUserIdMappingsOptions* Options, void* ClientData, const EOS_Connect_OnQueryProductUserIdMappingsCallback CompletionDelegate)
{
    TRACE_FUNC();

    SE_CB_BEGIN(EOS_Connect_QueryProductUserIdMappingsCallbackInfo, CompletionDelegate);
    info.LocalUserId = get_myself()->first;

    if (Options == nullptr)
    {
        info.ResultCode = EOS_EResult::EOS_InvalidParameters;
    }
    else
    {
        info.ResultCode = EOS_EResult::EOS_Success;
    }

    SE_CB_END(this);
}

EOS_ProductUserId EOSSDK_Connect::GetExternalAccountMapping(const EOS_Connect_GetExternalAccountMappingsOptions* Options)
{
    TRACE_FUNC();

    if (Options == nullptr || Options->TargetExternalUserId == nullptr || Options->AccountIdType != EOS_EExternalAccountType::EOS_EAT_EPIC)
        return GetInvalidProductUserId();

    for (auto const& user : m_users)
    {
        if (user.second.infos.userid() == Options->TargetExternalUserId)
        {
            return user.first;
        }
    }

    return GetInvalidProductUserId();
}

EOS_EResult EOSSDK_Connect::GetProductUserIdMapping(const EOS_Connect_GetProductUserIdMappingOptions* Options, char* OutBuffer, int32_t* InOutBufferLength)
{
    TRACE_FUNC();

    if(Options == nullptr || Options->TargetProductUserId == nullptr || InOutBufferLength == nullptr)
        return EOS_EResult::EOS_InvalidParameters;

    if (OutBuffer != nullptr)
        *OutBuffer = 0;

    if (Options->AccountIdType != EOS_EExternalAccountType::EOS_EAT_EPIC)
    {
        *InOutBufferLength = 1;
        return EOS_EResult::EOS_NotFound;
    }

    if (*InOutBufferLength < (Options->TargetProductUserId->to_string().length() + 1))
    {
        *InOutBufferLength = static_cast<int32_t>(Options->TargetProductUserId->to_string().length() + 1);
        return EOS_EResult::EOS_LimitExceeded;
    }

    strncpy(OutBuffer, Options->TargetProductUserId->to_string().c_str(), Options->TargetProductUserId->to_string().length() + 1);

    return EOS_EResult::EOS_Success;
}

int32_t EOSSDK_Connect::GetLoggedInUsersCount()
{
    TRACE_FUNC();

    return 1;
}

EOS_ProductUserId EOSSDK_Connect::GetLoggedInUserByIndex(int32_t Index)
{
    TRACE_FUNC();

    if (Index == 0)
        return Settings::Inst().productuserid;

    return GetInvalidProductUserId();
}

EOS_ELoginStatus EOSSDK_Connect::GetLoginStatus(EOS_ProductUserId LocalUserId)
{
    TRACE_FUNC();

    if (LocalUserId == Settings::Inst().productuserid)
        return (get_myself()->second.connected ? EOS_ELoginStatus::EOS_LS_LoggedIn : EOS_ELoginStatus::EOS_LS_NotLoggedIn);

    return EOS_ELoginStatus::EOS_LS_NotLoggedIn;
}

EOS_NotificationId EOSSDK_Connect::AddNotifyAuthExpiration(const EOS_Connect_AddNotifyAuthExpirationOptions* Options, void* ClientData, const EOS_Connect_OnAuthExpirationCallback Notification)
{
    TRACE_FUNC();
    lock _(GLOBAL_MUTEX);

    if (Notification == nullptr)
        return EOS_INVALID_NOTIFICATIONID;

    pFrameResult_t res(new FrameResult);

    EOS_Connect_AuthExpirationCallbackInfo& aeci = res->CreateCallback<EOS_Connect_AuthExpirationCallbackInfo>((CallbackFunc)Notification);

    aeci.ClientData = ClientData;
    aeci.LocalUserId = Settings::Inst().productuserid;

    return GetCB_Manager().add_notification(this, res);
}

void EOSSDK_Connect::RemoveNotifyAuthExpiration(EOS_NotificationId InId)
{
    TRACE_FUNC();
    lock _(GLOBAL_MUTEX);

    GetCB_Manager().remove_notification(this, InId);
}

EOS_NotificationId EOSSDK_Connect::AddNotifyLoginStatusChanged(const EOS_Connect_AddNotifyLoginStatusChangedOptions* Options, void* ClientData, const EOS_Connect_OnLoginStatusChangedCallback Notification)
{
    TRACE_FUNC();
    lock _(GLOBAL_MUTEX);

    if (Notification == nullptr)
        return EOS_INVALID_NOTIFICATIONID;

    pFrameResult_t res(new FrameResult);

    EOS_Connect_LoginStatusChangedCallbackInfo& lscci = res->CreateCallback<EOS_Connect_LoginStatusChangedCallbackInfo>((CallbackFunc)Notification);

    lscci.ClientData = ClientData;
    lscci.PreviousStatus = EOS_ELoginStatus::EOS_LS_NotLoggedIn;
    lscci.CurrentStatus  = EOS_ELoginStatus::EOS_LS_NotLoggedIn;
    lscci.LocalUserId = Settings::Inst().productuserid;

    return GetCB_Manager().add_notification(this, res);
}

void EOSSDK_Connect::RemoveNotifyLoginStatusChanged(EOS_NotificationId InId)
{
    TRACE_FUNC();
    lock _(GLOBAL_MUTEX);

    GetCB_Manager().remove_notification(this, InId);
}

uint32_t EOSSDK_Connect::GetProductUserExternalAccountCount(const EOS_Connect_GetProductUserExternalAccountCountOptions* Options)
{
    TRACE_FUNC();
    lock _(GLOBAL_MUTEX);

    return 0;
}

EOS_EResult EOSSDK_Connect::CopyProductUserExternalAccountByIndex(const EOS_Connect_CopyProductUserExternalAccountByIndexOptions* Options, EOS_Connect_ExternalAccountInfo** OutExternalAccountInfo)
{
    TRACE_FUNC();
    lock _(GLOBAL_MUTEX);

    return EOS_EResult::EOS_NotFound;
}

EOS_EResult EOSSDK_Connect::CopyProductUserExternalAccountByAccountType(const EOS_Connect_CopyProductUserExternalAccountByAccountTypeOptions* Options, EOS_Connect_ExternalAccountInfo** OutExternalAccountInfo)
{
    TRACE_FUNC();
    lock _(GLOBAL_MUTEX);

    return EOS_EResult::EOS_NotFound;
}

EOS_EResult EOSSDK_Connect::CopyProductUserExternalAccountByAccountId(const EOS_Connect_CopyProductUserExternalAccountByAccountIdOptions* Options, EOS_Connect_ExternalAccountInfo** OutExternalAccountInfo)
{
    TRACE_FUNC();
    lock _(GLOBAL_MUTEX);

    return EOS_EResult::EOS_NotFound;
}

EOS_EResult EOSSDK_Connect::CopyProductUserInfo(const EOS_Connect_CopyProductUserInfoOptions* Options, EOS_Connect_ExternalAccountInfo** OutExternalAccountInfo)
{
    TRACE_FUNC();
    lock _(GLOBAL_MUTEX);

    if (Options == nullptr || OutExternalAccountInfo == nullptr)
        return EOS_EResult::EOS_InvalidParameters;

    if (Options->TargetUserId == nullptr)
        return EOS_EResult::EOS_InvalidUser;

    auto it = m_users.find(Options->TargetUserId);
    if (it == m_users.end())
        return EOS_EResult::EOS_NotFound;

    EOS_Connect_ExternalAccountInfo* info = new EOS_Connect_ExternalAccountInfo;
    info->ApiVersion = EOS_CONNECT_EXTERNALACCOUNTINFO_API_LATEST;
    info->ProductUserId = Options->TargetUserId;

    string display_name;
    string account_id_str;
    if (Options->TargetUserId == Settings::Inst().productuserid) {
        display_name = Settings::Inst().account_name;
        account_id_str = Settings::Inst().userid->to_string();
    } else {
        display_name = it->second.infos.displayname();
        account_id_str = it->second.infos.userid();
    }

    if (display_name.empty()) display_name = "Player";
    if (account_id_str.empty()) account_id_str = "user";

    char* c_disp = new char[display_name.length() + 1];
    strncpy(c_disp, display_name.c_str(), display_name.length() + 1);
    info->DisplayName = c_disp;

    char* c_acc = new char[account_id_str.length() + 1];
    strncpy(c_acc, account_id_str.c_str(), account_id_str.length() + 1);
    info->AccountId = c_acc;

    info->AccountIdType = EOS_EExternalAccountType::EOS_EAT_EPIC;
    info->LastLoginTime = 0;

    *OutExternalAccountInfo = info;

    return EOS_EResult::EOS_Success;
}

EOS_EResult EOSSDK_Connect::CopyIdToken(const EOS_Connect_CopyIdTokenOptions* Options, EOS_Connect_IdToken** OutIdToken)
{
    TRACE_FUNC();
    lock _(GLOBAL_MUTEX);

    if (Options == nullptr || OutIdToken == nullptr)
        return EOS_EResult::EOS_InvalidParameters;

    if (Options->LocalUserId != Settings::Inst().productuserid)
        return EOS_EResult::EOS_InvalidUser;

    EOS_Connect_IdToken* token = new EOS_Connect_IdToken;
    token->ApiVersion = EOS_CONNECT_IDTOKEN_API_LATEST;
    token->ProductUserId = Settings::Inst().productuserid;

    string jwt = jwt_utils::make_jwt(Settings::Inst().account_id, Settings::Inst().product_user_id);
    
    char* str = new char[jwt.length() + 1];
    strncpy(str, jwt.c_str(), jwt.length() + 1);
    token->JsonWebToken = str;

    *OutIdToken = token;
    return EOS_EResult::EOS_Success;
}

bool EOSSDK_Connect::send_connect_infos_request(Network::peer_t const& peerid, Connect_Request_Info_pb* req)
{
    
    string const& user_id = Settings::Inst().productuserid->to_string();

    Network_Message_pb msg;
    Connect_Message_pb* conn = new Connect_Message_pb;

    conn->set_allocated_request(req);
    msg.set_allocated_connect(conn);

    msg.set_source_id(user_id);
    msg.set_dest_id(peerid);
    msg.set_game_id(Settings::Inst().appid);

    return GetNetwork().TCPSendTo(msg);
}

bool EOSSDK_Connect::send_connect_infos(Network::peer_t const& peerid, Connect_Infos_pb* infos)
{
    
    string const& user_id = Settings::Inst().productuserid->to_string();

    Network_Message_pb msg;
    Connect_Message_pb* conn = new Connect_Message_pb;

    conn->set_allocated_infos(infos);
    msg.set_allocated_connect(conn);

    msg.set_source_id(user_id);
    msg.set_dest_id(peerid);
    msg.set_game_id(Settings::Inst().appid);

    return GetNetwork().TCPSendTo(msg);
}

bool EOSSDK_Connect::on_peer_connect(Network_Message_pb const& msg, Network_Peer_Connect_pb const& peer)
{
    TRACE_FUNC();
    lock _(GLOBAL_MUTEX);

    EOS_ProductUserId product_id = GetProductUserId(msg.source_id());
    auto& user = m_users[product_id];
    user.connected = true;
    user.infos = Connect_Infos_pb{};
    user.last_infos = time_pt{};

    return true;
}

bool EOSSDK_Connect::on_peer_disconnect(Network_Message_pb const& msg, Network_Peer_Disconnect_pb const& peer)
{
    TRACE_FUNC();
    lock _(GLOBAL_MUTEX);

    GetEOS_Presence().on_peer_disconnect(msg, peer);
    GetEOS_Lobby().on_peer_disconnect(msg, peer);
    GetEOS_Sessions().on_peer_disconnect(msg, peer);
    GetEOS_P2P().on_peer_disconnect(msg, peer);

    EOS_ProductUserId product_id = GetProductUserId(msg.source_id());
    m_users[product_id].connected = false;
    m_users[product_id].authentified = false;

    return true;
}

bool EOSSDK_Connect::on_connect_infos_request(Network_Message_pb const& msg, Connect_Request_Info_pb const& req)
{
    
    lock _(GLOBAL_MUTEX);

    Connect_Infos_pb* infos = new Connect_Infos_pb;

    infos->set_userid(Settings::Inst().userid->to_string());

    return send_connect_infos(msg.source_id(), infos);
}

bool EOSSDK_Connect::on_connect_infos(Network_Message_pb const& msg, Connect_Infos_pb const& infos)
{
    
    lock _(GLOBAL_MUTEX);

    auto& user = m_users[GetProductUserId(msg.source_id())];

    if (user.connected)
    {
        user.infos = infos;
        user.last_infos = clock::now();
        if (!user.authentified)
        {
            user.authentified = true;

            Network_Peer_Connect_pb connect;
            GetEOS_Presence().on_peer_connect(msg, connect);
            GetEOS_P2P().on_peer_connect(msg, connect);
        }

        std::vector<pFrameResult_t> notifs = std::move(GetCB_Manager().get_notifications(&GetEOS_Friends(), EOS_Friends_OnFriendsUpdateInfo::k_iCallback));
        for (auto& notif : notifs)
        {
            EOS_Friends_OnFriendsUpdateInfo& ofui = notif->GetCallback<EOS_Friends_OnFriendsUpdateInfo>();
            ofui.TargetUserId = GetEpicUserId(user.infos.userid());
            notif->GetFunc()(notif->GetFuncParam());
        }
    }

    return true;
}

bool EOSSDK_Connect::CBRunFrame()
{
    lock _(GLOBAL_MUTEX);

    if (!get_myself()->second.connected)
        return true;

    auto now = clock::now();

    auto user_it = m_users.begin();
    ++user_it;
    for (; user_it != m_users.end(); ++user_it)
    {
        if (!user_it->second.connected)
            continue;

        if ((now - user_it->second.last_infos) > user_infos_rate)
        {
            Connect_Request_Info_pb* req = new Connect_Request_Info_pb;
            send_connect_infos_request(user_it->first->to_string(), req);
            user_it->second.last_infos = now;
        }
    }

    return true;
}

bool EOSSDK_Connect::RunNetwork(Network_Message_pb const& msg)
{
    if (GetProductUserId(msg.source_id()) == Settings::Inst().productuserid)
        return true;

    switch (msg.messages_case())
    {
        case Network_Message_pb::MessagesCase::kNetworkAdvertise:
        {
            Network_Advertise_pb const& adv = msg.network_advertise();
            switch (adv.message_case())
            {
                case Network_Advertise_pb::MessageCase::kPeerConnect   : return on_peer_connect(msg, adv.peer_connect());
                case Network_Advertise_pb::MessageCase::kPeerDisconnect: return on_peer_disconnect(msg, adv.peer_disconnect());
            }
        }
        break;

        case Network_Message_pb::MessagesCase::kConnect:
        {
            Connect_Message_pb const& conn = msg.connect();
            switch (conn.message_case())
            {
                case Connect_Message_pb::MessageCase::kRequest  : return on_connect_infos_request(msg, conn.request());
                case Connect_Message_pb::MessageCase::kInfos    : return on_connect_infos(msg, conn.infos());
                default: APP_LOG(Log::LogLevel::WARN, "Unhandled network message %d", conn.message_case());
            }
        }
        break;
    }

    return true;
}

bool EOSSDK_Connect::RunCallbacks(pFrameResult_t res)
{
    lock _(GLOBAL_MUTEX);

    return res->done;
}

void EOSSDK_Connect::FreeCallback(pFrameResult_t res)
{
    lock _(GLOBAL_MUTEX);

}

}