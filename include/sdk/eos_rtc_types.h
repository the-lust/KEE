

#pragma once

#include "eos_common.h"

#pragma pack(push, 8)

EOS_EXTERN_C typedef struct EOS_RTCHandle* EOS_HRTC;

EOS_ENUM(EOS_ERTCParticipantStatus,
	
	EOS_RTCPS_Joined = 0,
	
	EOS_RTCPS_Left = 1
);

#define EOS_RTC_JOINROOM_API_LATEST 1

#define EOS_RTC_JOINROOMFLAGS_ENABLE_ECHO 0x01

#define EOS_RTC_JOINROOMFLAGS_ENABLE_DATACHANNEL 0x04

#define EOS_RTC_JOINROOMFLAGS_RESERVED_VOICE_FEATURE 0x08

EOS_STRUCT(EOS_RTC_JoinRoomOptions, (
	
	int32_t ApiVersion;
	
	EOS_ProductUserId LocalUserId;
	
	const char* RoomName;
	
	const char* ClientBaseUrl;
	
	const char* ParticipantToken;
	
	EOS_ProductUserId ParticipantId;

	uint32_t Flags;
	
	EOS_Bool bManualAudioInputEnabled;
	
	EOS_Bool bManualAudioOutputEnabled;
));

#define EOS_RTC_OPTION_KEY_MAXCHARCOUNT 256

#define EOS_RTC_OPTION_VALUE_MAXCHARCOUNT 256

#define EOS_RTC_OPTION_API_LATEST 1

EOS_STRUCT(EOS_RTC_Option, (
	
	int32_t ApiVersion;
	
	const char* Key;
	
	const char* Value;
));

EOS_STRUCT(EOS_RTC_JoinRoomCallbackInfo, (
	
	EOS_EResult ResultCode;
	
	void* ClientData;
	
	EOS_ProductUserId LocalUserId;
	
	const char* RoomName;
	
	uint32_t RoomOptionsCount;
	
	const EOS_RTC_Option* RoomOptions;
));

EOS_DECLARE_CALLBACK(EOS_RTC_OnJoinRoomCallback, const EOS_RTC_JoinRoomCallbackInfo* Data);

#define EOS_RTC_LEAVEROOM_API_LATEST 1

EOS_STRUCT(EOS_RTC_LeaveRoomOptions, (
	
	int32_t ApiVersion;
	
	EOS_ProductUserId LocalUserId;
	
	const char* RoomName;
));

EOS_STRUCT(EOS_RTC_LeaveRoomCallbackInfo, (
	
	EOS_EResult ResultCode;
	
	void* ClientData;
	
	EOS_ProductUserId LocalUserId;
	
	const char* RoomName;
));

EOS_DECLARE_CALLBACK(EOS_RTC_OnLeaveRoomCallback, const EOS_RTC_LeaveRoomCallbackInfo* Data);

#define EOS_RTC_BLOCKPARTICIPANT_API_LATEST 1

EOS_STRUCT(EOS_RTC_BlockParticipantOptions, (
	
	int32_t ApiVersion;
	
	EOS_ProductUserId LocalUserId;
	
	const char* RoomName;
	
	EOS_ProductUserId ParticipantId;
	
	EOS_Bool bBlocked;
));

EOS_STRUCT(EOS_RTC_BlockParticipantCallbackInfo, (
	
	EOS_EResult ResultCode;
	
	void* ClientData;
	
	EOS_ProductUserId LocalUserId;
	
	const char* RoomName;
	
	EOS_ProductUserId ParticipantId;
	
	EOS_Bool bBlocked;
));

EOS_DECLARE_CALLBACK(EOS_RTC_OnBlockParticipantCallback, const EOS_RTC_BlockParticipantCallbackInfo* Data);

#define EOS_RTC_ADDNOTIFYDISCONNECTED_API_LATEST 1

EOS_STRUCT(EOS_RTC_AddNotifyDisconnectedOptions, (
	
	int32_t ApiVersion;
	
	EOS_ProductUserId LocalUserId;
	
	const char* RoomName;
));

