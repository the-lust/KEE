#include "eos_client_api.h"
#include "eossdk_platform.h"
#include "eossdk_auth.h"
#include "settings.h"
#include "overlay/OverlayManager.h"
#include "../../third_party/mini_detour/mini_detour.h"

#include "sdk/eos_auth.h"

EOSSDK_Client::EOSSDK_Client():
    m_sdk_initialized(false)
{}

EOSSDK_Client::~EOSSDK_Client()
{
    for (auto& id : m_epicuserids)
        delete id.second;

    for (auto& id : m_productuserids)
        delete id.second;
}

EOSSDK_Client& EOSSDK_Client::Inst()
{
    static EOSSDK_Client inst;
    return inst;
}

EOS_EpicAccountId EOSSDK_Client::get_epicuserid(string const& userid)
{
    EOS_EpicAccountId res;
    if (userid.empty())
        return GetInvalidEpicUserId();

    auto it = m_epicuserids.find(userid);
    if (it == m_epicuserids.end())
    {
        EOS_EpicAccountId& accountid = m_epicuserids[userid];
        accountid = new EOS_EpicAccountIdDetails;
        accountid->from_string(userid);
        res = accountid;
    }
    else
        res = it->second;

    return res;
}

EOS_ProductUserId EOSSDK_Client::get_productuserid(string const& userid)
{
    EOS_ProductUserId res;
    if (userid.empty())
        return GetInvalidProductUserId();

    auto it = m_productuserids.find(userid);
    if (it == m_productuserids.end())
    {
        EOS_ProductUserId& accountid = m_productuserids[userid];
        accountid = new EOS_ProductUserIdDetails;
        accountid->from_string(userid);
        res = accountid;
    }
    else
        res = it->second;

    return res;
}

extern "C" EOS_EResult EOS_CALL EOS_Auth_CopyUserAuthTokenNew(EOS_HAuth Handle, const void* Options, EOS_EpicAccountId LocalUserId, EOS_Auth_Token ** OutUserAuthToken);
extern "C" EOS_EResult EOS_CALL EOS_Auth_CopyUserAuthTokenOld(EOS_HAuth Handle, EOS_EpicAccountId LocalUserId, EOS_Auth_Token ** OutUserAuthToken);
extern "C" EOS_NotificationId EOS_CALL EOS_Auth_AddNotifyLoginStatusChangedNew(EOS_HAuth Handle, const void* Options, void* ClientData, const EOS_Auth_OnLoginStatusChangedCallback Notification);
extern "C" EOS_NotificationId EOS_CALL EOS_Auth_AddNotifyLoginStatusChangedOld(EOS_HAuth Handle, void* ClientData, const EOS_Auth_OnLoginStatusChangedCallback Notification);

static bool set_eos_compat(int32_t compat_version)
{

#if defined(__WINDOWS_64__) || defined(__LINUX_64__) || defined(__APPLE_64__)
    bool failed = false;

    void* pAuthCopyTokenHook = (void*)&EOS_Auth_CopyUserAuthTokenNew;
    void* pAuthNotifyHook = (void*)&EOS_Auth_AddNotifyLoginStatusChangedNew;

    if (compat_version == 1)
    {
        APP_LOG(Log::LogLevel::INFO, "Selecting legacy (v1) entry points for EOS Auth");
        pAuthCopyTokenHook = (void*)&EOS_Auth_CopyUserAuthTokenOld;
        pAuthNotifyHook = (void*)&EOS_Auth_AddNotifyLoginStatusChangedOld;
    }

    APP_LOG(Log::LogLevel::DEBUG, "Swapping entry points using mini_detour...");

    if (mini_detour::replace_func((void*)&EOS_Auth_CopyUserAuthToken, pAuthCopyTokenHook)) failed = true;
    if (mini_detour::replace_func((void*)&EOS_Auth_AddNotifyLoginStatusChanged, pAuthNotifyHook)) failed = true;

    return failed;
#else
    
    return false; 
#endif
}

