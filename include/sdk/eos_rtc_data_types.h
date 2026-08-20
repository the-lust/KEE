

#pragma once

#include "eos_common.h"

#pragma pack(push, 8)

EOS_EXTERN_C typedef struct EOS_RTCDataHandle* EOS_HRTCData;

#define EOS_RTCDATA_MAX_PACKET_SIZE 1170

#define EOS_RTCDATA_SENDDATA_API_LATEST 1

EOS_ENUM(EOS_ERTCDataStatus,
	
	EOS_RTCDS_Unsupported = 0,
	
	EOS_RTCDS_Enabled = 1,
	
	EOS_RTCDS_Disabled = 2
);

#define EOS_RTCDATA_ADDNOTIFYPARTICIPANTUPDATED_API_LATEST 1

EOS_STRUCT(EOS_RTCData_AddNotifyParticipantUpdatedOptions, (
	
	int32_t ApiVersion;
	
	EOS_ProductUserId LocalUserId;
	
	const char* RoomName;
));

EOS_STRUCT(EOS_RTCData_ParticipantUpdatedCallbackInfo, (
	
	void* ClientData;
	
	EOS_ProductUserId LocalUserId;
	
	const char* RoomName;
	
	EOS_ProductUserId ParticipantId;
	
	EOS_ERTCDataStatus DataStatus;
));

EOS_DECLARE_CALLBACK(EOS_RTCData_OnParticipantUpdatedCallback, const EOS_RTCData_ParticipantUpdatedCallbackInfo* Data);

EOS_STRUCT(EOS_RTCData_SendDataOptions, (
	
	int32_t ApiVersion;
	
	EOS_ProductUserId LocalUserId;
	
	const char* RoomName;
	
	uint32_t DataLengthBytes;
	
	const void* Data;
));

#define EOS_RTCDATA_ADDNOTIFYDATARECEIVED_API_LATEST 1

EOS_STRUCT(EOS_RTCData_AddNotifyDataReceivedOptions, (
	
	int32_t ApiVersion;
	
	EOS_ProductUserId LocalUserId;
	
	const char* RoomName;
));

EOS_STRUCT(EOS_RTCData_DataReceivedCallbackInfo, (
	
	void* ClientData;
	
	EOS_ProductUserId LocalUserId;
	
	const char* RoomName;
	
	uint32_t DataLengthBytes;
	
	const void* Data;
	
	EOS_ProductUserId ParticipantId;
));

EOS_DECLARE_CALLBACK(EOS_RTCData_OnDataReceivedCallback, const EOS_RTCData_DataReceivedCallbackInfo* Data);

#define EOS_RTCDATA_UPDATESENDING_API_LATEST 1

EOS_STRUCT(EOS_RTCData_UpdateSendingOptions, (
	
	int32_t ApiVersion;
	
	EOS_ProductUserId LocalUserId;
	
	const char* RoomName;
	
	EOS_Bool bDataEnabled;
));

EOS_STRUCT(EOS_RTCData_UpdateSendingCallbackInfo, (
	
	EOS_EResult ResultCode;
	
	void* ClientData;
	
	EOS_ProductUserId LocalUserId;
	
	const char* RoomName;
	
	EOS_Bool bDataEnabled;
));

EOS_DECLARE_CALLBACK(EOS_RTCData_OnUpdateSendingCallback, const EOS_RTCData_UpdateSendingCallbackInfo* Data);

#define EOS_RTCDATA_UPDATERECEIVING_API_LATEST 1

EOS_STRUCT(EOS_RTCData_UpdateReceivingOptions, (
	
	int32_t ApiVersion;
	
	EOS_ProductUserId LocalUserId;
	
	const char* RoomName;
	
	EOS_ProductUserId ParticipantId;
	
	EOS_Bool bDataEnabled;
));

EOS_STRUCT(EOS_RTCData_UpdateReceivingCallbackInfo, (
	
	EOS_EResult ResultCode;
	
	void* ClientData;
	
	EOS_ProductUserId LocalUserId;
	
	const char* RoomName;
	
	EOS_ProductUserId ParticipantId;
	
	EOS_Bool bDataEnabled;
));

EOS_DECLARE_CALLBACK(EOS_RTCData_OnUpdateReceivingCallback, const EOS_RTCData_UpdateReceivingCallbackInfo* Data);

#pragma pack(pop)

