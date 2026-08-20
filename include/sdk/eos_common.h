

#pragma once

#include "eos_base.h"

#pragma pack(push, 8)

#undef EOS_RESULT_VALUE
#undef EOS_RESULT_VALUE_LAST
#define EOS_RESULT_VALUE(Name, Value) Name = Value,
#define EOS_RESULT_VALUE_LAST(Name, Value) Name = Value

EOS_ENUM_START(EOS_EResult)
#include "eos_result.h"
EOS_ENUM_END(EOS_EResult);

#undef EOS_RESULT_VALUE
#undef EOS_RESULT_VALUE_LAST

EOS_DECLARE_FUNC(const char*) EOS_EResult_ToString(EOS_EResult Result);

EOS_DECLARE_FUNC(EOS_Bool) EOS_EResult_IsOperationComplete(EOS_EResult Result);

EOS_DECLARE_FUNC(EOS_EResult) EOS_ByteArray_ToString(const uint8_t* ByteArray, const uint32_t Length, char* OutBuffer, uint32_t* InOutBufferLength);

typedef struct EOS_EpicAccountIdDetails* EOS_EpicAccountId;

EOS_DECLARE_FUNC(EOS_Bool) EOS_EpicAccountId_IsValid(EOS_EpicAccountId AccountId);

EOS_DECLARE_FUNC(EOS_EResult) EOS_EpicAccountId_ToString(EOS_EpicAccountId AccountId, char* OutBuffer, int32_t* InOutBufferLength);

EOS_DECLARE_FUNC(EOS_EpicAccountId) EOS_EpicAccountId_FromString(const char* AccountIdString);

#define EOS_EPICACCOUNTID_MAX_LENGTH 32

typedef struct EOS_ProductUserIdDetails* EOS_ProductUserId;

EOS_DECLARE_FUNC(EOS_Bool) EOS_ProductUserId_IsValid(EOS_ProductUserId AccountId);

EOS_DECLARE_FUNC(EOS_EResult) EOS_ProductUserId_ToString(EOS_ProductUserId AccountId, char* OutBuffer, int32_t* InOutBufferLength);

EOS_DECLARE_FUNC(EOS_ProductUserId) EOS_ProductUserId_FromString(const char* ProductUserIdString);

#define EOS_PRODUCTUSERID_MAX_LENGTH 32

EOS_EXTERN_C typedef uint64_t EOS_NotificationId;

#define EOS_INVALID_NOTIFICATIONID ((EOS_NotificationId)0)

typedef struct EOS_ContinuanceTokenDetails* EOS_ContinuanceToken;

EOS_DECLARE_FUNC(EOS_EResult) EOS_ContinuanceToken_ToString(EOS_ContinuanceToken ContinuanceToken, char* OutBuffer, int32_t* InOutBufferLength);

EOS_ENUM(EOS_ELoginStatus,
	
	EOS_LS_NotLoggedIn = 0,
	
	EOS_LS_UsingLocalProfile = 1,
	
	EOS_LS_LoggedIn = 2
);

EOS_ENUM(EOS_EAttributeType,
	
	EOS_AT_BOOLEAN = 0,
	
	EOS_AT_INT64 = 1,
	
	EOS_AT_DOUBLE = 2,
	
	EOS_AT_STRING = 3
);

typedef EOS_EAttributeType EOS_ESessionAttributeType;
typedef EOS_EAttributeType EOS_ELobbyAttributeType;

EOS_ENUM(EOS_EComparisonOp,
	
	EOS_CO_EQUAL = 0,
	
	EOS_CO_NOTEQUAL = 1,
	
	EOS_CO_GREATERTHAN = 2,
	
	EOS_CO_GREATERTHANOREQUAL = 3,
	
	EOS_CO_LESSTHAN = 4,
	
	EOS_CO_LESSTHANOREQUAL = 5,
	
	EOS_CO_DISTANCE = 6,
	
	EOS_CO_ANYOF = 7,
	
	EOS_CO_NOTANYOF = 8,
	
	EOS_CO_ONEOF = 9,
	
	EOS_CO_NOTONEOF = 10,
	
	EOS_CO_CONTAINS = 11,
	
	EOS_CO_REGEXMATCH = 12,
	
	EOS_CO_SIZE = 13
);

typedef EOS_EComparisonOp EOS_EOnlineComparisonOp;

EOS_ENUM(EOS_EExternalAccountType,
	
	EOS_EAT_EPIC = 0,
	
	EOS_EAT_STEAM = 1,
	
	EOS_EAT_PSN = 2,
	
	EOS_EAT_XBL = 3,
	
	EOS_EAT_DISCORD = 4,
	
	EOS_EAT_GOG = 5,
	
	EOS_EAT_NINTENDO = 6,
	
	EOS_EAT_UPLAY = 7,
	
	EOS_EAT_OPENID = 8,
	
	EOS_EAT_APPLE = 9,
	
	EOS_EAT_GOOGLE = 10,
	
	EOS_EAT_OCULUS = 11,
	
	EOS_EAT_ITCHIO = 12,
	
	EOS_EAT_AMAZON = 13,
	
	EOS_EAT_VIVEPORT = 14
);

EOS_ENUM(EOS_EExternalCredentialType,
	
	EOS_ECT_EPIC = 0,
	
	EOS_ECT_STEAM_APP_TICKET = 1,
	
	EOS_ECT_PSN_ID_TOKEN = 2,
	
	EOS_ECT_XBL_XSTS_TOKEN = 3,
	
	EOS_ECT_DISCORD_ACCESS_TOKEN = 4,
	
	EOS_ECT_GOG_SESSION_TICKET = 5,
	
	EOS_ECT_NINTENDO_ID_TOKEN = 6,
	
	EOS_ECT_NINTENDO_NSA_ID_TOKEN = 7,
	
	EOS_ECT_UPLAY_ACCESS_TOKEN = 8,
	
	EOS_ECT_OPENID_ACCESS_TOKEN = 9,
	
	EOS_ECT_DEVICEID_ACCESS_TOKEN = 10,
	
	EOS_ECT_APPLE_ID_TOKEN = 11,
	
	EOS_ECT_GOOGLE_ID_TOKEN = 12,
	
	EOS_ECT_OCULUS_USERID_NONCE = 13,
	
	EOS_ECT_ITCHIO_JWT = 14,
	
	EOS_ECT_ITCHIO_KEY = 15,
	
	EOS_ECT_EPIC_ID_TOKEN = 16,
	
	EOS_ECT_AMAZON_ACCESS_TOKEN = 17,
	
	EOS_ECT_STEAM_SESSION_TICKET = 18,
	
	EOS_ECT_VIVEPORT_USER_TOKEN = 19
);

EOS_EXTERN_C typedef const char* EOS_IntegratedPlatformType;

#define EOS_IPT_Unknown (const char*)NULL

EOS_EXTERN_C typedef uint32_t EOS_OnlinePlatformType;

#define EOS_OPT_Unknown 0

#define EOS_OPT_Epic 100

#define EOS_OPT_PSN 1000

#define EOS_OPT_Nintendo 2000

#define EOS_OPT_XBL 3000

#define EOS_OPT_Steam 4000

#pragma pack(pop)
