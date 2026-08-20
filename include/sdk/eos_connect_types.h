
#pragma once

#include "eos_common.h"

#pragma pack(push, 8)

EOS_EXTERN_C typedef struct EOS_ConnectHandle* EOS_HConnect;

#define EOS_CONNECT_EXTERNAL_ACCOUNT_ID_MAX_LENGTH 256

#define EOS_CONNECT_CREDENTIALS_API_LATEST 1

EOS_STRUCT(EOS_Connect_Credentials, (
	
	int32_t ApiVersion;
	
	const char* Token;
	
	EOS_EExternalCredentialType Type;
));

#define EOS_CONNECT_USERLOGININFO_DISPLAYNAME_MAX_LENGTH 32

#define EOS_CONNECT_USERLOGININFO_API_LATEST 2

EOS_STRUCT(EOS_Connect_UserLoginInfo, (
	
	int32_t ApiVersion;
	
	const char* DisplayName;
	
	const char* NsaIdToken;
));

#define EOS_CONNECT_LOGIN_API_LATEST 2

EOS_STRUCT(EOS_Connect_LoginOptions, (
	
	int32_t ApiVersion;
	
	const EOS_Connect_Credentials* Credentials;
	
	const EOS_Connect_UserLoginInfo* UserLoginInfo;
));

EOS_STRUCT(EOS_Connect_LoginCallbackInfo, (
	
	EOS_EResult ResultCode;
	
	void* ClientData;
	
	EOS_ProductUserId LocalUserId;
	
	EOS_ContinuanceToken ContinuanceToken;
));

EOS_DECLARE_CALLBACK(EOS_Connect_OnLoginCallback, const EOS_Connect_LoginCallbackInfo* Data);

#define EOS_CONNECT_LOGOUT_API_LATEST 1

EOS_STRUCT(EOS_Connect_LogoutOptions, (
	
	int32_t ApiVersion;

	EOS_ProductUserId LocalUserId;
));

EOS_STRUCT(EOS_Connect_LogoutCallbackInfo, (
	
	EOS_EResult ResultCode;

	void* ClientData;

	EOS_ProductUserId LocalUserId;
));

EOS_DECLARE_CALLBACK(EOS_Connect_OnLogoutCallback, const EOS_Connect_LogoutCallbackInfo* Data);

#define EOS_CONNECT_CREATEUSER_API_LATEST 1

EOS_STRUCT(EOS_Connect_CreateUserOptions, (
	
	int32_t ApiVersion;
	
	EOS_ContinuanceToken ContinuanceToken;
));

EOS_STRUCT(EOS_Connect_CreateUserCallbackInfo, (
	
	EOS_EResult ResultCode;
	
	void* ClientData;
	
	EOS_ProductUserId LocalUserId;
));

EOS_DECLARE_CALLBACK(EOS_Connect_OnCreateUserCallback, const EOS_Connect_CreateUserCallbackInfo* Data);

#define EOS_CONNECT_LINKACCOUNT_API_LATEST 1

EOS_STRUCT(EOS_Connect_LinkAccountOptions, (
	
	int32_t ApiVersion;
	
	EOS_ProductUserId LocalUserId;
	
	EOS_ContinuanceToken ContinuanceToken;
));

EOS_STRUCT(EOS_Connect_LinkAccountCallbackInfo, (
	
	EOS_EResult ResultCode;
	
	void* ClientData;
	
	EOS_ProductUserId LocalUserId;
));

EOS_DECLARE_CALLBACK(EOS_Connect_OnLinkAccountCallback, const EOS_Connect_LinkAccountCallbackInfo* Data);

#define EOS_CONNECT_UNLINKACCOUNT_API_LATEST 1

EOS_STRUCT(EOS_Connect_UnlinkAccountOptions, (
	
	int32_t ApiVersion;
	
	EOS_ProductUserId LocalUserId;
));

EOS_STRUCT(EOS_Connect_UnlinkAccountCallbackInfo, (
	
	EOS_EResult ResultCode;
	
	void* ClientData;
	
	EOS_ProductUserId LocalUserId;
));

EOS_DECLARE_CALLBACK(EOS_Connect_OnUnlinkAccountCallback, const EOS_Connect_UnlinkAccountCallbackInfo* Data);

#define EOS_CONNECT_CREATEDEVICEID_API_LATEST 1

#define EOS_CONNECT_CREATEDEVICEID_DEVICEMODEL_MAX_LENGTH 64

EOS_STRUCT(EOS_Connect_CreateDeviceIdOptions, (
	
	int32_t ApiVersion;
	
	const char* DeviceModel;
));

EOS_STRUCT(EOS_Connect_CreateDeviceIdCallbackInfo, (
	
	EOS_EResult ResultCode;
	
	void* ClientData;
));

EOS_DECLARE_CALLBACK(EOS_Connect_OnCreateDeviceIdCallback, const EOS_Connect_CreateDeviceIdCallbackInfo* Data);