EOS_DECLARE_FUNC(EOS_EResult) EOS_Initialize(const EOS_InitializeOptions* Options) {
    GLOBAL_LOCK();
    Settings::Inst();
    TRACE_FUNC();

    auto &inst = EOSSDK_Client::Inst();
    
    // check if we already initilized everything
    if (inst.m_sdk_initialized) return EOS_EResult::EOS_AlreadyConfigured;
    if (!Options) return EOS_EResult::EOS_InvalidParameters;

    // networking might be disabled in config
    if (Settings::Inst().disable_online_networking) disable_online_networking();
    
    // setup hooks for different sdk versions
    if (set_eos_compat(Options->ApiVersion)) {
        APP_LOG(Log::LogLevel::FATAL, "Failed to install compatibility hooks.");
        abort();
    }

    // grab the mem funcs from the game
    auto p = reinterpret_cast<const EOS_InitializeOptions001*>(Options);
    inst.m_allocate_memory_func = p->AllocateMemoryFunction;
    inst.m_reallocate_memory_func = p->ReallocateMemoryFunction;
    inst.m_release_memory_func = p->ReleaseMemoryFunction;
    inst.api_version = p->ApiVersion;

    if (p->ProductName && *p->ProductName) {
        inst.m_product_name = p->ProductName;
        // save the game name if we dont have one yet
        if (Settings::Inst().gamename.empty() || Settings::Inst().gamename == "DefaultGameName") {
            Settings::Inst().gamename = inst.m_product_name;
            Settings::Inst().save_settings();
        }
    } else inst.m_product_name = Settings::Inst().gamename;

    if (p->ProductVersion) inst.m_product_version = p->ProductVersion;

    inst.m_sdk_initialized = true;
    
    // start the overlay system
    GetOverlayManager().Init();

    return EOS_EResult::EOS_Success;
}

EOS_DECLARE_FUNC(EOS_EResult) EOS_Shutdown()
{
    TRACE_FUNC();
    lock _(GLOBAL_MUTEX);

    if (!EOSSDK_Client::Inst().m_sdk_initialized)
        return EOS_EResult::EOS_NotConfigured;

    EOSSDK_Client::Inst().m_sdk_initialized = false;
    return EOS_EResult::EOS_Success;
}

