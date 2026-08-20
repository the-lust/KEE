#pragma once

#include "common_includes.h"

#include "callback_manager.h"
#include "network.h"

#include "eossdk_achievements.h"
#include "eossdk_auth.h"
#include "eossdk_connect.h"
#include "eossdk_ecom.h"
#include "eossdk_friends.h"
#include "eossdk_leaderboards.h"
#include "eossdk_lobby.h"
#include "eossdk_metrics.h"
#include "eossdk_p2p.h"
#include "eossdk_playerdatastorage.h"
#include "eossdk_presence.h"
#include "eossdk_sessions.h"
#include "eossdk_stats.h"
#include "eossdk_titlestorage.h"
#include "eossdk_ui.h"
#include "eossdk_userinfo.h"

namespace sdk {

class EOSSDK_RTC;
class EOSSDK_RTCAdmin;
class EOSSDK_CustomInvites;
class EOSSDK_KWS;
class EOSSDK_Sanctions;
class EOSSDK_Mods;
class EOSSDK_Reports;
class EOSSDK_ProgressionSnapshot;
class EOSSDK_AntiCheatClient;
class EOSSDK_AntiCheatServer;
class EOSSDK_IntegratedPlatform;

class EOSSDK_Platform
{
  EOSSDK_Platform();

  bool m_platform_init;

public:
  int32_t m_api_version;
  void *m_reserved;
  string m_product_id;
  string m_sandbox_id;
  string m_client_id;
  string m_client_secret;
  EOS_Bool m_is_server;
  string m_encryption_key;
  string m_override_country_code;
  string m_override_locale_code;
  string m_deployment_id;
  uint64_t m_flags;
  string m_cache_directory;
  uint32_t m_ticket_budget_in_milliseconds;
  bool m_enable_overlay        = true;
  bool m_enable_social_overlay = true;

  Callback_Manager *m_cb_manager;
  Network *m_network;

  EOSSDK_Metrics *m_metrics;
  EOSSDK_Auth *m_auth;
  EOSSDK_Connect *m_connect;
  EOSSDK_Ecom *m_ecom;
  EOSSDK_UI *m_ui;
  EOSSDK_Friends *m_friends;
  EOSSDK_Presence *m_presence;
  EOSSDK_Sessions *m_sessions;
  EOSSDK_Lobby *m_lobby;
  EOSSDK_UserInfo *m_userinfo;
  EOSSDK_P2P *m_p2p;
  EOSSDK_PlayerDataStorage *m_playerdatastorage;
  EOSSDK_Achievements *m_achievements;
  EOSSDK_Stats *m_stats;
  EOSSDK_TitleStorage *m_titlestorage;
  EOSSDK_Leaderboards *m_leaderboards;

  EOSSDK_RTC *m_rtc;
  EOSSDK_RTCAdmin *m_rtcadmin;
  EOSSDK_Mods *m_mods;
  EOSSDK_AntiCheatClient *m_anticheatclient;
  EOSSDK_AntiCheatServer *m_anticheatserver;
  EOSSDK_ProgressionSnapshot *m_progressionsnapshot;
  EOSSDK_Reports *m_reports;
  EOSSDK_Sanctions *m_sanctions;
  EOSSDK_KWS *m_kws;
  EOSSDK_CustomInvites *m_custominvites;
  EOSSDK_IntegratedPlatform *m_integratedplatform;

  void *m_crypto;
  void *m_eulatracking;
  void *m_epicconnect;
  void *m_gameclips;
  void *m_inventory;
  void *m_k3s;
  void *m_matchmaking;
  void *m_moderation;
  void *m_parties;
  void *m_partiesinternal;
  void *m_textchatclient;
  void *m_textchatserver;
  void *m_texttospeech;
  void *m_weblinks;
  void *m_worldinventory;

  ~EOSSDK_Platform();

  static EOSSDK_Platform &Inst();
  void Init(const EOS_Platform_Options *Options);

  void Release();
  void Tick();
  EOS_HMetrics GetMetricsInterface();
  EOS_HAuth GetAuthInterface();
  EOS_HConnect GetConnectInterface();
  EOS_HEcom GetEcomInterface();
  EOS_HUI GetUIInterface();
  EOS_HFriends GetFriendsInterface();
  EOS_HPresence GetPresenceInterface();
  EOS_HSessions GetSessionsInterface();
  EOS_HLobby GetLobbyInterface();
  EOS_HUserInfo GetUserInfoInterface();
  EOS_HP2P GetP2PInterface();
  EOS_HPlayerDataStorage GetPlayerDataStorageInterface();
  EOS_HTitleStorage GetTitleStorageInterface();
  EOS_HAchievements GetAchievementsInterface();
  EOS_HStats GetStatsInterface();
  EOS_HLeaderboards GetLeaderboardsInterface();

