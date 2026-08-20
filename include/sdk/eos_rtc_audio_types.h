

#pragma once

#include "eos_common.h"

#pragma pack(push, 8)

EOS_EXTERN_C typedef struct EOS_RTCAudioHandle* EOS_HRTCAudio;

EOS_ENUM(EOS_ERTCAudioStatus,
	
	EOS_RTCAS_Unsupported = 0,
	
	EOS_RTCAS_Enabled = 1,
	
	EOS_RTCAS_Disabled = 2,
	
	EOS_RTCAS_AdminDisabled = 3,
	
	EOS_RTCAS_NotListeningDisabled = 4
);

#define EOS_RTCAUDIO_ADDNOTIFYPARTICIPANTUPDATED_API_LATEST 1

EOS_STRUCT(EOS_RTCAudio_AddNotifyParticipantUpdatedOptions, (
	
	int32_t ApiVersion;
	
	EOS_ProductUserId LocalUserId;
	
	const char* RoomName;
));

EOS_STRUCT(EOS_RTCAudio_ParticipantUpdatedCallbackInfo, (
	
	void* ClientData;
	
	EOS_ProductUserId LocalUserId;
	
	const char* RoomName;
	
	EOS_ProductUserId ParticipantId;
	
	EOS_Bool bSpeaking;
	
	EOS_ERTCAudioStatus AudioStatus;
));

EOS_DECLARE_CALLBACK(EOS_RTCAudio_OnParticipantUpdatedCallback, const EOS_RTCAudio_ParticipantUpdatedCallbackInfo* Data);

#define EOS_RTCAUDIO_ADDNOTIFYAUDIODEVICESCHANGED_API_LATEST 1

EOS_STRUCT(EOS_RTCAudio_AddNotifyAudioDevicesChangedOptions, (
	
	int32_t ApiVersion;
));

EOS_STRUCT(EOS_RTCAudio_AudioDevicesChangedCallbackInfo, (
	
	void* ClientData;
));

EOS_DECLARE_CALLBACK(EOS_RTCAudio_OnAudioDevicesChangedCallback, const EOS_RTCAudio_AudioDevicesChangedCallbackInfo* Data);

EOS_ENUM(EOS_ERTCAudioInputStatus,
	
	EOS_RTCAIS_Idle = 0,
	
	EOS_RTCAIS_Recording = 1,
	
	EOS_RTCAIS_RecordingSilent = 2,
	
	EOS_RTCAIS_RecordingDisconnected = 3,
	
	EOS_RTCAIS_Failed = 4
);

#define EOS_RTCAUDIO_ADDNOTIFYAUDIOINPUTSTATE_API_LATEST 1

EOS_STRUCT(EOS_RTCAudio_AddNotifyAudioInputStateOptions, (
	
	int32_t ApiVersion;
	
	EOS_ProductUserId LocalUserId;
	
	const char* RoomName;
));

EOS_STRUCT(EOS_RTCAudio_AudioInputStateCallbackInfo, (
	
	void* ClientData;
	
	EOS_ProductUserId LocalUserId;
	
	const char* RoomName;
	
	EOS_ERTCAudioInputStatus Status;
));

EOS_DECLARE_CALLBACK(EOS_RTCAudio_OnAudioInputStateCallback, const EOS_RTCAudio_AudioInputStateCallbackInfo* Data);

EOS_ENUM(EOS_ERTCAudioOutputStatus,
	
	EOS_RTCAOS_Idle = 0,
	
	EOS_RTCAOS_Playing = 1,
	
	EOS_RTCAOS_Failed = 2
);

#define EOS_RTCAUDIO_ADDNOTIFYAUDIOOUTPUTSTATE_API_LATEST 1

EOS_STRUCT(EOS_RTCAudio_AddNotifyAudioOutputStateOptions, (
	
	int32_t ApiVersion;
	
	EOS_ProductUserId LocalUserId;
	
	const char* RoomName;
));

EOS_STRUCT(EOS_RTCAudio_AudioOutputStateCallbackInfo, (
	
	void* ClientData;
	
	EOS_ProductUserId LocalUserId;
	
	const char* RoomName;
	
	EOS_ERTCAudioOutputStatus Status;
));

EOS_DECLARE_CALLBACK(EOS_RTCAudio_OnAudioOutputStateCallback, const EOS_RTCAudio_AudioOutputStateCallbackInfo* Data);

#define EOS_RTCAUDIO_ADDNOTIFYAUDIOBEFORESEND_API_LATEST 1

