

#pragma once

#include "eos_common.h"
#include "eos_anticheatcommon_types.h"

#pragma pack(push, 8)

EOS_EXTERN_C typedef struct EOS_AntiCheatServerHandle* EOS_HAntiCheatServer;

#define EOS_ANTICHEATSERVER_ONMESSAGETOCLIENTCALLBACK_MAX_MESSAGE_SIZE 512

EOS_DECLARE_CALLBACK(EOS_AntiCheatServer_OnMessageToClientCallback, const EOS_AntiCheatCommon_OnMessageToClientCallbackInfo* Data);

EOS_DECLARE_CALLBACK(EOS_AntiCheatServer_OnClientActionRequiredCallback, const EOS_AntiCheatCommon_OnClientActionRequiredCallbackInfo* Data);

EOS_DECLARE_CALLBACK(EOS_AntiCheatServer_OnClientAuthStatusChangedCallback, const EOS_AntiCheatCommon_OnClientAuthStatusChangedCallbackInfo* Data);

#define EOS_ANTICHEATSERVER_ADDNOTIFYMESSAGETOCLIENT_API_LATEST 1

EOS_STRUCT(EOS_AntiCheatServer_AddNotifyMessageToClientOptions, (
	
	int32_t ApiVersion;
));

#define EOS_ANTICHEATSERVER_ADDNOTIFYCLIENTACTIONREQUIRED_API_LATEST 1

EOS_STRUCT(EOS_AntiCheatServer_AddNotifyClientActionRequiredOptions, (
	
	int32_t ApiVersion;
));

#define EOS_ANTICHEATSERVER_ADDNOTIFYCLIENTAUTHSTATUSCHANGED_API_LATEST 1

EOS_STRUCT(EOS_AntiCheatServer_AddNotifyClientAuthStatusChangedOptions, (
	
	int32_t ApiVersion;
));

#define EOS_ANTICHEATSERVER_BEGINSESSION_MIN_REGISTERTIMEOUT 10

#define EOS_ANTICHEATSERVER_BEGINSESSION_MAX_REGISTERTIMEOUT 120

#define EOS_ANTICHEATSERVER_BEGINSESSION_API_LATEST 3

EOS_STRUCT(EOS_AntiCheatServer_BeginSessionOptions, (
	
	int32_t ApiVersion;
	
	uint32_t RegisterTimeoutSeconds;
	
	const char* ServerName;
	
	EOS_Bool bEnableGameplayData;
	
	EOS_ProductUserId LocalUserId;
));

#define EOS_ANTICHEATSERVER_ENDSESSION_API_LATEST 1

EOS_STRUCT(EOS_AntiCheatServer_EndSessionOptions, (
	
	int32_t ApiVersion;
));

#define EOS_ANTICHEATSERVER_REGISTERCLIENT_API_LATEST 3

EOS_STRUCT(EOS_AntiCheatServer_RegisterClientOptions, (
	
	int32_t ApiVersion;
	
	EOS_AntiCheatCommon_ClientHandle ClientHandle;
	
	EOS_EAntiCheatCommonClientType ClientType;
	
	EOS_EAntiCheatCommonClientPlatform ClientPlatform;
	
	const char* AccountId_DEPRECATED;
	
	const char* IpAddress;
	
	EOS_ProductUserId UserId;
	
	int32_t Reserved01;
));

#define EOS_ANTICHEATSERVER_UNREGISTERCLIENT_API_LATEST 1

EOS_STRUCT(EOS_AntiCheatServer_UnregisterClientOptions, (
	
	int32_t ApiVersion;
	
	EOS_AntiCheatCommon_ClientHandle ClientHandle;
));

#define EOS_ANTICHEATSERVER_RECEIVEMESSAGEFROMCLIENT_API_LATEST 1

EOS_STRUCT(EOS_AntiCheatServer_ReceiveMessageFromClientOptions, (
	
	int32_t ApiVersion;
	
	EOS_AntiCheatCommon_ClientHandle ClientHandle;
	
	uint32_t DataLengthBytes;
	
	const void* Data;
));

#define EOS_ANTICHEATSERVER_SETCLIENTNETWORKSTATE_API_LATEST 1

EOS_STRUCT(EOS_AntiCheatServer_SetClientNetworkStateOptions, (
	
	int32_t ApiVersion;
	
	EOS_AntiCheatCommon_ClientHandle ClientHandle;
	
	EOS_Bool bIsNetworkActive;
));

#define EOS_ANTICHEATSERVER_GETPROTECTMESSAGEOUTPUTLENGTH_API_LATEST 1

EOS_STRUCT(EOS_AntiCheatServer_GetProtectMessageOutputLengthOptions, (
	
	int32_t ApiVersion;
	
	uint32_t DataLengthBytes;
));

#define EOS_ANTICHEATSERVER_PROTECTMESSAGE_API_LATEST 1

EOS_STRUCT(EOS_AntiCheatServer_ProtectMessageOptions, (
	
	int32_t ApiVersion;
	
	EOS_AntiCheatCommon_ClientHandle ClientHandle;
	
	uint32_t DataLengthBytes;
	
	const void* Data;
	
	uint32_t OutBufferSizeBytes;
));

#define EOS_ANTICHEATSERVER_UNPROTECTMESSAGE_API_LATEST 1

EOS_STRUCT(EOS_AntiCheatServer_UnprotectMessageOptions, (
	
	int32_t ApiVersion;
	
	EOS_AntiCheatCommon_ClientHandle ClientHandle;
	
	uint32_t DataLengthBytes;
	
	const void* Data;
	
	uint32_t OutBufferSizeBytes;
));

#pragma pack(pop)