  EOS_HRTC GetRTCInterface();
  EOS_HRTCAdmin GetRTCAdminInterface();
  EOS_HMods GetModsInterface();
  EOS_HAntiCheatClient GetAntiCheatClientInterface();
  EOS_HAntiCheatServer GetAntiCheatServerInterface();
  EOS_HProgressionSnapshot GetProgressionSnapshotInterface();
  EOS_HReports GetReportsInterface();
  EOS_HSanctions GetSanctionsInterface();
  EOS_HKWS GetKWSInterface();
  EOS_HCustomInvites GetCustomInvitesInterface();
  EOS_HIntegratedPlatform GetIntegratedPlatformInterface();

  void* GetCryptoInterface();
  void* GetEULATrackingInterface();
  void* GetEpicConnectInterface();
  void* GetGameClipsInterface();
  void* GetInventoryInterface();
  void* GetK3SInterface();
  void* GetMatchmakingInterface();
  void* GetModerationInterface();
  void* GetPartiesInterface();
  void* GetPartiesInternalInterface();
  void* GetTextChatClientInterface();
  void* GetTextChatServerInterface();
  void* GetTextToSpeechInterface();
  void* GetWeblinksInterface();
  void* GetWorldInventoryInterface();

  EOS_EResult GetActiveCountryCode(EOS_EpicAccountId LocalUserId,
                                   char *OutBuffer, int32_t *InOutBufferLength);
  EOS_EResult GetActiveLocaleCode(EOS_EpicAccountId LocalUserId,
                                  char *OutBuffer, int32_t *InOutBufferLength);
  EOS_EResult GetOverrideCountryCode(char *OutBuffer,
                                     int32_t *InOutBufferLength);
  EOS_EResult GetOverrideLocaleCode(char *OutBuffer,
                                    int32_t *InOutBufferLength);
  EOS_EResult SetOverrideCountryCode(const char *NewCountryCode);
  EOS_EResult SetOverrideLocaleCode(const char *NewLocaleCode);
  EOS_EResult CheckForLauncherAndRestart();
};
} 

inline sdk::EOSSDK_Platform &GetEOS_Platform() {
  return sdk::EOSSDK_Platform::Inst();
}

inline Callback_Manager &GetCB_Manager() {
  return *GetEOS_Platform().m_cb_manager;
}
inline Network &GetNetwork() { return *GetEOS_Platform().m_network; }

inline sdk::EOSSDK_Metrics &GetEOS_Metrics() {
  if (!GetEOS_Platform().m_metrics) { APP_LOG(Log::LogLevel::ERR, "Attempted to access unitialized Metrics interface!"); abort(); }
  return *GetEOS_Platform().m_metrics;
}

inline sdk::EOSSDK_Auth &GetEOS_Auth() {
  if (!GetEOS_Platform().m_auth) { APP_LOG(Log::LogLevel::ERR, "Attempted to access unitialized Auth interface!"); abort(); }
  return *GetEOS_Platform().m_auth;
}

inline sdk::EOSSDK_Connect &GetEOS_Connect() {
  if (!GetEOS_Platform().m_connect) { APP_LOG(Log::LogLevel::ERR, "Attempted to access unitialized Connect interface!"); abort(); }
  return *GetEOS_Platform().m_connect;
}

inline sdk::EOSSDK_Ecom &GetEOS_Ecom() {
  if (!GetEOS_Platform().m_ecom) { APP_LOG(Log::LogLevel::ERR, "Attempted to access unitialized Ecom interface!"); abort(); }
  return *GetEOS_Platform().m_ecom;
}

inline sdk::EOSSDK_UI &GetEOS_UI() {
  if (!GetEOS_Platform().m_ui) { APP_LOG(Log::LogLevel::ERR, "Attempted to access unitialized UI interface!"); abort(); }
  return *GetEOS_Platform().m_ui;
}

inline sdk::EOSSDK_Friends &GetEOS_Friends() {
  if (!GetEOS_Platform().m_friends) { APP_LOG(Log::LogLevel::ERR, "Attempted to access unitialized Friends interface!"); abort(); }
  return *GetEOS_Platform().m_friends;
}

inline sdk::EOSSDK_Presence &GetEOS_Presence() {
  if (!GetEOS_Platform().m_presence) { APP_LOG(Log::LogLevel::ERR, "Attempted to access unitialized Presence interface!"); abort(); }
  return *GetEOS_Platform().m_presence;
}

inline sdk::EOSSDK_Sessions &GetEOS_Sessions() {
  if (!GetEOS_Platform().m_sessions) { APP_LOG(Log::LogLevel::ERR, "Attempted to access unitialized Sessions interface!"); abort(); }
  return *GetEOS_Platform().m_sessions;
}