EOS_STRUCT(EOS_RTCAudio_AddNotifyAudioBeforeSendOptions, (
	
	int32_t ApiVersion;
	
	EOS_ProductUserId LocalUserId;
	
	const char* RoomName;
));

#define EOS_RTCAUDIO_AUDIOBUFFER_API_LATEST 1

EOS_STRUCT(EOS_RTCAudio_AudioBuffer, (
	
	int32_t ApiVersion;
	
	int16_t* Frames;
	
	uint32_t FramesCount;
	
	uint32_t SampleRate;
	
	uint32_t Channels;
));

EOS_STRUCT(EOS_RTCAudio_AudioBeforeSendCallbackInfo, (
	
	void* ClientData;
	
	EOS_ProductUserId LocalUserId;
	
	const char* RoomName;
	
	EOS_RTCAudio_AudioBuffer* Buffer;
));

EOS_DECLARE_CALLBACK(EOS_RTCAudio_OnAudioBeforeSendCallback, const EOS_RTCAudio_AudioBeforeSendCallbackInfo* Data);

#define EOS_RTCAUDIO_ADDNOTIFYAUDIOBEFORERENDER_API_LATEST 1

EOS_STRUCT(EOS_RTCAudio_AddNotifyAudioBeforeRenderOptions, (
	
	int32_t ApiVersion;
	
	EOS_ProductUserId LocalUserId;
	
	const char* RoomName;
	
	EOS_Bool bUnmixedAudio;
));

EOS_STRUCT(EOS_RTCAudio_AudioBeforeRenderCallbackInfo, (
	
	void* ClientData;
	
	EOS_ProductUserId LocalUserId;
	
	const char* RoomName;
	
	EOS_RTCAudio_AudioBuffer* Buffer;
	
	EOS_ProductUserId ParticipantId;
));

EOS_DECLARE_CALLBACK(EOS_RTCAudio_OnAudioBeforeRenderCallback, const EOS_RTCAudio_AudioBeforeRenderCallbackInfo* Data);

#define EOS_RTCAUDIO_SENDAUDIO_API_LATEST 1

EOS_STRUCT(EOS_RTCAudio_SendAudioOptions, (
	
	int32_t ApiVersion;
	
	EOS_ProductUserId LocalUserId;
	
	const char* RoomName;
	
	EOS_RTCAudio_AudioBuffer* Buffer;
));

#define EOS_RTCAUDIO_UPDATESENDING_API_LATEST 1

EOS_STRUCT(EOS_RTCAudio_UpdateSendingOptions, (
	
	int32_t ApiVersion;
	
	EOS_ProductUserId LocalUserId;
	
	const char* RoomName;
	
	EOS_ERTCAudioStatus AudioStatus;
));

EOS_STRUCT(EOS_RTCAudio_UpdateSendingCallbackInfo, (
	
	EOS_EResult ResultCode;
	
	void* ClientData;
	
	EOS_ProductUserId LocalUserId;
	
	const char* RoomName;
	
	EOS_ERTCAudioStatus AudioStatus;
));

EOS_DECLARE_CALLBACK(EOS_RTCAudio_OnUpdateSendingCallback, const EOS_RTCAudio_UpdateSendingCallbackInfo* Data);

#define EOS_RTCAUDIO_UPDATERECEIVING_API_LATEST 1

EOS_STRUCT(EOS_RTCAudio_UpdateReceivingOptions, (
	
	int32_t ApiVersion;
	
	EOS_ProductUserId LocalUserId;
	
	const char* RoomName;
	
	EOS_ProductUserId ParticipantId;
	
	EOS_Bool bAudioEnabled;
));

EOS_STRUCT(EOS_RTCAudio_UpdateReceivingCallbackInfo, (
	
	EOS_EResult ResultCode;
	
	void* ClientData;
	
	EOS_ProductUserId LocalUserId;
	
	const char* RoomName;
	
	EOS_ProductUserId ParticipantId;
	
	EOS_Bool bAudioEnabled;
));

EOS_DECLARE_CALLBACK(EOS_RTCAudio_OnUpdateReceivingCallback, const EOS_RTCAudio_UpdateReceivingCallbackInfo* Data);

#define EOS_RTCAUDIO_UPDATESENDINGVOLUME_API_LATEST 1

EOS_STRUCT(EOS_RTCAudio_UpdateSendingVolumeOptions, (
	
	int32_t ApiVersion;
	
	EOS_ProductUserId LocalUserId;
	
	const char* RoomName;
	
	float Volume;
));

