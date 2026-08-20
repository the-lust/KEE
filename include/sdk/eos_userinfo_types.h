
#pragma once

#include "eos_common.h"

#pragma pack(push, 8)

EOS_EXTERN_C typedef struct EOS_UserInfoHandle* EOS_HUserInfo;

#define EOS_USERINFO_QUERYUSERINFO_API_LATEST 1

EOS_STRUCT(EOS_UserInfo_QueryUserInfoOptions, (
	
	int32_t ApiVersion;
	
	EOS_EpicAccountId LocalUserId;
	
	EOS_EpicAccountId TargetUserId;
));

EOS_STRUCT(EOS_UserInfo_QueryUserInfoCallbackInfo, (
	
	EOS_EResult ResultCode;
	
	void* ClientData;
	
	EOS_EpicAccountId LocalUserId;
	
	EOS_EpicAccountId TargetUserId;
));

EOS_DECLARE_CALLBACK(EOS_UserInfo_OnQueryUserInfoCallback, const EOS_UserInfo_QueryUserInfoCallbackInfo* Data);

#define EOS_USERINFO_QUERYUSERINFOBYDISPLAYNAME_API_LATEST 1

EOS_STRUCT(EOS_UserInfo_QueryUserInfoByDisplayNameOptions, (
	
	int32_t ApiVersion;
	
	EOS_EpicAccountId LocalUserId;
	
	const char* DisplayName;
));

EOS_STRUCT(EOS_UserInfo_QueryUserInfoByDisplayNameCallbackInfo, (
	
	EOS_EResult ResultCode;
	
	void* ClientData;
	
	EOS_EpicAccountId LocalUserId;
	
	EOS_EpicAccountId TargetUserId;
	
	const char* DisplayName;
));

EOS_DECLARE_CALLBACK(EOS_UserInfo_OnQueryUserInfoByDisplayNameCallback, const EOS_UserInfo_QueryUserInfoByDisplayNameCallbackInfo* Data);

#define EOS_USERINFO_QUERYUSERINFOBYEXTERNALACCOUNT_API_LATEST 1

EOS_STRUCT(EOS_UserInfo_QueryUserInfoByExternalAccountOptions, (
	
	int32_t ApiVersion;
	
	EOS_EpicAccountId LocalUserId;
	
	const char* ExternalAccountId;
	
	EOS_EExternalAccountType AccountType;
));

EOS_STRUCT(EOS_UserInfo_QueryUserInfoByExternalAccountCallbackInfo, (
	
	EOS_EResult ResultCode;
	
	void* ClientData;
	
	EOS_EpicAccountId LocalUserId;
	
	const char* ExternalAccountId;
	
	EOS_EExternalAccountType AccountType;
	
	EOS_EpicAccountId TargetUserId;
));

EOS_DECLARE_CALLBACK(EOS_UserInfo_OnQueryUserInfoByExternalAccountCallback, const EOS_UserInfo_QueryUserInfoByExternalAccountCallbackInfo* Data);

#define EOS_USERINFO_MAX_DISPLAYNAME_CHARACTERS 16

#define EOS_USERINFO_MAX_DISPLAYNAME_UTF8m_LENGTH 64

#define EOS_USERINFO_COPYUSERINFO_API_LATEST 3

EOS_STRUCT(EOS_UserInfo, (
	
	int32_t ApiVersion;
	
	EOS_EpicAccountId UserId;
	
	const char* Country;
	
	const char* DisplayName;
	
	const char* PreferredLanguage;
	
	const char* Nickname;
	
	const char* DisplayNameSanitized;
));

EOS_STRUCT(EOS_UserInfo_CopyUserInfoOptions, (
	
	int32_t ApiVersion;
	
	EOS_EpicAccountId LocalUserId;
	
	EOS_EpicAccountId TargetUserId;
));

EOS_DECLARE_FUNC(void) EOS_UserInfo_Release(EOS_UserInfo* UserInfo);

#define EOS_USERINFO_EXTERNALUSERINFO_API_LATEST 2

EOS_STRUCT(EOS_UserInfo_ExternalUserInfo, (
	
	int32_t ApiVersion;
	
	EOS_EExternalAccountType AccountType;
	
	const char* AccountId;
	
	const char* DisplayName;
	
	const char* DisplayNameSanitized;
));

#define EOS_USERINFO_GETEXTERNALUSERINFOCOUNT_API_LATEST 1

EOS_STRUCT(EOS_UserInfo_GetExternalUserInfoCountOptions, (
	
	int32_t ApiVersion;
	
	EOS_EpicAccountId LocalUserId;
	
	EOS_EpicAccountId TargetUserId;
));

#define EOS_USERINFO_COPYEXTERNALUSERINFOBYINDEX_API_LATEST 1

EOS_STRUCT(EOS_UserInfo_CopyExternalUserInfoByIndexOptions, (
	
	int32_t ApiVersion;
	
	EOS_EpicAccountId LocalUserId;
	
	EOS_EpicAccountId TargetUserId;
	
	uint32_t Index;
));

#define EOS_USERINFO_COPYEXTERNALUSERINFOBYACCOUNTTYPE_API_LATEST 1

EOS_STRUCT(EOS_UserInfo_CopyExternalUserInfoByAccountTypeOptions, (
	
	int32_t ApiVersion;
	
	EOS_EpicAccountId LocalUserId;
	
	EOS_EpicAccountId TargetUserId;
	
	EOS_EExternalAccountType AccountType;
));

#define EOS_USERINFO_COPYEXTERNALUSERINFOBYACCOUNTID_API_LATEST 1

EOS_STRUCT(EOS_UserInfo_CopyExternalUserInfoByAccountIdOptions, (
	
	int32_t ApiVersion;
	
	EOS_EpicAccountId LocalUserId;
	
	EOS_EpicAccountId TargetUserId;
	
	const char* AccountId;
));

EOS_DECLARE_FUNC(void) EOS_UserInfo_ExternalUserInfo_Release(EOS_UserInfo_ExternalUserInfo* ExternalUserInfo);

#define EOS_USERINFO_BESTDISPLAYNAME_API_LATEST 1

EOS_STRUCT(EOS_UserInfo_BestDisplayName, (
	
	int32_t ApiVersion;
	
	EOS_EpicAccountId UserId;
	
	const char* DisplayName;
	
	const char* DisplayNameSanitized;
	
	const char* Nickname;
	
	EOS_OnlinePlatformType PlatformType;
));

EOS_DECLARE_FUNC(void) EOS_UserInfo_BestDisplayName_Release(EOS_UserInfo_BestDisplayName* BestDisplayName);

#define EOS_USERINFO_COPYBESTDISPLAYNAME_API_LATEST 1

EOS_STRUCT(EOS_UserInfo_CopyBestDisplayNameOptions, (
	
	int32_t ApiVersion;
	
	EOS_EpicAccountId LocalUserId;
	
	EOS_EpicAccountId TargetUserId;
));

#define EOS_USERINFO_COPYBESTDISPLAYNAMEWITHPLATFORM_API_LATEST 1

EOS_STRUCT(EOS_UserInfo_CopyBestDisplayNameWithPlatformOptions, (
	
	int32_t ApiVersion;
	
	EOS_EpicAccountId LocalUserId;
	
	EOS_EpicAccountId TargetUserId;
	
	EOS_OnlinePlatformType TargetPlatformType;
));

#define EOS_USERINFO_GETLOCALPLATFORMTYPE_API_LATEST 1

EOS_STRUCT(EOS_UserInfo_GetLocalPlatformTypeOptions, (
	
	int32_t ApiVersion;
));

#pragma pack(pop)
