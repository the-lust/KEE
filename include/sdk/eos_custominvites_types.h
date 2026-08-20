
#pragma once

#include "eos_common.h"
#include "eos_ui_types.h"

#pragma pack(push, 8)

EOS_EXTERN_C typedef struct EOS_CustomInvitesHandle* EOS_HCustomInvites;

#define EOS_CUSTOMINVITES_MAX_PAYLOAD_LENGTH 500

#define EOS_CUSTOMINVITES_SETCUSTOMINVITE_API_LATEST 1

EOS_STRUCT(EOS_CustomInvites_SetCustomInviteOptions, (
	
	int32_t ApiVersion;
	
	EOS_ProductUserId LocalUserId;
	
	const char* Payload;
));

#define EOS_CUSTOMINVITES_SENDCUSTOMINVITE_API_LATEST 1

EOS_STRUCT(EOS_CustomInvites_SendCustomInviteOptions, (
	
	int32_t ApiVersion;
	
	EOS_ProductUserId LocalUserId;
	
	EOS_ProductUserId* TargetUserIds;
	
	uint32_t TargetUserIdsCount;
));

EOS_STRUCT(EOS_CustomInvites_SendCustomInviteCallbackInfo, (
	
	EOS_EResult ResultCode;
	
	void* ClientData;
	
	EOS_ProductUserId LocalUserId;
	
	EOS_ProductUserId* TargetUserIds;
	
	uint32_t TargetUserIdsCount;
));

EOS_DECLARE_CALLBACK(EOS_CustomInvites_OnSendCustomInviteCallback, const EOS_CustomInvites_SendCustomInviteCallbackInfo* Data);

#define EOS_CUSTOMINVITES_ADDNOTIFYCUSTOMINVITERECEIVED_API_LATEST 1

EOS_STRUCT(EOS_CustomInvites_AddNotifyCustomInviteReceivedOptions, (
	
	int32_t ApiVersion;
));

EOS_STRUCT(EOS_CustomInvites_OnCustomInviteReceivedCallbackInfo, (
	
	void* ClientData;
	
	EOS_ProductUserId TargetUserId;
	
	EOS_ProductUserId LocalUserId;
	
	const char* CustomInviteId;
	
	const char* Payload;
));

EOS_DECLARE_CALLBACK(EOS_CustomInvites_OnCustomInviteReceivedCallback, const EOS_CustomInvites_OnCustomInviteReceivedCallbackInfo* Data);

#define EOS_CUSTOMINVITES_ADDNOTIFYCUSTOMINVITEACCEPTED_API_LATEST 1

EOS_STRUCT(EOS_CustomInvites_AddNotifyCustomInviteAcceptedOptions, (
	
	int32_t ApiVersion;
));

EOS_STRUCT(EOS_CustomInvites_OnCustomInviteAcceptedCallbackInfo, (
	
	void* ClientData;
	
	EOS_ProductUserId TargetUserId;
	
	EOS_ProductUserId LocalUserId;
	
	const char* CustomInviteId;
	
	const char* Payload;
));

EOS_DECLARE_CALLBACK(EOS_CustomInvites_OnCustomInviteAcceptedCallback, const EOS_CustomInvites_OnCustomInviteAcceptedCallbackInfo* Data);

#define EOS_CUSTOMINVITES_ADDNOTIFYCUSTOMINVITEREJECTED_API_LATEST 1

EOS_STRUCT(EOS_CustomInvites_AddNotifyCustomInviteRejectedOptions, (
	
	int32_t ApiVersion;
));

EOS_STRUCT(EOS_CustomInvites_CustomInviteRejectedCallbackInfo, (
	
	void* ClientData;
	
	EOS_ProductUserId TargetUserId;
	
	EOS_ProductUserId LocalUserId;
	
	const char* CustomInviteId;
	
	const char* Payload;
));

EOS_DECLARE_CALLBACK(EOS_CustomInvites_OnCustomInviteRejectedCallback, const EOS_CustomInvites_CustomInviteRejectedCallbackInfo* Data);

#define EOS_CUSTOMINVITES_FINALIZEINVITE_API_LATEST 1

EOS_STRUCT(EOS_CustomInvites_FinalizeInviteOptions, (
	
	int32_t ApiVersion;
	
	EOS_ProductUserId TargetUserId;
	
	EOS_ProductUserId LocalUserId;
	
	const char* CustomInviteId;
	
	EOS_EResult ProcessingResult;
));

#define EOS_CUSTOMINVITES_SENDREQUESTTOJOIN_API_LATEST 1

EOS_STRUCT(EOS_CustomInvites_SendRequestToJoinOptions, (
	
	int32_t ApiVersion;
	
	EOS_ProductUserId LocalUserId;
	
	EOS_ProductUserId TargetUserId;
));

EOS_STRUCT(EOS_CustomInvites_SendRequestToJoinCallbackInfo, (
	
	EOS_EResult ResultCode;
	
	void* ClientData;
	
	EOS_ProductUserId LocalUserId;
	
	EOS_ProductUserId TargetUserId;
));

EOS_DECLARE_CALLBACK(EOS_CustomInvites_OnSendRequestToJoinCallback, const EOS_CustomInvites_SendRequestToJoinCallbackInfo* Data);

#define EOS_CUSTOMINVITES_ADDNOTIFYREQUESTTOJOINRESPONSERECEIVED_API_LATEST 1

EOS_STRUCT(EOS_CustomInvites_AddNotifyRequestToJoinResponseReceivedOptions, (
	
	int32_t ApiVersion;
));

