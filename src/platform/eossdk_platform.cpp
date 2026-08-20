#include "eossdk_platform.h"
#include "settings.h"

#include "eossdk_anticheatclient.h"
#include "eossdk_anticheatserver.h"
#include "eossdk_crypto.h"
#include "eossdk_custominvites.h"
#include "eossdk_epicconnect.h"
#include "eossdk_eulatracking.h"
#include "eossdk_gameclips.h"
#include "eossdk_integratedplatform.h"
#include "eossdk_inventory.h"
#include "eossdk_k3s.h"
#include "eossdk_kws.h"
#include "eossdk_matchmaking.h"
#include "eossdk_moderation.h"
#include "eossdk_mods.h"
#include "eossdk_parties.h"
#include "eossdk_partiesinternal.h"
#include "eossdk_progressionsnapshot.h"
#include "eossdk_reports.h"
#include "eossdk_rtc.h"
#include "eossdk_rtcadmin.h"
#include "eossdk_sanctions.h"
#include "eossdk_textchatclient.h"
#include "eossdk_textchatserver.h"
#include "eossdk_texttospeech.h"
#include "eossdk_weblinks.h"
#include "eossdk_worldinventory.h"

namespace sdk
{

EOSSDK_Platform::EOSSDK_Platform():
    m_platform_init(false),
    m_ticket_budget_in_milliseconds(0),

