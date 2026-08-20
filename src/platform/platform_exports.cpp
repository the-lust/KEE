#include "utils/common_includes.h"
#include "platform/eossdk_platform.h"

using namespace sdk;

extern "C"
{
    EOS_DECLARE_FUNC(void) EOS_Platform_Tick(EOS_HPlatform Handle)
    {
        if (Handle == nullptr)
            return;

        auto pInst = reinterpret_cast<EOSSDK_Platform*>(Handle);
        pInst->Tick();
    }

    EOS_DECLARE_FUNC(EOS_HMetrics) EOS_Platform_GetMetricsInterface(EOS_HPlatform Handle)
    {
        if (Handle == nullptr)
            return nullptr;

        auto pInst = reinterpret_cast<EOSSDK_Platform*>(Handle);
        return pInst->GetMetricsInterface();
    }

    EOS_DECLARE_FUNC(EOS_HAuth) EOS_Platform_GetAuthInterface(EOS_HPlatform Handle)
    {
        if (Handle == nullptr)
            return nullptr;

        auto pInst = reinterpret_cast<EOSSDK_Platform*>(Handle);
        return pInst->GetAuthInterface();
    }

    EOS_DECLARE_FUNC(EOS_HConnect) EOS_Platform_GetConnectInterface(EOS_HPlatform Handle)
    {
        if (Handle == nullptr)
            return nullptr;

        auto pInst = reinterpret_cast<EOSSDK_Platform*>(Handle);
        return pInst->GetConnectInterface();
    }

    EOS_DECLARE_FUNC(EOS_HEcom) EOS_Platform_GetEcomInterface(EOS_HPlatform Handle)
    {
        if (Handle == nullptr)
            return nullptr;

        auto pInst = reinterpret_cast<EOSSDK_Platform*>(Handle);
        return pInst->GetEcomInterface();
    }

    EOS_DECLARE_FUNC(EOS_HUI) EOS_Platform_GetUIInterface(EOS_HPlatform Handle)
    {
        if (Handle == nullptr)
            return nullptr;

        auto pInst = reinterpret_cast<EOSSDK_Platform*>(Handle);
        return pInst->GetUIInterface();
    }

    EOS_DECLARE_FUNC(EOS_HFriends) EOS_Platform_GetFriendsInterface(EOS_HPlatform Handle)
    {
        if (Handle == nullptr)
            return nullptr;

        auto pInst = reinterpret_cast<EOSSDK_Platform*>(Handle);
        return pInst->GetFriendsInterface();
    }

    EOS_DECLARE_FUNC(EOS_HPresence) EOS_Platform_GetPresenceInterface(EOS_HPlatform Handle)
    {
        if (Handle == nullptr)
            return nullptr;

        auto pInst = reinterpret_cast<EOSSDK_Platform*>(Handle);
        return pInst->GetPresenceInterface();
    }

    EOS_DECLARE_FUNC(EOS_HSessions) EOS_Platform_GetSessionsInterface(EOS_HPlatform Handle)
    {
        if (Handle == nullptr)
            return nullptr;

        auto pInst = reinterpret_cast<EOSSDK_Platform*>(Handle);
        return pInst->GetSessionsInterface();
    }

    EOS_DECLARE_FUNC(EOS_HLobby) EOS_Platform_GetLobbyInterface(EOS_HPlatform Handle)
    {
        if (Handle == nullptr)
            return nullptr;

        auto pInst = reinterpret_cast<EOSSDK_Platform*>(Handle);
        return pInst->GetLobbyInterface();
    }

    EOS_DECLARE_FUNC(EOS_HUserInfo) EOS_Platform_GetUserInfoInterface(EOS_HPlatform Handle)
    {
        if (Handle == nullptr)
            return nullptr;

        auto pInst = reinterpret_cast<EOSSDK_Platform*>(Handle);
        return pInst->GetUserInfoInterface();
    }

    EOS_DECLARE_FUNC(EOS_HP2P) EOS_Platform_GetP2PInterface(EOS_HPlatform Handle)
    {
        if (Handle == nullptr)
            return nullptr;

        auto pInst = reinterpret_cast<EOSSDK_Platform*>(Handle);
        return pInst->GetP2PInterface();
    }

    EOS_DECLARE_FUNC(EOS_HPlayerDataStorage) EOS_Platform_GetPlayerDataStorageInterface(EOS_HPlatform Handle)
    {
        if (Handle == nullptr)
            return nullptr;

        auto pInst = reinterpret_cast<EOSSDK_Platform*>(Handle);
        return pInst->GetPlayerDataStorageInterface();
    }

    EOS_DECLARE_FUNC(EOS_HTitleStorage) EOS_Platform_GetTitleStorageInterface(EOS_HPlatform Handle)
    {
        if (Handle == nullptr)
            return nullptr;

        auto pInst = reinterpret_cast<EOSSDK_Platform*>(Handle);
        return pInst->GetTitleStorageInterface();
    }

    EOS_DECLARE_FUNC(EOS_HAchievements) EOS_Platform_GetAchievementsInterface(EOS_HPlatform Handle)
    {
        if (Handle == nullptr)
            return nullptr;

        auto pInst = reinterpret_cast<EOSSDK_Platform*>(Handle);
        return pInst->GetAchievementsInterface();
    }

    EOS_DECLARE_FUNC(EOS_HStats) EOS_Platform_GetStatsInterface(EOS_HPlatform Handle)
    {
        if (Handle == nullptr)
            return nullptr;

        auto pInst = reinterpret_cast<EOSSDK_Platform*>(Handle);
        return pInst->GetStatsInterface();
    }

    EOS_DECLARE_FUNC(EOS_HLeaderboards) EOS_Platform_GetLeaderboardsInterface(EOS_HPlatform Handle)
    {
        if (Handle == nullptr)
            return nullptr;

        auto pInst = reinterpret_cast<EOSSDK_Platform*>(Handle);
        return pInst->GetLeaderboardsInterface();
    }

    EOS_DECLARE_FUNC(EOS_EResult) EOS_Platform_GetActiveCountryCode(EOS_HPlatform Handle, EOS_EpicAccountId LocalUserId, char* OutBuffer, int32_t* InOutBufferLength)
    {
        if (Handle == nullptr)
            return EOS_EResult::EOS_InvalidParameters;

        auto pInst = reinterpret_cast<EOSSDK_Platform*>(Handle);
        return pInst->GetActiveCountryCode(LocalUserId, OutBuffer, InOutBufferLength);
    }

    EOS_DECLARE_FUNC(EOS_EResult) EOS_Platform_GetActiveLocaleCode(EOS_HPlatform Handle, EOS_EpicAccountId LocalUserId, char* OutBuffer, int32_t* InOutBufferLength)
    {
        if (Handle == nullptr)
            return EOS_EResult::EOS_InvalidParameters;

        auto pInst = reinterpret_cast<EOSSDK_Platform*>(Handle);
        return pInst->GetActiveLocaleCode(LocalUserId, OutBuffer, InOutBufferLength);
    }

    EOS_DECLARE_FUNC(EOS_EResult) EOS_Platform_GetOverrideCountryCode(EOS_HPlatform Handle, char* OutBuffer, int32_t* InOutBufferLength)
    {
        if (Handle == nullptr)
            return EOS_EResult::EOS_InvalidParameters;

        auto pInst = reinterpret_cast<EOSSDK_Platform*>(Handle);
        return pInst->GetOverrideCountryCode(OutBuffer, InOutBufferLength);
    }

    EOS_DECLARE_FUNC(EOS_EResult) EOS_Platform_GetOverrideLocaleCode(EOS_HPlatform Handle, char* OutBuffer, int32_t* InOutBufferLength)
    {
        if (Handle == nullptr)
            return EOS_EResult::EOS_InvalidParameters;

        auto pInst = reinterpret_cast<EOSSDK_Platform*>(Handle);
        return pInst->GetOverrideLocaleCode(OutBuffer, InOutBufferLength);
    }

    EOS_DECLARE_FUNC(EOS_EResult) EOS_Platform_SetOverrideCountryCode(EOS_HPlatform Handle, const char* NewCountryCode)
    {
        if (Handle == nullptr)
            return EOS_EResult::EOS_InvalidParameters;

        auto pInst = reinterpret_cast<EOSSDK_Platform*>(Handle);
        return pInst->SetOverrideCountryCode(NewCountryCode);
    }

    EOS_DECLARE_FUNC(EOS_EResult) EOS_Platform_SetOverrideLocaleCode(EOS_HPlatform Handle, const char* NewLocaleCode)
    {
        if (Handle == nullptr)
            return EOS_EResult::EOS_InvalidParameters;

        auto pInst = reinterpret_cast<EOSSDK_Platform*>(Handle);
        return pInst->SetOverrideLocaleCode(NewLocaleCode);
    }

    EOS_DECLARE_FUNC(EOS_EResult) EOS_Platform_CheckForLauncherAndRestart(EOS_HPlatform Handle)
    {
        if (Handle == nullptr)
            return EOS_EResult::EOS_InvalidParameters;

        auto pInst = reinterpret_cast<EOSSDK_Platform*>(Handle);
        return pInst->CheckForLauncherAndRestart();
    }

    EOS_DECLARE_FUNC(EOS_HPlatform) EOS_Platform_Create(const EOS_Platform_Options* Options)
    {
        auto &inst = EOSSDK_Platform::Inst();
        inst.Init(Options);
        return reinterpret_cast<EOS_HPlatform>(&inst);
    }

    EOS_DECLARE_FUNC(void) EOS_Platform_Release(EOS_HPlatform Handle)
    {
        auto pInst = reinterpret_cast<EOSSDK_Platform*>(Handle);
        if (pInst != &EOSSDK_Platform::Inst())
            return;
        pInst->Release();
    }

    EOS_DECLARE_FUNC(EOS_HRTC) EOS_Platform_GetRTCInterface(EOS_HPlatform Handle) { return GetEOS_Platform().GetRTCInterface(); }
    EOS_DECLARE_FUNC(EOS_HRTCAdmin) EOS_Platform_GetRTCAdminInterface(EOS_HPlatform Handle) { return GetEOS_Platform().GetRTCAdminInterface(); }
    EOS_DECLARE_FUNC(EOS_HMods) EOS_Platform_GetModsInterface(EOS_HPlatform Handle) { return GetEOS_Platform().GetModsInterface(); }
    EOS_DECLARE_FUNC(EOS_HAntiCheatClient) EOS_Platform_GetAntiCheatClientInterface(EOS_HPlatform Handle) { return GetEOS_Platform().GetAntiCheatClientInterface(); }
    EOS_DECLARE_FUNC(EOS_HAntiCheatServer) EOS_Platform_GetAntiCheatServerInterface(EOS_HPlatform Handle) { return GetEOS_Platform().GetAntiCheatServerInterface(); }
    EOS_DECLARE_FUNC(EOS_HProgressionSnapshot) EOS_Platform_GetProgressionSnapshotInterface(EOS_HPlatform Handle) { return GetEOS_Platform().GetProgressionSnapshotInterface(); }
    EOS_DECLARE_FUNC(EOS_HReports) EOS_Platform_GetReportsInterface(EOS_HPlatform Handle) { return GetEOS_Platform().GetReportsInterface(); }
    EOS_DECLARE_FUNC(EOS_HSanctions) EOS_Platform_GetSanctionsInterface(EOS_HPlatform Handle) { return GetEOS_Platform().GetSanctionsInterface(); }
    EOS_DECLARE_FUNC(EOS_HKWS) EOS_Platform_GetKWSInterface(EOS_HPlatform Handle) { return GetEOS_Platform().GetKWSInterface(); }
    EOS_DECLARE_FUNC(EOS_HCustomInvites) EOS_Platform_GetCustomInvitesInterface(EOS_HPlatform Handle) { return GetEOS_Platform().GetCustomInvitesInterface(); }
    EOS_DECLARE_FUNC(EOS_HIntegratedPlatform) EOS_Platform_GetIntegratedPlatformInterface(EOS_HPlatform Handle) { return GetEOS_Platform().GetIntegratedPlatformInterface(); }

    __declspec(dllexport) void* EOS_Platform_GetCryptoInterface(EOS_HPlatform Handle) { return GetEOS_Platform().GetCryptoInterface(); }
    __declspec(dllexport) void* EOS_Platform_GetEULATrackingInterface(EOS_HPlatform Handle) { return GetEOS_Platform().GetEULATrackingInterface(); }
    __declspec(dllexport) void* EOS_Platform_GetEpicConnectInterface(EOS_HPlatform Handle) { return GetEOS_Platform().GetEpicConnectInterface(); }
    __declspec(dllexport) void* EOS_Platform_GetGameClipsInterface(EOS_HPlatform Handle) { return GetEOS_Platform().GetGameClipsInterface(); }
    __declspec(dllexport) void* EOS_Platform_GetInventoryInterface(EOS_HPlatform Handle) { return GetEOS_Platform().GetInventoryInterface(); }
    __declspec(dllexport) void* EOS_Platform_GetK3SInterface(EOS_HPlatform Handle) { return GetEOS_Platform().GetK3SInterface(); }
    __declspec(dllexport) void* EOS_Platform_GetKmsInterface(EOS_HPlatform Handle) { return nullptr; }
    __declspec(dllexport) void* EOS_Platform_GetMatchmakingInterface(EOS_HPlatform Handle) { return GetEOS_Platform().GetMatchmakingInterface(); }
    __declspec(dllexport) void* EOS_Platform_GetModerationInterface(EOS_HPlatform Handle) { return GetEOS_Platform().GetModerationInterface(); }
    __declspec(dllexport) void* EOS_Platform_GetPartiesInterface(EOS_HPlatform Handle) { return GetEOS_Platform().GetPartiesInterface(); }
    __declspec(dllexport) void* EOS_Platform_GetPartiesInternalInterface(EOS_HPlatform Handle) { return GetEOS_Platform().GetPartiesInternalInterface(); }
    __declspec(dllexport) void* EOS_Platform_GetTextChatClientInterface(EOS_HPlatform Handle) { return GetEOS_Platform().GetTextChatClientInterface(); }
    __declspec(dllexport) void* EOS_Platform_GetTextChatServerInterface(EOS_HPlatform Handle) { return GetEOS_Platform().GetTextChatServerInterface(); }
    __declspec(dllexport) void* EOS_Platform_GetTextToSpeechInterface(EOS_HPlatform Handle) { return GetEOS_Platform().GetTextToSpeechInterface(); }
    __declspec(dllexport) void* EOS_Platform_GetWeblinksInterface(EOS_HPlatform Handle) { return GetEOS_Platform().GetWeblinksInterface(); }
    __declspec(dllexport) void* EOS_Platform_GetWorldInventoryInterface(EOS_HPlatform Handle) { return GetEOS_Platform().GetWorldInventoryInterface(); }

    EOS_DECLARE_FUNC(EOS_EResult) EOS_Platform_GetDesktopCrossplayStatus(EOS_HPlatform Handle, const EOS_Platform_GetDesktopCrossplayStatusOptions* Options, EOS_Platform_DesktopCrossplayStatusInfo* OutDesktopCrossplayStatusInfo)
{
    if (OutDesktopCrossplayStatusInfo)
    {
        OutDesktopCrossplayStatusInfo->Status            = EOS_EDesktopCrossplayStatus::EOS_DCS_OK;
        OutDesktopCrossplayStatusInfo->ServiceInitResult = 0;
    }
    return EOS_EResult::EOS_Success;
}
    EOS_DECLARE_FUNC(EOS_EResult) EOS_Platform_SetApplicationStatus(EOS_HPlatform Handle, const EOS_EApplicationStatus NewStatus) { return EOS_EResult::EOS_Success; }
    EOS_DECLARE_FUNC(EOS_EApplicationStatus) EOS_Platform_GetApplicationStatus(EOS_HPlatform Handle) { return EOS_EApplicationStatus::EOS_AS_Foreground; }
    EOS_DECLARE_FUNC(EOS_EResult) EOS_Platform_SetNetworkStatus(EOS_HPlatform Handle, const EOS_ENetworkStatus NewStatus) { return EOS_EResult::EOS_Success; }
    EOS_DECLARE_FUNC(EOS_ENetworkStatus) EOS_Platform_GetNetworkStatus(EOS_HPlatform Handle) { return EOS_ENetworkStatus::EOS_NS_Online; }
}
