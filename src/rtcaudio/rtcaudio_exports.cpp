
#include "utils/common_includes.h"
#include "eossdk_platform.h"

EOS_DECLARE_FUNC(EOS_EResult) EOS_RTCAudio_SendAudio(EOS_HRTCAudio, const EOS_RTCAudio_SendAudioOptions*)
    { return EOS_EResult::EOS_Success; }

EOS_DECLARE_FUNC(void) EOS_RTCAudio_UpdateSending(
    EOS_HRTCAudio, const EOS_RTCAudio_UpdateSendingOptions*,
    void* ClientData, const EOS_RTCAudio_OnUpdateSendingCallback Cb)
{
    if (!Cb) return;
    EOS_RTCAudio_UpdateSendingCallbackInfo info{};
    info.ResultCode = EOS_EResult::EOS_Success;
    info.ClientData = ClientData;
    Cb(&info);
}

EOS_DECLARE_FUNC(void) EOS_RTCAudio_UpdateReceiving(
    EOS_HRTCAudio, const EOS_RTCAudio_UpdateReceivingOptions*,
    void* ClientData, const EOS_RTCAudio_OnUpdateReceivingCallback Cb)
{
    if (!Cb) return;
    EOS_RTCAudio_UpdateReceivingCallbackInfo info{};
    info.ResultCode = EOS_EResult::EOS_Success;
    info.ClientData = ClientData;
    Cb(&info);
}

EOS_DECLARE_FUNC(void) EOS_RTCAudio_UpdateSendingVolume(
    EOS_HRTCAudio, const EOS_RTCAudio_UpdateSendingVolumeOptions*,
    void* ClientData, const EOS_RTCAudio_OnUpdateSendingVolumeCallback Cb)
{
    if (!Cb) return;
    EOS_RTCAudio_UpdateSendingVolumeCallbackInfo info{};
    info.ResultCode = EOS_EResult::EOS_Success;
    info.ClientData = ClientData;
    Cb(&info);
}

EOS_DECLARE_FUNC(void) EOS_RTCAudio_UpdateReceivingVolume(
    EOS_HRTCAudio, const EOS_RTCAudio_UpdateReceivingVolumeOptions*,
    void* ClientData, const EOS_RTCAudio_OnUpdateReceivingVolumeCallback Cb)
{
    if (!Cb) return;
    EOS_RTCAudio_UpdateReceivingVolumeCallbackInfo info{};
    info.ResultCode = EOS_EResult::EOS_Success;
    info.ClientData = ClientData;
    Cb(&info);
}

EOS_DECLARE_FUNC(void) EOS_RTCAudio_UpdateParticipantVolume(
    EOS_HRTCAudio, const EOS_RTCAudio_UpdateParticipantVolumeOptions*,
    void* ClientData, const EOS_RTCAudio_OnUpdateParticipantVolumeCallback Cb)
{
    if (!Cb) return;
    EOS_RTCAudio_UpdateParticipantVolumeCallbackInfo info{};
    info.ResultCode = EOS_EResult::EOS_Success;
    info.ClientData = ClientData;
    Cb(&info);
}

EOS_DECLARE_FUNC(EOS_NotificationId) EOS_RTCAudio_AddNotifyParticipantUpdated(
    EOS_HRTCAudio, const EOS_RTCAudio_AddNotifyParticipantUpdatedOptions*,
    void*, const EOS_RTCAudio_OnParticipantUpdatedCallback)
    { return static_cast<EOS_NotificationId>(1); }

EOS_DECLARE_FUNC(void) EOS_RTCAudio_RemoveNotifyParticipantUpdated(EOS_HRTCAudio, EOS_NotificationId) {}

EOS_DECLARE_FUNC(EOS_NotificationId) EOS_RTCAudio_AddNotifyAudioDevicesChanged(
    EOS_HRTCAudio, const EOS_RTCAudio_AddNotifyAudioDevicesChangedOptions*,
    void*, const EOS_RTCAudio_OnAudioDevicesChangedCallback)
    { return static_cast<EOS_NotificationId>(1); }

EOS_DECLARE_FUNC(void) EOS_RTCAudio_RemoveNotifyAudioDevicesChanged(EOS_HRTCAudio, EOS_NotificationId) {}

EOS_DECLARE_FUNC(EOS_NotificationId) EOS_RTCAudio_AddNotifyAudioInputState(
    EOS_HRTCAudio, const EOS_RTCAudio_AddNotifyAudioInputStateOptions*,
    void*, const EOS_RTCAudio_OnAudioInputStateCallback)
    { return static_cast<EOS_NotificationId>(1); }

EOS_DECLARE_FUNC(void) EOS_RTCAudio_RemoveNotifyAudioInputState(EOS_HRTCAudio, EOS_NotificationId) {}

