#include "utils/common_includes.h"
#include "custominvites/eossdk_custominvites.h"
#include "eossdk_platform.h"

EOS_DECLARE_FUNC(EOS_EResult) EOS_CustomInvites_SetCustomInvite(
    EOS_HCustomInvites Handle,
    const EOS_CustomInvites_SetCustomInviteOptions* Options)
{
    return GetEOS_CustomInvites().SetCustomInvite(Options);
}

EOS_DECLARE_FUNC(void) EOS_CustomInvites_SendCustomInvite(
    EOS_HCustomInvites Handle,
    const EOS_CustomInvites_SendCustomInviteOptions* Options,
    void* ClientData,
    const EOS_CustomInvites_OnSendCustomInviteCallback CompletionDelegate)
{
    GetEOS_CustomInvites().SendCustomInvite(Options, ClientData, CompletionDelegate);
}

EOS_DECLARE_FUNC(EOS_EResult) EOS_CustomInvites_FinalizeInvite(
    EOS_HCustomInvites Handle,
    const EOS_CustomInvites_FinalizeInviteOptions* Options)
{
    return GetEOS_CustomInvites().FinalizeInvite(Options);
}

EOS_DECLARE_FUNC(EOS_NotificationId) EOS_CustomInvites_AddNotifyCustomInviteReceived(
    EOS_HCustomInvites Handle,
    const EOS_CustomInvites_AddNotifyCustomInviteReceivedOptions* Options,
    void* ClientData,
    const EOS_CustomInvites_OnCustomInviteReceivedCallback NotificationFn)
{
    return GetEOS_CustomInvites().AddNotifyCustomInviteReceived(Options, ClientData, NotificationFn);
}

EOS_DECLARE_FUNC(void) EOS_CustomInvites_RemoveNotifyCustomInviteReceived(
    EOS_HCustomInvites Handle, EOS_NotificationId InId)
{
    GetEOS_CustomInvites().RemoveNotifyCustomInviteReceived(InId);
}

EOS_DECLARE_FUNC(EOS_NotificationId) EOS_CustomInvites_AddNotifyCustomInviteAccepted(
    EOS_HCustomInvites Handle,
    const EOS_CustomInvites_AddNotifyCustomInviteAcceptedOptions* Options,
    void* ClientData,
    const EOS_CustomInvites_OnCustomInviteAcceptedCallback NotificationFn)
{
    return GetEOS_CustomInvites().AddNotifyCustomInviteAccepted(Options, ClientData, NotificationFn);
}

EOS_DECLARE_FUNC(void) EOS_CustomInvites_RemoveNotifyCustomInviteAccepted(
    EOS_HCustomInvites Handle, EOS_NotificationId InId)
{
    GetEOS_CustomInvites().RemoveNotifyCustomInviteAccepted(InId);
}

EOS_DECLARE_FUNC(EOS_NotificationId) EOS_CustomInvites_AddNotifyCustomInviteRejected(
    EOS_HCustomInvites Handle,
    const EOS_CustomInvites_AddNotifyCustomInviteRejectedOptions* Options,
    void* ClientData,
    const EOS_CustomInvites_OnCustomInviteRejectedCallback NotificationFn)
{
    return GetEOS_CustomInvites().AddNotifyCustomInviteRejected(Options, ClientData, NotificationFn);
}

EOS_DECLARE_FUNC(void) EOS_CustomInvites_RemoveNotifyCustomInviteRejected(
    EOS_HCustomInvites Handle, EOS_NotificationId InId)
{
    GetEOS_CustomInvites().RemoveNotifyCustomInviteRejected(InId);
}

EOS_DECLARE_FUNC(void) EOS_CustomInvites_RejectRequestToJoin(
    EOS_HCustomInvites Handle,
    const EOS_CustomInvites_RejectRequestToJoinOptions* Options,
    void* ClientData,
    const EOS_CustomInvites_OnRejectRequestToJoinCallback CompletionDelegate)
{
    if (CompletionDelegate) {
        EOS_CustomInvites_RejectRequestToJoinCallbackInfo info{};
        info.ResultCode = EOS_EResult::EOS_Success;
        info.ClientData = ClientData;
        CompletionDelegate(&info);
    }
}

EOS_DECLARE_FUNC(void) EOS_CustomInvites_RemoveNotifyRequestToJoinReceived(
    EOS_HCustomInvites Handle, EOS_NotificationId InId)
{
}