EOS_DECLARE_FUNC(const char*) EOS_EResult_ToString(EOS_EResult Result)
{
    TRACE_FUNC();

    switch (Result)
    {
#define CASE_RESULT(RES) case EOS_EResult::RES: return #RES
        CASE_RESULT(EOS_Success);
        CASE_RESULT(EOS_NoConnection);
        CASE_RESULT(EOS_InvalidCredentials);
        CASE_RESULT(EOS_InvalidUser);
        CASE_RESULT(EOS_InvalidAuth);
        CASE_RESULT(EOS_AccessDenied);
        CASE_RESULT(EOS_MissingPermissions);
        CASE_RESULT(EOS_Token_Not_Account);
        CASE_RESULT(EOS_TooManyRequests);
        CASE_RESULT(EOS_AlreadyPending);
        CASE_RESULT(EOS_InvalidParameters);
        CASE_RESULT(EOS_InvalidRequest);
        CASE_RESULT(EOS_UnrecognizedResponse);
        CASE_RESULT(EOS_IncompatibleVersion);
        CASE_RESULT(EOS_NotConfigured);
        CASE_RESULT(EOS_AlreadyConfigured);
        CASE_RESULT(EOS_NotImplemented);
        CASE_RESULT(EOS_Canceled);
        CASE_RESULT(EOS_NotFound);
        CASE_RESULT(EOS_OperationWillRetry);
        CASE_RESULT(EOS_NoChange);
        CASE_RESULT(EOS_VersionMismatch);
        CASE_RESULT(EOS_LimitExceeded);
        CASE_RESULT(EOS_Disabled);
        CASE_RESULT(EOS_DuplicateNotAllowed);
        CASE_RESULT(EOS_MissingParameters_DEPRECATED);

        CASE_RESULT(EOS_InvalidSandboxId);
        CASE_RESULT(EOS_TimedOut);
        CASE_RESULT(EOS_PartialResult);
        CASE_RESULT(EOS_Missing_Role);
        CASE_RESULT(EOS_Missing_Feature);
        CASE_RESULT(EOS_Invalid_Sandbox);
        CASE_RESULT(EOS_Invalid_Deployment);
        CASE_RESULT(EOS_Invalid_Product);
        CASE_RESULT(EOS_Invalid_ProductUserID);
        CASE_RESULT(EOS_ServiceFailure);
        CASE_RESULT(EOS_CacheDirectoryMissing);
        CASE_RESULT(EOS_CacheDirectoryInvalid);
        CASE_RESULT(EOS_InvalidState);

        CASE_RESULT(EOS_Auth_AccountLocked);
        CASE_RESULT(EOS_Auth_AccountLockedForUpdate);
        CASE_RESULT(EOS_Auth_InvalidRefreshToken);
        CASE_RESULT(EOS_Auth_InvalidToken);
        CASE_RESULT(EOS_Auth_AuthenticationFailure);
        CASE_RESULT(EOS_Auth_InvalidPlatformToken);
        CASE_RESULT(EOS_Auth_WrongAccount);
        CASE_RESULT(EOS_Auth_WrongClient);
        CASE_RESULT(EOS_Auth_FullAccountRequired);
        CASE_RESULT(EOS_Auth_HeadlessAccountRequired);
        CASE_RESULT(EOS_Auth_PasswordResetRequired);
        CASE_RESULT(EOS_Auth_PasswordCannotBeReused);
        CASE_RESULT(EOS_Auth_Expired);
        CASE_RESULT(EOS_Auth_ScopeConsentRequired);
        CASE_RESULT(EOS_Auth_ApplicationNotFound);
        CASE_RESULT(EOS_Auth_ScopeNotFound);
        CASE_RESULT(EOS_Auth_AccountFeatureRestricted);
        CASE_RESULT(EOS_Auth_PinGrantCode);
        CASE_RESULT(EOS_Auth_PinGrantExpired);
        CASE_RESULT(EOS_Auth_PinGrantPending);
        CASE_RESULT(EOS_Auth_ExternalAuthNotLinked);
        CASE_RESULT(EOS_Auth_ExternalAuthRevoked);
        CASE_RESULT(EOS_Auth_ExternalAuthInvalid);
        CASE_RESULT(EOS_Auth_ExternalAuthRestricted);
        CASE_RESULT(EOS_Auth_ExternalAuthCannotLogin);
        CASE_RESULT(EOS_Auth_ExternalAuthExpired);
        CASE_RESULT(EOS_Auth_ExternalAuthIsLastLoginType);
        CASE_RESULT(EOS_Auth_ExchangeCodeNotFound);
        CASE_RESULT(EOS_Auth_OriginatingExchangeCodeSessionExpired);
        //CASE_RESULT(EOS_Auth_PersistentAuth_AccountNotActive); // not in this SDK
        CASE_RESULT(EOS_Auth_MFARequired);
        CASE_RESULT(EOS_Auth_ParentalControls);
        CASE_RESULT(EOS_Auth_NoRealId);

        CASE_RESULT(EOS_Friends_InviteAwaitingAcceptance);
        CASE_RESULT(EOS_Friends_NoInvitation);
        CASE_RESULT(EOS_Friends_AlreadyFriends);
        CASE_RESULT(EOS_Friends_NotFriends);
        CASE_RESULT(EOS_Friends_TargetUserTooManyInvites);
        CASE_RESULT(EOS_Friends_LocalUserTooManyInvites);
        CASE_RESULT(EOS_Friends_TargetUserFriendLimitExceeded);
        CASE_RESULT(EOS_Friends_LocalUserFriendLimitExceeded);

        CASE_RESULT(EOS_Presence_DataInvalid);
        CASE_RESULT(EOS_Presence_DataLengthInvalid);
        CASE_RESULT(EOS_Presence_DataKeyInvalid);
        CASE_RESULT(EOS_Presence_DataKeyLengthInvalid);
        CASE_RESULT(EOS_Presence_DataValueInvalid);
        CASE_RESULT(EOS_Presence_DataValueLengthInvalid);
        CASE_RESULT(EOS_Presence_RichTextInvalid);
        CASE_RESULT(EOS_Presence_RichTextLengthInvalid);
        CASE_RESULT(EOS_Presence_StatusInvalid);

        CASE_RESULT(EOS_Ecom_EntitlementStale);
        CASE_RESULT(EOS_Ecom_CatalogOfferStale);
        CASE_RESULT(EOS_Ecom_CatalogItemStale);
        CASE_RESULT(EOS_Ecom_CatalogOfferPriceInvalid);
        CASE_RESULT(EOS_Ecom_CheckoutLoadError);

        CASE_RESULT(EOS_Sessions_SessionInProgress);
        CASE_RESULT(EOS_Sessions_TooManyPlayers);
        CASE_RESULT(EOS_Sessions_NoPermission);
        CASE_RESULT(EOS_Sessions_SessionAlreadyExists);
        CASE_RESULT(EOS_Sessions_InvalidLock);
        CASE_RESULT(EOS_Sessions_InvalidSession);
        CASE_RESULT(EOS_Sessions_SandboxNotAllowed);
        CASE_RESULT(EOS_Sessions_InviteFailed);
        CASE_RESULT(EOS_Sessions_InviteNotFound);
        CASE_RESULT(EOS_Sessions_UpsertNotAllowed);
        CASE_RESULT(EOS_Sessions_AggregationFailed);
        CASE_RESULT(EOS_Sessions_HostAtCapacity);
        CASE_RESULT(EOS_Sessions_SandboxAtCapacity);
        CASE_RESULT(EOS_Sessions_SessionNotAnonymous);
        CASE_RESULT(EOS_Sessions_OutOfSync);
        CASE_RESULT(EOS_Sessions_TooManyInvites);
        CASE_RESULT(EOS_Sessions_PresenceSessionExists);
        CASE_RESULT(EOS_Sessions_DeploymentAtCapacity);
        CASE_RESULT(EOS_Sessions_NotAllowed);

        CASE_RESULT(EOS_PlayerDataStorage_FilenameInvalid);
        CASE_RESULT(EOS_PlayerDataStorage_FilenameLengthInvalid);
        CASE_RESULT(EOS_PlayerDataStorage_FilenameInvalidChars);
        CASE_RESULT(EOS_PlayerDataStorage_FileSizeTooLarge);
        CASE_RESULT(EOS_PlayerDataStorage_FileSizeInvalid);
        CASE_RESULT(EOS_PlayerDataStorage_FileHandleInvalid);
        CASE_RESULT(EOS_PlayerDataStorage_DataInvalid);
        CASE_RESULT(EOS_PlayerDataStorage_DataLengthInvalid);
        CASE_RESULT(EOS_PlayerDataStorage_StartIndexInvalid);
        CASE_RESULT(EOS_PlayerDataStorage_RequestInProgress);
        CASE_RESULT(EOS_PlayerDataStorage_UserThrottled);
        CASE_RESULT(EOS_PlayerDataStorage_EncryptionKeyNotSet);
        CASE_RESULT(EOS_PlayerDataStorage_UserErrorFromDataCallback);
        CASE_RESULT(EOS_PlayerDataStorage_FileHeaderHasNewerVersion);
        CASE_RESULT(EOS_PlayerDataStorage_FileCorrupted);

        CASE_RESULT(EOS_Connect_ExternalTokenValidationFailed);
        CASE_RESULT(EOS_Connect_UserAlreadyExists);
        CASE_RESULT(EOS_Connect_AuthExpired);
        CASE_RESULT(EOS_Connect_InvalidToken);
        CASE_RESULT(EOS_Connect_UnsupportedTokenType);
        CASE_RESULT(EOS_Connect_LinkAccountFailed);
        CASE_RESULT(EOS_Connect_ExternalServiceUnavailable);
        CASE_RESULT(EOS_Connect_ExternalServiceConfigurationFailure);
        CASE_RESULT(EOS_Connect_LinkAccountFailedMissingNintendoIdAccount_DEPRECATED);

        CASE_RESULT(EOS_UI_SocialOverlayLoadError);

        CASE_RESULT(EOS_Lobby_NotOwner);
        CASE_RESULT(EOS_Lobby_InvalidLock);
        CASE_RESULT(EOS_Lobby_LobbyAlreadyExists);
        CASE_RESULT(EOS_Lobby_SessionInProgress);
        CASE_RESULT(EOS_Lobby_TooManyPlayers);
        CASE_RESULT(EOS_Lobby_NoPermission);
        CASE_RESULT(EOS_Lobby_InvalidSession);
        CASE_RESULT(EOS_Lobby_SandboxNotAllowed);
        CASE_RESULT(EOS_Lobby_InviteFailed);
        CASE_RESULT(EOS_Lobby_InviteNotFound);
        CASE_RESULT(EOS_Lobby_UpsertNotAllowed);
        CASE_RESULT(EOS_Lobby_AggregationFailed);
        CASE_RESULT(EOS_Lobby_HostAtCapacity);
        CASE_RESULT(EOS_Lobby_SandboxAtCapacity);
        CASE_RESULT(EOS_Lobby_TooManyInvites);
        CASE_RESULT(EOS_Lobby_DeploymentAtCapacity);
        CASE_RESULT(EOS_Lobby_NotAllowed);
        CASE_RESULT(EOS_Lobby_MemberUpdateOnly);
        CASE_RESULT(EOS_Lobby_PresenceLobbyExists);

        default: return "EOS_UnexpectedError";
#undef CASE_RESULT
    }
}

