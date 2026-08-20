

#pragma once

#include "eos_common.h"
#include "eos_anticheatcommon_types.h"

#pragma pack(push, 8)

EOS_EXTERN_C typedef struct EOS_AntiCheatClientHandle* EOS_HAntiCheatClient;

EOS_ENUM(EOS_EAntiCheatClientMode,
	
	EOS_ACCM_Invalid = 0,
	
	EOS_ACCM_ClientServer = 1,
	
	EOS_ACCM_PeerToPeer = 2
);

EOS_ENUM(EOS_EAntiCheatClientViolationType,
	
	EOS_ACCVT_Invalid = 0,
	
	EOS_ACCVT_IntegrityCatalogNotFound = 1,
	
	EOS_ACCVT_IntegrityCatalogError = 2,
	
	EOS_ACCVT_IntegrityCatalogCertificateRevoked = 3,
	
	EOS_ACCVT_IntegrityCatalogMissingMainExecutable = 4,
	
	EOS_ACCVT_GameFileMismatch = 5,
	
	EOS_ACCVT_RequiredGameFileNotFound = 6,
	
	EOS_ACCVT_UnknownGameFileForbidden = 7,
	
	EOS_ACCVT_SystemFileUntrusted = 8,
	
	EOS_ACCVT_ForbiddenModuleLoaded = 9,
	
	EOS_ACCVT_CorruptedMemory = 10,
	
	EOS_ACCVT_ForbiddenToolDetected = 11,
	
	EOS_ACCVT_InternalAntiCheatViolation = 12,
	
	EOS_ACCVT_CorruptedNetworkMessageFlow = 13,
	
	EOS_ACCVT_VirtualMachineNotAllowed = 14,
	
	EOS_ACCVT_ForbiddenSystemConfiguration = 15
);

EOS_STRUCT(EOS_AntiCheatClient_OnMessageToServerCallbackInfo, (
	
	void* ClientData;
	
	const void* MessageData;
	
	uint32_t MessageDataSizeBytes;
));

EOS_STRUCT(EOS_AntiCheatClient_OnClientIntegrityViolatedCallbackInfo, (
	
	void* ClientData;
	
	EOS_EAntiCheatClientViolationType ViolationType;
	
	const char* ViolationMessage;
));

#define EOS_ANTICHEATCLIENT_ONMESSAGETOSERVERCALLBACK_MAX_MESSAGE_SIZE 512

EOS_DECLARE_CALLBACK(EOS_AntiCheatClient_OnMessageToServerCallback, const EOS_AntiCheatClient_OnMessageToServerCallbackInfo* Data);

EOS_DECLARE_CALLBACK(EOS_AntiCheatClient_OnClientIntegrityViolatedCallback, const EOS_AntiCheatClient_OnClientIntegrityViolatedCallbackInfo* Data);

#define EOS_ANTICHEATCLIENT_ONMESSAGETOPEERCALLBACK_MAX_MESSAGE_SIZE 512

EOS_DECLARE_CALLBACK(EOS_AntiCheatClient_OnMessageToPeerCallback, const EOS_AntiCheatCommon_OnMessageToClientCallbackInfo* Data);

EOS_DECLARE_CALLBACK(EOS_AntiCheatClient_OnPeerActionRequiredCallback, const EOS_AntiCheatCommon_OnClientActionRequiredCallbackInfo* Data);

EOS_DECLARE_CALLBACK(EOS_AntiCheatClient_OnPeerAuthStatusChangedCallback, const EOS_AntiCheatCommon_OnClientAuthStatusChangedCallbackInfo* Data);

#define EOS_ANTICHEATCLIENT_ADDNOTIFYMESSAGETOSERVER_API_LATEST 1

EOS_STRUCT(EOS_AntiCheatClient_AddNotifyMessageToServerOptions, (
	
	int32_t ApiVersion;
));

#define EOS_ANTICHEATCLIENT_ADDNOTIFYMESSAGETOPEER_API_LATEST 1

EOS_STRUCT(EOS_AntiCheatClient_AddNotifyMessageToPeerOptions, (
	
	int32_t ApiVersion;
));

#define EOS_ANTICHEATCLIENT_ADDNOTIFYPEERACTIONREQUIRED_API_LATEST 1

EOS_STRUCT(EOS_AntiCheatClient_AddNotifyPeerActionRequiredOptions, (
	
	int32_t ApiVersion;
));

#define EOS_ANTICHEATCLIENT_ADDNOTIFYPEERAUTHSTATUSCHANGED_API_LATEST 1

EOS_STRUCT(EOS_AntiCheatClient_AddNotifyPeerAuthStatusChangedOptions, (
	
	int32_t ApiVersion;
));

#define EOS_ANTICHEATCLIENT_ADDNOTIFYCLIENTINTEGRITYVIOLATED_API_LATEST 1

EOS_STRUCT(EOS_AntiCheatClient_AddNotifyClientIntegrityViolatedOptions, (
	
	int32_t ApiVersion;
));