EOS_STRUCT(EOS_RTC_DisconnectedCallbackInfo, (
	
	EOS_EResult ResultCode;
	
	void* ClientData;
	
	EOS_ProductUserId LocalUserId;
	
	const char* RoomName;
));

EOS_DECLARE_CALLBACK(EOS_RTC_OnDisconnectedCallback, const EOS_RTC_DisconnectedCallbackInfo* Data);

#define EOS_RTC_PARTICIPANTMETADATA_KEY_MAXCHARCOUNT 256

#define EOS_RTC_PARTICIPANTMETADATA_VALUE_MAXCHARCOUNT 256

#define EOS_RTC_PARTICIPANTMETADATA_API_LATEST 1

EOS_STRUCT(EOS_RTC_ParticipantMetadata, (
	
	int32_t ApiVersion;
	
	const char* Key;
	
	const char* Value;
));

#define EOS_RTC_ADDNOTIFYPARTICIPANTSTATUSCHANGED_API_LATEST 1

EOS_STRUCT(EOS_RTC_AddNotifyParticipantStatusChangedOptions, (
	
	int32_t ApiVersion;
	
	EOS_ProductUserId LocalUserId;
	
	const char* RoomName;
));

EOS_STRUCT(EOS_RTC_ParticipantStatusChangedCallbackInfo, (
	
	void* ClientData;
	
	EOS_ProductUserId LocalUserId;
	
	const char* RoomName;
	
	EOS_ProductUserId ParticipantId;
	
	EOS_ERTCParticipantStatus ParticipantStatus;
	
	uint32_t ParticipantMetadataCount;
	
	const EOS_RTC_ParticipantMetadata* ParticipantMetadata;
	
	EOS_Bool bParticipantInBlocklist;
));

EOS_DECLARE_CALLBACK(EOS_RTC_OnParticipantStatusChangedCallback, const EOS_RTC_ParticipantStatusChangedCallbackInfo* Data);

#define EOS_RTC_ADDNOTIFYROOMBEFOREJOIN_API_LATEST 1

EOS_STRUCT(EOS_RTC_AddNotifyRoomBeforeJoinOptions, (
	
	int32_t ApiVersion;
	
	EOS_ProductUserId LocalUserId;
));

EOS_STRUCT(EOS_RTC_RoomBeforeJoinCallbackInfo, (
	
	void* ClientData;
	
	EOS_ProductUserId LocalUserId;
	
	const char* RoomName;
));

EOS_DECLARE_CALLBACK(EOS_RTC_OnRoomBeforeJoinCallback, const EOS_RTC_RoomBeforeJoinCallbackInfo* Data);

#define EOS_RTC_SETSETTING_API_LATEST 1

EOS_STRUCT(EOS_RTC_SetSettingOptions, (
	
	int32_t ApiVersion;
	
	const char* SettingName;
	
	const char* SettingValue;
));

#define EOS_RTC_SETROOMSETTING_API_LATEST 1

EOS_STRUCT(EOS_RTC_SetRoomSettingOptions, (
	
	int32_t ApiVersion;
	
	EOS_ProductUserId LocalUserId;
	
	const char* RoomName;
	
	const char* SettingName;
	
	const char* SettingValue;
));

#define EOS_RTC_ADDNOTIFYROOMSTATISTICSUPDATED_API_LATEST 1

EOS_STRUCT(EOS_RTC_AddNotifyRoomStatisticsUpdatedOptions, (
	
	int32_t ApiVersion;
	
	EOS_ProductUserId LocalUserId;
	
	const char* RoomName;
));

EOS_STRUCT(EOS_RTC_RoomStatisticsUpdatedInfo, (
	
	void* ClientData;
	
	EOS_ProductUserId LocalUserId;
	
	const char* RoomName;
	
	const char* Statistic;
));

EOS_DECLARE_CALLBACK(EOS_RTC_OnRoomStatisticsUpdatedCallback, const EOS_RTC_RoomStatisticsUpdatedInfo* Data);

#pragma pack(pop)
