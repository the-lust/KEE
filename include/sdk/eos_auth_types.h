
#pragma once

#include "eos_auth_types_deprecated.inl"
#include "eos_common.h"

#pragma pack(push, 8)

EOS_EXTERN_C typedef struct EOS_AuthHandle* EOS_HAuth;

EOS_ENUM(EOS_ELoginCredentialType,
	
	EOS_LCT_Password = 0,
	
	EOS_LCT_ExchangeCode = 1,
	
	EOS_LCT_PersistentAuth = 2,
	
	EOS_LCT_DeviceCode = 3,
	
	EOS_LCT_Developer = 4,
	
	EOS_LCT_RefreshToken = 5,
	
	EOS_LCT_AccountPortal = 6,
	
	EOS_LCT_ExternalAuth = 7
);

#define EOS_AUTH_TOKEN_API_LATEST 2

EOS_ENUM(EOS_EAuthTokenType,
	
	EOS_ATT_Client = 0,
	
	EOS_ATT_User = 1
);

EOS_STRUCT(EOS_Auth_Token, (
	
	int32_t ApiVersion;
	
	const char* App;
	
	const char* ClientId;
	
	EOS_EpicAccountId AccountId;
	
	const char* AccessToken;
	
	double ExpiresIn;
	
	const char* ExpiresAt;
	
	EOS_EAuthTokenType AuthType;
	
	const char* RefreshToken;
	
	double RefreshExpiresIn;
	
	const char* RefreshExpiresAt;
));

EOS_DECLARE_FUNC(void) EOS_Auth_Token_Release(EOS_Auth_Token* AuthToken);

#define EOS_AUTH_CREDENTIALS_API_LATEST 4

EOS_STRUCT(EOS_Auth_Credentials, (
	
	int32_t ApiVersion;
	
	const char* Id;
	
	const char* Token;
	
	EOS_ELoginCredentialType Type;
	
	void* SystemAuthCredentialsOptions;
	
	EOS_EExternalCredentialType ExternalType;
));

#define EOS_AUTH_PINGRANTINFO_API_LATEST 2

EOS_STRUCT(EOS_Auth_PinGrantInfo, (
	
	int32_t ApiVersion;
	
	const char* UserCode;
	
	const char* VerificationURI;
	
	int32_t ExpiresIn;
	
	const char* VerificationURIComplete;
));

EOS_ENUM(EOS_EAuthScopeFlags,
	
	EOS_AS_NoFlags = 0x0,
	
	EOS_AS_BasicProfile = 0x1,
	
	EOS_AS_FriendsList = 0x2,
	
	EOS_AS_Presence = 0x4,
	
	EOS_AS_FriendsManagement = 0x8,
	
	EOS_AS_Email = 0x10,
	
	EOS_AS_Country = 0x20
);

EOS_ENUM_BOOLEAN_OPERATORS(EOS_EAuthScopeFlags)

#define EOS_LF_NO_USER_INTERFACE 0x00001

#define EOS_AUTH_LOGIN_API_LATEST 3

EOS_STRUCT(EOS_Auth_LoginOptions, (
	
	int32_t ApiVersion;
	
	const EOS_Auth_Credentials* Credentials;
	
	EOS_EAuthScopeFlags ScopeFlags;
	
	uint64_t LoginFlags;
));

EOS_STRUCT(EOS_Auth_LoginCallbackInfo, (
	
	EOS_EResult ResultCode;
	
	void* ClientData;
	
	EOS_EpicAccountId LocalUserId;
	
	const EOS_Auth_PinGrantInfo* PinGrantInfo;
	
	EOS_ContinuanceToken ContinuanceToken;
	
	const EOS_Auth_AccountFeatureRestrictedInfo* AccountFeatureRestrictedInfo_DEPRECATED;
	
	EOS_EpicAccountId SelectedAccountId;
));

EOS_DECLARE_CALLBACK(EOS_Auth_OnLoginCallback, const EOS_Auth_LoginCallbackInfo* Data);

#define EOS_AUTH_LOGOUT_API_LATEST 1

EOS_STRUCT(EOS_Auth_LogoutOptions, (
	
	int32_t ApiVersion;
	
	EOS_EpicAccountId LocalUserId;
));

EOS_STRUCT(EOS_Auth_LogoutCallbackInfo, (
	
	EOS_EResult ResultCode;
	
	void* ClientData;
	
	EOS_EpicAccountId LocalUserId;
));

EOS_DECLARE_CALLBACK(EOS_Auth_OnLogoutCallback, const EOS_Auth_LogoutCallbackInfo* Data);

EOS_ENUM(EOS_ELinkAccountFlags,
	
	EOS_LA_NoFlags = 0x0,
	
	EOS_LA_NintendoNsaId = 0x1
);

#define EOS_AUTH_LINKACCOUNT_API_LATEST 1

EOS_STRUCT(EOS_Auth_LinkAccountOptions, (
	
	int32_t ApiVersion;
	
	EOS_ELinkAccountFlags LinkAccountFlags;
	
	EOS_ContinuanceToken ContinuanceToken;
	
	EOS_EpicAccountId LocalUserId;
));

