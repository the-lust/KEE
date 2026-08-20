

#include "rtc/eossdk_rtc.h"
#include "eossdk_platform.h"
#include "eos_client_api.h"
#include "settings.h"

namespace sdk {

EOSSDK_RTC::EOSSDK_RTC()
{
    GetCB_Manager().register_callbacks(this);
}

EOSSDK_RTC::~EOSSDK_RTC()
{
    GetCB_Manager().unregister_callbacks(this);
    GetCB_Manager().remove_all_notifications(this);
}

void EOSSDK_RTC::JoinRoom(
    const EOS_RTC_JoinRoomOptions*  Options,
    void*                           ClientData,
    EOS_RTC_OnJoinRoomCallback      CompletionDelegate)
{
    if (!CompletionDelegate) return;

    pFrameResult_t res(new FrameResult);
    auto& cb = res->CreateCallback<EOS_RTC_JoinRoomCallbackInfo>(
        [CompletionDelegate](void* data) {
            CompletionDelegate(reinterpret_cast<EOS_RTC_JoinRoomCallbackInfo*>(data));
        });
    cb.ResultCode  = EOS_EResult::EOS_Success;
    cb.ClientData  = ClientData;
    cb.LocalUserId = Settings::Inst().productuserid;
    cb.RoomName    = nullptr;
    cb.RoomOptionsCount = 0;
    cb.RoomOptions = nullptr;
    SE_CB_END(this);
}

void EOSSDK_RTC::LeaveRoom(
    const EOS_RTC_LeaveRoomOptions* Options,
    void*                           ClientData,
    EOS_RTC_OnLeaveRoomCallback     CompletionDelegate)
{
    if (!CompletionDelegate) return;

    pFrameResult_t res(new FrameResult);
    auto& cb = res->CreateCallback<EOS_RTC_LeaveRoomCallbackInfo>(
        [CompletionDelegate](void* data) {
            CompletionDelegate(reinterpret_cast<EOS_RTC_LeaveRoomCallbackInfo*>(data));
        });
    cb.ResultCode  = EOS_EResult::EOS_Success;
    cb.ClientData  = ClientData;
    cb.LocalUserId = Settings::Inst().productuserid;
    cb.RoomName    = nullptr;
    SE_CB_END(this);
}

void EOSSDK_RTC::BlockParticipant(
    const EOS_RTC_BlockParticipantOptions*  Options,
    void*                                   ClientData,
    EOS_RTC_OnBlockParticipantCallback      CompletionDelegate)
{
    if (!CompletionDelegate) return;

    pFrameResult_t res(new FrameResult);
    auto& cb = res->CreateCallback<EOS_RTC_BlockParticipantCallbackInfo>(
        [CompletionDelegate](void* data) {
            CompletionDelegate(reinterpret_cast<EOS_RTC_BlockParticipantCallbackInfo*>(data));
        });
    cb.ResultCode   = EOS_EResult::EOS_Success;
    cb.ClientData   = ClientData;
    cb.LocalUserId  = Settings::Inst().productuserid;
    cb.RoomName     = nullptr;
    cb.ParticipantId= Options ? Options->ParticipantId : nullptr;
    cb.bBlocked     = Options ? Options->bBlocked : EOS_FALSE;
    SE_CB_END(this);
}

EOS_EResult EOSSDK_RTC::AddNotifyDisconnected(
    const EOS_RTC_AddNotifyDisconnectedOptions* Options,
    void*                                       ClientData,
    EOS_RTC_OnDisconnectedCallback              CompletionDelegate,
    EOS_NotificationId*                         OutNotificationId)
{
    if (!CompletionDelegate) { if (OutNotificationId) *OutNotificationId = EOS_INVALID_NOTIFICATIONID; return EOS_EResult::EOS_InvalidParameters; }

    pFrameResult_t res(new FrameResult);
    auto& cb = res->CreateCallback<EOS_RTC_DisconnectedCallbackInfo>(
        [CompletionDelegate](void* data) {
            CompletionDelegate(reinterpret_cast<EOS_RTC_DisconnectedCallbackInfo*>(data));
        });
    cb.ClientData  = ClientData;
    cb.LocalUserId = Settings::Inst().productuserid;
    cb.RoomName    = nullptr;

    EOS_NotificationId id = GetCB_Manager().add_notification(this, res);
    if (OutNotificationId) *OutNotificationId = id;
    return EOS_EResult::EOS_Success;
}

void EOSSDK_RTC::RemoveNotifyDisconnected(EOS_NotificationId NotificationId)
{
    GetCB_Manager().remove_notification(this, NotificationId);
}

EOS_EResult EOSSDK_RTC::AddNotifyParticipantStatusChanged(
    const EOS_RTC_AddNotifyParticipantStatusChangedOptions* Options,
    void*                                                   ClientData,
    EOS_RTC_OnParticipantStatusChangedCallback              CompletionDelegate,
    EOS_NotificationId*                                     OutNotificationId)
{
    if (!CompletionDelegate) { if (OutNotificationId) *OutNotificationId = EOS_INVALID_NOTIFICATIONID; return EOS_EResult::EOS_InvalidParameters; }

    pFrameResult_t res(new FrameResult);
    auto& cb = res->CreateCallback<EOS_RTC_ParticipantStatusChangedCallbackInfo>(
        [CompletionDelegate](void* data) {
            CompletionDelegate(reinterpret_cast<EOS_RTC_ParticipantStatusChangedCallbackInfo*>(data));
        });
    cb.ClientData     = ClientData;
    cb.LocalUserId    = Settings::Inst().productuserid;
    cb.RoomName       = nullptr;
    cb.ParticipantId  = nullptr;
    cb.ParticipantStatus = EOS_ERTCParticipantStatus::EOS_RTCPS_Joined;
    cb.ParticipantMetadataCount = 0;
    cb.ParticipantMetadata = nullptr;
    cb.bParticipantInBlocklist = EOS_FALSE;

    EOS_NotificationId id = GetCB_Manager().add_notification(this, res);
    if (OutNotificationId) *OutNotificationId = id;
    return EOS_EResult::EOS_Success;
}

void EOSSDK_RTC::RemoveNotifyParticipantStatusChanged(EOS_NotificationId NotificationId)
{
    GetCB_Manager().remove_notification(this, NotificationId);
}

void* EOSSDK_RTC::GetAudioInterface()
{
    return nullptr;
}

bool EOSSDK_RTC::CBRunFrame()  { return false; }
bool EOSSDK_RTC::RunCallbacks(pFrameResult_t res)
{
    res->GetCallbackMsg().cb_func(res->GetFuncParam());
    return true;
}
void EOSSDK_RTC::FreeCallback(pFrameResult_t) {}

} 
