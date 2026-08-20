#include "eossdk_lobby.h"
#include "eossdk_platform.h"
#include "eos_client_api.h"
#include "settings.h"

namespace sdk
{

decltype(EOSSDK_Lobby::join_timeout) EOSSDK_Lobby::join_timeout;

decltype(EOSSDK_Lobby::join_id) EOSSDK_Lobby::join_id(0);

EOSSDK_Lobby::EOSSDK_Lobby()
{
    GetCB_Manager().register_frame(this);
    GetCB_Manager().register_callbacks(this);
    GetNetwork().register_listener(this, 0, Network_Message_pb::MessagesCase::kLobby);
    GetNetwork().register_listener(this, 0, Network_Message_pb::MessagesCase::kLobbiesSearch);
}

EOSSDK_Lobby::~EOSSDK_Lobby()
{
    GetNetwork().unregister_listener(this, 0, Network_Message_pb::MessagesCase::kLobbiesSearch);
    GetNetwork().unregister_listener(this, 0, Network_Message_pb::MessagesCase::kLobby);
    GetCB_Manager().unregister_callbacks(this);
    GetCB_Manager().unregister_frame(this);

    GetCB_Manager().remove_all_notifications(this);
}

lobby_state_t* EOSSDK_Lobby::get_lobby_by_id(string const& lobby_id)
{
    auto it = m_lobbies.find(lobby_id);
    if (it == m_lobbies.end())
        return nullptr;

    return &(it->second);
}

template<typename T>
bool compare_attribute_values(T&& v1, EOS_EOnlineComparisonOp op, T&& v2, string const& attr_name)
{
    bool res = false;
    try
    {
        switch (op)
        {
            case EOS_EOnlineComparisonOp::EOS_CO_EQUAL             : res = v1 == v2; break;
            case EOS_EOnlineComparisonOp::EOS_CO_NOTEQUAL          : res = v1 != v2; break;
            case EOS_EOnlineComparisonOp::EOS_CO_GREATERTHAN       : res = v1 >  v2; break;
            case EOS_EOnlineComparisonOp::EOS_CO_GREATERTHANOREQUAL: res = v1 >= v2; break;
            case EOS_EOnlineComparisonOp::EOS_CO_LESSTHAN          : res = v1 <  v2; break;
            case EOS_EOnlineComparisonOp::EOS_CO_LESSTHANOREQUAL   : res = v1 <= v2; break;
            default: res = true;
        }
    }
    catch (...)
    {}

    auto to_str = [](auto const& v) {
        if constexpr (std::is_same_v<std::decay_t<decltype(v)>, string>)
            return v;
        else
            return std::to_string(v);
    };

    APP_LOG(Log::LogLevel::DEBUG, "Testing Lobby Attr: %s: (lobby)%s %s (search)%s, result: %s", attr_name.c_str(), to_str(v1).c_str(), search_attr_to_string(op), to_str(v2).c_str(), res ? "true" : "false");
    return res;
}

std::vector<lobby_state_t*> EOSSDK_Lobby::get_lobbies_from_attributes(std::map<string, Lobby_Search_Parameter> const& parameters)
{
    std::vector<lobby_state_t*> res;
    for (auto& lobby : m_lobbies)
    {
        bool found = true;
        for (auto& param : parameters)
        {
            
            string key = param.first;
            if (key == EOS_LOBBY_SEARCH_MINCURRENTMEMBERS)
            {
                auto it = param.second.param().find(static_cast<int>(EOS_EOnlineComparisonOp::EOS_CO_GREATERTHANOREQUAL));
                if (it != param.second.param().end())
                {
                    switch(it->second.value_case())
                    {
                        case Session_Attr_Value::ValueCase::kI:
                        {
                            int64_t lobby_current_members = lobby.second.infos.members_size();
                            int64_t min_current_members = it->second.i();
                            found = compare_attribute_values(lobby_current_members, EOS_EOnlineComparisonOp::EOS_CO_GREATERTHANOREQUAL, min_current_members, param.first);
                        }
                        break;

                        default:
                        {
                            APP_LOG(Log::LogLevel::INFO, "Triied " EOS_LOBBY_SEARCH_MINCURRENTMEMBERS " with a comparator different than EOS_CO_GREATERTHANOREQUAL: FIX ME!");
                            found = false;
                        }
                    }
                }
            }
            else if (key == EOS_LOBBY_SEARCH_MINSLOTSAVAILABLE)
            {
                auto it = param.second.param().find(static_cast<int>(EOS_EOnlineComparisonOp::EOS_CO_GREATERTHANOREQUAL));
                if (it != param.second.param().end())
                {
                    switch(it->second.value_case())
                    {
                        case Session_Attr_Value::ValueCase::kI:
                        {
                            int64_t lobby_slots_available = static_cast<int64_t>(lobby.second.infos.max_lobby_member()) - lobby.second.infos.members_size();
                            int64_t min_slots_available = it->second.i();
                            found = compare_attribute_values(lobby_slots_available, EOS_EOnlineComparisonOp::EOS_CO_GREATERTHANOREQUAL, min_slots_available, param.first);
                        }
                        break;

                        default:
                        {
                            APP_LOG(Log::LogLevel::INFO, "Triied " EOS_LOBBY_SEARCH_MINSLOTSAVAILABLE " with a comparator different than EOS_CO_GREATERTHANOREQUAL: FIX ME!");
                            found = false;
                        }
                    }
                }
            }
            else
            {
                auto it = lobby.second.infos.attributes().find(param.first);
                if (it == lobby.second.infos.attributes().end())
                {
                    found = false;
                }
                else
                {
                    for (auto& comparisons : param.second.param())
                    {
                        
                        if (comparisons.second.value_case() != it->second.value().value_case())
                        {
                            found = false;
                            break;
                        }

                        EOS_EOnlineComparisonOp comp = static_cast<EOS_EOnlineComparisonOp>(comparisons.first);

                        switch (comparisons.second.value_case())
                        {
                            case Session_Attr_Value::ValueCase::kB:
                            {
                                bool b_session = it->second.value().b();
                                bool b_search = comparisons.second.b();
                                found = compare_attribute_values(b_session, comp, b_search, param.first);
                            }
                            break;
                            case Session_Attr_Value::ValueCase::kI:
                            {
                                int64_t i_lobby = it->second.value().i();
                                int64_t i_search = comparisons.second.i();
                                found = compare_attribute_values(i_lobby, comp, i_search, param.first);
                            }
                            break;
                            case Session_Attr_Value::ValueCase::kD:
                            {
                                double d_lobby = it->second.value().d();
                                double d_search = comparisons.second.d();
                                found = compare_attribute_values(d_lobby, comp, d_search, param.first);
                            }
                            break;
                            case Session_Attr_Value::ValueCase::kS:
                            {
                                string s_lobby = it->second.value().s();
                                string s_search = comparisons.second.s();
                                found = compare_attribute_values(s_lobby, comp, s_search, param.first);
                            }
                            break;
                        }
                    }
                }
            }
            if (found == false)
            {
                APP_LOG(Log::LogLevel::DEBUG, "This lobby didn't match: %s", lobby.second.infos.lobby_id().c_str());
                break;
            }
        }

        if (found)
        {
            res.emplace_back(&lobby.second);
        }
    }

    return res;
}

bool EOSSDK_Lobby::add_member_to_lobby(string const& member, lobby_state_t* lobby)
{
    APP_LOG(Log::LogLevel::TRACE, "");
    assert(lobby != nullptr);

    auto& members = *lobby->infos.mutable_members();
    auto it = members.find(member);
    if (it != members.end())
    {
        return false;
    }

    (*lobby->infos.mutable_members())[member];
    return true;
}

bool EOSSDK_Lobby::remove_member_from_lobby(string const& member, lobby_state_t* lobby)
{
    APP_LOG(Log::LogLevel::TRACE, "");
    assert(lobby != nullptr);

    auto& members = *lobby->infos.mutable_members();
    auto it = members.find(member);
    if (it != members.end())
    {
        members.erase(it);
        return true;
    }

    return false;
}

bool EOSSDK_Lobby::is_member_in_lobby(string const& member, lobby_state_t* lobby)
{
    assert(lobby != nullptr);

    auto& members = lobby->infos.members();
    return (members.find(member) != members.end());
}

bool EOSSDK_Lobby::i_am_owner(lobby_state_t* lobby)
{
    assert(lobby != nullptr);
    
    return (GetProductUserId(lobby->infos.owner_id()) == GetEOS_Connect().get_myself()->first);    
}

void EOSSDK_Lobby::notify_lobby_update(lobby_state_t* lobby)
{
    assert(lobby != nullptr);

    std::vector<pFrameResult_t> notifs(std::move(GetCB_Manager().get_notifications(this, EOS_Lobby_LobbyUpdateReceivedCallbackInfo::k_iCallback)));
    for (auto& notif : notifs)
    {
        EOS_Lobby_LobbyUpdateReceivedCallbackInfo& lurci = notif->GetCallback<EOS_Lobby_LobbyUpdateReceivedCallbackInfo>();
        strncpy(const_cast<char*>(lurci.LobbyId), lobby->infos.lobby_id().c_str(), max_accountid_length);

        notif->GetFunc()(notif->GetFuncParam());
    }
}

void EOSSDK_Lobby::notify_lobby_member_status_update(string const& member, EOS_ELobbyMemberStatus new_status, lobby_state_t* lobby)
{
    assert(lobby != nullptr);

    EOS_ProductUserId member_id = GetProductUserId(member);
    std::vector<pFrameResult_t> notifs(std::move(GetCB_Manager().get_notifications(this, EOS_Lobby_LobbyMemberStatusReceivedCallbackInfo::k_iCallback)));
    for (auto& notif : notifs)
    {
        EOS_Lobby_LobbyMemberStatusReceivedCallbackInfo& lmsrci = notif->GetCallback<EOS_Lobby_LobbyMemberStatusReceivedCallbackInfo>();
        strncpy(const_cast<char*>(lmsrci.LobbyId), lobby->infos.lobby_id().c_str(), max_accountid_length);
        lmsrci.TargetUserId = member_id;
        lmsrci.CurrentStatus = new_status;

        notif->GetFunc()(notif->GetFuncParam());
    }
}

void EOSSDK_Lobby::notify_lobby_member_update(string const& member, lobby_state_t* lobby)
{
    assert(lobby != nullptr);

    EOS_ProductUserId member_id = GetProductUserId(member);
    std::vector<pFrameResult_t> notifs(std::move(GetCB_Manager().get_notifications(this, EOS_Lobby_LobbyMemberUpdateReceivedCallbackInfo::k_iCallback)));
    for (auto& notif : notifs)
    {
        EOS_Lobby_LobbyMemberUpdateReceivedCallbackInfo& lmurci = notif->GetCallback<EOS_Lobby_LobbyMemberUpdateReceivedCallbackInfo>();
        strncpy(const_cast<char*>(lmurci.LobbyId), lobby->infos.lobby_id().c_str(), max_accountid_length);
        lmurci.TargetUserId = member_id;
        notif->GetFunc()(notif->GetFuncParam());
    }
}

void EOSSDK_Lobby::notify_lobby_invite_received(string const& invite_id, EOS_ProductUserId from_id)
{
    std::vector<pFrameResult_t> notifs(std::move(GetCB_Manager().get_notifications(this, EOS_Lobby_LobbyInviteReceivedCallbackInfo::k_iCallback)));
    for (auto& notif : notifs)
    {
        EOS_Lobby_LobbyInviteReceivedCallbackInfo& lirci = notif->GetCallback<EOS_Lobby_LobbyInviteReceivedCallbackInfo>();
        strncpy(const_cast<char*>(lirci.InviteId), invite_id.c_str(), max_accountid_length);
        lirci.TargetUserId = from_id;
        notif->GetFunc()(notif->GetFuncParam());
    }
}

void EOSSDK_Lobby::CreateLobby(const EOS_Lobby_CreateLobbyOptions* Options, void* ClientData, const EOS_Lobby_OnCreateLobbyCallback CompletionDelegate)
{
    TRACE_FUNC();

    SE_CB_BEGIN(EOS_Lobby_CreateLobbyCallbackInfo, CompletionDelegate);
    
    {
        char* str = new char[sdk::max_accountid_length];
        *str = '\0';
        info.LobbyId = str;
    }

    if (Options == nullptr || Options->MaxLobbyMembers < 1 || Options->MaxLobbyMembers > EOS_LOBBY_MAX_LOBBY_MEMBERS)
    {
        info.ResultCode = EOS_EResult::EOS_InvalidParameters;
    }
    else if (m_lobbies.size() >= EOS_LOBBY_MAX_LOBBIES)
    {
        info.ResultCode = EOS_EResult::EOS_LimitExceeded;
    }
    else
    {
        string lobby_id(std::move(generate_account_id()));

        strncpy(const_cast<char*>(info.LobbyId), lobby_id.c_str(), max_accountid_length);
        const_cast<char*>(info.LobbyId)[64] = 0;

        auto& infos = m_lobbies[lobby_id];
        infos.infos.set_lobby_id(lobby_id);
        infos.infos.set_owner_id(GetEOS_Connect().get_myself()->first->to_string());
        infos.infos.set_max_lobby_member(Options->MaxLobbyMembers);
        infos.infos.set_permission_level(static_cast<int>(Options->PermissionLevel));
        (*infos.infos.mutable_members())[GetEOS_Connect().get_myself()->first->to_string()];
        infos.state = lobby_state_t::created;

        info.ResultCode = EOS_EResult::EOS_Success;
    }

    SE_CB_END(this);
}

void EOSSDK_Lobby::DestroyLobby(const EOS_Lobby_DestroyLobbyOptions* Options, void* ClientData, const EOS_Lobby_OnDestroyLobbyCallback CompletionDelegate)
{
    TRACE_FUNC();
    APP_LOG(Log::LogLevel::INFO, "TODO");

    SE_CB_BEGIN(EOS_Lobby_DestroyLobbyCallbackInfo, CompletionDelegate);
    
    {
        char* str;
        if (Options->LobbyId == nullptr)
        {
            str = new char[1];
            *str = '\0';
        }
        else
        {
            size_t len = strlen(Options->LobbyId) + 1;
            str = new char[len];
            strncpy(str, Options->LobbyId, len);
        }
        info.LobbyId = str;
    }

    if (Options == nullptr || Options->LobbyId == nullptr)
    {
        info.ResultCode = EOS_EResult::EOS_InvalidParameters;
    }
    else
    {
        lobby_state_t* pLobby = get_lobby_by_id(Options->LobbyId);

        info.ResultCode = EOS_EResult::EOS_NotFound;
    }

    SE_CB_END(this);
}

void EOSSDK_Lobby::JoinLobby(const EOS_Lobby_JoinLobbyOptions* Options, void* ClientData, const EOS_Lobby_OnJoinLobbyCallback CompletionDelegate)
{
    TRACE_FUNC();

    if (CompletionDelegate == nullptr)
        return;

    pFrameResult_t res(new FrameResult);
    EOS_Lobby_JoinLobbyCallbackInfo& jlci = res->CreateCallback<EOS_Lobby_JoinLobbyCallbackInfo>((CallbackFunc)CompletionDelegate);

    EOSSDK_LobbyDetails* pLobbyDetails = reinterpret_cast<EOSSDK_LobbyDetails*>(Options->LobbyDetailsHandle);

    jlci.ClientData = ClientData;
    
    {
        char* str;
        if (pLobbyDetails == nullptr)
        {
            str = new char[1];
            *str = '\0';
        }
        else
        {
            size_t len = pLobbyDetails->m_infos.lobby_id().length() + 1;
            str = new char[len];
            strncpy(str, pLobbyDetails->m_infos.lobby_id().c_str(), len);
        }
        jlci.LobbyId = str;
    }

    if (Options == nullptr || Options->LobbyDetailsHandle == nullptr)
    {
        jlci.ResultCode = EOS_EResult::EOS_InvalidParameters;
        res->done = true;
    }
    else if (m_lobbies.size() >= EOS_LOBBY_MAX_LOBBIES)
    {
        jlci.ResultCode = EOS_EResult::EOS_LimitExceeded;
        res->done = true;
    }
    else
    {
        Lobby_Join_Request_pb* request = new Lobby_Join_Request_pb;
        request->set_lobby_id(pLobbyDetails->m_infos.lobby_id());
        m_joins_requests[join_id].cb = res;
        request->set_join_id(join_id++);

        send_lobby_join_request(pLobbyDetails->m_infos.owner_id(), request);
        jlci.ResultCode = EOS_EResult::EOS_TimedOut;
    }

    GetCB_Manager().add_callback(this, res);
}

void EOSSDK_Lobby::LeaveLobby(const EOS_Lobby_LeaveLobbyOptions* Options, void* ClientData, const EOS_Lobby_OnLeaveLobbyCallback CompletionDelegate)
{
    TRACE_FUNC();

    SE_CB_BEGIN(EOS_Lobby_LeaveLobbyCallbackInfo, CompletionDelegate);
    
    {
        char* str;
        if (Options->LobbyId == nullptr)
        {
            str = new char[1];
            *str = '\0';
        }
        else
        {
            size_t len = strlen(Options->LobbyId) + 1;
            str = new char[len];
            strncpy(str, Options->LobbyId, len);
        }
        info.LobbyId = str;
    }

    if (Options == nullptr || Options->LobbyId == nullptr)
    {
        info.ResultCode = EOS_EResult::EOS_InvalidParameters;
    }
    else
    {
        auto it = m_lobbies.find(Options->LobbyId);
        if (it != m_lobbies.end())
        {
            
            send_lobby_member_leave(GetEOS_Connect().get_myself()->first->to_string(), &it->second, EOS_ELobbyMemberStatus::EOS_LMS_LEFT);
            info.ResultCode = EOS_EResult::EOS_Success;
            m_lobbies.erase(it);
        }
        else
        {
            info.ResultCode = EOS_EResult::EOS_NotFound;
        }
    }

    SE_CB_END(this);
}

EOS_EResult EOSSDK_Lobby::UpdateLobbyModification(const EOS_Lobby_UpdateLobbyModificationOptions* Options, EOS_HLobbyModification* OutLobbyModificationHandle)
{
    TRACE_FUNC();

    if (Options == nullptr || OutLobbyModificationHandle == nullptr)
    {
        set_nullptr(OutLobbyModificationHandle);
        return EOS_EResult::EOS_InvalidParameters;
    }

    auto lobby_it = m_lobbies.find(Options->LobbyId);
    if (lobby_it == m_lobbies.end())
    {
        *OutLobbyModificationHandle = nullptr;
        return EOS_EResult::EOS_InvalidParameters;
    }

    EOSSDK_LobbyModification* pLobbyModif = new EOSSDK_LobbyModification;
    pLobbyModif->m_infos = lobby_it->second.infos;

    *OutLobbyModificationHandle = reinterpret_cast<EOS_HLobbyModification>(pLobbyModif);

    return EOS_EResult::EOS_Success;
}

void EOSSDK_Lobby::UpdateLobby(const EOS_Lobby_UpdateLobbyOptions* Options, void* ClientData, const EOS_Lobby_OnUpdateLobbyCallback CompletionDelegate)
{
    TRACE_FUNC();

    if (CompletionDelegate == nullptr)
        return;

    pFrameResult_t res(new FrameResult);
    EOS_Lobby_UpdateLobbyCallbackInfo& ulci = res->CreateCallback<EOS_Lobby_UpdateLobbyCallbackInfo>((CallbackFunc)CompletionDelegate);

    EOSSDK_LobbyModification* pLobbyModif = reinterpret_cast<EOSSDK_LobbyModification*>(Options->LobbyModificationHandle);
    
    ulci.ClientData = ClientData;

    {
        char* str;
        if (pLobbyModif == nullptr)
        {
            str = new char[1];
            *str = '\0';
        }
        else
        {
            size_t len = pLobbyModif->m_infos.lobby_id().length() + 1;
            str = new char[len];
            strncpy(str, pLobbyModif->m_infos.lobby_id().c_str(), len);
        }
        ulci.LobbyId = str;
    }

    if (Options == nullptr || Options->LobbyModificationHandle == nullptr)
    {
        ulci.ResultCode = EOS_EResult::EOS_InvalidParameters;
    }
    else
    {
        lobby_state_t* pLobby = get_lobby_by_id(pLobbyModif->m_infos.lobby_id());
        if(pLobby == nullptr)
        {
            ulci.ResultCode = EOS_EResult::EOS_NotFound;
        }
        else
        {
            if (pLobbyModif->m_lobby_modified)
            {
                if (i_am_owner(pLobby))
                {
                    pLobby->infos = pLobbyModif->m_infos;
                    ulci.ResultCode = EOS_EResult::EOS_Success;
                    send_lobby_update(pLobby);

                    if (pLobbyModif->m_member_modified)
                    {
                        send_lobby_member_update(GetEOS_Connect().get_myself()->first->to_string(), pLobby);
                    }
                }
                else
                {
                    ulci.ResultCode = EOS_EResult::EOS_Lobby_NotOwner;
                }
            }
            else if (pLobbyModif->m_member_modified)
            {
                ulci.ResultCode = EOS_EResult::EOS_Success;

                *pLobby->infos.mutable_members() = pLobbyModif->m_infos.members();
                send_lobby_member_update(GetEOS_Connect().get_myself()->first->to_string(), pLobby);
            }
        }
    }

    SE_CB_END(this);
}

void EOSSDK_Lobby::PromoteMember(const EOS_Lobby_PromoteMemberOptions* Options, void* ClientData, const EOS_Lobby_OnPromoteMemberCallback CompletionDelegate)
{
    TRACE_FUNC();

    SE_CB_BEGIN(EOS_Lobby_PromoteMemberCallbackInfo, CompletionDelegate);
    {
        char* str;
        if (Options->LobbyId == nullptr)
        {
            str = new char[1];
            *str = '\0';
        }
        else
        {
            size_t len = strlen(Options->LobbyId) + 1;
            str = new char[len];
            strncpy(str, Options->LobbyId, len);
        }
        info.LobbyId = str;
    }

    if (Options == nullptr || Options->LobbyId == nullptr || Options->TargetUserId == nullptr)
    {
        info.ResultCode = EOS_EResult::EOS_InvalidParameters;
    }
    else
    {
        lobby_state_t* pLobby = get_lobby_by_id(Options->LobbyId);
        if (pLobby != nullptr)
        {
            if (i_am_owner(pLobby))
            {
                if (is_member_in_lobby(Options->TargetUserId->to_string(), pLobby))
                {
                    send_lobby_member_promote(Options->TargetUserId->to_string(), pLobby);
                    pLobby->infos.set_owner_id(Options->TargetUserId->to_string());

                    notify_lobby_member_status_update(Options->TargetUserId->to_string(), EOS_ELobbyMemberStatus::EOS_LMS_PROMOTED, pLobby);
                }
                else
                {
                    info.ResultCode = EOS_EResult::EOS_NotFound;
                }
            }
            else
            {
                info.ResultCode = EOS_EResult::EOS_Lobby_NotOwner;
            }
        }
        else
        {
            info.ResultCode = EOS_EResult::EOS_NotFound;
        }
    }

    SE_CB_END(this);
}

void EOSSDK_Lobby::KickMember(const EOS_Lobby_KickMemberOptions* Options, void* ClientData, const EOS_Lobby_OnKickMemberCallback CompletionDelegate)
{
    TRACE_FUNC();

    SE_CB_BEGIN(EOS_Lobby_KickMemberCallbackInfo, CompletionDelegate);
    {
        char* str;
        if (Options->LobbyId == nullptr)
        {
            str = new char[1];
            *str = '\0';
        }
        else
        {
            size_t len = strlen(Options->LobbyId) + 1;
            str = new char[len];
            strncpy(str, Options->LobbyId, len);
        }
        info.LobbyId = str;
    }

    if (Options == nullptr || Options->LobbyId == nullptr || Options->TargetUserId == nullptr)
    {
        info.ResultCode = EOS_EResult::EOS_InvalidParameters;
    }
    else
    {
        lobby_state_t* pLobby = get_lobby_by_id(Options->LobbyId);
        if (pLobby != nullptr)
        {
            if (i_am_owner(pLobby))
            {
                if (is_member_in_lobby(Options->TargetUserId->to_string(), pLobby))
                {
                    send_lobby_member_leave(Options->TargetUserId->to_string(), pLobby, EOS_ELobbyMemberStatus::EOS_LMS_KICKED);
                    info.ResultCode = EOS_EResult::EOS_Success;
                }
                else
                {
                    info.ResultCode = EOS_EResult::EOS_NotFound;
                }
            }
            else
            {
                info.ResultCode = EOS_EResult::EOS_Lobby_NotOwner;
            }
        }
        else
        {
            info.ResultCode = EOS_EResult::EOS_NotFound;
        }
    }

    SE_CB_END(this);
}

EOS_NotificationId EOSSDK_Lobby::AddNotifyLobbyUpdateReceived(const EOS_Lobby_AddNotifyLobbyUpdateReceivedOptions* Options, void* ClientData, const EOS_Lobby_OnLobbyUpdateReceivedCallback NotificationFn)
{
    TRACE_FUNC();

    if (Options == nullptr || NotificationFn == nullptr)
        return EOS_INVALID_NOTIFICATIONID;

    pFrameResult_t res(new  FrameResult);
    EOS_Lobby_LobbyUpdateReceivedCallbackInfo& lurci = res->CreateCallback<EOS_Lobby_LobbyUpdateReceivedCallbackInfo>((CallbackFunc)NotificationFn);

    lurci.ClientData = ClientData;
    {
        char* str = new char[max_accountid_length];
        *str = '\0';
        lurci.LobbyId = str;
    }

    return GetCB_Manager().add_notification(this, res);
}

void EOSSDK_Lobby::RemoveNotifyLobbyUpdateReceived(EOS_NotificationId InId)
{
    TRACE_FUNC();

    GetCB_Manager().remove_notification(this, InId);
}

EOS_NotificationId EOSSDK_Lobby::AddNotifyLobbyMemberUpdateReceived(const EOS_Lobby_AddNotifyLobbyMemberUpdateReceivedOptions* Options, void* ClientData, const EOS_Lobby_OnLobbyMemberUpdateReceivedCallback NotificationFn)
{
    TRACE_FUNC();

    if (Options == nullptr || NotificationFn == nullptr)
        return EOS_INVALID_NOTIFICATIONID;

    pFrameResult_t res(new  FrameResult);
    EOS_Lobby_LobbyMemberUpdateReceivedCallbackInfo& lmurci = res->CreateCallback<EOS_Lobby_LobbyMemberUpdateReceivedCallbackInfo>((CallbackFunc)NotificationFn);

    lmurci.ClientData = ClientData;
    {
        char* str = new char[max_accountid_length];
        *str = '\0';
        lmurci.LobbyId = str;
    }
    lmurci.TargetUserId = GetInvalidProductUserId();

    return GetCB_Manager().add_notification(this, res);
}

void EOSSDK_Lobby::RemoveNotifyLobbyMemberUpdateReceived(EOS_NotificationId InId)
{
    TRACE_FUNC();

    GetCB_Manager().remove_notification(this, InId);
}

EOS_NotificationId EOSSDK_Lobby::AddNotifyLobbyMemberStatusReceived(const EOS_Lobby_AddNotifyLobbyMemberStatusReceivedOptions* Options, void* ClientData, const EOS_Lobby_OnLobbyMemberStatusReceivedCallback NotificationFn)
{
    TRACE_FUNC();

    if (Options == nullptr || NotificationFn == nullptr)
        return EOS_INVALID_NOTIFICATIONID;

    pFrameResult_t res(new  FrameResult);
    EOS_Lobby_LobbyMemberStatusReceivedCallbackInfo& lmsrci = res->CreateCallback<EOS_Lobby_LobbyMemberStatusReceivedCallbackInfo>((CallbackFunc)NotificationFn);

    lmsrci.ClientData = ClientData;
    {
        char* str = new char[max_accountid_length];
        *str = '\0';
        lmsrci.LobbyId = str;
    }
    lmsrci.CurrentStatus = EOS_ELobbyMemberStatus::EOS_LMS_CLOSED;
    lmsrci.TargetUserId = GetInvalidProductUserId();

    return GetCB_Manager().add_notification(this, res);
}

void EOSSDK_Lobby::RemoveNotifyLobbyMemberStatusReceived(EOS_NotificationId InId)
{
    TRACE_FUNC();

    GetCB_Manager().remove_notification(this, InId);
}

void EOSSDK_Lobby::SendInvite(const EOS_Lobby_SendInviteOptions* Options, void* ClientData, const EOS_Lobby_OnSendInviteCallback CompletionDelegate)
{
    TRACE_FUNC();

    SE_CB_BEGIN(EOS_Lobby_SendInviteCallbackInfo, CompletionDelegate);

    {
        char* str;
        if (Options->LobbyId == nullptr)
        {
            str = new char[1];
            *str = '\0';
        }
        else
        {
            size_t len = strlen(Options->LobbyId) + 1;
            str = new char[len];
            strncpy(str, Options->LobbyId, len);
        }
        info.LobbyId = str;
    }

    if (Options == nullptr || Options->LobbyId == nullptr || Options->TargetUserId == nullptr)
    {
        info.ResultCode = EOS_EResult::EOS_InvalidParameters;
    }
    else
    {
        lobby_state_t* pLobby = get_lobby_by_id(Options->LobbyId);
        if (pLobby != nullptr && is_member_in_lobby(GetEOS_Connect().get_myself()->first->to_string(), pLobby))
        {
            Lobby_Invite_pb* invite = new Lobby_Invite_pb;

            *invite->mutable_infos() = pLobby->infos;
            send_lobby_invite(Options->TargetUserId->to_string(), invite);
        }
        else
        {
            info.ResultCode = EOS_EResult::EOS_NotFound;
        }
    }

    res->done = true;
    GetCB_Manager().add_callback(this, res);
}

void EOSSDK_Lobby::RejectInvite(const EOS_Lobby_RejectInviteOptions* Options, void* ClientData, const EOS_Lobby_OnRejectInviteCallback CompletionDelegate)
{
    TRACE_FUNC();

    SE_CB_BEGIN(EOS_Lobby_RejectInviteCallbackInfo, CompletionDelegate);

    {
        char* str;
        if (Options->InviteId == nullptr)
        {
            str = new char[1];
            *str = '\0';
        }
        else
        {
            size_t len = strlen(Options->InviteId) + 1;
            str = new char[len];
            strncpy(str, Options->InviteId, len);
        }
        info.InviteId = str;
    }
    
    if (Options == nullptr || Options->InviteId == nullptr)
    {
        info.ResultCode = EOS_EResult::EOS_InvalidParameters;
    }
    else
    {
        auto it = m_lobby_invites.find(Options->InviteId);
        if (it != m_lobby_invites.end())
        {
            m_lobby_invites.erase(it);
            info.ResultCode = EOS_EResult::EOS_Success;
        }
        else
        {
            info.ResultCode = EOS_EResult::EOS_NotFound;
        }
    }

    res->done = true;
    GetCB_Manager().add_callback(this, res);
}

void EOSSDK_Lobby::QueryInvites(const EOS_Lobby_QueryInvitesOptions* Options, void* ClientData, const EOS_Lobby_OnQueryInvitesCallback CompletionDelegate)
{
    TRACE_FUNC();

    SE_CB_BEGIN(EOS_Lobby_QueryInvitesCallbackInfo, CompletionDelegate);
    info.LocalUserId = GetEOS_Connect().get_myself()->first;

    if (Options == nullptr)
    {
        info.ResultCode = EOS_EResult::EOS_InvalidParameters;
    }
    else
    {
        info.ResultCode = EOS_EResult::EOS_Success;
    }
    
    res->done = true;
    GetCB_Manager().add_callback(this, res);
}

uint32_t EOSSDK_Lobby::GetInviteCount(const EOS_Lobby_GetInviteCountOptions* Options)
{
    TRACE_FUNC();

    if (Options == nullptr)
        return 0;

    return m_lobby_invites.size();
}

EOS_EResult EOSSDK_Lobby::GetInviteIdByIndex(const EOS_Lobby_GetInviteIdByIndexOptions* Options, char* OutBuffer, int32_t* InOutBufferLength)
{
    TRACE_FUNC();

    if (Options == nullptr || Options->Index >= m_lobby_invites.size() || InOutBufferLength == nullptr )
        return EOS_EResult::EOS_InvalidParameters;

    auto it = m_lobby_invites.begin();
    std::advance(it, Options->Index);

    if (OutBuffer != nullptr)
    {
        *InOutBufferLength = std::min<int32_t>(*InOutBufferLength, it->first.length() + 1);
        strncpy(OutBuffer, it->first.c_str(), *InOutBufferLength);
    }
    else
    {
        *InOutBufferLength = it->first.length() + 1;
    }
    
    return EOS_EResult::EOS_Success;
}

EOS_EResult EOSSDK_Lobby::CreateLobbySearch(const EOS_Lobby_CreateLobbySearchOptions* Options, EOS_HLobbySearch* OutLobbySearchHandle)
{
    TRACE_FUNC();

    if (Options == nullptr || Options->MaxResults == 0 || OutLobbySearchHandle == nullptr)
    {
        set_nullptr(OutLobbySearchHandle);
        return EOS_EResult::EOS_InvalidParameters;
    }

    m_lobbies_searchs.emplace_back();
    EOSSDK_LobbySearch*& search = m_lobbies_searchs.back();
    search = new EOSSDK_LobbySearch;
    search->m_max_results = Options->MaxResults;

    *OutLobbySearchHandle = reinterpret_cast<EOS_HLobbySearch>(search);

    return EOS_EResult::EOS_Success;
}

EOS_NotificationId EOSSDK_Lobby::AddNotifyLobbyInviteReceived(const EOS_Lobby_AddNotifyLobbyInviteReceivedOptions* Options, void* ClientData, const EOS_Lobby_OnLobbyInviteReceivedCallback NotificationFn)
{
    TRACE_FUNC();

    if (Options == nullptr || NotificationFn == nullptr)
        return EOS_INVALID_NOTIFICATIONID;

    pFrameResult_t res(new FrameResult);

    EOS_Lobby_LobbyInviteReceivedCallbackInfo& lirci = res->CreateCallback<EOS_Lobby_LobbyInviteReceivedCallbackInfo>((CallbackFunc)NotificationFn);
    lirci.ClientData = ClientData;
    lirci.LocalUserId = GetEOS_Connect().get_myself()->first;
    lirci.TargetUserId = GetInvalidProductUserId();
    {
        char *str = new char[EOS_LOBBY_INVITEID_MAX_LENGTH + 1];
        *str = '\0';
        lirci.InviteId = str;
    }

    return GetCB_Manager().add_notification(this, res);
}

void EOSSDK_Lobby::RemoveNotifyLobbyInviteReceived(EOS_NotificationId InId)
{
    TRACE_FUNC();

    GetCB_Manager().remove_notification(this, InId);
}

EOS_NotificationId EOSSDK_Lobby::AddNotifyLobbyInviteAccepted(const EOS_Lobby_AddNotifyLobbyInviteAcceptedOptions* Options, void* ClientData, const EOS_Lobby_OnLobbyInviteAcceptedCallback NotificationFn)
{
    TRACE_FUNC();

    if (Options == nullptr || NotificationFn == nullptr)
        return EOS_INVALID_NOTIFICATIONID;

    pFrameResult_t res(new FrameResult);

    EOS_Lobby_LobbyInviteAcceptedCallbackInfo& liaci = res->CreateCallback<EOS_Lobby_LobbyInviteAcceptedCallbackInfo>((CallbackFunc)NotificationFn);
    liaci.ClientData = ClientData;
    liaci.LocalUserId = GetEOS_Connect().get_myself()->first;
    liaci.TargetUserId = GetInvalidProductUserId();
    {
        char* str = new char[EOS_LOBBY_INVITEID_MAX_LENGTH + 1];
        *str = '\0';
        liaci.InviteId = str;
    }

    return GetCB_Manager().add_notification(this, res);
}

void EOSSDK_Lobby::RemoveNotifyLobbyInviteAccepted(EOS_NotificationId InId)
{
    TRACE_FUNC();

    GetCB_Manager().remove_notification(this, InId);
}

EOS_NotificationId EOSSDK_Lobby::AddNotifyJoinLobbyAccepted(const EOS_Lobby_AddNotifyJoinLobbyAcceptedOptions* Options, void* ClientData, const EOS_Lobby_OnJoinLobbyAcceptedCallback NotificationFn)
{
    TRACE_FUNC();

    if (Options == nullptr || NotificationFn == nullptr)
        return EOS_INVALID_NOTIFICATIONID;

    pFrameResult_t res(new FrameResult);

    EOS_Lobby_JoinLobbyAcceptedCallbackInfo& jlaci = res->CreateCallback<EOS_Lobby_JoinLobbyAcceptedCallbackInfo>((CallbackFunc)NotificationFn);
    jlaci.ClientData = ClientData;
    jlaci.LocalUserId = GetEOS_Connect().get_myself()->first;
    jlaci.UiEventId = EOS_UI_EVENTID_INVALID;

    return GetCB_Manager().add_notification(this, res);
}

void EOSSDK_Lobby::RemoveNotifyJoinLobbyAccepted(EOS_NotificationId InId)
{
    TRACE_FUNC();

    GetCB_Manager().remove_notification(this, InId);
}

EOS_EResult EOSSDK_Lobby::CopyLobbyDetailsHandleByInviteId(const EOS_Lobby_CopyLobbyDetailsHandleByInviteIdOptions* Options, EOS_HLobbyDetails* OutLobbyDetailsHandle)
{
    TRACE_FUNC();

    if (Options == nullptr || Options->InviteId == nullptr || OutLobbyDetailsHandle == nullptr)
    {
        set_nullptr(OutLobbyDetailsHandle);
        return EOS_EResult::EOS_InvalidParameters;
    }

    auto it = m_lobby_invites.find(Options->InviteId);
    if (it == m_lobby_invites.end())
    {
        *OutLobbyDetailsHandle = nullptr;
        return EOS_EResult::EOS_NotFound;
    }

    EOSSDK_LobbyDetails* pLobbyDetails = new EOSSDK_LobbyDetails;
    pLobbyDetails->m_infos = it->second.infos;
    *OutLobbyDetailsHandle = reinterpret_cast<EOS_HLobbyDetails>(pLobbyDetails);

    return EOS_EResult::EOS_Success;
}

EOS_EResult EOSSDK_Lobby::CopyLobbyDetailsHandleByUiEventId(const EOS_Lobby_CopyLobbyDetailsHandleByUiEventIdOptions* Options, EOS_HLobbyDetails* OutLobbyDetailsHandle)
{
    TRACE_FUNC();

    if (Options == nullptr || Options->UiEventId == EOS_UI_EVENTID_INVALID || OutLobbyDetailsHandle == nullptr)
    {
        set_nullptr(OutLobbyDetailsHandle);
        return EOS_EResult::EOS_InvalidParameters;
    }

    *OutLobbyDetailsHandle = nullptr;
    return EOS_EResult::EOS_NotFound;
}

EOS_EResult EOSSDK_Lobby::CopyLobbyDetailsHandle(const EOS_Lobby_CopyLobbyDetailsHandleOptions* Options, EOS_HLobbyDetails* OutLobbyDetailsHandle)
{
    TRACE_FUNC();

    if (Options == nullptr || OutLobbyDetailsHandle == nullptr)
    {
        set_nullptr(OutLobbyDetailsHandle);
        return EOS_EResult::EOS_InvalidParameters;
    }

    auto it = m_lobbies.find(Options->LobbyId);
    if (it == m_lobbies.end())
    {
        *OutLobbyDetailsHandle = nullptr;
        return EOS_EResult::EOS_NotFound;
    }
    
    EOSSDK_LobbyDetails* pLobbyDetails = new EOSSDK_LobbyDetails;

    pLobbyDetails->m_infos = it->second.infos;

    *OutLobbyDetailsHandle = reinterpret_cast<EOS_HLobbyDetails>(pLobbyDetails);

    return EOS_EResult::EOS_Success;
}

bool EOSSDK_Lobby::send_to_all_members(Network_Message_pb& msg, lobby_state_t* lobby)
{
    TRACE_FUNC();
    assert(lobby != nullptr);

    for (auto const& member : lobby->infos.members())
    {
        if (member.first != msg.source_id())
        {
            msg.set_dest_id(member.first);
            GetNetwork().TCPSendTo(msg);
        }
    }
    return true;
}

bool EOSSDK_Lobby::send_to_all_members_or_owner(Network_Message_pb& msg, lobby_state_t* lobby)
{
    TRACE_FUNC();
    
    assert(lobby != nullptr);

    if (i_am_owner(lobby))
    {
        return send_to_all_members(msg, lobby);
    }
    
    msg.set_dest_id(lobby->infos.owner_id());
    return GetNetwork().TCPSendTo(msg);
}

bool EOSSDK_Lobby::send_lobby_update(lobby_state_t* pLobby)
{
    TRACE_FUNC();
    string const& user_id = Settings::Inst().productuserid->to_string();
    
    Network_Message_pb msg;
    Lobby_Message_pb* lobby = new Lobby_Message_pb;
    Lobby_Update_pb* update = new Lobby_Update_pb;

    update->set_lobby_id(pLobby->infos.lobby_id());
    update->set_max_lobby_member(pLobby->infos.max_lobby_member());
    update->set_permission_level(pLobby->infos.permission_level());
    *update->mutable_attributes() = pLobby->infos.attributes();

    lobby->set_allocated_lobby_update(update);
    msg.set_allocated_lobby(lobby);

    msg.set_source_id(user_id);
    msg.set_game_id(Settings::Inst().appid);

    return send_to_all_members(msg, pLobby);
}

bool EOSSDK_Lobby::send_lobbies_search_response(Network::peer_t const& peerid, Lobbies_Search_response_pb* resp)
{
    TRACE_FUNC();
    string const& user_id = Settings::Inst().productuserid->to_string();

    Network_Message_pb msg;
    Lobbies_Search_Message_pb* search = new Lobbies_Search_Message_pb;

    search->set_allocated_search_response(resp);
    msg.set_allocated_lobbies_search(search);

    msg.set_source_id(user_id);
    msg.set_dest_id(peerid);
    msg.set_game_id(Settings::Inst().appid);

    return GetNetwork().TCPSendTo(msg);
}

bool EOSSDK_Lobby::send_lobby_join_request(Network::peer_t const& peerid, Lobby_Join_Request_pb* req)
{
    TRACE_FUNC();
    string const& user_id = Settings::Inst().productuserid->to_string();

    Network_Message_pb msg;
    Lobby_Message_pb* lobby_pb = new Lobby_Message_pb;

    lobby_pb->set_allocated_lobby_join_request(req);
    msg.set_allocated_lobby(lobby_pb);

    msg.set_source_id(user_id);
    msg.set_dest_id(peerid);
    msg.set_game_id(Settings::Inst().appid);

    return GetNetwork().TCPSendTo(msg);
}

bool EOSSDK_Lobby::send_lobby_join_response(Network::peer_t const& peerid, Lobby_Join_Response_pb* resp)
{
    TRACE_FUNC();
    string const& user_id = Settings::Inst().productuserid->to_string();

    Network_Message_pb msg;
    Lobby_Message_pb* lobby_pb = new Lobby_Message_pb;

    lobby_pb->set_allocated_lobby_join_response(resp);
    msg.set_allocated_lobby(lobby_pb);

    msg.set_source_id(user_id);
    msg.set_dest_id(peerid);
    msg.set_game_id(Settings::Inst().appid);

    return GetNetwork().TCPSendTo(msg);
}

bool EOSSDK_Lobby::send_lobby_invite(Network::peer_t const& peerid, Lobby_Invite_pb* invite)
{
    TRACE_FUNC();
    string const& user_id = Settings::Inst().productuserid->to_string();

    Network_Message_pb msg;
    Lobby_Message_pb* lobby_pb = new Lobby_Message_pb;

    lobby_pb->set_allocated_lobby_invite(invite);
    msg.set_allocated_lobby(lobby_pb);

    msg.set_source_id(user_id);
    msg.set_dest_id(peerid);
    msg.set_game_id(Settings::Inst().appid);

    return GetNetwork().TCPSendTo(msg);
}

bool EOSSDK_Lobby::send_lobby_member_update(Network::peer_t const& member_id, lobby_state_t* pLobby)
{
    TRACE_FUNC();
    string const& user_id = Settings::Inst().productuserid->to_string();

    auto it = pLobby->infos.members().find(member_id);
    if (it != pLobby->infos.members().end())
    {
        Network_Message_pb msg;
        Lobby_Message_pb* lobby = new Lobby_Message_pb;

        Lobby_Member_Update_pb* update = new Lobby_Member_Update_pb;
        update->set_lobby_id(pLobby->infos.lobby_id());
        (*update->mutable_member())[member_id] = it->second;

        lobby->set_allocated_member_update(update);
        msg.set_allocated_lobby(lobby);
        msg.set_source_id(user_id);
        msg.set_game_id(Settings::Inst().appid);

        return send_to_all_members_or_owner(msg, pLobby);
    }
    return false;
}

bool EOSSDK_Lobby::send_lobby_member_join(Network::peer_t const& member_id, lobby_state_t* lobby)
{
    TRACE_FUNC();
    string const& user_id = Settings::Inst().productuserid->to_string();

    Network_Message_pb msg;
    Lobby_Message_pb* lobby_pb = new Lobby_Message_pb;
    Lobby_Member_Join_pb* join = new Lobby_Member_Join_pb;

    join->set_lobby_id(lobby->infos.lobby_id());
    join->set_member_id(member_id);

    lobby_pb->set_allocated_member_join(join);
    msg.set_allocated_lobby(lobby_pb);
    msg.set_source_id(user_id);
    msg.set_game_id(Settings::Inst().appid);

    return send_to_all_members(msg, lobby);
}

bool EOSSDK_Lobby::send_lobby_member_leave(Network::peer_t const& member_id, lobby_state_t* lobby, EOS_ELobbyMemberStatus reason)
{
    TRACE_FUNC();
    string const& user_id = Settings::Inst().productuserid->to_string();

    Network_Message_pb msg;
    Lobby_Message_pb* lobby_pb = new Lobby_Message_pb;
    Lobby_Member_Leave_pb* leave = new Lobby_Member_Leave_pb;

    leave->set_lobby_id(lobby->infos.lobby_id());
    leave->set_member_id(member_id);
    leave->set_reason(static_cast<int>(reason));

    lobby_pb->set_allocated_member_leave(leave);
    msg.set_allocated_lobby(lobby_pb);
    msg.set_source_id(user_id);
    msg.set_game_id(Settings::Inst().appid);

    auto& my_presence = GetEOS_Presence().get_myself();
    if (my_presence.richtext().find("In Lobby: " + lobby->infos.lobby_id()) != string::npos)
    {
        my_presence.set_richtext("");
        GetEOS_Presence().send_my_presence_info_to_all_peers();
    }

    return send_to_all_members_or_owner(msg, lobby);
}

bool EOSSDK_Lobby::send_lobby_member_promote(Network::peer_t const& member_id, lobby_state_t* lobby)
{
    TRACE_FUNC();
    string const& user_id = Settings::Inst().productuserid->to_string();

    Network_Message_pb msg;
    Lobby_Message_pb* lobby_pb = new Lobby_Message_pb;
    Lobby_Member_Promote_pb* promote = new Lobby_Member_Promote_pb;

    promote->set_lobby_id(lobby->infos.lobby_id());
    promote->set_member_id(member_id);

    lobby_pb->set_allocated_member_promote(promote);
    msg.set_allocated_lobby(lobby_pb);

    msg.set_source_id(user_id);
    msg.set_game_id(Settings::Inst().appid);

    return send_to_all_members(msg, lobby);
}

bool EOSSDK_Lobby::on_peer_disconnect(Network_Message_pb const& msg, Network_Peer_Disconnect_pb const& peer)
{
    TRACE_FUNC();
    lock _(GLOBAL_MUTEX);

    for (auto& lobby : m_lobbies)
    {
        if (i_am_owner(&lobby.second))
        {
            if (remove_member_from_lobby(msg.source_id(), &lobby.second))
            {
                string const& user_id = Settings::Inst().productuserid->to_string();

                Network_Message_pb msg_resp;
                Lobby_Message_pb* lobby_pb = new Lobby_Message_pb;
                Lobby_Member_Leave_pb* member_leave = new Lobby_Member_Leave_pb;

                member_leave->set_lobby_id(lobby.second.infos.lobby_id());
                member_leave->set_member_id(msg.source_id());
                member_leave->set_reason(static_cast<int>(EOS_ELobbyMemberStatus::EOS_LMS_DISCONNECTED));

                lobby_pb->set_allocated_member_leave(member_leave);
                msg_resp.set_allocated_lobby(lobby_pb);

                msg_resp.set_source_id(user_id);

                send_to_all_members(msg_resp, &lobby.second);

                notify_lobby_member_status_update(msg.source_id(), EOS_ELobbyMemberStatus::EOS_LMS_DISCONNECTED, &lobby.second);
            }
        }
    }

    return true;
}

bool EOSSDK_Lobby::on_lobby_update(Network_Message_pb const& msg, Lobby_Update_pb const& update)
{
    TRACE_FUNC();
    lock _(GLOBAL_MUTEX);

    lobby_state_t* pLobby = get_lobby_by_id(update.lobby_id());

    if (pLobby != nullptr)
    {
        pLobby->infos.set_max_lobby_member(update.max_lobby_member());
        pLobby->infos.set_permission_level(update.permission_level());
        *pLobby->infos.mutable_attributes() = update.attributes();

        notify_lobby_update(pLobby);
    }

    return true;
}

bool EOSSDK_Lobby::on_lobby_member_update(Network_Message_pb const& msg, Lobby_Member_Update_pb const& update)
{
    TRACE_FUNC();
    lock _(GLOBAL_MUTEX);

    lobby_state_t* pLobby = get_lobby_by_id(update.lobby_id());

    if (pLobby != nullptr)
    {
        auto const& member = *update.member().begin();
        (*pLobby->infos.mutable_members())[member.first] = member.second;

        notify_lobby_member_update(member.first, pLobby);
    }

    return true;
}

bool EOSSDK_Lobby::on_lobbies_search(Network_Message_pb const& msg, Lobbies_Search_pb const& search)
{
    TRACE_FUNC();
    lock _(GLOBAL_MUTEX);

    Lobbies_Search_response_pb* resp = new Lobbies_Search_response_pb;
    resp->set_search_id(search.search_id());

    if (msg.game_id() == Settings::Inst().appid)
    {
        if (search.parameters_size() > 0)
        {
            std::map<string, Lobby_Search_Parameter> params;
            for (auto const& p : search.parameters()) params[p.first] = p.second;
            std::vector<lobby_state_t*> lobbies = std::move(get_lobbies_from_attributes(params));
            for (auto& lobby : lobbies)
            {
                if (i_am_owner(lobby))
                {
                    *resp->mutable_lobbies()->Add() = lobby->infos;
                }
            }
        }
        else if (!search.lobby_id().empty())
        {
            lobby_state_t* pLobby = get_lobby_by_id(search.lobby_id());
            if (pLobby != nullptr && i_am_owner(pLobby))
            {
                *resp->mutable_lobbies()->Add() = pLobby->infos;
            }
        }
        else if (GetProductUserId(search.target_id()) == GetEOS_Connect().get_myself()->first)
        {
            for (auto& lobby : m_lobbies)
            {
                if (i_am_owner(&lobby.second))
                {
                    *resp->mutable_lobbies()->Add() = lobby.second.infos;
                }
            }
        }
    }

    return send_lobbies_search_response(msg.source_id(), resp);
}

bool EOSSDK_Lobby::on_lobby_join_request(Network_Message_pb const& msg, Lobby_Join_Request_pb const& req)
{
    TRACE_FUNC();
    lock _(GLOBAL_MUTEX);

    lobby_state_t* pLobby = get_lobby_by_id(req.lobby_id());
    
    Lobby_Join_Response_pb* resp = new Lobby_Join_Response_pb;

    resp->set_join_id(req.join_id());

    if (pLobby == nullptr)
    {
        resp->set_reason(static_cast<int>(EOS_EResult::EOS_NotFound));
    }
    else
    {
        if (i_am_owner(pLobby))
        {
            if (pLobby->infos.max_lobby_member() - pLobby->infos.members_size() > 0)
            {
                resp->set_reason(static_cast<int>(EOS_EResult::EOS_Success));
                *resp->mutable_infos() = pLobby->infos;

                send_lobby_member_join(msg.source_id(), pLobby);

                if (add_member_to_lobby(msg.source_id(), pLobby))
                {
                    notify_lobby_member_status_update(msg.source_id(), EOS_ELobbyMemberStatus::EOS_LMS_JOINED, pLobby);
                }
            }
            else
            {
                resp->set_reason(static_cast<int>(EOS_EResult::EOS_Lobby_TooManyPlayers));
            }
        }
        else
        {
            resp->set_reason(static_cast<int>(EOS_EResult::EOS_Lobby_NoPermission));
        }
    }

    return send_lobby_join_response(msg.source_id(), resp);
}

bool EOSSDK_Lobby::on_lobby_join_response(Network_Message_pb const& msg, Lobby_Join_Response_pb const& resp)
{
    TRACE_FUNC();
    lock _(GLOBAL_MUTEX);

    auto it = m_joins_requests.find(resp.join_id());
    if (it != m_joins_requests.end())
    {
        EOS_Lobby_JoinLobbyCallbackInfo& jlci = it->second.cb->GetCallback<EOS_Lobby_JoinLobbyCallbackInfo>();
        it->second.cb->done = true;

        if ((EOS_EResult)resp.reason() == EOS_EResult::EOS_Success)
        {
            jlci.ResultCode = EOS_EResult::EOS_Success;
            auto& lobby = m_lobbies[resp.infos().lobby_id()];
            lobby.infos = resp.infos();
            lobby.state = lobby_state_t::joined;
            add_member_to_lobby(msg.dest_id(), &lobby);

            auto& my_presence = GetEOS_Presence().get_myself();
            my_presence.set_richtext("In Lobby: " + resp.infos().lobby_id());
            GetEOS_Presence().send_my_presence_info_to_all_peers();
        }
        else
        {
            jlci.ResultCode = (EOS_EResult)resp.reason();
        }

        m_joins_requests.erase(it);
    }

    return true;
}

bool EOSSDK_Lobby::on_lobby_invite(Network_Message_pb const& msg, Lobby_Invite_pb const& invite)
{
    TRACE_FUNC();
    lock _(GLOBAL_MUTEX);

    lobby_invite_t new_invite;
    new_invite.peer_id = GetProductUserId(msg.source_id());
    new_invite.infos = invite.infos();
    m_lobby_invites.emplace(generate_account_id(), std::move(new_invite));

    auto& lobby_invite = *m_lobby_invites.rbegin();
    notify_lobby_invite_received(lobby_invite.first, lobby_invite.second.peer_id);

    return true;
}

bool EOSSDK_Lobby::on_lobby_member_join(Network_Message_pb const& msg, Lobby_Member_Join_pb const& join)
{
    TRACE_FUNC();
    lock _(GLOBAL_MUTEX);

    lobby_state_t* pLobby = get_lobby_by_id(join.lobby_id());
    if (pLobby != nullptr)
    {
        add_member_to_lobby(join.member_id(), pLobby);
        notify_lobby_member_status_update(msg.source_id(), EOS_ELobbyMemberStatus::EOS_LMS_JOINED, pLobby);
    }

    return true;
}

bool EOSSDK_Lobby::on_lobby_member_leave(Network_Message_pb const& msg, Lobby_Member_Leave_pb const& leave)
{
    TRACE_FUNC();
    lock _(GLOBAL_MUTEX);

    lobby_state_t* pLobby = get_lobby_by_id(leave.lobby_id());
    if (pLobby != nullptr && remove_member_from_lobby(leave.member_id(), pLobby))
    {
        if (i_am_owner(pLobby))
        {
            string const& user_id = Settings::Inst().productuserid->to_string();

            Network_Message_pb msg_resp;
            Lobby_Message_pb* lobby_pb = new Lobby_Message_pb;
            Lobby_Member_Leave_pb* leave_pb = new Lobby_Member_Leave_pb(leave);

            lobby_pb->set_allocated_member_leave(leave_pb);
            msg_resp.set_allocated_lobby(lobby_pb);

            msg_resp.set_source_id(user_id);

            send_to_all_members(msg_resp, pLobby);
        }

        notify_lobby_member_status_update(leave.member_id(), (EOS_ELobbyMemberStatus)leave.reason(), pLobby);

        switch ((EOS_ELobbyMemberStatus)leave.reason())
        {
            case EOS_ELobbyMemberStatus::EOS_LMS_KICKED:
            {
                if (GetProductUserId(leave.member_id()) == Settings::Inst().productuserid)
                {
                    m_lobbies.erase(leave.lobby_id());
                }
            }
            break;
        }
    }

    return true;
}

bool EOSSDK_Lobby::on_lobby_member_promote(Network_Message_pb const& msg, Lobby_Member_Promote_pb const& promote)
{
    TRACE_FUNC();
    lock _(GLOBAL_MUTEX);

    lobby_state_t* pLobby = get_lobby_by_id(promote.lobby_id());
    if (pLobby != nullptr && is_member_in_lobby(promote.member_id(), pLobby))
    {
        pLobby->infos.set_owner_id(promote.member_id());

        notify_lobby_member_status_update(promote.member_id(), EOS_ELobbyMemberStatus::EOS_LMS_PROMOTED, pLobby);
    }

    return true;
}

bool EOSSDK_Lobby::CBRunFrame()
{
    lock _(GLOBAL_MUTEX);

    auto now = clock::now();
    for (auto it = m_joins_requests.begin(); it != m_joins_requests.end();)
    {
        if ((now - it->second.cb->created_time) > join_timeout)
        {
            it->second.cb->done = true;
            it = m_joins_requests.erase(it);
        }
        else
        {
            ++it;
        }
    }

    for (auto it = m_lobbies_searchs.begin(); it != m_lobbies_searchs.end();)
    {
        if ((*it)->released())
        {
            delete *it;
            it = m_lobbies_searchs.erase(it);
        }
        else
        {
            ++it;
        }
    }

    return true;
}

bool EOSSDK_Lobby::RunNetwork(Network_Message_pb const& msg)
{
    switch (msg.messages_case())
    {
        case Network_Message_pb::MessagesCase::kLobby:
        {
            Lobby_Message_pb const& lobby = msg.lobby();
            switch (lobby.message_case())
            {
                case Lobby_Message_pb::MessageCase::kLobbyUpdate      : return on_lobby_update        (msg, lobby.lobby_update());
                case Lobby_Message_pb::MessageCase::kLobbyJoinRequest : return on_lobby_join_request  (msg, lobby.lobby_join_request());
                case Lobby_Message_pb::MessageCase::kLobbyJoinResponse: return on_lobby_join_response (msg, lobby.lobby_join_response());
                case Lobby_Message_pb::MessageCase::kLobbyInvite      : return on_lobby_invite        (msg, lobby.lobby_invite());

                case Lobby_Message_pb::MessageCase::kMemberUpdate     : return on_lobby_member_update (msg, lobby.member_update());
                case Lobby_Message_pb::MessageCase::kMemberJoin       : return on_lobby_member_join   (msg, lobby.member_join());
                case Lobby_Message_pb::MessageCase::kMemberLeave      : return on_lobby_member_leave  (msg, lobby.member_leave());
                case Lobby_Message_pb::MessageCase::kMemberPromote    : return on_lobby_member_promote(msg, lobby.member_promote());
            }
        }
        break;

        case Network_Message_pb::MessagesCase::kLobbiesSearch:
        {
            Lobbies_Search_Message_pb const& search = msg.lobbies_search();
            switch (search.message_case())
            {
                case Lobbies_Search_Message_pb::MessageCase::kSearch: return on_lobbies_search(msg, search.search());
            }
        }
        break;
    }

    return true;
}

bool EOSSDK_Lobby::RunCallbacks(pFrameResult_t res)
{
    lock _(GLOBAL_MUTEX);

    return res->done;;
}

void EOSSDK_Lobby::FreeCallback(pFrameResult_t res)
{
    lock _(GLOBAL_MUTEX);

    switch (res->ICallback())
    {

        case EOS_Lobby_CreateLobbyCallbackInfo::k_iCallback:
        {
            EOS_Lobby_CreateLobbyCallbackInfo& callback = res->GetCallback<EOS_Lobby_CreateLobbyCallbackInfo>();
            
            delete[]callback.LobbyId;
        }
        break;

        case EOS_Lobby_DestroyLobbyCallbackInfo::k_iCallback:
        {
            EOS_Lobby_DestroyLobbyCallbackInfo& callback = res->GetCallback<EOS_Lobby_DestroyLobbyCallbackInfo>();
            
            delete[]callback.LobbyId;
        }
        break;

        case EOS_Lobby_JoinLobbyCallbackInfo::k_iCallback:
        {
            EOS_Lobby_JoinLobbyCallbackInfo& callback = res->GetCallback<EOS_Lobby_JoinLobbyCallbackInfo>();
            
            delete[]callback.LobbyId;
        }
        break;

        case EOS_Lobby_LeaveLobbyCallbackInfo::k_iCallback:
        {
            EOS_Lobby_LeaveLobbyCallbackInfo& callback = res->GetCallback<EOS_Lobby_LeaveLobbyCallbackInfo>();
            
            delete[]callback.LobbyId;
        }
        break;

        case EOS_Lobby_UpdateLobbyCallbackInfo::k_iCallback:
        {
            EOS_Lobby_UpdateLobbyCallbackInfo& callback = res->GetCallback<EOS_Lobby_UpdateLobbyCallbackInfo>();
            
            delete[]callback.LobbyId;
        }
        break;

        case EOS_Lobby_PromoteMemberCallbackInfo::k_iCallback:
        {
            EOS_Lobby_PromoteMemberCallbackInfo& callback = res->GetCallback<EOS_Lobby_PromoteMemberCallbackInfo>();
            
            delete[]callback.LobbyId;
        }
        break;

        case EOS_Lobby_KickMemberCallbackInfo::k_iCallback:
        {
            EOS_Lobby_KickMemberCallbackInfo& callback = res->GetCallback<EOS_Lobby_KickMemberCallbackInfo>();
            
            delete[]callback.LobbyId;
        }
        break;

        case EOS_Lobby_SendInviteCallbackInfo::k_iCallback:
        {
            EOS_Lobby_SendInviteCallbackInfo& callback = res->GetCallback<EOS_Lobby_SendInviteCallbackInfo>();
            
            delete[]callback.LobbyId;
        }
        break;

        case EOS_Lobby_RejectInviteCallbackInfo::k_iCallback:
        {
            EOS_Lobby_RejectInviteCallbackInfo& callback = res->GetCallback<EOS_Lobby_RejectInviteCallbackInfo>();
            
            delete[]callback.InviteId;
        }
        break;

        case EOS_Lobby_LobbyUpdateReceivedCallbackInfo::k_iCallback:
        {
            EOS_Lobby_LobbyUpdateReceivedCallbackInfo& callback = res->GetCallback<EOS_Lobby_LobbyUpdateReceivedCallbackInfo>();
            
            delete[]callback.LobbyId;
        }
        break;

        case EOS_Lobby_LobbyMemberUpdateReceivedCallbackInfo::k_iCallback:
        {
            EOS_Lobby_LobbyMemberUpdateReceivedCallbackInfo& callback = res->GetCallback<EOS_Lobby_LobbyMemberUpdateReceivedCallbackInfo>();
            
            delete[]callback.LobbyId;
        }
        break;

        case EOS_Lobby_LobbyMemberStatusReceivedCallbackInfo::k_iCallback:
        {
            EOS_Lobby_LobbyMemberStatusReceivedCallbackInfo& callback = res->GetCallback<EOS_Lobby_LobbyMemberStatusReceivedCallbackInfo>();
            
            delete[]callback.LobbyId;
        }
        break;

        case EOS_Lobby_LobbyInviteReceivedCallbackInfo::k_iCallback:
        {
            EOS_Lobby_LobbyInviteReceivedCallbackInfo& callback = res->GetCallback<EOS_Lobby_LobbyInviteReceivedCallbackInfo>();
            
            delete[]callback.InviteId;
        }
        break;

        case EOS_Lobby_LobbyInviteAcceptedCallbackInfo::k_iCallback:
        {
            EOS_Lobby_LobbyInviteAcceptedCallbackInfo& callback = res->GetCallback<EOS_Lobby_LobbyInviteAcceptedCallbackInfo>();
            
            delete[]callback.InviteId;
        }
        break;

    }
}

}