EOS_ENUM(EOS_ERequestToJoinResponse,
	
	EOS_RTJR_ACCEPTED = 0,
	
	EOS_RTJR_REJECTED = 1
);

EOS_STRUCT(EOS_CustomInvites_RequestToJoinResponseReceivedCallbackInfo, (
	
	void* ClientData;
	
	EOS_ProductUserId FromUserId;
	
	EOS_ProductUserId ToUserId;
	
	EOS_ERequestToJoinResponse Response;
));

EOS_DECLARE_CALLBACK(EOS_CustomInvites_OnRequestToJoinResponseReceivedCallback, const EOS_CustomInvites_RequestToJoinResponseReceivedCallbackInfo* Data);

#define EOS_CUSTOMINVITES_ADDNOTIFYREQUESTTOJOINRECEIVED_API_LATEST 1

EOS_STRUCT(EOS_CustomInvites_AddNotifyRequestToJoinReceivedOptions, (
	
	int32_t ApiVersion;
));

EOS_STRUCT(EOS_CustomInvites_RequestToJoinReceivedCallbackInfo, (
	
	void* ClientData;
	
	EOS_ProductUserId FromUserId;
	
	EOS_ProductUserId ToUserId;
));

EOS_DECLARE_CALLBACK(EOS_CustomInvites_OnRequestToJoinReceivedCallback, const EOS_CustomInvites_RequestToJoinReceivedCallbackInfo* Data);

#define EOS_CUSTOMINVITES_ACCEPTREQUESTTOJOIN_API_LATEST 1

EOS_STRUCT(EOS_CustomInvites_AcceptRequestToJoinOptions, (
	
	int32_t ApiVersion;
	
	EOS_ProductUserId LocalUserId;
	
	EOS_ProductUserId TargetUserId;
));

EOS_STRUCT(EOS_CustomInvites_AcceptRequestToJoinCallbackInfo, (
	
	EOS_EResult ResultCode;
	
	void* ClientData;
	
	EOS_ProductUserId LocalUserId;
	
	EOS_ProductUserId TargetUserId;
));

EOS_DECLARE_CALLBACK(EOS_CustomInvites_OnAcceptRequestToJoinCallback, const EOS_CustomInvites_AcceptRequestToJoinCallbackInfo* Data);

#define EOS_CUSTOMINVITES_REJECTREQUESTTOJOIN_API_LATEST 1

EOS_STRUCT(EOS_CustomInvites_RejectRequestToJoinOptions, (
	
	int32_t ApiVersion;
	
	EOS_ProductUserId LocalUserId;
	
	EOS_ProductUserId TargetUserId;
));

EOS_STRUCT(EOS_CustomInvites_RejectRequestToJoinCallbackInfo, (
	
	EOS_EResult ResultCode;
	
	void* ClientData;
	
	EOS_ProductUserId LocalUserId;
	
	EOS_ProductUserId TargetUserId;
));

EOS_DECLARE_CALLBACK(EOS_CustomInvites_OnRejectRequestToJoinCallback, const EOS_CustomInvites_RejectRequestToJoinCallbackInfo* Data);

#define EOS_CUSTOMINVITES_ADDNOTIFYSENDCUSTOMNATIVEINVITEREQUESTED_API_LATEST 1

EOS_STRUCT(EOS_CustomInvites_AddNotifySendCustomNativeInviteRequestedOptions, (
	
	int32_t ApiVersion;
));

EOS_STRUCT(EOS_CustomInvites_SendCustomNativeInviteRequestedCallbackInfo, (
	
	void* ClientData;
	
	EOS_UI_EventId UiEventId;
	
	EOS_ProductUserId LocalUserId;
	
	EOS_IntegratedPlatformType TargetNativeAccountType;
	
	const char* TargetUserNativeAccountId;
	
	const char* InviteId;
));

EOS_DECLARE_CALLBACK(EOS_CustomInvites_OnSendCustomNativeInviteRequestedCallback, const EOS_CustomInvites_SendCustomNativeInviteRequestedCallbackInfo* Data);

#define EOS_CUSTOMINVITES_ADDNOTIFYREQUESTTOJOINACCEPTED_API_LATEST 1

EOS_STRUCT(EOS_CustomInvites_AddNotifyRequestToJoinAcceptedOptions, (
	
	int32_t ApiVersion;
));

EOS_STRUCT(EOS_CustomInvites_OnRequestToJoinAcceptedCallbackInfo, (
	
	void* ClientData;
	
	EOS_ProductUserId TargetUserId;
	
	EOS_ProductUserId LocalUserId;
));

EOS_DECLARE_CALLBACK(EOS_CustomInvites_OnRequestToJoinAcceptedCallback, const EOS_CustomInvites_OnRequestToJoinAcceptedCallbackInfo* Data);

#define EOS_CUSTOMINVITES_ADDNOTIFYREQUESTTOJOINREJECTED_API_LATEST 1

EOS_STRUCT(EOS_CustomInvites_AddNotifyRequestToJoinRejectedOptions, (
	
	int32_t ApiVersion;
));

EOS_STRUCT(EOS_CustomInvites_OnRequestToJoinRejectedCallbackInfo, (
	
	void* ClientData;
	
	EOS_ProductUserId TargetUserId;
	
	EOS_ProductUserId LocalUserId;
));

EOS_DECLARE_CALLBACK(EOS_CustomInvites_OnRequestToJoinRejectedCallback, const EOS_CustomInvites_OnRequestToJoinRejectedCallbackInfo* Data);

#pragma pack(pop)
