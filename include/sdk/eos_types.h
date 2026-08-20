
#pragma once

#include "eos_common.h"
#include "eos_integratedplatform_types.h"

#pragma pack(push, 8)

EOS_EXTERN_C typedef struct EOS_PlatformHandle* EOS_HPlatform;

#define EOS_PLATFORM_CLIENTCREDENTIALS_CLIENTID_MAX_LENGTH 64

#define EOS_PLATFORM_CLIENTCREDENTIALS_CLIENTSECRET_MAX_LENGTH 64

EOS_STRUCT(EOS_Platform_ClientCredentials, (
	
	const char* ClientId;
	
	const char* ClientSecret;
));

EOS_ENUM(EOS_ERTCBackgroundMode,
	
	EOS_RTCBM_LeaveRooms = 0,
	
	EOS_RTCBM_KeepRoomsAlive = 1
);

#define EOS_PLATFORM_RTCOPTIONS_API_LATEST 3

EOS_STRUCT(EOS_Platform_RTCOptions, (
	
	int32_t ApiVersion;
	
	void* PlatformSpecificOptions;
	
	EOS_ERTCBackgroundMode BackgroundMode;
	
	void* Reserved;
));

#define EOS_COUNTRYCODE_MAX_LENGTH 4

#define EOS_COUNTRYCODE_MAX_BUFFER_LEN (EOS_COUNTRYCODE_MAX_LENGTH + 1)

#define EOS_LOCALECODE_MAX_LENGTH 9

#define EOS_LOCALECODE_MAX_BUFFER_LEN (EOS_LOCALECODE_MAX_LENGTH + 1)

#define EOS_PLATFORM_OPTIONS_API_LATEST 15

#define EOS_PF_LOADING_IN_EDITOR				0x00001

#define EOS_PF_DISABLE_OVERLAY					0x00002

#define EOS_PF_DISABLE_SOCIAL_OVERLAY			0x00004

#define EOS_PF_RESERVED1						0x00008

#define EOS_PF_WINDOWS_ENABLE_OVERLAY_D3D9		0x00010

#define EOS_PF_WINDOWS_ENABLE_OVERLAY_D3D10		0x00020

#define EOS_PF_WINDOWS_ENABLE_OVERLAY_OPENGL	0x00040

#define EOS_PF_CONSOLE_ENABLE_OVERLAY_AUTOMATIC_UNLOADING 	0x00080

#define EOS_PF_ENABLE_OVERLAY_DEBUG_LOGGING		0x00100

#define EOS_PLATFORM_OPTIONS_PRODUCTID_MAX_LENGTH 64

#define EOS_PLATFORM_OPTIONS_SANDBOXID_MAX_LENGTH 64

#define EOS_PLATFORM_OPTIONS_ENCRYPTIONKEY_LENGTH 64

#define EOS_PLATFORM_OPTIONS_DEPLOYMENTID_MAX_LENGTH 64

EOS_STRUCT(EOS_Platform_Options, (
	
	int32_t ApiVersion;
	
	void* Reserved;
	
	const char* ProductId;
	
	const char* SandboxId;
	
	EOS_Platform_ClientCredentials ClientCredentials;
	
	EOS_Bool bIsServer;
	
	const char* EncryptionKey;
	
	const char* OverrideCountryCode;
	
	const char* OverrideLocaleCode;
	
	const char* DeploymentId;
	
	uint64_t Flags;
	
	const char* CacheDirectory;
	
	uint32_t TickBudgetInMilliseconds;
	
	const EOS_Platform_RTCOptions* RTCOptions;
	
	EOS_HIntegratedPlatformOptionsContainer IntegratedPlatformOptionsContainerHandle;
	
	const void* SystemSpecificOptions;
	
	double* TaskNetworkTimeoutSeconds;
));

EOS_ENUM(EOS_EApplicationStatus,
	
	EOS_AS_BackgroundConstrained = 0,
	
	EOS_AS_BackgroundUnconstrained = 1,
	
	EOS_AS_BackgroundSuspended = 2,
	
	EOS_AS_Foreground = 3
);

EOS_DECLARE_FUNC(const char*) EOS_EApplicationStatus_ToString(EOS_EApplicationStatus ApplicationStatus);

EOS_ENUM(EOS_ENetworkStatus,
	
	EOS_NS_Disabled = 0,
	
	EOS_NS_Offline = 1,
	
	EOS_NS_Online = 2
);

EOS_DECLARE_FUNC(const char*) EOS_ENetworkStatus_ToString(EOS_ENetworkStatus NetworkStatus);

EOS_ENUM(EOS_EDesktopCrossplayStatus,
	
	EOS_DCS_OK = 0,
	
	EOS_DCS_ApplicationNotBootstrapped = 1,
	
	EOS_DCS_ServiceNotInstalled = 2,
	
	EOS_DCS_ServiceStartFailed = 3,
	
	EOS_DCS_ServiceNotRunning = 4,
	
	EOS_DCS_OverlayDisabled = 5,
	
	EOS_DCS_OverlayNotInstalled = 6,
	
	EOS_DCS_OverlayTrustCheckFailed = 7,
	
	EOS_DCS_OverlayLoadFailed = 8
);

#define EOS_PLATFORM_GETDESKTOPCROSSPLAYSTATUS_API_LATEST 1

EOS_STRUCT(EOS_Platform_GetDesktopCrossplayStatusOptions, (
	
	int32_t ApiVersion;
));

EOS_STRUCT(EOS_Platform_DesktopCrossplayStatusInfo, (
	
	EOS_EDesktopCrossplayStatus Status;
	
	int32_t ServiceInitResult;
));

#define EOS_Platform_GetDesktopCrossplayStatusInfo EOS_Platform_DesktopCrossplayStatusInfo

#define EOS_PLATFORM_CHECKFORLAUNCHERANDRESTART_ENV_VAR "EOS_LAUNCHED_BY_EPIC"

#pragma pack(pop)