EOS_DECLARE_FUNC(EOS_NotificationId) EOS_RTCAudio_AddNotifyAudioOutputState(
    EOS_HRTCAudio, const EOS_RTCAudio_AddNotifyAudioOutputStateOptions*,
    void*, const EOS_RTCAudio_OnAudioOutputStateCallback)
    { return static_cast<EOS_NotificationId>(1); }

EOS_DECLARE_FUNC(void) EOS_RTCAudio_RemoveNotifyAudioOutputState(EOS_HRTCAudio, EOS_NotificationId) {}

EOS_DECLARE_FUNC(EOS_NotificationId) EOS_RTCAudio_AddNotifyAudioBeforeSend(
    EOS_HRTCAudio, const EOS_RTCAudio_AddNotifyAudioBeforeSendOptions*,
    void*, const EOS_RTCAudio_OnAudioBeforeSendCallback)
    { return static_cast<EOS_NotificationId>(1); }

EOS_DECLARE_FUNC(void) EOS_RTCAudio_RemoveNotifyAudioBeforeSend(EOS_HRTCAudio, EOS_NotificationId) {}

EOS_DECLARE_FUNC(EOS_NotificationId) EOS_RTCAudio_AddNotifyAudioBeforeRender(
    EOS_HRTCAudio, const EOS_RTCAudio_AddNotifyAudioBeforeRenderOptions*,
    void*, const EOS_RTCAudio_OnAudioBeforeRenderCallback)
    { return static_cast<EOS_NotificationId>(1); }

EOS_DECLARE_FUNC(void) EOS_RTCAudio_RemoveNotifyAudioBeforeRender(EOS_HRTCAudio, EOS_NotificationId) {}

EOS_DECLARE_FUNC(void) EOS_RTCAudio_RegisterPlatformUser(
    EOS_HRTCAudio, const EOS_RTCAudio_RegisterPlatformUserOptions*,
    void* ClientData, const EOS_RTCAudio_OnRegisterPlatformUserCallback Cb)
{
    if (!Cb) return;
    EOS_RTCAudio_OnRegisterPlatformUserCallbackInfo info{};
    info.ResultCode = EOS_EResult::EOS_Success;
    info.ClientData = ClientData;
    Cb(&info);
}

EOS_DECLARE_FUNC(void) EOS_RTCAudio_UnregisterPlatformUser(
    EOS_HRTCAudio, const EOS_RTCAudio_UnregisterPlatformUserOptions*,
    void* ClientData, const EOS_RTCAudio_OnUnregisterPlatformUserCallback Cb)
{
    if (!Cb) return;
    EOS_RTCAudio_OnUnregisterPlatformUserCallbackInfo info{};
    info.ResultCode = EOS_EResult::EOS_Success;
    info.ClientData = ClientData;
    Cb(&info);
}

EOS_DECLARE_FUNC(void) EOS_RTCAudio_QueryInputDevicesInformation(
    EOS_HRTCAudio, const EOS_RTCAudio_QueryInputDevicesInformationOptions*,
    void* ClientData, const EOS_RTCAudio_OnQueryInputDevicesInformationCallback Cb)
{
    if (!Cb) return;
    EOS_RTCAudio_OnQueryInputDevicesInformationCallbackInfo info{};
    info.ResultCode = EOS_EResult::EOS_Success;
    info.ClientData = ClientData;
    Cb(&info);
}

EOS_DECLARE_FUNC(uint32_t) EOS_RTCAudio_GetInputDevicesCount(EOS_HRTCAudio, const EOS_RTCAudio_GetInputDevicesCountOptions*)
    { return 1; }

EOS_DECLARE_FUNC(EOS_EResult) EOS_RTCAudio_CopyInputDeviceInformationByIndex(
    EOS_HRTCAudio, const EOS_RTCAudio_CopyInputDeviceInformationByIndexOptions* Opts,
    EOS_RTCAudio_InputDeviceInformation** Out)
{
    if (!Out) return EOS_EResult::EOS_InvalidParameters;
    *Out = nullptr;
    return EOS_EResult::EOS_NotFound;
}

EOS_DECLARE_FUNC(void) EOS_RTCAudio_QueryOutputDevicesInformation(
    EOS_HRTCAudio, const EOS_RTCAudio_QueryOutputDevicesInformationOptions*,
    void* ClientData, const EOS_RTCAudio_OnQueryOutputDevicesInformationCallback Cb)
{
    if (!Cb) return;
    EOS_RTCAudio_OnQueryOutputDevicesInformationCallbackInfo info{};
    info.ResultCode = EOS_EResult::EOS_Success;
    info.ClientData = ClientData;
    Cb(&info);
}

EOS_DECLARE_FUNC(uint32_t) EOS_RTCAudio_GetOutputDevicesCount(EOS_HRTCAudio, const EOS_RTCAudio_GetOutputDevicesCountOptions*)
    { return 1; }