EOS_DECLARE_FUNC(EOS_Bool) EOS_EResult_IsOperationComplete(EOS_EResult Result)
{
    TRACE_FUNC();

    switch (Result)
    {
        case EOS_EResult::EOS_OperationWillRetry:
        case EOS_EResult::EOS_Auth_PinGrantCode:
        case EOS_EResult::EOS_Auth_MFARequired:
            return EOS_FALSE;
    }

    return EOS_TRUE;
}

EOS_DECLARE_FUNC(EOS_EResult) EOS_ByteArray_ToString(const uint8_t* ByteArray, const uint32_t Length, char* OutBuffer, uint32_t* InOutBufferLength)
{
    TRACE_FUNC();
    APP_LOG(Log::LogLevel::INFO, "TODO");

    return EOS_EResult::EOS_Success;
}

EOS_DECLARE_FUNC(EOS_Bool) EOS_AccountId_IsValid(EOS_AccountId AccountId)
{
    return EOS_EpicAccountId_IsValid(AccountId);
}

EOS_DECLARE_FUNC(EOS_Bool) EOS_EpicAccountId_IsValid(EOS_EpicAccountId AccountId)
{
    
    if (AccountId == nullptr)
        return EOS_FALSE;

    auto& user_ids = EOSSDK_Client::Inst().m_epicuserids;
    auto it = std::find_if(user_ids.begin(), user_ids.end(), [AccountId](std::pair<string const, EOS_EpicAccountId>& user_id)
    {
        return user_id.second == AccountId;
    });
    if (it == user_ids.end())
    {
        APP_LOG(Log::LogLevel::WARN, "Epic User Id (%p) not found in the cache, wrong parameter returned in a function ?", AccountId);
        return EOS_FALSE;
    }

    return AccountId->IsValid();
}