EOS_STRUCT(EOS_RTCAudio_UpdateSendingVolumeCallbackInfo, (
	
	EOS_EResult ResultCode;
	
	void* ClientData;
	
	EOS_ProductUserId LocalUserId;
	
	const char* RoomName;
	
	float Volume;
));

EOS_DECLARE_CALLBACK(EOS_RTCAudio_OnUpdateSendingVolumeCallback, const EOS_RTCAudio_UpdateSendingVolumeCallbackInfo* Data);

#define EOS_RTCAUDIO_UPDATERECEIVINGVOLUME_API_LATEST 1

EOS_STRUCT(EOS_RTCAudio_UpdateReceivingVolumeOptions, (
	
	int32_t ApiVersion;
	
	EOS_ProductUserId LocalUserId;
	
	const char* RoomName;
	
	float Volume;
));

EOS_STRUCT(EOS_RTCAudio_UpdateReceivingVolumeCallbackInfo, (
	
	EOS_EResult ResultCode;
	
	void* ClientData;
	
	EOS_ProductUserId LocalUserId;
	
	const char* RoomName;
	
	float Volume;
));

EOS_DECLARE_CALLBACK(EOS_RTCAudio_OnUpdateReceivingVolumeCallback, const EOS_RTCAudio_UpdateReceivingVolumeCallbackInfo* Data);

#define EOS_RTCAUDIO_UPDATEPARTICIPANTVOLUME_API_LATEST 1

EOS_STRUCT(EOS_RTCAudio_UpdateParticipantVolumeOptions, (
	
	int32_t ApiVersion;
	
	EOS_ProductUserId LocalUserId;
	
	const char* RoomName;
	
	EOS_ProductUserId ParticipantId;
	
	float Volume;
));

EOS_STRUCT(EOS_RTCAudio_UpdateParticipantVolumeCallbackInfo, (
	
	EOS_EResult ResultCode;
	
	void* ClientData;
	
	EOS_ProductUserId LocalUserId;
	
	const char* RoomName;
	
	EOS_ProductUserId ParticipantId;
	
	float Volume;
));

EOS_DECLARE_CALLBACK(EOS_RTCAudio_OnUpdateParticipantVolumeCallback, const EOS_RTCAudio_UpdateParticipantVolumeCallbackInfo* Data);

#define EOS_RTCAUDIO_REGISTERPLATFORMUSER_API_LATEST 1

EOS_STRUCT(EOS_RTCAudio_RegisterPlatformUserOptions, (
	
	int32_t ApiVersion;
	
	const char* PlatformUserId;
));

EOS_STRUCT(EOS_RTCAudio_OnRegisterPlatformUserCallbackInfo, (
	
	EOS_EResult ResultCode;
	
	void* ClientData;
	
	const char* PlatformUserId;
));

EOS_DECLARE_CALLBACK(EOS_RTCAudio_OnRegisterPlatformUserCallback, const EOS_RTCAudio_OnRegisterPlatformUserCallbackInfo* Data);

#define EOS_RTCAUDIO_UNREGISTERPLATFORMUSER_API_LATEST 1

EOS_STRUCT(EOS_RTCAudio_UnregisterPlatformUserOptions, (
	
	int32_t ApiVersion;
	
	const char* PlatformUserId;
));

EOS_STRUCT(EOS_RTCAudio_OnUnregisterPlatformUserCallbackInfo, (
	
	EOS_EResult ResultCode;
	
	void* ClientData;
	
	const char* PlatformUserId;
));

EOS_DECLARE_CALLBACK(EOS_RTCAudio_OnUnregisterPlatformUserCallback, const EOS_RTCAudio_OnUnregisterPlatformUserCallbackInfo* Data);

#define EOS_RTCAUDIO_QUERYINPUTDEVICESINFORMATION_API_LATEST 1

EOS_STRUCT(EOS_RTCAudio_QueryInputDevicesInformationOptions, (
	
	int32_t ApiVersion;
));

EOS_STRUCT(EOS_RTCAudio_OnQueryInputDevicesInformationCallbackInfo, (
	
	EOS_EResult ResultCode;
	
	void* ClientData;
));

EOS_DECLARE_CALLBACK(EOS_RTCAudio_OnQueryInputDevicesInformationCallback, const EOS_RTCAudio_OnQueryInputDevicesInformationCallbackInfo* Data);

#define EOS_RTCAUDIO_GETINPUTDEVICESCOUNT_API_LATEST 1