EOS_DECLARE_FUNC(EOS_EResult) EOS_RTCAudio_CopyOutputDeviceInformationByIndex(
    EOS_HRTCAudio, const EOS_RTCAudio_CopyOutputDeviceInformationByIndexOptions* Opts,
    EOS_RTCAudio_OutputDeviceInformation** Out)
{
    if (!Out) return EOS_EResult::EOS_InvalidParameters;
    *Out = nullptr;
    return EOS_EResult::EOS_NotFound;
}

EOS_DECLARE_FUNC(void) EOS_RTCAudio_SetInputDeviceSettings(
    EOS_HRTCAudio, const EOS_RTCAudio_SetInputDeviceSettingsOptions*,
    void* ClientData, const EOS_RTCAudio_OnSetInputDeviceSettingsCallback Cb)
{
    if (!Cb) return;
    EOS_RTCAudio_OnSetInputDeviceSettingsCallbackInfo info{};
    info.ResultCode = EOS_EResult::EOS_Success;
    info.ClientData = ClientData;
    Cb(&info);
}

EOS_DECLARE_FUNC(void) EOS_RTCAudio_SetOutputDeviceSettings(
    EOS_HRTCAudio, const EOS_RTCAudio_SetOutputDeviceSettingsOptions*,
    void* ClientData, const EOS_RTCAudio_OnSetOutputDeviceSettingsCallback Cb)
{
    if (!Cb) return;
    EOS_RTCAudio_OnSetOutputDeviceSettingsCallbackInfo info{};
    info.ResultCode = EOS_EResult::EOS_Success;
    info.ClientData = ClientData;
    Cb(&info);
}

EOS_DECLARE_FUNC(EOS_EResult) EOS_RTCAudio_RegisterPlatformAudioUser(EOS_HRTCAudio, const EOS_RTCAudio_RegisterPlatformAudioUserOptions*)
    { return EOS_EResult::EOS_Success; }

EOS_DECLARE_FUNC(EOS_EResult) EOS_RTCAudio_UnregisterPlatformAudioUser(EOS_HRTCAudio, const EOS_RTCAudio_UnregisterPlatformAudioUserOptions*)
    { return EOS_EResult::EOS_Success; }

EOS_DECLARE_FUNC(uint32_t) EOS_RTCAudio_GetAudioInputDevicesCount(EOS_HRTCAudio, const EOS_RTCAudio_GetAudioInputDevicesCountOptions*)
    { return 1; }

EOS_DECLARE_FUNC(const EOS_RTCAudio_AudioInputDeviceInfo*) EOS_RTCAudio_GetAudioInputDeviceByIndex(EOS_HRTCAudio, const EOS_RTCAudio_GetAudioInputDeviceByIndexOptions* Opts)
{
    (void)Opts;
    return nullptr;
}

EOS_DECLARE_FUNC(uint32_t) EOS_RTCAudio_GetAudioOutputDevicesCount(EOS_HRTCAudio, const EOS_RTCAudio_GetAudioOutputDevicesCountOptions*)
    { return 1; }

EOS_DECLARE_FUNC(const EOS_RTCAudio_AudioOutputDeviceInfo*) EOS_RTCAudio_GetAudioOutputDeviceByIndex(EOS_HRTCAudio, const EOS_RTCAudio_GetAudioOutputDeviceByIndexOptions* Opts)
{
    (void)Opts;
    return nullptr;
}

EOS_DECLARE_FUNC(EOS_EResult) EOS_RTCAudio_SetAudioInputSettings(EOS_HRTCAudio, const EOS_RTCAudio_SetAudioInputSettingsOptions*)
    { return EOS_EResult::EOS_Success; }

EOS_DECLARE_FUNC(EOS_EResult) EOS_RTCAudio_SetAudioOutputSettings(EOS_HRTCAudio, const EOS_RTCAudio_SetAudioOutputSettingsOptions*)
    { return EOS_EResult::EOS_Success; }

EOS_DECLARE_FUNC(void) EOS_RTCAudio_InputDeviceInformation_Release(EOS_RTCAudio_InputDeviceInformation* p)
    { if (p) delete p; }

EOS_DECLARE_FUNC(void) EOS_RTCAudio_OutputDeviceInformation_Release(EOS_RTCAudio_OutputDeviceInformation* p)
    { if (p) delete p; }

extern "C" EOS_DECLARE_FUNC(void*) EOS_RTCAudio_GetModerationInterface() { return nullptr; }
extern "C" EOS_DECLARE_FUNC(void*) EOS_RTCAudio_SetPosition()            { return nullptr; }
extern "C" EOS_DECLARE_FUNC(void*) EOS_RTCAudio_SetPriorityList()        { return nullptr; }
extern "C" EOS_DECLARE_FUNC(void*) EOS_RTCAudio_UpdateEchoReference()    { return nullptr; }
