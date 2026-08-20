
#include "utils/common_includes.h"
#include "rtc/eossdk_rtc.h"
#include "eossdk_platform.h"

EOS_DECLARE_FUNC(EOS_HRTCAudio) EOS_RTC_GetAudioInterface(EOS_HRTC Handle)
{
    if (!Handle) return nullptr;
    return (EOS_HRTCAudio)GetEOS_RTC().GetAudioInterface();
}

EOS_DECLARE_FUNC(EOS_HRTCData) EOS_RTC_GetDataInterface(EOS_HRTC)
{
    return nullptr;
}

EOS_DECLARE_FUNC(void) EOS_RTC_JoinRoom(
    EOS_HRTC Handle,
    const EOS_RTC_JoinRoomOptions* Options,
    void* ClientData,
    const EOS_RTC_OnJoinRoomCallback CompletionDelegate)
{
    if (Handle) GetEOS_RTC().JoinRoom(Options, ClientData, CompletionDelegate);
}

EOS_DECLARE_FUNC(void) EOS_RTC_LeaveRoom(
    EOS_HRTC Handle,
    const EOS_RTC_LeaveRoomOptions* Options,
    void* ClientData,
    const EOS_RTC_OnLeaveRoomCallback CompletionDelegate)
{
    if (Handle) GetEOS_RTC().LeaveRoom(Options, ClientData, CompletionDelegate);
}

EOS_DECLARE_FUNC(void) EOS_RTC_BlockParticipant(
    EOS_HRTC Handle,
    const EOS_RTC_BlockParticipantOptions* Options,
    void* ClientData,
    const EOS_RTC_OnBlockParticipantCallback CompletionDelegate)
{
    if (Handle) GetEOS_RTC().BlockParticipant(Options, ClientData, CompletionDelegate);
}

EOS_DECLARE_FUNC(EOS_NotificationId) EOS_RTC_AddNotifyDisconnected(
    EOS_HRTC Handle,
    const EOS_RTC_AddNotifyDisconnectedOptions* Options,
    void* ClientData,
    const EOS_RTC_OnDisconnectedCallback CompletionDelegate)
{
    EOS_NotificationId id = EOS_INVALID_NOTIFICATIONID;
    if (Handle) GetEOS_RTC().AddNotifyDisconnected(Options, ClientData, CompletionDelegate, &id);
    return id;
}

EOS_DECLARE_FUNC(void) EOS_RTC_RemoveNotifyDisconnected(EOS_HRTC Handle, EOS_NotificationId NotificationId)
{
    if (Handle) GetEOS_RTC().RemoveNotifyDisconnected(NotificationId);
}

EOS_DECLARE_FUNC(EOS_NotificationId) EOS_RTC_AddNotifyParticipantStatusChanged(
    EOS_HRTC Handle,
    const EOS_RTC_AddNotifyParticipantStatusChangedOptions* Options,
    void* ClientData,
    const EOS_RTC_OnParticipantStatusChangedCallback CompletionDelegate)
{
    EOS_NotificationId id = EOS_INVALID_NOTIFICATIONID;
    if (Handle) GetEOS_RTC().AddNotifyParticipantStatusChanged(Options, ClientData, CompletionDelegate, &id);
    return id;
}

EOS_DECLARE_FUNC(void) EOS_RTC_RemoveNotifyParticipantStatusChanged(EOS_HRTC Handle, EOS_NotificationId NotificationId)
{
    if (Handle) GetEOS_RTC().RemoveNotifyParticipantStatusChanged(NotificationId);
}

EOS_DECLARE_FUNC(EOS_EResult) EOS_RTC_SetSetting(EOS_HRTC, const EOS_RTC_SetSettingOptions*)
    { return EOS_EResult::EOS_Success; }

EOS_DECLARE_FUNC(EOS_EResult) EOS_RTC_SetRoomSetting(EOS_HRTC, const EOS_RTC_SetRoomSettingOptions*)
    { return EOS_EResult::EOS_Success; }

EOS_DECLARE_FUNC(EOS_NotificationId) EOS_RTC_AddNotifyRoomBeforeJoin(
    EOS_HRTC, const EOS_RTC_AddNotifyRoomBeforeJoinOptions*,
    void*, const EOS_RTC_OnRoomBeforeJoinCallback)
    { return static_cast<EOS_NotificationId>(1); }

EOS_DECLARE_FUNC(void) EOS_RTC_RemoveNotifyRoomBeforeJoin(EOS_HRTC, EOS_NotificationId) {}

EOS_DECLARE_FUNC(EOS_NotificationId) EOS_RTC_AddNotifyRoomStatisticsUpdated(
    EOS_HRTC, const EOS_RTC_AddNotifyRoomStatisticsUpdatedOptions*,
    void*, const EOS_RTC_OnRoomStatisticsUpdatedCallback)
    { return static_cast<EOS_NotificationId>(1); }

EOS_DECLARE_FUNC(void) EOS_RTC_RemoveNotifyRoomStatisticsUpdated(EOS_HRTC, EOS_NotificationId) {}

extern "C" EOS_DECLARE_FUNC(void*) EOS_RTC_AddNotifyConnectionState()    { return nullptr; }
extern "C" EOS_DECLARE_FUNC(void*) EOS_RTC_RemoveNotifyConnectionState() { return nullptr; }
extern "C" EOS_DECLARE_FUNC(void*) EOS_RTC_GetVideoInterface()           { return nullptr; }
