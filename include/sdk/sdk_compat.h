
#pragma once

#define EOS_AUTH_LOGIN_API_001                      EOS_AUTH_LOGIN_API_LATEST
#define EOS_AUTH_LOGIN_API_002                      EOS_AUTH_LOGIN_API_LATEST
#define EOS_AUTH_CREDENTIALS_API_001                EOS_AUTH_CREDENTIALS_API_LATEST
#define EOS_AUTH_CREDENTIALS_API_002                EOS_AUTH_CREDENTIALS_API_LATEST
#define EOS_AUTH_CREDENTIALS_API_003                EOS_AUTH_CREDENTIALS_API_LATEST
#define EOS_AUTH_COPYUSERAUTHTOKEN_API_001          EOS_AUTH_COPYUSERAUTHTOKEN_API_LATEST
#define EOS_AUTH_ADDNOTIFYLOGINSTATUSCHANGED_API_001 EOS_AUTH_ADDNOTIFYLOGINSTATUSCHANGED_API_LATEST

typedef EOS_Auth_Credentials EOS_Auth_Credentials001;
typedef EOS_Auth_Credentials EOS_Auth_Credentials002;
typedef EOS_Auth_Credentials EOS_Auth_Credentials003;

#ifndef EOS_AccountId
#define EOS_AccountId EOS_EpicAccountId
#endif

#define EOS_Auth_OnQueryIdTokenCallbackInfo   EOS_Auth_QueryIdTokenCallbackInfo
#define EOS_Auth_OnVerifyIdTokenCallbackInfo  EOS_Auth_VerifyIdTokenCallbackInfo

#define EOS_PLATFORM_OPTIONS_API_001 1
#define EOS_PLATFORM_OPTIONS_API_002 2
#define EOS_PLATFORM_OPTIONS_API_003 3
#define EOS_PLATFORM_OPTIONS_API_004 4

struct EOS_Platform_Options001 {
    int32_t                      ApiVersion;
    void*                        Reserved;
    const char*                  ProductId;
    const char*                  SandboxId;
    EOS_Platform_ClientCredentials ClientCredentials;
    EOS_Bool                     bIsServer;
};

struct EOS_Platform_Options002 : EOS_Platform_Options001 {
    const EOS_Platform_RTCOptions* RTCOptions;
};

struct EOS_Platform_Options003 : EOS_Platform_Options002 {
    EOS_Bool bEnableOverlay;
    EOS_Bool bEnableSocialOverlay;
};

struct EOS_Platform_Options004 : EOS_Platform_Options003 {};

#ifndef EOS_PLATFORM_OPTIONS_API_005
#define EOS_PLATFORM_OPTIONS_API_005 5
#endif
#ifndef EOS_PLATFORM_OPTIONS_API_006
#define EOS_PLATFORM_OPTIONS_API_006 6
#endif
#ifndef EOS_PLATFORM_OPTIONS_API_007
#define EOS_PLATFORM_OPTIONS_API_007 7
#endif
#ifndef EOS_PLATFORM_OPTIONS_API_008
#define EOS_PLATFORM_OPTIONS_API_008 8
#endif
#ifndef EOS_PLATFORM_OPTIONS_API_009
#define EOS_PLATFORM_OPTIONS_API_009 9
#endif
#ifndef EOS_PLATFORM_OPTIONS_API_010
#define EOS_PLATFORM_OPTIONS_API_010 10
#endif
#ifndef EOS_PLATFORM_OPTIONS_API_011
#define EOS_PLATFORM_OPTIONS_API_011 11
#endif
#ifndef EOS_PLATFORM_OPTIONS_API_012
#define EOS_PLATFORM_OPTIONS_API_012 12
#endif
#ifndef EOS_PLATFORM_OPTIONS_API_013
#define EOS_PLATFORM_OPTIONS_API_013 13
#endif
#ifndef EOS_PLATFORM_OPTIONS_API_014
#define EOS_PLATFORM_OPTIONS_API_014 14
#endif
#ifndef EOS_PLATFORM_OPTIONS_API_015
#define EOS_PLATFORM_OPTIONS_API_015 15
#endif

typedef EOS_Platform_Options EOS_Platform_Options005;
typedef EOS_Platform_Options EOS_Platform_Options006;
typedef EOS_Platform_Options EOS_Platform_Options007;

#ifndef EOS_ACCVT_None
#define EOS_ACCVT_None EOS_ACCVT_Invalid
#endif

#define EOS_CONNECT_LOGIN_API_001  EOS_CONNECT_LOGIN_API_LATEST
#define EOS_CONNECT_LOGIN_API_002  EOS_CONNECT_LOGIN_API_LATEST

#ifndef EOS_HEcom_Transaction
struct EOS_Ecom_TransactionPage;
typedef EOS_Ecom_TransactionPage* EOS_HEcom_Transaction;
#endif

#ifndef EOS_LOBBY_MAX_SEARCH_RESULTS
#define EOS_LOBBY_MAX_SEARCH_RESULTS 200
#endif

#ifndef EOS_SESSIONS_MAX_SEARCH_RESULTS
#define EOS_SESSIONS_MAX_SEARCH_RESULTS 200
#endif

#ifndef EOS_P2P_MAX_PACKET_SIZE
#define EOS_P2P_MAX_PACKET_SIZE 1170
#endif

#ifndef EOS_RTC_JOINROOM_API_001
#define EOS_RTC_JOINROOM_API_001 EOS_RTC_JOINROOM_API_LATEST
#endif
#ifndef EOS_RTC_LEAVEROOM_API_001
#define EOS_RTC_LEAVEROOM_API_001 EOS_RTC_LEAVEROOM_API_LATEST
#endif

#ifndef EOS_KWS_PERMISSION_GRANTED
#define EOS_KWS_PERMISSION_GRANTED EOS_EKWSPermissionStatus::EOS_KPS_Granted
#endif

#ifndef EOS_UserInfo_ExternalLinkage

#endif

#define EOS_FRIENDS_QUERYFRIENDSLIST_API_001 EOS_FRIENDS_QUERYFRIENDSLIST_API_LATEST

#define EOS_PRESENCE_QUERYPRESENCE_API_001 EOS_PRESENCE_QUERYPRESENCE_API_LATEST

#ifndef EOS_SANCTIONS_QUERYACTIVEPLAYERSANCTIONS_API_LATEST
#define EOS_SANCTIONS_QUERYACTIVEPLAYERSANCTIONS_API_LATEST 1
#endif

#ifndef EOS_RESULT_SUCCESS
#define EOS_RESULT_SUCCESS EOS_EResult::EOS_Success
#endif

#ifndef EOS_UI_ShowFriendsCommand
#define EOS_UI_ShowFriendsCommand EOS_UI_EInputStateButtonFlags::EOS_UISBF_None
#endif

typedef EOS_InitializeOptions EOS_InitializeOptions001;

#ifndef EOS_DECLARE_FUNC
#if defined(__WINDOWS__)
    #define EOS_DECLARE_FUNC(ret) extern "C" __declspec(dllexport) ret EOS_CALL
#else
    #define EOS_DECLARE_FUNC(ret) extern "C" __attribute__((visibility("default"))) ret
#endif
#endif