EOS_DECLARE_FUNC(EOS_NotificationId) EOS_CustomInvites_AddNotifyRequestToJoinReceived(
    EOS_HCustomInvites Handle,
    const EOS_CustomInvites_AddNotifyRequestToJoinReceivedOptions* Options,
    void* ClientData,
    const EOS_CustomInvites_OnRequestToJoinReceivedCallback NotificationFn)
{
    return EOS_INVALID_NOTIFICATIONID;
}

EOS_DECLARE_FUNC(void) EOS_CustomInvites_AcceptRequestToJoin(
    EOS_HCustomInvites Handle,
    const EOS_CustomInvites_AcceptRequestToJoinOptions* Options,
    void* ClientData,
    const EOS_CustomInvites_OnAcceptRequestToJoinCallback CompletionDelegate)
{
    if (CompletionDelegate) {
        EOS_CustomInvites_AcceptRequestToJoinCallbackInfo info{};
        info.ResultCode = EOS_EResult::EOS_Success;
        info.ClientData = ClientData;
        CompletionDelegate(&info);
    }
}

EOS_DECLARE_FUNC(EOS_NotificationId) EOS_CustomInvites_AddNotifyRequestToJoinAccepted(
    EOS_HCustomInvites Handle,
    const EOS_CustomInvites_AddNotifyRequestToJoinAcceptedOptions* Options,
    void* ClientData,
    const EOS_CustomInvites_OnRequestToJoinAcceptedCallback NotificationFn)
{
    return EOS_INVALID_NOTIFICATIONID;
}

EOS_DECLARE_FUNC(void) EOS_CustomInvites_RemoveNotifyRequestToJoinAccepted(
    EOS_HCustomInvites Handle, EOS_NotificationId InId)
{
}

EOS_DECLARE_FUNC(EOS_NotificationId) EOS_CustomInvites_AddNotifyRequestToJoinRejected(
    EOS_HCustomInvites Handle,
    const EOS_CustomInvites_AddNotifyRequestToJoinRejectedOptions* Options,
    void* ClientData,
    const EOS_CustomInvites_OnRequestToJoinRejectedCallback NotificationFn)
{
    return EOS_INVALID_NOTIFICATIONID;
}

EOS_DECLARE_FUNC(void) EOS_CustomInvites_RemoveNotifyRequestToJoinRejected(
    EOS_HCustomInvites Handle, EOS_NotificationId InId)
{
}

EOS_DECLARE_FUNC(EOS_NotificationId) EOS_CustomInvites_AddNotifyRequestToJoinResponseReceived(
    EOS_HCustomInvites Handle,
    const EOS_CustomInvites_AddNotifyRequestToJoinResponseReceivedOptions* Options,
    void* ClientData,
    const EOS_CustomInvites_OnRequestToJoinResponseReceivedCallback NotificationFn)
{
    return EOS_INVALID_NOTIFICATIONID;
}

EOS_DECLARE_FUNC(void) EOS_CustomInvites_RemoveNotifyRequestToJoinResponseReceived(
    EOS_HCustomInvites Handle, EOS_NotificationId InId)
{
}

EOS_DECLARE_FUNC(void) EOS_CustomInvites_SendRequestToJoin(
    EOS_HCustomInvites Handle,
    const EOS_CustomInvites_SendRequestToJoinOptions* Options,
    void* ClientData,
    const EOS_CustomInvites_OnSendRequestToJoinCallback CompletionDelegate)
{
    if (CompletionDelegate) {
        EOS_CustomInvites_SendRequestToJoinCallbackInfo info{};
        info.ResultCode = EOS_EResult::EOS_Success;
        info.ClientData = ClientData;
        CompletionDelegate(&info);
    }
}

EOS_DECLARE_FUNC(EOS_NotificationId) EOS_CustomInvites_AddNotifySendCustomNativeInviteRequested(
    EOS_HCustomInvites Handle,
    const EOS_CustomInvites_AddNotifySendCustomNativeInviteRequestedOptions* Options,
    void* ClientData,
    const EOS_CustomInvites_OnSendCustomNativeInviteRequestedCallback NotificationFn)
{
    return EOS_INVALID_NOTIFICATIONID;
}

EOS_DECLARE_FUNC(void) EOS_CustomInvites_RemoveNotifySendCustomNativeInviteRequested(
    EOS_HCustomInvites Handle, EOS_NotificationId InId)
{
}
