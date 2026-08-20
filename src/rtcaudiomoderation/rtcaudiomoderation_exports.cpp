
#include "utils/common_includes.h"
#include "sdk/eos_rtc.h"

#ifndef EOS_INVALID_NOTIFICATIONID
#define EOS_INVALID_NOTIFICATIONID 0
#endif

EOS_DECLARE_FUNC(EOS_NotificationId) EOS_RTCAudioModeration_AddNotifyParticipantStatusChanged(
    void* Handle, const void* Options, void* ClientData, void* NotificationFn)
{
    return EOS_INVALID_NOTIFICATIONID;
}

EOS_DECLARE_FUNC(void) EOS_RTCAudioModeration_RemoveNotifyParticipantStatusChanged(
    void* Handle, EOS_NotificationId NotificationId)
{
    (void)Handle; (void)NotificationId;
}

EOS_DECLARE_FUNC(EOS_EResult) EOS_RTCAudioModeration_CopyRecordInformation(
    void* Handle, const void* Options, void** OutRecordInformation)
{
    if (OutRecordInformation) *OutRecordInformation = nullptr;
    return EOS_EResult::EOS_NotImplemented;
}

EOS_DECLARE_FUNC(EOS_EResult) EOS_RTCAudioModeration_Create(
    void* Handle, const void* Options, void** OutModeration)
{
    if (OutModeration) *OutModeration = nullptr;
    return EOS_EResult::EOS_NotImplemented;
}

EOS_DECLARE_FUNC(EOS_EResult) EOS_RTCAudioModeration_CreateBuffer(
    void* Handle, const void* Options, void** OutBuffer)
{
    if (OutBuffer) *OutBuffer = nullptr;
    return EOS_EResult::EOS_NotImplemented;
}

EOS_DECLARE_FUNC(void) EOS_RTCAudioModeration_Destroy(void* Handle)
{
    (void)Handle;
}

EOS_DECLARE_FUNC(void) EOS_RTCAudioModeration_DestroyBuffer(void* Handle, void* Buffer)
{
    (void)Handle; (void)Buffer;
}

EOS_DECLARE_FUNC(void) EOS_RTCAudioModeration_QueryRecordInformation(
    void* Handle, const void* Options, void* ClientData, void* CompletionDelegate)
{
    (void)Handle; (void)Options; (void)ClientData; (void)CompletionDelegate;
}

EOS_DECLARE_FUNC(void) EOS_RTCAudioModeration_RecordInformation_Release(void* RecordInformation)
{
    (void)RecordInformation;
}

EOS_DECLARE_FUNC(EOS_EResult) EOS_RTCAudioModeration_Resume(
    void* Handle, const void* Options)
{
    return EOS_EResult::EOS_NotImplemented;
}

EOS_DECLARE_FUNC(EOS_EResult) EOS_RTCAudioModeration_StartUpload(
    void* Handle, const void* Options, void* Buffer)
{
    return EOS_EResult::EOS_NotImplemented;
}

EOS_DECLARE_FUNC(EOS_EResult) EOS_RTCAudioModeration_StopUpload(
    void* Handle, const void* Options)
{
    return EOS_EResult::EOS_NotImplemented;
}

EOS_DECLARE_FUNC(EOS_EResult) EOS_RTCAudioModeration_Suspend(
    void* Handle, const void* Options)
{
    return EOS_EResult::EOS_NotImplemented;
}

EOS_DECLARE_FUNC(EOS_EResult) EOS_RTCAudioModeration_UpdateSendingSignature(
    void* Handle, const void* Options)
{
    return EOS_EResult::EOS_NotImplemented;
}