EOS_STRUCT(EOS_RTCAudio_GetInputDevicesCountOptions, (
	
	int32_t ApiVersion;
));

#define EOS_RTCAUDIO_COPYINPUTDEVICEINFORMATIONBYINDEX_API_LATEST 1

EOS_STRUCT(EOS_RTCAudio_CopyInputDeviceInformationByIndexOptions, (
	
	int32_t ApiVersion;
	
	uint32_t DeviceIndex;
));

#define EOS_RTCAUDIO_INPUTDEVICEINFORMATION_API_LATEST 1

EOS_STRUCT(EOS_RTCAudio_InputDeviceInformation, (
	
	int32_t ApiVersion;
	
	EOS_Bool bDefaultDevice;
	
	const char* DeviceId;
	
	const char* DeviceName;
));

EOS_DECLARE_FUNC(void) EOS_RTCAudio_InputDeviceInformation_Release(EOS_RTCAudio_InputDeviceInformation* DeviceInformation);

#define EOS_RTCAUDIO_QUERYOUTPUTDEVICESINFORMATION_API_LATEST 1

EOS_STRUCT(EOS_RTCAudio_QueryOutputDevicesInformationOptions, (
	
	int32_t ApiVersion;
));

EOS_STRUCT(EOS_RTCAudio_OnQueryOutputDevicesInformationCallbackInfo, (
	
	EOS_EResult ResultCode;
	
	void* ClientData;
));

EOS_DECLARE_CALLBACK(EOS_RTCAudio_OnQueryOutputDevicesInformationCallback, const EOS_RTCAudio_OnQueryOutputDevicesInformationCallbackInfo* Data);

#define EOS_RTCAUDIO_GETOUTPUTDEVICESCOUNT_API_LATEST 1

EOS_STRUCT(EOS_RTCAudio_GetOutputDevicesCountOptions, (
	
	int32_t ApiVersion;
));

#define EOS_RTCAUDIO_COPYOUTPUTDEVICEINFORMATIONBYINDEX_API_LATEST 1

EOS_STRUCT(EOS_RTCAudio_CopyOutputDeviceInformationByIndexOptions, (
	
	int32_t ApiVersion;
	
	uint32_t DeviceIndex;
));

#define EOS_RTCAUDIO_OUTPUTDEVICEINFORMATION_API_LATEST 1

EOS_STRUCT(EOS_RTCAudio_OutputDeviceInformation, (
	
	int32_t ApiVersion;
	
	EOS_Bool bDefaultDevice;
	
	const char* DeviceId;
	
	const char* DeviceName;
));

EOS_DECLARE_FUNC(void) EOS_RTCAudio_OutputDeviceInformation_Release(EOS_RTCAudio_OutputDeviceInformation* DeviceInformation);

#define EOS_RTCAUDIO_SETINPUTDEVICESETTINGS_API_LATEST 1

EOS_STRUCT(EOS_RTCAudio_SetInputDeviceSettingsOptions, (
	
	int32_t ApiVersion;
	
	EOS_ProductUserId LocalUserId;
	
	const char* RealDeviceId;
	
	EOS_Bool bPlatformAEC;
));

EOS_STRUCT(EOS_RTCAudio_OnSetInputDeviceSettingsCallbackInfo, (
	
	EOS_EResult ResultCode;
	
	void* ClientData;
	
	const char* RealDeviceId;
));

EOS_DECLARE_CALLBACK(EOS_RTCAudio_OnSetInputDeviceSettingsCallback, const EOS_RTCAudio_OnSetInputDeviceSettingsCallbackInfo* Data);

#define EOS_RTCAUDIO_SETOUTPUTDEVICESETTINGS_API_LATEST 1

EOS_STRUCT(EOS_RTCAudio_SetOutputDeviceSettingsOptions, (
	
	int32_t ApiVersion;
	
	EOS_ProductUserId LocalUserId;
	
	const char* RealDeviceId;
));

EOS_STRUCT(EOS_RTCAudio_OnSetOutputDeviceSettingsCallbackInfo, (
	
	EOS_EResult ResultCode;
	
	void* ClientData;
	
	const char* RealDeviceId;
));

EOS_DECLARE_CALLBACK(EOS_RTCAudio_OnSetOutputDeviceSettingsCallback, const EOS_RTCAudio_OnSetOutputDeviceSettingsCallbackInfo* Data);

#pragma pack(pop)

#include "eos_rtc_audio_types_deprecated.inl"