EOS_STRUCT(EOS_Auth_LinkAccountCallbackInfo, (
	
	EOS_EResult ResultCode;
	
	void* ClientData;
	
	EOS_EpicAccountId LocalUserId;
	
	const EOS_Auth_PinGrantInfo* PinGrantInfo;
	
	EOS_EpicAccountId SelectedAccountId;
));

EOS_DECLARE_CALLBACK(EOS_Auth_OnLinkAccountCallback, const EOS_Auth_LinkAccountCallbackInfo* Data);

#define EOS_AUTH_VERIFYUSERAUTH_API_LATEST 1

EOS_STRUCT(EOS_Auth_VerifyUserAuthOptions, (
	
	int32_t ApiVersion;
	
	const EOS_Auth_Token* AuthToken;
));

EOS_STRUCT(EOS_Auth_VerifyUserAuthCallbackInfo, (
	
	EOS_EResult ResultCode;
	
	void* ClientData;
));

EOS_DECLARE_CALLBACK(EOS_Auth_OnVerifyUserAuthCallback, const EOS_Auth_VerifyUserAuthCallbackInfo* Data);

#define EOS_AUTH_COPYUSERAUTHTOKEN_API_LATEST 1

EOS_STRUCT(EOS_Auth_CopyUserAuthTokenOptions, (
	
	int32_t ApiVersion;
));

#define EOS_AUTH_COPYIDTOKEN_API_LATEST 1

EOS_STRUCT(EOS_Auth_CopyIdTokenOptions, (
	
	int32_t ApiVersion;
	
	EOS_EpicAccountId AccountId;
));

#define EOS_AUTH_IDTOKEN_API_LATEST 1

EOS_STRUCT(EOS_Auth_IdToken, (
	
	int32_t ApiVersion;
	
	EOS_EpicAccountId AccountId;
	
	const char* JsonWebToken;
));

EOS_DECLARE_FUNC(void) EOS_Auth_IdToken_Release(EOS_Auth_IdToken* IdToken);

#define EOS_AUTH_QUERYIDTOKEN_API_LATEST 1

EOS_STRUCT(EOS_Auth_QueryIdTokenOptions, (
	
	int32_t ApiVersion;
	
	EOS_EpicAccountId LocalUserId;
	
	EOS_EpicAccountId TargetAccountId;
));

EOS_STRUCT(EOS_Auth_QueryIdTokenCallbackInfo, (
	
	EOS_EResult ResultCode;
	
	void* ClientData;
	
	EOS_EpicAccountId LocalUserId;
	
	EOS_EpicAccountId TargetAccountId;
));

EOS_DECLARE_CALLBACK(EOS_Auth_OnQueryIdTokenCallback, const EOS_Auth_QueryIdTokenCallbackInfo* Data);

#define EOS_AUTH_VERIFYIDTOKEN_API_LATEST 1

EOS_STRUCT(EOS_Auth_VerifyIdTokenOptions, (
	
	int32_t ApiVersion;
	
	const EOS_Auth_IdToken* IdToken;
));

EOS_STRUCT(EOS_Auth_VerifyIdTokenCallbackInfo, (
	
	EOS_EResult ResultCode;
	
	void* ClientData;
	
	const char* ApplicationId;
	
	const char* ClientId;
	
	const char* ProductId;
	
	const char* SandboxId;
	
	const char* DeploymentId;
	
	const char* DisplayName;
	
	EOS_Bool bIsExternalAccountInfoPresent;
	
	EOS_EExternalAccountType ExternalAccountIdType;
	
	const char* ExternalAccountId;
	
	const char* ExternalAccountDisplayName;
	
	const char* Platform;
));

EOS_DECLARE_CALLBACK(EOS_Auth_OnVerifyIdTokenCallback, const EOS_Auth_VerifyIdTokenCallbackInfo* Data);

#define EOS_AUTH_ADDNOTIFYLOGINSTATUSCHANGED_API_LATEST 1

EOS_STRUCT(EOS_Auth_AddNotifyLoginStatusChangedOptions, (
	
	int32_t ApiVersion;
));

#define EOS_AUTH_DELETEPERSISTENTAUTH_API_LATEST 2

EOS_STRUCT(EOS_Auth_DeletePersistentAuthOptions, (
	
	int32_t ApiVersion;
	
	const char* RefreshToken;
));

EOS_STRUCT(EOS_Auth_DeletePersistentAuthCallbackInfo, (
	
	EOS_EResult ResultCode;
	
	void* ClientData;
));

EOS_DECLARE_CALLBACK(EOS_Auth_OnDeletePersistentAuthCallback, const EOS_Auth_DeletePersistentAuthCallbackInfo* Data);

EOS_STRUCT(EOS_Auth_LoginStatusChangedCallbackInfo, (
	
	void* ClientData;
	
	EOS_EpicAccountId LocalUserId;
	
	EOS_ELoginStatus PrevStatus;
	
	EOS_ELoginStatus CurrentStatus;
));

EOS_DECLARE_CALLBACK(EOS_Auth_OnLoginStatusChangedCallback, const EOS_Auth_LoginStatusChangedCallbackInfo* Data);

#pragma pack(pop)
