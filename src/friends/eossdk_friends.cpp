#include "eossdk_friends.h"
#include "eossdk_platform.h"
#include "eos_client_api.h"
#include "settings.h"

namespace sdk
{
EOSSDK_Friends::EOSSDK_Friends()
{
    
    m_friends_filename = FileManager::join(Settings::Inst().save_path, "friends.json");
    fifo_json saved;
    if (FileManager::load_json(m_friends_filename, saved))
    {
        for (auto& id_str : saved)
        {
            if (id_str.is_string())
            {
                auto epic_id = GetEpicUserId(id_str.get<string>());
                if (epic_id) m_persisted_friends.insert(epic_id);
            }
        }
    }
    GetCB_Manager().register_callbacks(this);
    GetNetwork().register_listener(this, 0,
        Network_Message_pb::MessagesCase::kFriendsInvite);
}

EOSSDK_Friends::~EOSSDK_Friends()
{
    
    fifo_json to_save = fifo_json::array();
    for (auto& f : m_friends)
        to_save.push_back(f->to_string());
    for (auto& f : m_persisted_friends)
        if (!m_friends.count(f))
            to_save.push_back(f->to_string());
    FileManager::save_json(m_friends_filename, to_save);

    GetNetwork().unregister_listener(this, 0,
        Network_Message_pb::MessagesCase::kFriendsInvite);
    GetCB_Manager().remove_all_notifications(this);
    GetCB_Manager().unregister_callbacks(this);

    GetCB_Manager().unregister_callbacks(this);
    GetCB_Manager().remove_all_notifications(this);
}

void EOSSDK_Friends::QueryFriends(const EOS_Friends_QueryFriendsOptions* Options, void* ClientData, const EOS_Friends_OnQueryFriendsCallback CompletionDelegate)
{
    TRACE_FUNC();
    lock _(GLOBAL_MUTEX);

    SE_CB_BEGIN(EOS_Friends_QueryFriendsCallbackInfo, CompletionDelegate);
    info.LocalUserId = Settings::Inst().userid;
    info.ResultCode = EOS_EResult::EOS_Success;

    m_friends.clear();

    for (auto user_it = GetEOS_Connect().get_other_users(); user_it != GetEOS_Connect().get_end_users(); ++user_it)
    {
        if (user_it->second.authentified)
        {
            auto epic_id = GetEpicUserId(user_it->second.infos.userid());
            m_friends.insert(epic_id);
        }
    }

    for (auto& f : m_persisted_friends)
        m_friends.insert(f);

    SE_CB_END(this);
}

void EOSSDK_Friends::SendInvite(const EOS_Friends_SendInviteOptions* Options, void* ClientData, const EOS_Friends_OnSendInviteCallback CompletionDelegate)
{
    TRACE_FUNC();
    lock _(GLOBAL_MUTEX);

    SE_CB_BEGIN(EOS_Friends_SendInviteCallbackInfo, CompletionDelegate);
    info.LocalUserId  = Settings::Inst().userid;
    info.TargetUserId = Options ? Options->TargetUserId : nullptr;

    if (!Options || !Options->TargetUserId)
    {
        info.ResultCode = EOS_EResult::EOS_InvalidParameters;
    }
    else
    {
        
        const string my_epic_id = Settings::Inst().userid->to_string();
        const string dest_id    = Options->TargetUserId->to_string();

        Network_Message_pb msg;
        msg.set_source_id(my_epic_id);
        msg.set_dest_id(dest_id);
        msg.set_game_id(Settings::Inst().appid);

        auto* fi_msg = msg.mutable_friends_invite();
        auto* fi     = fi_msg->mutable_friends_invite();
        fi->set_sender_id(my_epic_id);
        fi->set_recipient_id(dest_id);
        fi->set_action(0);  

        GetNetwork().TCPSendTo(msg);
        info.ResultCode = EOS_EResult::EOS_Success;
    }

    SE_CB_END(this);
}

void EOSSDK_Friends::AcceptInvite(const EOS_Friends_AcceptInviteOptions* Options, void* ClientData, const EOS_Friends_OnAcceptInviteCallback CompletionDelegate)
{
    TRACE_FUNC();
    lock _(GLOBAL_MUTEX);

    SE_CB_BEGIN(EOS_Friends_AcceptInviteCallbackInfo, CompletionDelegate);
    info.LocalUserId  = Settings::Inst().userid;
    info.TargetUserId = Options ? Options->TargetUserId : nullptr;

    if (!Options || !Options->TargetUserId)
    {
        info.ResultCode = EOS_EResult::EOS_InvalidParameters;
    }
    else
    {
        
        m_friends.insert(Options->TargetUserId);

        const string my_epic_id = Settings::Inst().userid->to_string();
        const string dest_id    = Options->TargetUserId->to_string();

        Network_Message_pb msg;
        msg.set_source_id(my_epic_id);
        msg.set_dest_id(dest_id);
        msg.set_game_id(Settings::Inst().appid);

        auto* fi_msg = msg.mutable_friends_invite();
        auto* fi     = fi_msg->mutable_friends_invite();
        fi->set_sender_id(my_epic_id);
        fi->set_recipient_id(dest_id);
        fi->set_action(1);  

        GetNetwork().TCPSendTo(msg);
        info.ResultCode = EOS_EResult::EOS_Success;
    }

    SE_CB_END(this);
}

void EOSSDK_Friends::RejectInvite(const EOS_Friends_RejectInviteOptions* Options, void* ClientData, const EOS_Friends_OnRejectInviteCallback CompletionDelegate)
{
    TRACE_FUNC();
    lock _(GLOBAL_MUTEX);

    SE_CB_BEGIN(EOS_Friends_RejectInviteCallbackInfo, CompletionDelegate);
    info.LocalUserId  = Settings::Inst().userid;
    info.TargetUserId = Options ? Options->TargetUserId : nullptr;

    if (!Options || !Options->TargetUserId)
    {
        info.ResultCode = EOS_EResult::EOS_InvalidParameters;
    }
    else
    {
        const string my_epic_id = Settings::Inst().userid->to_string();
        const string dest_id    = Options->TargetUserId->to_string();

        Network_Message_pb msg;
        msg.set_source_id(my_epic_id);
        msg.set_dest_id(dest_id);
        msg.set_game_id(Settings::Inst().appid);

        auto* fi_msg = msg.mutable_friends_invite();
        auto* fi     = fi_msg->mutable_friends_invite();
        fi->set_sender_id(my_epic_id);
        fi->set_recipient_id(dest_id);
        fi->set_action(2);  

        GetNetwork().TCPSendTo(msg);
        info.ResultCode = EOS_EResult::EOS_Success;
    }

    SE_CB_END(this);
}

int32_t EOSSDK_Friends::GetFriendsCount(const EOS_Friends_GetFriendsCountOptions* Options)
{
    TRACE_FUNC();
    lock _(GLOBAL_MUTEX);

    return static_cast<int32_t>(m_friends.size());
}

EOS_EpicAccountId EOSSDK_Friends::GetFriendAtIndex(const EOS_Friends_GetFriendAtIndexOptions* Options)
{
    TRACE_FUNC();
    lock _(GLOBAL_MUTEX);

    if (Options == nullptr || Options->Index >= m_friends.size())
        return nullptr;
    
    auto it = m_friends.begin();
    std::advance(it, Options->Index);

    return *it;
}

EOS_EFriendsStatus EOSSDK_Friends::GetStatus(const EOS_Friends_GetStatusOptions* Options)
{
    TRACE_FUNC();
    lock _(GLOBAL_MUTEX);

    if (Options == nullptr || Options->TargetUserId == nullptr)
        return EOS_EFriendsStatus::EOS_FS_NotFriends;

    auto it = m_friends.find(Options->TargetUserId);
    if(it == m_friends.end())
        return EOS_EFriendsStatus::EOS_FS_NotFriends;
    
    return EOS_EFriendsStatus::EOS_FS_Friends;
}

EOS_NotificationId EOSSDK_Friends::AddNotifyFriendsUpdate(const EOS_Friends_AddNotifyFriendsUpdateOptions* Options, void* ClientData, const EOS_Friends_OnFriendsUpdateCallback FriendsUpdateHandler)
{
    TRACE_FUNC();
    lock _(GLOBAL_MUTEX);

    if (FriendsUpdateHandler == nullptr)
        return EOS_INVALID_NOTIFICATIONID;

    pFrameResult_t res(new FrameResult);
    
    EOS_Friends_OnFriendsUpdateInfo& ofup = res->CreateCallback<EOS_Friends_OnFriendsUpdateInfo>((CallbackFunc)FriendsUpdateHandler);
    ofup.ClientData = ClientData;
    ofup.LocalUserId = Settings::Inst().userid;
    ofup.TargetUserId = GetEpicUserId(sdk::NULL_USER_ID);
    ofup.PreviousStatus = EOS_EFriendsStatus::EOS_FS_Friends;
    ofup.CurrentStatus = EOS_EFriendsStatus::EOS_FS_Friends;

    return GetCB_Manager().add_notification(this, res);
}

void EOSSDK_Friends::RemoveNotifyFriendsUpdate(EOS_NotificationId NotificationId)
{
    TRACE_FUNC();
    lock _(GLOBAL_MUTEX);

    GetCB_Manager().remove_notification(this, NotificationId);
}

EOS_NotificationId EOSSDK_Friends::AddNotifyBlockedUsersUpdate(const EOS_Friends_AddNotifyBlockedUsersUpdateOptions* Options, void* ClientData, const EOS_Friends_OnBlockedUsersUpdateCallback BlockedUsersUpdateHandler)
{
    TRACE_FUNC();
    lock _(GLOBAL_MUTEX);

    if (BlockedUsersUpdateHandler == nullptr)
        return EOS_INVALID_NOTIFICATIONID;

    pFrameResult_t res(new FrameResult);
    
    EOS_Friends_OnBlockedUsersUpdateInfo& ofup = res->CreateCallback<EOS_Friends_OnBlockedUsersUpdateInfo>((CallbackFunc)BlockedUsersUpdateHandler);
    ofup.ClientData = ClientData;
    ofup.LocalUserId = Settings::Inst().userid;
    ofup.TargetUserId = GetEpicUserId(sdk::NULL_USER_ID);
    ofup.bBlocked = EOS_FALSE;

    return GetCB_Manager().add_notification(this, res);
}

void EOSSDK_Friends::RemoveNotifyBlockedUsersUpdate(EOS_NotificationId NotificationId)
{
    TRACE_FUNC();
    lock _(GLOBAL_MUTEX);

    GetCB_Manager().remove_notification(this, NotificationId);
}

int32_t EOSSDK_Friends::GetBlockedUsersCount(const EOS_Friends_GetBlockedUsersCountOptions* Options)
{
    TRACE_FUNC();
    return 0;
}

EOS_EpicAccountId EOSSDK_Friends::GetBlockedUserAtIndex(const EOS_Friends_GetBlockedUserAtIndexOptions* Options)
{
    TRACE_FUNC();
    return nullptr;
}

bool EOSSDK_Friends::CBRunFrame()
{
    return false;
}

bool EOSSDK_Friends::RunCallbacks(pFrameResult_t res)
{
    return res->done;
}

void EOSSDK_Friends::FreeCallback(pFrameResult_t res)
{
    (void)res;
}

bool EOSSDK_Friends::RunNetwork(Network_Message_pb const& msg)
{
    if (msg.messages_case() != Network_Message_pb::MessagesCase::kFriendsInvite)
        return false;

    const Friends_Invite_Message_pb& fim = msg.friends_invite();
    if (fim.message_case() != Friends_Invite_Message_pb::kFriendsInvite)
        return false;

    return on_friends_invite(msg, fim.friends_invite());
}

bool EOSSDK_Friends::on_friends_invite(
    Network_Message_pb const& ,
    Friends_Invite_pb const& inv)
{
    const string my_epic_id = Settings::Inst().userid->to_string();
    if (inv.recipient_id() != my_epic_id) return false;

    EOS_EpicAccountId sender = GetEpicUserId(inv.sender_id());
    if (!sender) return false;

    EOS_EFriendsStatus new_status = EOS_EFriendsStatus::EOS_FS_NotFriends;

    switch (inv.action())
    {
        case 0: 
            new_status = EOS_EFriendsStatus::EOS_FS_InviteReceived;
            break;

        case 1: 
            m_friends.insert(sender);
            new_status = EOS_EFriendsStatus::EOS_FS_Friends;
            break;

        case 2: 
            new_status = EOS_EFriendsStatus::EOS_FS_NotFriends;
            break;

        default: return false;
    }

    auto notifs = GetCB_Manager().get_notifications(
        this, EOS_Friends_OnFriendsUpdateInfo::k_iCallback);
    for (auto& n : notifs)
    {
        EOS_Friends_OnFriendsUpdateInfo& ofui =
            n->GetCallback<EOS_Friends_OnFriendsUpdateInfo>();
        ofui.LocalUserId    = Settings::Inst().userid;
        ofui.TargetUserId   = sender;
        ofui.PreviousStatus = EOS_EFriendsStatus::EOS_FS_NotFriends;
        ofui.CurrentStatus  = new_status;
        n->GetFunc()(n->GetFuncParam());
    }
    return true;
}

}