
#include "utils/common_includes.h"

#ifndef EOS_HRTCVideo
#define EOS_HRTCVideo void*
#endif
#ifndef EOS_INVALID_NOTIFICATIONID
#define EOS_INVALID_NOTIFICATIONID 0
#endif

EOS_DECLARE_FUNC(EOS_NotificationId) EOS_RTCVideo_AddNotifyParticipantUpdated(
    EOS_HRTCVideo Handle, const void* Options, void* ClientData, void* NotificationFn)
{
    (void)Handle; (void)Options; (void)ClientData; (void)NotificationFn;
    return EOS_INVALID_NOTIFICATIONID;
}

EOS_DECLARE_FUNC(void) EOS_RTCVideo_RemoveNotifyParticipantUpdated(
    EOS_HRTCVideo Handle, EOS_NotificationId NotificationId)
{
    (void)Handle; (void)NotificationId;
}

EOS_DECLARE_FUNC(EOS_NotificationId) EOS_RTCVideo_AddNotifyVideoReceived(
    EOS_HRTCVideo Handle, const void* Options, void* ClientData, void* NotificationFn)
{
    (void)Handle; (void)Options; (void)ClientData; (void)NotificationFn;
    return EOS_INVALID_NOTIFICATIONID;
}

EOS_DECLARE_FUNC(void) EOS_RTCVideo_RemoveNotifyVideoReceived(
    EOS_HRTCVideo Handle, EOS_NotificationId NotificationId)
{
    (void)Handle; (void)NotificationId;
}

EOS_DECLARE_FUNC(EOS_EResult) EOS_RTCVideo_SendVideo(
    EOS_HRTCVideo Handle, const void* Options)
{
    (void)Handle; (void)Options;
    return EOS_EResult::EOS_NotImplemented;
}

EOS_DECLARE_FUNC(EOS_EResult) EOS_RTCVideo_UpdateReceiving(
    EOS_HRTCVideo Handle, const void* Options)
{
    (void)Handle; (void)Options;
    return EOS_EResult::EOS_NotImplemented;
}

EOS_DECLARE_FUNC(EOS_EResult) EOS_RTCVideo_UpdateSending(
    EOS_HRTCVideo Handle, const void* Options)
{
    (void)Handle; (void)Options;
    return EOS_EResult::EOS_NotImplemented;
}

EOS_DECLARE_FUNC(void) EOS_RTCVideo_SetAdaptVideoFrameCallback(
    EOS_HRTCVideo Handle, const void* Options, void* ClientData, void* Callback)
{
    (void)Handle; (void)Options; (void)ClientData; (void)Callback;
}

EOS_DECLARE_FUNC(void) EOS_RTCVideo_SetVideoAllocationCallback(
    EOS_HRTCVideo Handle, const void* Options, void* ClientData, void* Callback)
{
    (void)Handle; (void)Options; (void)ClientData; (void)Callback;
}

EOS_DECLARE_FUNC(void) EOS_RTCVideo_SetVideoReleaseCallback(
    EOS_HRTCVideo Handle, const void* Options, void* ClientData, void* Callback)
{
    (void)Handle; (void)Options; (void)ClientData; (void)Callback;
}

EOS_DECLARE_FUNC(EOS_EResult) EOS_RTCVideo_CreateOutgoingVideoFrameFormat(
    EOS_HRTCVideo Handle, const void* Options, void** OutVideoFrameFormat)
{
    (void)Handle; (void)Options;
    if (OutVideoFrameFormat) *OutVideoFrameFormat = nullptr;
    return EOS_EResult::EOS_NotImplemented;
}
