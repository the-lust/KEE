

#if defined(PROCESS_CATEGORY)

PROCESS_CATEGORY(LogEOS, EOS_LC_Core, 0)

PROCESS_CATEGORY(LogEOSAuth, EOS_LC_Auth, 1)

PROCESS_CATEGORY(LogEOSFriends, EOS_LC_Friends, 2)

PROCESS_CATEGORY(LogEOSPresence, EOS_LC_Presence, 3)

PROCESS_CATEGORY(LogEOSUserInfo, EOS_LC_UserInfo, 4)

PROCESS_CATEGORY(LogHttpSerialization, EOS_LC_HttpSerialization, 5)

PROCESS_CATEGORY(LogEOSEcom, EOS_LC_Ecom, 6)

PROCESS_CATEGORY(LogEOSP2P, EOS_LC_P2P, 7)

PROCESS_CATEGORY(LogEOSSessions, EOS_LC_Sessions, 8)

PROCESS_CATEGORY(LogEOSRateLimiter, EOS_LC_RateLimiter, 9)

PROCESS_CATEGORY(LogEOSPlayerDataStorage, EOS_LC_PlayerDataStorage, 10)

PROCESS_CATEGORY(LogEOSAnalytics, EOS_LC_Analytics, 11)

PROCESS_CATEGORY(LogEOSMessaging, EOS_LC_Messaging, 12)

PROCESS_CATEGORY(LogEOSConnect, EOS_LC_Connect, 13)

PROCESS_CATEGORY(LogEOSOverlay, EOS_LC_Overlay, 14)

PROCESS_CATEGORY(LogEOSAchievements, EOS_LC_Achievements, 15)

PROCESS_CATEGORY(LogEOSStats, EOS_LC_Stats, 16)

PROCESS_CATEGORY(LogEOSUI, EOS_LC_UI, 17)

PROCESS_CATEGORY(LogEOSLobby, EOS_LC_Lobby, 18)

PROCESS_CATEGORY(LogEOSLeaderboards, EOS_LC_Leaderboards, 19)

PROCESS_CATEGORY(LogEOSKeychain, EOS_LC_Keychain, 20)

PROCESS_CATEGORY(LogEOSIntegratedPlatform, EOS_LC_IntegratedPlatform, 21)

PROCESS_CATEGORY(LogEOSTitleStorage, EOS_LC_TitleStorage, 22)

PROCESS_CATEGORY(LogEOSMods, EOS_LC_Mods, 23)

PROCESS_CATEGORY(LogEOSAntiCheat, EOS_LC_AntiCheat, 24)

PROCESS_CATEGORY(LogEOSReports, EOS_LC_Reports, 25)

PROCESS_CATEGORY(LogEOSSanctions, EOS_LC_Sanctions, 26)

PROCESS_CATEGORY(LogEOSProgressionSnapshots, EOS_LC_ProgressionSnapshots, 27)

PROCESS_CATEGORY(LogEOSKWS, EOS_LC_KWS, 28)

PROCESS_CATEGORY(LogEOSRTC, EOS_LC_RTC, 29)

PROCESS_CATEGORY(LogEOSRTCAdmin, EOS_LC_RTCAdmin, 30)

PROCESS_CATEGORY(LogEOSCustomInvites, EOS_LC_CustomInvites, 31)

PROCESS_CATEGORY(LogEOSHTTP, EOS_LC_HTTP, 41)

#endif 

#if defined(PROCESS_CATEGORY_LAST)

PROCESS_CATEGORY_LAST(Invalid, EOS_LC_ALL_CATEGORIES, 0x7fffffff)

#endif 

#if !defined(PROCESS_CATEGORY) && !defined(PROCESS_CATEGORY_LAST)
#error "eos_logging_categories.h requires PROCESS_CATEGORY(CategoryName, EOSCategoryLabel, EOSCategoryValue) and/or PROCESS_CATEGORY_LAST(CategoryName, EOSCategoryLabel, EOSCategoryValue) to be defined before inclusion."
#endif 
