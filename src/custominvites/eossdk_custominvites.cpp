
#include "custominvites/eossdk_custominvites.h"
#include "eossdk_platform.h"
#include "settings.h"
#include "platform/eos_client_api.h"

namespace sdk {

EOSSDK_CustomInvites::EOSSDK_CustomInvites()
{
    GetCB_Manager().register_callbacks(this);
    GetNetwork().register_listener(this, 0,
        Network_Message_pb::MessagesCase::kCustomInvites);
}

EOSSDK_CustomInvites::~EOSSDK_CustomInvites()
{
    GetNetwork().unregister_listener(this, 0,
        Network_Message_pb::MessagesCase::kCustomInvites);
    GetCB_Manager().unregister_callbacks(this);
    GetCB_Manager().remove_all_notifications(this);
}

EOS_EResult EOSSDK_CustomInvites::SetCustomInvite(
    const EOS_CustomInvites_SetCustomInviteOptions* Options)
{
    if (!Options || !Options->Payload) return EOS_EResult::EOS_InvalidParameters;
    std::lock_guard<std::mutex> lk(m_mutex);
    m_payload = Options->Payload;
    return EOS_EResult::EOS_Success;
}

void EOSSDK_CustomInvites::SendCustomInvite(
    const EOS_CustomInvites_SendCustomInviteOptions* Options,
    void* ClientData,
    EOS_CustomInvites_OnSendCustomInviteCallback CompletionDelegate)
{
    if (!CompletionDelegate) return;

    string payload_copy;
    {
        std::lock_guard<std::mutex> lk(m_mutex);
        payload_copy = m_payload;
    }

    const string my_id =
        Settings::Inst().productuserid->to_string();

    if (Options && Options->TargetUserIds && Options->TargetUserIdsCount > 0)
    {
        for (uint32_t i = 0; i < Options->TargetUserIdsCount; ++i)
        {
            if (!Options->TargetUserIds[i]) continue;
            const string dest = Options->TargetUserIds[i]->to_string();

            Network_Message_pb msg;
            msg.set_source_id(my_id);
            msg.set_dest_id(dest);
            msg.set_game_id(Settings::Inst().appid);

            auto* ci_msg = msg.mutable_custom_invites();
            auto* ci     = ci_msg->mutable_custom_invite_send();
            ci->set_sender_id(my_id);
            ci->set_recipient_id(dest);
            ci->set_payload(payload_copy);

            GetNetwork().TCPSendTo(msg);
        }
    }

    pFrameResult_t res(new FrameResult);
    EOS_CustomInvites_SendCustomInviteCallbackInfo& cb =
        res->CreateCallback<EOS_CustomInvites_SendCustomInviteCallbackInfo>(
            (CallbackFunc)CompletionDelegate);
    cb.ResultCode         = EOS_EResult::EOS_Success;
    cb.ClientData         = ClientData;
    cb.LocalUserId        = Settings::Inst().productuserid;
    cb.TargetUserIds      = nullptr;
    cb.TargetUserIdsCount = 0;
    SE_CB_END(this);
}

bool EOSSDK_CustomInvites::RunNetwork(Network_Message_pb const& msg)
{
    if (msg.messages_case() != Network_Message_pb::MessagesCase::kCustomInvites)
        return false;

    const CustomInvites_Message_pb& cm = msg.custom_invites();
    if (cm.message_case() != CustomInvites_Message_pb::kCustomInviteSend)
        return false;

    return on_custom_invite_received(msg, cm.custom_invite_send());
}

bool EOSSDK_CustomInvites::on_custom_invite_received(
    Network_Message_pb const& ,
    CustomInvite_Send_pb const& inv)
{
    
    const string my_id = Settings::Inst().productuserid->to_string();
    if (inv.recipient_id() != my_id) return false;

    EOS_ProductUserId sender_puid = GetProductUserId(inv.sender_id());

    static thread_local string s_payload_cache;
    s_payload_cache = inv.payload();

    auto notifs = GetCB_Manager().get_notifications(
        this, EOS_CustomInvites_OnCustomInviteReceivedCallbackInfo::k_iCallback);
    for (auto& n : notifs)
    {
        EOS_CustomInvites_OnCustomInviteReceivedCallbackInfo& cb =
            n->GetCallback<EOS_CustomInvites_OnCustomInviteReceivedCallbackInfo>();
        cb.Payload      = s_payload_cache.c_str();
        cb.TargetUserId = sender_puid;
        cb.LocalUserId  = Settings::Inst().productuserid;
        n->GetFunc()(n->GetFuncParam());
    }
    return true;
}

EOS_EResult EOSSDK_CustomInvites::FinalizeInvite(
    const EOS_CustomInvites_FinalizeInviteOptions*)
{
    return EOS_EResult::EOS_Success;
}

EOS_NotificationId EOSSDK_CustomInvites::AddNotifyCustomInviteReceived(
    const EOS_CustomInvites_AddNotifyCustomInviteReceivedOptions*,
    void* ClientData,
    EOS_CustomInvites_OnCustomInviteReceivedCallback NotificationFn)
{
    if (!NotificationFn) return EOS_INVALID_NOTIFICATIONID;
    pFrameResult_t res(new FrameResult);
    EOS_CustomInvites_OnCustomInviteReceivedCallbackInfo& cb =
        res->CreateCallback<EOS_CustomInvites_OnCustomInviteReceivedCallbackInfo>(
            (CallbackFunc)NotificationFn);
    cb.ClientData   = ClientData;
    cb.LocalUserId  = Settings::Inst().productuserid;
    cb.TargetUserId = nullptr;
    cb.Payload      = "";
    return GetCB_Manager().add_notification(this, res);
}

void EOSSDK_CustomInvites::RemoveNotifyCustomInviteReceived(EOS_NotificationId InId)
{
    GetCB_Manager().remove_notification(this, InId);
}

EOS_NotificationId EOSSDK_CustomInvites::AddNotifyCustomInviteAccepted(
    const EOS_CustomInvites_AddNotifyCustomInviteAcceptedOptions*,
    void* ClientData,
    EOS_CustomInvites_OnCustomInviteAcceptedCallback NotificationFn)
{
    if (!NotificationFn) return EOS_INVALID_NOTIFICATIONID;
    pFrameResult_t res(new FrameResult);
    EOS_CustomInvites_OnCustomInviteAcceptedCallbackInfo& cb =
        res->CreateCallback<EOS_CustomInvites_OnCustomInviteAcceptedCallbackInfo>(
            (CallbackFunc)NotificationFn);
    cb.ClientData      = ClientData;
    cb.LocalUserId     = Settings::Inst().productuserid;
    cb.TargetUserId    = nullptr;
    cb.CustomInviteId  = "";
    cb.Payload         = "";
    return GetCB_Manager().add_notification(this, res);
}

void EOSSDK_CustomInvites::RemoveNotifyCustomInviteAccepted(EOS_NotificationId InId)
{
    GetCB_Manager().remove_notification(this, InId);
}

EOS_NotificationId EOSSDK_CustomInvites::AddNotifyCustomInviteRejected(
    const EOS_CustomInvites_AddNotifyCustomInviteRejectedOptions*,
    void* ClientData,
    EOS_CustomInvites_OnCustomInviteRejectedCallback NotificationFn)
{
    if (!NotificationFn) return EOS_INVALID_NOTIFICATIONID;
    pFrameResult_t res(new FrameResult);
    EOS_CustomInvites_CustomInviteRejectedCallbackInfo& cb =
        res->CreateCallback<EOS_CustomInvites_CustomInviteRejectedCallbackInfo>(
            (CallbackFunc)NotificationFn);
    cb.ClientData   = ClientData;
    cb.LocalUserId  = Settings::Inst().productuserid;
    cb.TargetUserId = nullptr;
    cb.Payload      = "";
    return GetCB_Manager().add_notification(this, res);
}

void EOSSDK_CustomInvites::RemoveNotifyCustomInviteRejected(EOS_NotificationId InId)
{
    GetCB_Manager().remove_notification(this, InId);
}

bool EOSSDK_CustomInvites::CBRunFrame()  { return false; }

bool EOSSDK_CustomInvites::RunCallbacks(pFrameResult_t res)
{
    lock _(GLOBAL_MUTEX);
    return res->done;
}

void EOSSDK_CustomInvites::FreeCallback(pFrameResult_t res)
{
    (void)res;
}

} 