inline sdk::EOSSDK_Lobby &GetEOS_Lobby() {
  if (!GetEOS_Platform().m_lobby) { APP_LOG(Log::LogLevel::ERR, "Attempted to access unitialized Lobby interface!"); abort(); }
  return *GetEOS_Platform().m_lobby;
}

inline sdk::EOSSDK_UserInfo &GetEOS_UserInfo() {
  if (!GetEOS_Platform().m_userinfo) { APP_LOG(Log::LogLevel::ERR, "Attempted to access unitialized UserInfo interface!"); abort(); }
  return *GetEOS_Platform().m_userinfo;
}

inline sdk::EOSSDK_P2P &GetEOS_P2P() {
  if (!GetEOS_Platform().m_p2p) { APP_LOG(Log::LogLevel::ERR, "Attempted to access unitialized P2P interface!"); abort(); }
  return *GetEOS_Platform().m_p2p;
}

inline sdk::EOSSDK_PlayerDataStorage &GetEOS_PlayerDataStorage() {
  if (!GetEOS_Platform().m_playerdatastorage) { APP_LOG(Log::LogLevel::ERR, "Attempted to access unitialized PlayerDataStorage interface!"); abort(); }
  return *GetEOS_Platform().m_playerdatastorage;
}

inline sdk::EOSSDK_Achievements &GetEOS_Achievements() {
  if (!GetEOS_Platform().m_achievements) { APP_LOG(Log::LogLevel::ERR, "Attempted to access unitialized Achievements interface!"); abort(); }
  return *GetEOS_Platform().m_achievements;
}

inline sdk::EOSSDK_Stats &GetEOS_Stats() {
  if (!GetEOS_Platform().m_stats) { APP_LOG(Log::LogLevel::ERR, "Attempted to access unitialized Stats interface!"); abort(); }
  return *GetEOS_Platform().m_stats;
}

inline sdk::EOSSDK_Leaderboards &GetEOS_Leaderboards() {
  if (!GetEOS_Platform().m_leaderboards) { APP_LOG(Log::LogLevel::ERR, "Attempted to access unitialized Leaderboards interface!"); abort(); }
  return *GetEOS_Platform().m_leaderboards;
}

inline sdk::EOSSDK_RTC &GetEOS_RTC() {
  if (!GetEOS_Platform().m_rtc) { APP_LOG(Log::LogLevel::ERR, "Attempted to access unitialized RTC interface!"); abort(); }
  return *GetEOS_Platform().m_rtc;
}
inline sdk::EOSSDK_RTCAdmin &GetEOS_RTCAdmin() {
  if (!GetEOS_Platform().m_rtcadmin) { APP_LOG(Log::LogLevel::ERR, "Attempted to access unitialized RTCAdmin interface!"); abort(); }
  return *GetEOS_Platform().m_rtcadmin;
}
inline sdk::EOSSDK_CustomInvites &GetEOS_CustomInvites() {
  if (!GetEOS_Platform().m_custominvites) { APP_LOG(Log::LogLevel::ERR, "Attempted to access unitialized CustomInvites interface!"); abort(); }
  return *GetEOS_Platform().m_custominvites;
}
inline sdk::EOSSDK_KWS &GetEOS_KWS() {
  if (!GetEOS_Platform().m_kws) { APP_LOG(Log::LogLevel::ERR, "Attempted to access unitialized KWS interface!"); abort(); }
  return *GetEOS_Platform().m_kws;
}
inline sdk::EOSSDK_Sanctions &GetEOS_Sanctions() {
  if (!GetEOS_Platform().m_sanctions) { APP_LOG(Log::LogLevel::ERR, "Attempted to access unitialized Sanctions interface!"); abort(); }
  return *GetEOS_Platform().m_sanctions;
}
inline sdk::EOSSDK_Mods &GetEOS_Mods() {
  if (!GetEOS_Platform().m_mods) { APP_LOG(Log::LogLevel::ERR, "Attempted to access unitialized Mods interface!"); abort(); }
  return *GetEOS_Platform().m_mods;
}
inline sdk::EOSSDK_Reports &GetEOS_Reports() {
  if (!GetEOS_Platform().m_reports) { APP_LOG(Log::LogLevel::ERR, "Attempted to access unitialized Reports interface!"); abort(); }
  return *GetEOS_Platform().m_reports;
}
inline sdk::EOSSDK_ProgressionSnapshot &GetEOS_ProgressionSnapshot() {
  if (!GetEOS_Platform().m_progressionsnapshot) { APP_LOG(Log::LogLevel::ERR, "Attempted to access unitialized ProgressionSnapshot interface!"); abort(); }
  return *GetEOS_Platform().m_progressionsnapshot;
}