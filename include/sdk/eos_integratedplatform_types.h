
#pragma once

#include "eos_common.h"

#pragma pack(push, 8)

typedef struct EOS_IntegratedPlatformOptionsContainerHandle* EOS_HIntegratedPlatformOptionsContainer;
typedef struct EOS_IntegratedPlatformHandle* EOS_HIntegratedPlatform;

EOS_ENUM(EOS_EIntegratedPlatformManagementFlags,
	
	EOS_IPMF_Disabled = 0x0001,
	
	EOS_IPMF_LibraryManagedByApplication = 0x0002,
	
	EOS_IPMF_LibraryManagedBySDK = 0x0004,
	
	EOS_IPMF_DisablePresenceMirroring = 0x0008,
	
	EOS_IPMF_DisableSDKManagedSessions = 0x0010,
	
	EOS_IPMF_PreferEOSIdentity = 0x0020,
	
	EOS_IPMF_PreferIntegratedIdentity = 0x0040,
	
	EOS_IPMF_ApplicationManagedIdentityLogin = 0x0080
);
EOS_ENUM_BOOLEAN_OPERATORS(EOS_EIntegratedPlatformManagementFlags);

#define EOS_IPT_Steam "STEAM"

#define EOS_INTEGRATEDPLATFORM_OPTIONS_API_LATEST 1

EOS_STRUCT(EOS_IntegratedPlatform_Options, (
	
	int32_t ApiVersion;
	
	EOS_IntegratedPlatformType Type;
	
	EOS_EIntegratedPlatformManagementFlags Flags;
	
	const void* InitOptions;
));

#define EOS_INTEGRATEDPLATFORM_STEAM_OPTIONS_API_LATEST 3

#define EOS_INTEGRATEDPLATFORM_STEAM_MAX_STEAMAPIINTERFACEVERSIONSARRAY_SIZE 4096

EOS_STRUCT(EOS_IntegratedPlatform_Steam_Options, (
	
	int32_t ApiVersion;
	
	const char* OverrideLibraryPath;
	
	uint32_t SteamMajorVersion;
	
	uint32_t SteamMinorVersion;

	const char* SteamApiInterfaceVersionsArray;

	uint32_t SteamApiInterfaceVersionsArrayBytes;
));

#define EOS_INTEGRATEDPLATFORM_CREATEINTEGRATEDPLATFORMOPTIONSCONTAINER_API_LATEST 1

EOS_STRUCT(EOS_IntegratedPlatform_CreateIntegratedPlatformOptionsContainerOptions, (
	
	int32_t ApiVersion;
));

EOS_DECLARE_FUNC(EOS_EResult) EOS_IntegratedPlatform_CreateIntegratedPlatformOptionsContainer(const EOS_IntegratedPlatform_CreateIntegratedPlatformOptionsContainerOptions* Options, EOS_HIntegratedPlatformOptionsContainer* OutIntegratedPlatformOptionsContainerHandle);

EOS_DECLARE_FUNC(void) EOS_IntegratedPlatformOptionsContainer_Release(EOS_HIntegratedPlatformOptionsContainer IntegratedPlatformOptionsContainerHandle);

#define EOS_INTEGRATEDPLATFORMOPTIONSCONTAINER_ADD_API_LATEST 1

EOS_STRUCT(EOS_IntegratedPlatformOptionsContainer_AddOptions, (
	
	int32_t ApiVersion;
	
	const EOS_IntegratedPlatform_Options* Options;
));

#define EOS_INTEGRATEDPLATFORM_SETUSERLOGINSTATUS_API_LATEST 1

EOS_STRUCT(EOS_IntegratedPlatform_SetUserLoginStatusOptions, (
	
	int32_t ApiVersion;

	EOS_IntegratedPlatformType PlatformType;

	const char* LocalPlatformUserId;

	EOS_ELoginStatus CurrentLoginStatus;
));

#define EOS_INTEGRATEDPLATFORM_ADDNOTIFYUSERLOGINSTATUSCHANGED_API_LATEST 1

EOS_STRUCT(EOS_IntegratedPlatform_AddNotifyUserLoginStatusChangedOptions, (
	
	int32_t ApiVersion;
));

EOS_STRUCT(EOS_IntegratedPlatform_UserLoginStatusChangedCallbackInfo, (
	
	void* ClientData;
	
	EOS_IntegratedPlatformType PlatformType;
	
	const char* LocalPlatformUserId;
	
	EOS_EpicAccountId AccountId;
	
	EOS_ProductUserId ProductUserId;
	
	EOS_ELoginStatus PreviousLoginStatus;
	
	EOS_ELoginStatus CurrentLoginStatus;
));

EOS_DECLARE_CALLBACK(EOS_IntegratedPlatform_OnUserLoginStatusChangedCallback, const EOS_IntegratedPlatform_UserLoginStatusChangedCallbackInfo* Data);

#define EOS_INTEGRATEDPLATFORM_SETUSERPRELOGOUTCALLBACK_API_LATEST 1

EOS_STRUCT(EOS_IntegratedPlatform_SetUserPreLogoutCallbackOptions, (
	
	int32_t ApiVersion;
));

EOS_ENUM(EOS_EIntegratedPlatformPreLogoutAction,
	
	EOS_IPLA_ProcessLogoutImmediately = 0,
	
	EOS_IPLA_DeferLogout = 1
);

EOS_STRUCT(EOS_IntegratedPlatform_UserPreLogoutCallbackInfo, (
	
	void* ClientData;
	
	EOS_IntegratedPlatformType PlatformType;
	
	const char* LocalPlatformUserId;
	
	EOS_EpicAccountId AccountId;
	
	EOS_ProductUserId ProductUserId;
));

EOS_DECLARE_CALLBACK_RETVALUE(EOS_EIntegratedPlatformPreLogoutAction, EOS_IntegratedPlatform_OnUserPreLogoutCallback, const EOS_IntegratedPlatform_UserPreLogoutCallbackInfo* Data);

#define EOS_INTEGRATEDPLATFORM_CLEARUSERPRELOGOUTCALLBACK_API_LATEST 1

EOS_STRUCT(EOS_IntegratedPlatform_ClearUserPreLogoutCallbackOptions, (
	
	int32_t ApiVersion;
));

#define EOS_INTEGRATEDPLATFORM_FINALIZEDEFERREDUSERLOGOUT_API_LATEST 1

EOS_STRUCT(EOS_IntegratedPlatform_FinalizeDeferredUserLogoutOptions, (
	
	int32_t ApiVersion;

	EOS_IntegratedPlatformType PlatformType;

	const char* LocalPlatformUserId;

	EOS_ELoginStatus ExpectedLoginStatus;
));

#pragma pack(pop)