EOS_DECLARE_FUNC(EOS_EResult) EOS_AccountId_ToString(EOS_AccountId AccountId, char* OutBuffer, int32_t* InOutBufferLength)
{
    return EOS_EpicAccountId_ToString(AccountId, OutBuffer, InOutBufferLength);
}

EOS_DECLARE_FUNC(EOS_EResult) EOS_EpicAccountId_ToString(EOS_EpicAccountId AccountId, char* OutBuffer, int32_t* InOutBufferLength)
{
    
    if (AccountId == nullptr || !AccountId->IsValid())
        return EOS_EResult::EOS_InvalidUser;

    auto& user_ids = EOSSDK_Client::Inst().m_epicuserids;
    auto it = std::find_if(user_ids.begin(), user_ids.end(), [AccountId](std::pair<string const, EOS_EpicAccountId>& user_id)
    {
        return user_id.second == AccountId;
    });
    if (it == user_ids.end())
    {
        APP_LOG(Log::LogLevel::WARN, "Epic User Id (%p) not found in the cache, wrong parameter returned in a function ?", AccountId);
        return EOS_EResult::EOS_InvalidUser;
    }

    return AccountId->ToString(OutBuffer, InOutBufferLength);
}

EOS_DECLARE_FUNC(EOS_AccountId) EOS_AccountId_FromString(const char* AccountIdString)
{
    return EOS_EpicAccountId_FromString(AccountIdString);
}