#define EOS_CONNECT_DELETEDEVICEID_API_LATEST 1

EOS_STRUCT(EOS_Connect_DeleteDeviceIdOptions, (
	
	int32_t ApiVersion;
));

EOS_STRUCT(EOS_Connect_DeleteDeviceIdCallbackInfo, (
	
	EOS_EResult ResultCode;
	
	void* ClientData;
));

EOS_DECLARE_CALLBACK(EOS_Connect_OnDeleteDeviceIdCallback, const EOS_Connect_DeleteDeviceIdCallbackInfo* Data);

#define EOS_CONNECT_TRANSFERDEVICEIDACCOUNT_API_LATEST 1

EOS_STRUCT(EOS_Connect_TransferDeviceIdAccountOptions, (
	
	int32_t ApiVersion;
	
	EOS_ProductUserId PrimaryLocalUserId;
	
	EOS_ProductUserId LocalDeviceUserId;
	
	EOS_ProductUserId ProductUserIdToPreserve;
));

EOS_STRUCT(EOS_Connect_TransferDeviceIdAccountCallbackInfo, (
	
	EOS_EResult ResultCode;
	
	void* ClientData;
	
	EOS_ProductUserId LocalUserId;
));

EOS_DECLARE_CALLBACK(EOS_Connect_OnTransferDeviceIdAccountCallback, const EOS_Connect_TransferDeviceIdAccountCallbackInfo* Data);

#define EOS_CONNECT_QUERYEXTERNALACCOUNTMAPPINGS_API_LATEST 1

#define EOS_CONNECT_QUERYEXTERNALACCOUNTMAPPINGS_MAX_ACCOUNT_IDS 128

EOS_STRUCT(EOS_Connect_QueryExternalAccountMappingsOptions, (
	
	int32_t ApiVersion;
	
	EOS_ProductUserId LocalUserId;
	
	EOS_EExternalAccountType AccountIdType;
	
	const char** ExternalAccountIds;
	
	uint32_t ExternalAccountIdCount;
));

EOS_STRUCT(EOS_Connect_QueryExternalAccountMappingsCallbackInfo, (
	
	EOS_EResult ResultCode;
	
	void* ClientData;
	
	EOS_ProductUserId LocalUserId;
));

EOS_DECLARE_CALLBACK(EOS_Connect_OnQueryExternalAccountMappingsCallback, const EOS_Connect_QueryExternalAccountMappingsCallbackInfo* Data);

#define EOS_CONNECT_GETEXTERNALACCOUNTMAPPING_API_LATEST 1

#define EOS_CONNECT_GETEXTERNALACCOUNTMAPPINGS_API_LATEST EOS_CONNECT_GETEXTERNALACCOUNTMAPPING_API_LATEST

EOS_STRUCT(EOS_Connect_GetExternalAccountMappingsOptions, (
	
	int32_t ApiVersion;
	
	EOS_ProductUserId LocalUserId;
	
	EOS_EExternalAccountType AccountIdType;
	
	const char* TargetExternalUserId;
));

#define EOS_CONNECT_QUERYPRODUCTUSERIDMAPPINGS_API_LATEST 2

EOS_STRUCT(EOS_Connect_QueryProductUserIdMappingsOptions, (
	
	int32_t ApiVersion;
	
	EOS_ProductUserId LocalUserId;
	
	EOS_EExternalAccountType AccountIdType_DEPRECATED;
	
	EOS_ProductUserId* ProductUserIds;
	
	uint32_t ProductUserIdCount;
));

EOS_STRUCT(EOS_Connect_QueryProductUserIdMappingsCallbackInfo, (
	
	EOS_EResult ResultCode;
	
	void* ClientData;
	
	EOS_ProductUserId LocalUserId;
));

EOS_DECLARE_CALLBACK(EOS_Connect_OnQueryProductUserIdMappingsCallback, const EOS_Connect_QueryProductUserIdMappingsCallbackInfo* Data);

#define EOS_CONNECT_GETPRODUCTUSERIDMAPPING_API_LATEST 1

EOS_STRUCT(EOS_Connect_GetProductUserIdMappingOptions, (
	
	int32_t ApiVersion;
	
	EOS_ProductUserId LocalUserId;
	
	EOS_EExternalAccountType AccountIdType;
	
	EOS_ProductUserId TargetProductUserId;
));

#define EOS_CONNECT_GETPRODUCTUSEREXTERNALACCOUNTCOUNT_API_LATEST 1

EOS_STRUCT(EOS_Connect_GetProductUserExternalAccountCountOptions, (
	
	int32_t ApiVersion;
	
	EOS_ProductUserId TargetUserId;
));

#define EOS_CONNECT_COPYPRODUCTUSEREXTERNALACCOUNTBYINDEX_API_LATEST 1

