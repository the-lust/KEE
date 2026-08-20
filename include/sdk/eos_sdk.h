
#pragma once

#include "eos_init.h"
#include "eos_metrics_types.h"
#include "eos_auth_types.h"
#include "eos_ecom_types.h"
#include "eos_ui_types.h"
#include "eos_friends_types.h"
#include "eos_presence_types.h"
#include "eos_p2p_types.h"
#include "eos_sessions_types.h"
#include "eos_lobby_types.h"
#include "eos_userinfo_types.h"
#include "eos_playerdatastorage_types.h"
#include "eos_titlestorage_types.h"
#include "eos_connect.h"
#include "eos_achievements_types.h"
#include "eos_stats_types.h"
#include "eos_leaderboards_types.h"
#include "eos_mods_types.h"
#include "eos_anticheatclient_types.h"
#include "eos_anticheatserver_types.h"
#include "eos_reports_types.h"
#include "eos_sanctions_types.h"
#include "eos_kws_types.h"
#include "eos_rtc_types.h"
#include "eos_rtc_admin_types.h"
#include "eos_progressionsnapshot_types.h"
#include "eos_custominvites_types.h"
#include "eos_integratedplatform_types.h"

EOS_DECLARE_FUNC(void) EOS_Platform_Tick(EOS_HPlatform Handle);

EOS_DECLARE_FUNC(EOS_HMetrics) EOS_Platform_GetMetricsInterface(EOS_HPlatform Handle);

EOS_DECLARE_FUNC(EOS_HAuth) EOS_Platform_GetAuthInterface(EOS_HPlatform Handle);

EOS_DECLARE_FUNC(EOS_HConnect) EOS_Platform_GetConnectInterface(EOS_HPlatform Handle);

EOS_DECLARE_FUNC(EOS_HEcom) EOS_Platform_GetEcomInterface(EOS_HPlatform Handle);

EOS_DECLARE_FUNC(EOS_HUI) EOS_Platform_GetUIInterface(EOS_HPlatform Handle);

EOS_DECLARE_FUNC(EOS_HFriends) EOS_Platform_GetFriendsInterface(EOS_HPlatform Handle);

EOS_DECLARE_FUNC(EOS_HPresence) EOS_Platform_GetPresenceInterface(EOS_HPlatform Handle);

EOS_DECLARE_FUNC(EOS_HSessions) EOS_Platform_GetSessionsInterface(EOS_HPlatform Handle);

EOS_DECLARE_FUNC(EOS_HLobby) EOS_Platform_GetLobbyInterface(EOS_HPlatform Handle);

EOS_DECLARE_FUNC(EOS_HUserInfo) EOS_Platform_GetUserInfoInterface(EOS_HPlatform Handle);

EOS_DECLARE_FUNC(EOS_HP2P) EOS_Platform_GetP2PInterface(EOS_HPlatform Handle);

EOS_DECLARE_FUNC(EOS_HRTC) EOS_Platform_GetRTCInterface(EOS_HPlatform Handle);

EOS_DECLARE_FUNC(EOS_HRTCAdmin) EOS_Platform_GetRTCAdminInterface(EOS_HPlatform Handle);

EOS_DECLARE_FUNC(EOS_HPlayerDataStorage) EOS_Platform_GetPlayerDataStorageInterface(EOS_HPlatform Handle);

EOS_DECLARE_FUNC(EOS_HTitleStorage) EOS_Platform_GetTitleStorageInterface(EOS_HPlatform Handle);

EOS_DECLARE_FUNC(EOS_HAchievements) EOS_Platform_GetAchievementsInterface(EOS_HPlatform Handle);

EOS_DECLARE_FUNC(EOS_HStats) EOS_Platform_GetStatsInterface(EOS_HPlatform Handle);

EOS_DECLARE_FUNC(EOS_HLeaderboards) EOS_Platform_GetLeaderboardsInterface(EOS_HPlatform Handle);

EOS_DECLARE_FUNC(EOS_HMods) EOS_Platform_GetModsInterface(EOS_HPlatform Handle);

EOS_DECLARE_FUNC(EOS_HAntiCheatClient) EOS_Platform_GetAntiCheatClientInterface(EOS_HPlatform Handle);

EOS_DECLARE_FUNC(EOS_HAntiCheatServer) EOS_Platform_GetAntiCheatServerInterface(EOS_HPlatform Handle);

EOS_DECLARE_FUNC(EOS_HProgressionSnapshot) EOS_Platform_GetProgressionSnapshotInterface(EOS_HPlatform Handle);

EOS_DECLARE_FUNC(EOS_HReports) EOS_Platform_GetReportsInterface(EOS_HPlatform Handle);

EOS_DECLARE_FUNC(EOS_HSanctions) EOS_Platform_GetSanctionsInterface(EOS_HPlatform Handle);

EOS_DECLARE_FUNC(EOS_HKWS) EOS_Platform_GetKWSInterface(EOS_HPlatform Handle);

EOS_DECLARE_FUNC(EOS_HCustomInvites) EOS_Platform_GetCustomInvitesInterface(EOS_HPlatform Handle);

EOS_DECLARE_FUNC(EOS_HIntegratedPlatform) EOS_Platform_GetIntegratedPlatformInterface(EOS_HPlatform Handle);

EOS_DECLARE_FUNC(EOS_EResult) EOS_Platform_GetActiveCountryCode(EOS_HPlatform Handle, EOS_EpicAccountId LocalUserId, char* OutBuffer, int32_t* InOutBufferLength);

EOS_DECLARE_FUNC(EOS_EResult) EOS_Platform_GetActiveLocaleCode(EOS_HPlatform Handle, EOS_EpicAccountId LocalUserId, char* OutBuffer, int32_t* InOutBufferLength);

EOS_DECLARE_FUNC(EOS_EResult) EOS_Platform_GetOverrideCountryCode(EOS_HPlatform Handle, char* OutBuffer, int32_t* InOutBufferLength);

EOS_DECLARE_FUNC(EOS_EResult) EOS_Platform_GetOverrideLocaleCode(EOS_HPlatform Handle, char* OutBuffer, int32_t* InOutBufferLength);

EOS_DECLARE_FUNC(EOS_EResult) EOS_Platform_SetOverrideCountryCode(EOS_HPlatform Handle, const char* NewCountryCode);

EOS_DECLARE_FUNC(EOS_EResult) EOS_Platform_SetOverrideLocaleCode(EOS_HPlatform Handle, const char* NewLocaleCode);

EOS_DECLARE_FUNC(EOS_EResult) EOS_Platform_CheckForLauncherAndRestart(EOS_HPlatform Handle);

EOS_DECLARE_FUNC(EOS_EResult) EOS_Platform_GetDesktopCrossplayStatus(EOS_HPlatform Handle, const EOS_Platform_GetDesktopCrossplayStatusOptions* Options, EOS_Platform_DesktopCrossplayStatusInfo* OutDesktopCrossplayStatusInfo);

EOS_DECLARE_FUNC(EOS_EResult) EOS_Platform_SetApplicationStatus(EOS_HPlatform Handle, const EOS_EApplicationStatus NewStatus);

EOS_DECLARE_FUNC(EOS_EApplicationStatus) EOS_Platform_GetApplicationStatus(EOS_HPlatform Handle);

EOS_DECLARE_FUNC(EOS_EResult) EOS_Platform_SetNetworkStatus(EOS_HPlatform Handle, const EOS_ENetworkStatus NewStatus);

EOS_DECLARE_FUNC(EOS_ENetworkStatus) EOS_Platform_GetNetworkStatus(EOS_HPlatform Handle);
