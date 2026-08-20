
#include "utils/common_includes.h"

EOS_DECLARE_FUNC(EOS_NotificationId) EOS_RTCData_AddNotifyDataReceived(EOS_HRTCData Handle, const EOS_RTCData_AddNotifyDataReceivedOptions* Options, void* ClientData, const EOS_RTCData_OnDataReceivedCallback CompletionDelegate)
{
    return EOS_INVALID_NOTIFICATIONID;
}

EOS_DECLARE_FUNC(void) EOS_RTCData_RemoveNotifyDataReceived(EOS_HRTCData Handle, EOS_NotificationId NotificationId)
{
}

EOS_DECLARE_FUNC(EOS_EResult) EOS_RTCData_SendData(EOS_HRTCData Handle, const EOS_RTCData_SendDataOptions* Options)
{
    return EOS_EResult::EOS_Success;
}

EOS_DECLARE_FUNC(void) EOS_RTCData_UpdateSending(EOS_HRTCData Handle, const EOS_RTCData_UpdateSendingOptions* Options, void* ClientData, const EOS_RTCData_OnUpdateSendingCallback CompletionDelegate)
{
}

EOS_DECLARE_FUNC(void) EOS_RTCData_UpdateReceiving(EOS_HRTCData Handle, const EOS_RTCData_UpdateReceivingOptions* Options, void* ClientData, const EOS_RTCData_OnUpdateReceivingCallback CompletionDelegate)
{
}

EOS_DECLARE_FUNC(EOS_NotificationId) EOS_RTCData_AddNotifyParticipantUpdated(EOS_HRTCData Handle, const EOS_RTCData_AddNotifyParticipantUpdatedOptions* Options, void* ClientData, const EOS_RTCData_OnParticipantUpdatedCallback CompletionDelegate)
{
    return EOS_INVALID_NOTIFICATIONID;
}

EOS_DECLARE_FUNC(void) EOS_RTCData_RemoveNotifyParticipantUpdated(EOS_HRTCData Handle, EOS_NotificationId NotificationId)
{
}