EOS_STRUCT(EOS_Connect_CopyProductUserExternalAccountByIndexOptions, (
	
	int32_t ApiVersion;
	
	EOS_ProductUserId TargetUserId;
	
	uint32_t ExternalAccountInfoIndex;
));

#define EOS_CONNECT_COPYPRODUCTUSEREXTERNALACCOUNTBYACCOUNTTYPE_API_LATEST 1

EOS_STRUCT(EOS_Connect_CopyProductUserExternalAccountByAccountTypeOptions, (
	
	int32_t ApiVersion;
	
	EOS_ProductUserId TargetUserId;
	
	EOS_EExternalAccountType AccountIdType;
));

#define EOS_CONNECT_COPYPRODUCTUSEREXTERNALACCOUNTBYACCOUNTID_API_LATEST 1

EOS_STRUCT(EOS_Connect_CopyProductUserExternalAccountByAccountIdOptions, (
	
	int32_t ApiVersion;
	
	EOS_ProductUserId TargetUserId;
	
	const char* AccountId;
));

#define EOS_CONNECT_COPYPRODUCTUSERINFO_API_LATEST 1

EOS_STRUCT(EOS_Connect_CopyProductUserInfoOptions, (
	
	int32_t ApiVersion;
	
	EOS_ProductUserId TargetUserId;
));

#define EOS_CONNECT_TIME_UNDEFINED -1

#define EOS_CONNECT_EXTERNALACCOUNTINFO_API_LATEST 1

EOS_STRUCT(EOS_Connect_ExternalAccountInfo, (
	
	int32_t ApiVersion;
	
	EOS_ProductUserId ProductUserId;
	
	const char* DisplayName;
	
	const char* AccountId;
	
	EOS_EExternalAccountType AccountIdType;
	
	int64_t LastLoginTime;
));

EOS_DECLARE_FUNC(void) EOS_Connect_ExternalAccountInfo_Release(EOS_Connect_ExternalAccountInfo* ExternalAccountInfo);

#define EOS_CONNECT_ADDNOTIFYAUTHEXPIRATION_API_LATEST 1

EOS_STRUCT(EOS_Connect_AddNotifyAuthExpirationOptions, (
	
	int32_t ApiVersion;
));

EOS_STRUCT(EOS_Connect_AuthExpirationCallbackInfo, (
	
	void* ClientData;
	
	EOS_ProductUserId LocalUserId;
));

EOS_DECLARE_CALLBACK(EOS_Connect_OnAuthExpirationCallback, const EOS_Connect_AuthExpirationCallbackInfo* Data);

#define EOS_CONNECT_ADDNOTIFYLOGINSTATUSCHANGED_API_LATEST 1

EOS_STRUCT(EOS_Connect_AddNotifyLoginStatusChangedOptions, (
	
	int32_t ApiVersion;
));

EOS_STRUCT(EOS_Connect_LoginStatusChangedCallbackInfo, (
	
	void* ClientData;
	
	EOS_ProductUserId LocalUserId;
	
	EOS_ELoginStatus PreviousStatus;
	
	EOS_ELoginStatus CurrentStatus;
));

EOS_DECLARE_CALLBACK(EOS_Connect_OnLoginStatusChangedCallback, const EOS_Connect_LoginStatusChangedCallbackInfo* Data);

#define EOS_CONNECT_IDTOKEN_API_LATEST 1

EOS_STRUCT(EOS_Connect_IdToken, (
	
	int32_t ApiVersion;
	
	EOS_ProductUserId ProductUserId;
	
	const char* JsonWebToken;
));

EOS_DECLARE_FUNC(void) EOS_Connect_IdToken_Release(EOS_Connect_IdToken* IdToken);

#define EOS_CONNECT_COPYIDTOKEN_API_LATEST 1

EOS_STRUCT(EOS_Connect_CopyIdTokenOptions, (
	
	int32_t ApiVersion;
	
	EOS_ProductUserId LocalUserId;
));

#define EOS_CONNECT_VERIFYIDTOKEN_API_LATEST 1

EOS_STRUCT(EOS_Connect_VerifyIdTokenOptions, (
	
	int32_t ApiVersion;
	
	const EOS_Connect_IdToken* IdToken;
));

EOS_STRUCT(EOS_Connect_VerifyIdTokenCallbackInfo, (
	
	EOS_EResult ResultCode;
	
	void* ClientData;
	
	EOS_ProductUserId ProductUserId;
	
	EOS_Bool bIsAccountInfoPresent;
	
	EOS_EExternalAccountType AccountIdType;
	
	const char* AccountId;
	
	const char* Platform;
	
	const char* DeviceType;
	
	const char* ClientId;
	
	const char* ProductId;
	
	const char* SandboxId;
	
	const char* DeploymentId;
));

EOS_DECLARE_CALLBACK(EOS_Connect_OnVerifyIdTokenCallback, const EOS_Connect_VerifyIdTokenCallbackInfo* Data);

#pragma pack(pop)