#define EOS_ANTICHEATCLIENT_BEGINSESSION_API_LATEST 3

EOS_STRUCT(EOS_AntiCheatClient_BeginSessionOptions, (
	
	int32_t ApiVersion;
	
	EOS_ProductUserId LocalUserId;
	
	EOS_EAntiCheatClientMode Mode;
));

#define EOS_ANTICHEATCLIENT_ENDSESSION_API_LATEST 1

EOS_STRUCT(EOS_AntiCheatClient_EndSessionOptions, (
	
	int32_t ApiVersion;
));

#define EOS_ANTICHEATCLIENT_RESERVED01m_API_LATEST 1

EOS_STRUCT(EOS_AntiCheatClient_Reserved01Options, (
	
	int32_t ApiVersion;
));

#define EOS_ANTICHEATCLIENT_RESERVED02m_API_LATEST 1

EOS_STRUCT(EOS_AntiCheatClient_Reserved02Options, (
	
	int32_t ApiVersion;
	
	int64_t Reserved1;
	
	uint32_t Reserved2;
	
	uint32_t Reserved3;
	
	const void* Reserved4;
));

#define EOS_ANTICHEATCLIENT_GETMODULEBUILDID_API_LATEST 1

EOS_STRUCT(EOS_AntiCheatClient_GetModuleBuildIdOptions, (
	
	int32_t ApiVersion;
));

#define EOS_ANTICHEATCLIENT_ADDEXTERNALINTEGRITYCATALOG_API_LATEST 1

EOS_STRUCT(EOS_AntiCheatClient_AddExternalIntegrityCatalogOptions, (
	
	int32_t ApiVersion;
	
	const char* PathToBinFile;
));

#define EOS_ANTICHEATCLIENT_RECEIVEMESSAGEFROMSERVER_API_LATEST 1

EOS_STRUCT(EOS_AntiCheatClient_ReceiveMessageFromServerOptions, (
	
	int32_t ApiVersion;
	
	uint32_t DataLengthBytes;
	
	const void* Data;
));

#define EOS_ANTICHEATCLIENT_GETPROTECTMESSAGEOUTPUTLENGTH_API_LATEST 1

EOS_STRUCT(EOS_AntiCheatClient_GetProtectMessageOutputLengthOptions, (
	
	int32_t ApiVersion;
	
	uint32_t DataLengthBytes;
));

#define EOS_ANTICHEATCLIENT_PROTECTMESSAGE_API_LATEST 1

EOS_STRUCT(EOS_AntiCheatClient_ProtectMessageOptions, (
	
	int32_t ApiVersion;
	
	uint32_t DataLengthBytes;
	
	const void* Data;
	
	uint32_t OutBufferSizeBytes;
));

#define EOS_ANTICHEATCLIENT_UNPROTECTMESSAGE_API_LATEST 1

EOS_STRUCT(EOS_AntiCheatClient_UnprotectMessageOptions, (
	
	int32_t ApiVersion;
	
	uint32_t DataLengthBytes;
	
	const void* Data;
	
	uint32_t OutBufferSizeBytes;
));

#define EOS_ANTICHEATCLIENT_PEER_SELF (EOS_AntiCheatCommon_ClientHandle)(-1)

#define EOS_ANTICHEATCLIENT_REGISTERPEER_MIN_AUTHENTICATIONTIMEOUT 40

#define EOS_ANTICHEATCLIENT_REGISTERPEER_MAX_AUTHENTICATIONTIMEOUT 120

#define EOS_ANTICHEATCLIENT_REGISTERPEER_API_LATEST 3

EOS_STRUCT(EOS_AntiCheatClient_RegisterPeerOptions, (
	
	int32_t ApiVersion;
	
	EOS_AntiCheatCommon_ClientHandle PeerHandle;
	
	EOS_EAntiCheatCommonClientType ClientType;
	
	EOS_EAntiCheatCommonClientPlatform ClientPlatform;
	
	uint32_t AuthenticationTimeout;
	
	const char* AccountId_DEPRECATED;
	
	const char* IpAddress;
	
	EOS_ProductUserId PeerProductUserId;
));

#define EOS_ANTICHEATCLIENT_UNREGISTERPEER_API_LATEST 1

EOS_STRUCT(EOS_AntiCheatClient_UnregisterPeerOptions, (
	
	int32_t ApiVersion;
	
	EOS_AntiCheatCommon_ClientHandle PeerHandle;
));

#define EOS_ANTICHEATCLIENT_RECEIVEMESSAGEFROMPEER_API_LATEST 1

EOS_STRUCT(EOS_AntiCheatClient_ReceiveMessageFromPeerOptions, (
	
	int32_t ApiVersion;
	
	EOS_AntiCheatCommon_ClientHandle PeerHandle;
	
	uint32_t DataLengthBytes;
	
	const void* Data;
));

#pragma pack(pop)

#include "eos_anticheatclient_types_deprecated.inl"