    m_cb_manager       (nullptr),
    m_network          (nullptr),
    m_metrics          (nullptr),
    m_auth             (nullptr),
    m_connect          (nullptr),
    m_ecom             (nullptr),
    m_ui               (nullptr),
    m_friends          (nullptr),
    m_presence         (nullptr),
    m_sessions         (nullptr),
    m_lobby            (nullptr),
    m_userinfo         (nullptr),
    m_p2p              (nullptr),
    m_playerdatastorage(nullptr),
    m_achievements     (nullptr),
    m_stats            (nullptr),
    m_leaderboards     (nullptr),
    m_rtc              (nullptr),
    m_rtcadmin         (nullptr),
    m_mods             (nullptr),
    m_anticheatclient  (nullptr),
    m_anticheatserver  (nullptr),
    m_progressionsnapshot(nullptr),
    m_reports          (nullptr),
    m_sanctions        (nullptr),
    m_kws              (nullptr),
    m_custominvites    (nullptr),
    m_integratedplatform(nullptr)
{
    m_cb_manager        = new Callback_Manager;
    m_network           = new Network;
}

EOSSDK_Platform::~EOSSDK_Platform()
{
    Release();
    delete m_network;
    delete m_cb_manager;
}

EOSSDK_Platform& EOSSDK_Platform::Inst()
{
    static EOSSDK_Platform instance;
    return instance;
}

void EOSSDK_Platform::Init(const EOS_Platform_Options* Options)
{
    lock _(GLOBAL_MUTEX);
    
    // check if we are already up and running
    if(!m_platform_init)
    {
        if (Options != nullptr)
        {
            m_api_version = Options->ApiVersion;

            // handle versioned platform options... what a mess lol
            const EOS_Platform_Options* pf = read_versioned_struct<EOS_Platform_Options, EOS_Platform_Options001>(Options, m_api_version, EOS_PLATFORM_OPTIONS_API_001);

            if (m_api_version >= EOS_PLATFORM_OPTIONS_API_001)
            {
                m_reserved = pf->Reserved;
                if (pf->ProductId != nullptr) m_product_id = pf->ProductId;

                if (pf->SandboxId != nullptr) m_sandbox_id = pf->SandboxId;
                if (pf->ClientCredentials.ClientId != nullptr) m_client_id = pf->ClientCredentials.ClientId;
                if (pf->ClientCredentials.ClientSecret != nullptr) m_client_secret = pf->ClientCredentials.ClientSecret;
                m_is_server = pf->bIsServer;
            }

            if (m_api_version >= EOS_PLATFORM_OPTIONS_API_002)
            {
                
                (void)pf->RTCOptions;
            }
            
            if (m_api_version >= EOS_PLATFORM_OPTIONS_API_005)
            {
                m_enable_overlay        = ((pf->Flags & EOS_PF_DISABLE_OVERLAY) == 0);
                m_enable_social_overlay = ((pf->Flags & EOS_PF_DISABLE_SOCIAL_OVERLAY) == 0);
            }

            if (m_api_version >= EOS_PLATFORM_OPTIONS_API_005)
            {
                if (pf->EncryptionKey != nullptr) m_encryption_key = pf->EncryptionKey;
                if (pf->OverrideCountryCode != nullptr) m_override_country_code = pf->OverrideCountryCode;
                if (pf->OverrideLocaleCode != nullptr) m_override_locale_code = pf->OverrideLocaleCode;
                if (pf->DeploymentId != nullptr) m_deployment_id = pf->DeploymentId;
                m_flags = pf->Flags;
            }

            if (m_api_version >= EOS_PLATFORM_OPTIONS_API_006)
            {
                if (pf->CacheDirectory != nullptr) m_cache_directory = pf->CacheDirectory;
            }

            if (m_api_version >= EOS_PLATFORM_OPTIONS_API_007)
            {
                m_ticket_budget_in_milliseconds = pf->TickBudgetInMilliseconds;
            }

            if (m_api_version >= EOS_PLATFORM_OPTIONS_API_008)
            {
                (void)pf->IntegratedPlatformOptionsContainerHandle;
            }

            if (m_api_version >= EOS_PLATFORM_OPTIONS_API_009)
            {
                (void)pf->SystemSpecificOptions;
            }

            if (m_api_version >= EOS_PLATFORM_OPTIONS_API_010)
            {
                (void)pf->TaskNetworkTimeoutSeconds;
            }
            
            APP_LOG(Log::LogLevel::DEBUG, "Initialized Platform with ApiVersion = %d", m_api_version);
        }

        m_enable_overlay = true;
        m_enable_social_overlay = true;
        m_platform_init = true;

#define INIT_SVC(svc, cls) if (!m_##svc) m_##svc = new cls
        INIT_SVC(auth, EOSSDK_Auth);
        INIT_SVC(friends, EOSSDK_Friends);
        INIT_SVC(presence, EOSSDK_Presence);
        INIT_SVC(connect, EOSSDK_Connect);
        INIT_SVC(metrics, EOSSDK_Metrics);
        INIT_SVC(ecom, EOSSDK_Ecom);
        INIT_SVC(ui, EOSSDK_UI);
        INIT_SVC(sessions, EOSSDK_Sessions);
        INIT_SVC(lobby, EOSSDK_Lobby);
        INIT_SVC(userinfo, EOSSDK_UserInfo);
        INIT_SVC(p2p, EOSSDK_P2P);
        INIT_SVC(playerdatastorage, EOSSDK_PlayerDataStorage);
        INIT_SVC(achievements, EOSSDK_Achievements);
        INIT_SVC(stats, EOSSDK_Stats);
        INIT_SVC(titlestorage, EOSSDK_TitleStorage);
        INIT_SVC(leaderboards, EOSSDK_Leaderboards);
#undef INIT_SVC

        
#define INIT_SVC(svc, cls) if (!m_##svc) m_##svc = new cls
        INIT_SVC(rtc, EOSSDK_RTC);
        INIT_SVC(rtcadmin, EOSSDK_RTCAdmin);
        INIT_SVC(mods, EOSSDK_Mods);
        INIT_SVC(anticheatclient, EOSSDK_AntiCheatClient);
        INIT_SVC(anticheatserver, EOSSDK_AntiCheatServer);
        INIT_SVC(progressionsnapshot, EOSSDK_ProgressionSnapshot);
        INIT_SVC(reports, EOSSDK_Reports);
        INIT_SVC(sanctions, EOSSDK_Sanctions);
        INIT_SVC(kws, EOSSDK_KWS);
        INIT_SVC(custominvites, EOSSDK_CustomInvites);
        INIT_SVC(integratedplatform, EOSSDK_IntegratedPlatform);
#undef INIT_SVC

        
        if (!m_crypto) m_crypto            = new EOSSDK_Crypto;
        if (!m_eulatracking) m_eulatracking      = new EOSSDK_Eulatracking;
        if (!m_epicconnect) m_epicconnect       = new EOSSDK_EpicConnect;
        if (!m_gameclips) m_gameclips         = new EOSSDK_Gameclips;
        if (!m_inventory) m_inventory         = new EOSSDK_Inventory;
        if (!m_k3s) m_k3s               = new EOSSDK_K3s;
        if (!m_matchmaking) m_matchmaking       = new EOSSDK_Matchmaking;
        if (!m_moderation) m_moderation        = new EOSSDK_Moderation;
        if (!m_parties) m_parties           = new EOSSDK_Parties;
        if (!m_partiesinternal) m_partiesinternal   = new EOSSDK_Partiesinternal;
        if (!m_textchatclient) m_textchatclient    = new EOSSDK_Textchatclient;
        if (!m_textchatserver) m_textchatserver    = new EOSSDK_Textchatserver;
        if (!m_texttospeech) m_texttospeech      = new EOSSDK_Texttospeech;
        if (!m_weblinks) m_weblinks          = new EOSSDK_Weblinks;
        if (!m_worldinventory) m_worldinventory    = new EOSSDK_Worldinventory;

        m_presence->setup_myself();
        m_userinfo->setup_myself();

        m_platform_init = true;
    }
}

void EOSSDK_Platform::Release()
{
    lock _(GLOBAL_MUTEX);

    if (m_platform_init)
    {
        delete m_integratedplatform; m_integratedplatform = nullptr;
        delete m_custominvites;      m_custominvites      = nullptr;

        delete m_worldinventory;  m_worldinventory  = nullptr;
        delete m_weblinks;        m_weblinks        = nullptr;
        delete m_texttospeech;    m_texttospeech    = nullptr;
        delete m_textchatserver;  m_textchatserver  = nullptr;
        delete m_textchatclient;  m_textchatclient  = nullptr;
        delete m_partiesinternal; m_partiesinternal = nullptr;
        delete m_parties;         m_parties         = nullptr;
        delete m_moderation;      m_moderation      = nullptr;
        delete m_matchmaking;     m_matchmaking     = nullptr;
        delete m_k3s;             m_k3s             = nullptr;
        delete m_inventory;       m_inventory       = nullptr;
        delete m_gameclips;       m_gameclips       = nullptr;
        delete m_epicconnect;     m_epicconnect     = nullptr;
        delete m_eulatracking;    m_eulatracking    = nullptr;
        delete m_crypto;          m_crypto          = nullptr;
        delete m_kws;             m_kws             = nullptr;
        delete m_sanctions;       m_sanctions       = nullptr;
        delete m_reports;         m_reports         = nullptr;
        delete m_progressionsnapshot; m_progressionsnapshot = nullptr;
        delete m_anticheatserver; m_anticheatserver = nullptr;
        delete m_anticheatclient; m_anticheatclient = nullptr;
        delete m_mods;            m_mods            = nullptr;
        delete m_rtcadmin;        m_rtcadmin        = nullptr;
        delete m_rtc;             m_rtc             = nullptr;

        delete m_leaderboards;       m_leaderboards       = nullptr;
        delete m_titlestorage;       m_titlestorage       = nullptr;
        delete m_stats;              m_stats              = nullptr;
        delete m_achievements;       m_achievements       = nullptr;
        delete m_playerdatastorage;  m_playerdatastorage  = nullptr;
        delete m_p2p;                m_p2p                = nullptr;
        delete m_userinfo;           m_userinfo           = nullptr;
        delete m_lobby;              m_lobby              = nullptr;
        delete m_sessions;           m_sessions           = nullptr;
        delete m_presence;           m_presence           = nullptr;
        delete m_friends;            m_friends            = nullptr;
        delete m_ui;                 m_ui                 = nullptr;
        delete m_ecom;               m_ecom               = nullptr;
        delete m_connect;            m_connect            = nullptr;
        delete m_auth;               m_auth               = nullptr;
        delete m_metrics;            m_metrics            = nullptr;

        m_platform_init = false;
    }
}

void EOSSDK_Platform::Tick()
{
    lock _(GLOBAL_MUTEX);
    GetCB_Manager().set_max_tick_budget(m_ticket_budget_in_milliseconds);
    GetCB_Manager().tick();
}

EOS_HMetrics           EOSSDK_Platform::GetMetricsInterface()
{
    lock _(GLOBAL_MUTEX);
    return reinterpret_cast<EOS_HMetrics>(m_metrics);
}

EOS_HAuth              EOSSDK_Platform::GetAuthInterface()
{
    lock _(GLOBAL_MUTEX);
    return reinterpret_cast<EOS_HAuth>(m_auth);
}

EOS_HConnect           EOSSDK_Platform::GetConnectInterface()
{
    lock _(GLOBAL_MUTEX);
    return reinterpret_cast<EOS_HConnect>(m_connect);
}

EOS_HEcom              EOSSDK_Platform::GetEcomInterface()
{
    lock _(GLOBAL_MUTEX);
    return reinterpret_cast<EOS_HEcom>(m_ecom);
}

EOS_HUI                EOSSDK_Platform::GetUIInterface()
{
    lock _(GLOBAL_MUTEX);
    return reinterpret_cast<EOS_HUI>(m_ui);
}

EOS_HFriends           EOSSDK_Platform::GetFriendsInterface()
{
    lock _(GLOBAL_MUTEX);
    return reinterpret_cast<EOS_HFriends>(m_friends);
}

EOS_HPresence          EOSSDK_Platform::GetPresenceInterface()
{
    lock _(GLOBAL_MUTEX);
    return reinterpret_cast<EOS_HPresence>(m_presence);
}

EOS_HSessions          EOSSDK_Platform::GetSessionsInterface()
{
    lock _(GLOBAL_MUTEX);
    return reinterpret_cast<EOS_HSessions>(m_sessions);
}

EOS_HLobby             EOSSDK_Platform::GetLobbyInterface()
{
    lock _(GLOBAL_MUTEX);
    return reinterpret_cast<EOS_HLobby>(m_lobby);
}

EOS_HUserInfo          EOSSDK_Platform::GetUserInfoInterface()
{
    lock _(GLOBAL_MUTEX);
    return reinterpret_cast<EOS_HUserInfo>(m_userinfo);
}

EOS_HP2P               EOSSDK_Platform::GetP2PInterface()
{
    lock _(GLOBAL_MUTEX);
    return reinterpret_cast<EOS_HP2P>(m_p2p);
}

EOS_HPlayerDataStorage EOSSDK_Platform::GetPlayerDataStorageInterface()
{
    lock _(GLOBAL_MUTEX);
    return reinterpret_cast<EOS_HPlayerDataStorage>(m_playerdatastorage);
}

EOS_HTitleStorage EOSSDK_Platform::GetTitleStorageInterface()
{
    lock _(GLOBAL_MUTEX);
    if (!m_platform_init) return nullptr;
    return reinterpret_cast<EOS_HTitleStorage>(m_titlestorage);
}

EOS_HAchievements      EOSSDK_Platform::GetAchievementsInterface()
{
    lock _(GLOBAL_MUTEX);
    return reinterpret_cast<EOS_HAchievements>(m_achievements);
}

EOS_HStats             EOSSDK_Platform::GetStatsInterface()
{
    lock _(GLOBAL_MUTEX);
    return reinterpret_cast<EOS_HStats>(m_stats);
}

EOS_HLeaderboards      EOSSDK_Platform::GetLeaderboardsInterface()
{
    lock _(GLOBAL_MUTEX);
    return reinterpret_cast<EOS_HLeaderboards>(m_leaderboards);
}

EOS_HRTC EOSSDK_Platform::GetRTCInterface() {
    lock _(GLOBAL_MUTEX);
    return reinterpret_cast<EOS_HRTC>(m_rtc);
}

EOS_HRTCAdmin EOSSDK_Platform::GetRTCAdminInterface() {
    lock _(GLOBAL_MUTEX);
    return reinterpret_cast<EOS_HRTCAdmin>(m_rtcadmin);
}

EOS_HMods EOSSDK_Platform::GetModsInterface() {
    lock _(GLOBAL_MUTEX);
    return reinterpret_cast<EOS_HMods>(m_mods);
}

EOS_HAntiCheatClient EOSSDK_Platform::GetAntiCheatClientInterface() {
    lock _(GLOBAL_MUTEX);
    return reinterpret_cast<EOS_HAntiCheatClient>(m_anticheatclient);
}

EOS_HAntiCheatServer EOSSDK_Platform::GetAntiCheatServerInterface() {
    lock _(GLOBAL_MUTEX);
    return reinterpret_cast<EOS_HAntiCheatServer>(m_anticheatserver);
}

EOS_HProgressionSnapshot EOSSDK_Platform::GetProgressionSnapshotInterface() {
    lock _(GLOBAL_MUTEX);
    return reinterpret_cast<EOS_HProgressionSnapshot>(m_progressionsnapshot);
}

EOS_HReports EOSSDK_Platform::GetReportsInterface() {
    lock _(GLOBAL_MUTEX);
    return reinterpret_cast<EOS_HReports>(m_reports);
}

EOS_HSanctions EOSSDK_Platform::GetSanctionsInterface() {
    lock _(GLOBAL_MUTEX);
    return reinterpret_cast<EOS_HSanctions>(m_sanctions);
}

EOS_HKWS EOSSDK_Platform::GetKWSInterface() {
    lock _(GLOBAL_MUTEX);
    return reinterpret_cast<EOS_HKWS>(m_kws);
}

EOS_HCustomInvites EOSSDK_Platform::GetCustomInvitesInterface() {
    lock _(GLOBAL_MUTEX);
    return reinterpret_cast<EOS_HCustomInvites>(m_custominvites);
}

EOS_HIntegratedPlatform EOSSDK_Platform::GetIntegratedPlatformInterface() {
    lock _(GLOBAL_MUTEX);
    return reinterpret_cast<EOS_HIntegratedPlatform>(m_integratedplatform);
}

void* EOSSDK_Platform::GetCryptoInterface() {
    lock _(GLOBAL_MUTEX);
    return m_crypto;
}

void* EOSSDK_Platform::GetEULATrackingInterface() {
    lock _(GLOBAL_MUTEX);
    return m_eulatracking;
}

void* EOSSDK_Platform::GetEpicConnectInterface() {
    lock _(GLOBAL_MUTEX);
    return m_epicconnect;
}

void* EOSSDK_Platform::GetGameClipsInterface() {
    lock _(GLOBAL_MUTEX);
    return m_gameclips;
}

void* EOSSDK_Platform::GetInventoryInterface() {
    lock _(GLOBAL_MUTEX);
    return m_inventory;
}

void* EOSSDK_Platform::GetK3SInterface() {
    lock _(GLOBAL_MUTEX);
    return m_k3s;
}

void* EOSSDK_Platform::GetMatchmakingInterface() {
    lock _(GLOBAL_MUTEX);
    return m_matchmaking;
}

void* EOSSDK_Platform::GetModerationInterface() {
    lock _(GLOBAL_MUTEX);
    return m_moderation;
}

void* EOSSDK_Platform::GetPartiesInterface() {
    lock _(GLOBAL_MUTEX);
    return m_parties;
}

void* EOSSDK_Platform::GetPartiesInternalInterface() {
    lock _(GLOBAL_MUTEX);
    return m_partiesinternal;
}

void* EOSSDK_Platform::GetTextChatClientInterface() {
    lock _(GLOBAL_MUTEX);
    return m_textchatclient;
}

void* EOSSDK_Platform::GetTextChatServerInterface() {
    lock _(GLOBAL_MUTEX);
    return m_textchatserver;
}

void* EOSSDK_Platform::GetTextToSpeechInterface() {
    lock _(GLOBAL_MUTEX);
    return m_texttospeech;
}

void* EOSSDK_Platform::GetWeblinksInterface() {
    lock _(GLOBAL_MUTEX);
    return m_weblinks;
}

void* EOSSDK_Platform::GetWorldInventoryInterface() {
    lock _(GLOBAL_MUTEX);
    return m_worldinventory;
}

EOS_EResult EOSSDK_Platform::GetOverrideCountryCode(char* OutBuffer, int32_t* InOutBufferLength)
{
    TRACE_FUNC();

    if (OutBuffer == nullptr || InOutBufferLength == nullptr)
        return EOS_EResult::EOS_InvalidParameters;

    if (*InOutBufferLength < (m_override_country_code.length() + 1))
    {
        *InOutBufferLength = m_override_country_code.length() + 1;
        return EOS_EResult::EOS_LimitExceeded;
    }

    strncpy(OutBuffer, m_override_country_code.c_str(), m_override_country_code.length() + 1);

    return EOS_EResult::EOS_Success;
}

EOS_EResult EOSSDK_Platform::GetOverrideLocaleCode(char* OutBuffer, int32_t* InOutBufferLength)
{
    TRACE_FUNC();

    if (OutBuffer == nullptr || InOutBufferLength == nullptr)
        return EOS_EResult::EOS_InvalidParameters;

    if (*InOutBufferLength < (m_override_locale_code.length() + 1))
    {
        *InOutBufferLength = m_override_locale_code.length() + 1;
        return EOS_EResult::EOS_LimitExceeded;
    }

    strncpy(OutBuffer, m_override_locale_code.c_str(), m_override_locale_code.length() + 1);

    return EOS_EResult::EOS_Success;
}

EOS_EResult EOSSDK_Platform::GetActiveCountryCode(EOS_EpicAccountId LocalUserId, char* OutBuffer, int32_t* InOutBufferLength) {
    TRACE_FUNC();
    string code = m_override_country_code.empty() ? "US" : m_override_country_code;
    if (!OutBuffer || !InOutBufferLength || *InOutBufferLength < (int32_t)code.length() + 1) {
        if (InOutBufferLength) *InOutBufferLength = (int32_t)code.length() + 1;
        return EOS_EResult::EOS_LimitExceeded;
    }
    strncpy(OutBuffer, code.c_str(), code.length() + 1);
    *InOutBufferLength = (int32_t)code.length();
    return EOS_EResult::EOS_Success;
}

EOS_EResult EOSSDK_Platform::GetActiveLocaleCode(EOS_EpicAccountId LocalUserId, char* OutBuffer, int32_t* InOutBufferLength) {
    TRACE_FUNC();
    string code = m_override_locale_code.empty() ? "en" : m_override_locale_code;
    if (!OutBuffer || !InOutBufferLength || *InOutBufferLength < (int32_t)code.length() + 1) {
        if (InOutBufferLength) *InOutBufferLength = (int32_t)code.length() + 1;
        return EOS_EResult::EOS_LimitExceeded;
    }
    strncpy(OutBuffer, code.c_str(), code.length() + 1);
    *InOutBufferLength = (int32_t)code.length();
    return EOS_EResult::EOS_Success;
}

EOS_EResult EOSSDK_Platform::SetOverrideCountryCode(const char* NewCountryCode) {
    TRACE_FUNC();
    if (!NewCountryCode) return EOS_EResult::EOS_InvalidParameters;
    m_override_country_code = NewCountryCode;
    return EOS_EResult::EOS_Success;
}

EOS_EResult EOSSDK_Platform::SetOverrideLocaleCode(const char* NewLocaleCode) {
    TRACE_FUNC();
    if (!NewLocaleCode) return EOS_EResult::EOS_InvalidParameters;
    m_override_locale_code = NewLocaleCode;
    return EOS_EResult::EOS_Success;
}


EOS_EResult EOSSDK_Platform::CheckForLauncherAndRestart()
{
    TRACE_FUNC();

    return EOS_EResult::EOS_NoChange;
}

}