EOS_DECLARE_FUNC(EOS_EpicAccountId) EOS_EpicAccountId_FromString(const char* AccountIdString)
{
    
    if (AccountIdString == nullptr)
        return EOSSDK_Client::Inst().get_epicuserid(sdk::NULL_USER_ID);

    return EOSSDK_Client::Inst().get_epicuserid(AccountIdString);
}

EOS_DECLARE_FUNC(EOS_Bool) EOS_ProductUserId_IsValid(EOS_ProductUserId AccountId)
{
    
    if (AccountId == nullptr)
        return EOS_FALSE;

    auto& product_ids = EOSSDK_Client::Inst().m_productuserids;
    auto it = std::find_if(product_ids.begin(), product_ids.end(), [AccountId]( std::pair<string const, EOS_ProductUserId>& product_id)
    {
        return product_id.second == AccountId;
    });
    if (it == product_ids.end())
    {
        APP_LOG(Log::LogLevel::WARN, "Product User Id (%p) not found in the cache, wrong parameter returned in a function ?", AccountId);
        return EOS_FALSE;
    }

    return AccountId->IsValid();
}

EOS_DECLARE_FUNC(EOS_EResult) EOS_ProductUserId_ToString(EOS_ProductUserId AccountId, char* OutBuffer, int32_t* InOutBufferLength)
{

    if (AccountId == nullptr || !AccountId->IsValid())
        return EOS_EResult::EOS_InvalidUser;

    auto& product_ids = EOSSDK_Client::Inst().m_productuserids;
    auto it = std::find_if(product_ids.begin(), product_ids.end(), [AccountId]( std::pair<string const, EOS_ProductUserId>& product_id)
    {
        return product_id.second == AccountId;
    });
    if (it == product_ids.end())
    {
        APP_LOG(Log::LogLevel::WARN, "Product User Id (%p) not found in the cache, wrong parameter returned in a function ?", AccountId);
        return EOS_EResult::EOS_InvalidUser;
    }

    return AccountId->ToString(OutBuffer, InOutBufferLength);
}

EOS_DECLARE_FUNC(EOS_ProductUserId) EOS_ProductUserId_FromString(const char* AccountIdString)
{
    
    if (AccountIdString == nullptr)
        return EOSSDK_Client::Inst().get_productuserid(sdk::NULL_USER_ID);

    return EOSSDK_Client::Inst().get_productuserid(AccountIdString);
}

EOS_DECLARE_FUNC(EOS_EResult) EOS_ContinuanceToken_ToString(EOS_ContinuanceToken ContinuanceToken, char* OutBuffer, int32_t* InOutBufferLength)
{
    TRACE_FUNC();
    if (ContinuanceToken == nullptr)
        return EOS_EResult::EOS_InvalidUser;

    if (OutBuffer == nullptr || InOutBufferLength == nullptr)
        return EOS_EResult::EOS_InvalidParameters;

    return EOS_EResult::EOS_InvalidUser;
}

EOS_DECLARE_FUNC(EOS_EResult) EOS_Logging_SetCallback(EOS_LogMessageFunc Callback)
{
    TRACE_FUNC();

    return EOS_EResult::EOS_Success;
}

EOS_DECLARE_FUNC(EOS_EResult) EOS_Logging_SetLogLevel(EOS_ELogCategory LogCategory, EOS_ELogLevel LogLevel)
{
    TRACE_FUNC();

    return EOS_EResult::EOS_Success;
}

EOS_DECLARE_FUNC(const char*) EOS_GetVersion(void)
{
    std::map<string, const char*> versions{
        { "1.0.0", "1.0.0-5464091"  },
        { "1.1.0", "1.1.0-6537116"  },
        { "1.2.0", "1.2.0-9765216"  },
        { "1.3.0", "1.3.0-11034880" },
        { "1.3.1", "1.3.1-11123224" },
        { "1.5.0", "1.5.0-12496671" },
        { "1.6.0", "1.6.0-13289764" },
        { "1.6.1", "1.6.1-13568552" },
        { "1.6.2", "1.6.2-13619780" },
        { "1.7.0", "1.7.0-13812567" },
        { "1.7.1", "1.7.1-13992660" },
        { "1.8.0", "1.8.0-14316386" },
    };
    
    return versions[EOS_VERSION_STRING];
}