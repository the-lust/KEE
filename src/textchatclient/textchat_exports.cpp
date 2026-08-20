
#include "utils/compat_types.h"
#include "utils/common_includes.h"
#include "eossdk_platform.h"

EOS_DECLARE_FUNC(void) EOS_TextChat_SendMessage(
    EOS_HTextChat Handle,
    const EOS_TextChat_SendMessageOptions* Options,
    void* ClientData,
    const EOS_TextChat_OnSendMessageCallback CompletionDelegate)
{
    if (!CompletionDelegate) return;
    EOS_TextChat_SendMessageCallbackInfo info{};
    info.ResultCode = EOS_EResult::EOS_Success;
    info.ClientData = ClientData;
    info.LocalUserId = nullptr;
    CompletionDelegate(&info);
}

EOS_DECLARE_FUNC(EOS_NotificationId) EOS_TextChat_AddNotifyMessageReceived(
    EOS_HTextChat Handle,
    const EOS_TextChat_AddNotifyMessageReceivedOptions* Options,
    void* ClientData,
    const EOS_TextChat_OnMessageReceivedCallback NotificationFn)
{

    return static_cast<EOS_NotificationId>(1);
}

EOS_DECLARE_FUNC(void) EOS_TextChat_RemoveNotifyMessageReceived(EOS_HTextChat Handle, EOS_NotificationId NotificationId) {}

EOS_DECLARE_FUNC(EOS_EResult) EOS_TextChat_SetTextChatOptions(EOS_HTextChat Handle, const EOS_TextChat_SetTextChatOptionsOptions* Options)
{ return EOS_EResult::EOS_Success; }

EOS_DECLARE_FUNC(void) EOS_TextChatServer_SendMessage(
    EOS_HTextChatServer Handle,
    const EOS_TextChatServer_SendMessageOptions* Options,
    void* ClientData,
    const EOS_TextChatServer_OnSendMessageCallback CompletionDelegate)
{
    if (!CompletionDelegate) return;
    EOS_TextChatServer_SendMessageCallbackInfo info{};
    info.ResultCode = EOS_EResult::EOS_Success;
    info.ClientData = ClientData;
    CompletionDelegate(&info);
}

EOS_DECLARE_FUNC(EOS_NotificationId) EOS_TextChatServer_AddNotifyMessageReceived(
    EOS_HTextChatServer Handle,
    const EOS_TextChatServer_AddNotifyMessageReceivedOptions* Options,
    void* ClientData,
    const EOS_TextChatServer_OnMessageReceivedCallback NotificationFn)
{ return static_cast<EOS_NotificationId>(1); }

EOS_DECLARE_FUNC(void) EOS_TextChatServer_RemoveNotifyMessageReceived(EOS_HTextChatServer Handle, EOS_NotificationId NotificationId) {}
