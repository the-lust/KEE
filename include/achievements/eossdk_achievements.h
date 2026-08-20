#pragma once

#include "common_includes.h"
#include "callback_manager.h"
#include "network.h"

namespace sdk
{
    class EOSSDK_Achievements :
        public IRunCallback
    {
        string achievements_filename;
        string achievements_db_filename;

        fifo_json m_achievements_db;
        fifo_json m_achievements;
        fifo_json m_stats_cache;   
        std::unordered_map<string, fifo_json*> m_unlocked_achievements;

    public:
        EOSSDK_Achievements();
        ~EOSSDK_Achievements();

        EOS_EResult copy_definition(typename decltype(m_achievements_db)::iterator it, EOS_Achievements_Definition** OutDefinition);
        EOS_EResult copy_definition_v2(typename decltype(m_achievements_db)::iterator it, EOS_Achievements_DefinitionV2** OutDefinition);
        EOS_EResult copy_unlocked_achievement(typename decltype(m_unlocked_achievements)::iterator it, EOS_Achievements_UnlockedAchievement** OutAchievement);
        EOS_EResult copy_player_achievement(typename decltype(m_achievements)::iterator it, EOS_Achievements_PlayerAchievement** OutAchievement);

        void check_stat_thresholds(const string& stat_name, int32_t new_value);

        virtual bool CBRunFrame();
        virtual bool RunCallbacks(pFrameResult_t res);
        virtual void FreeCallback(pFrameResult_t res);

        void               QueryDefinitions(const EOS_Achievements_QueryDefinitionsOptions* Options, void* ClientData, const EOS_Achievements_OnQueryDefinitionsCompleteCallback CompletionDelegate);
        uint32_t           GetAchievementDefinitionCount(const EOS_Achievements_GetAchievementDefinitionCountOptions* Options);
        EOS_EResult        CopyAchievementDefinitionV2ByIndex(const EOS_Achievements_CopyAchievementDefinitionV2ByIndexOptions* Options, EOS_Achievements_DefinitionV2** OutDefinition);
        EOS_EResult        CopyAchievementDefinitionV2ByAchievementId(const EOS_Achievements_CopyAchievementDefinitionV2ByAchievementIdOptions* Options, EOS_Achievements_DefinitionV2** OutDefinition);
        void               QueryPlayerAchievements(const EOS_Achievements_QueryPlayerAchievementsOptions* Options, void* ClientData, const EOS_Achievements_OnQueryPlayerAchievementsCompleteCallback CompletionDelegate);
        uint32_t           GetPlayerAchievementCount(const EOS_Achievements_GetPlayerAchievementCountOptions* Options);
        EOS_EResult        CopyPlayerAchievementByIndex(const EOS_Achievements_CopyPlayerAchievementByIndexOptions* Options, EOS_Achievements_PlayerAchievement** OutAchievement);
        EOS_EResult        CopyPlayerAchievementByAchievementId(const EOS_Achievements_CopyPlayerAchievementByAchievementIdOptions* Options, EOS_Achievements_PlayerAchievement** OutAchievement);
        void               UnlockAchievements(const EOS_Achievements_UnlockAchievementsOptions* Options, void* ClientData, const EOS_Achievements_OnUnlockAchievementsCompleteCallback CompletionDelegate);
        uint32_t           GetUnlockedAchievementCount(const EOS_Achievements_GetUnlockedAchievementCountOptions* Options);
        EOS_EResult        CopyUnlockedAchievementByIndex(const EOS_Achievements_CopyUnlockedAchievementByIndexOptions* Options, EOS_Achievements_UnlockedAchievement** OutAchievement);
        EOS_EResult        CopyUnlockedAchievementByAchievementId(const EOS_Achievements_CopyUnlockedAchievementByAchievementIdOptions* Options, EOS_Achievements_UnlockedAchievement** OutAchievement);
        EOS_NotificationId AddNotifyAchievementsUnlocked(const EOS_Achievements_AddNotifyAchievementsUnlockedOptions* Options, void* ClientData, const EOS_Achievements_OnAchievementsUnlockedCallback NotificationFn);
        EOS_NotificationId AddNotifyAchievementsUnlockedV2(const EOS_Achievements_AddNotifyAchievementsUnlockedV2Options* Options, void* ClientData, const EOS_Achievements_OnAchievementsUnlockedCallbackV2 NotificationFn);
        void               RemoveNotifyAchievementsUnlocked(EOS_NotificationId InId);
        EOS_EResult        CopyAchievementDefinitionByIndex(const EOS_Achievements_CopyAchievementDefinitionByIndexOptions* Options, EOS_Achievements_Definition** OutDefinition);
        EOS_EResult        CopyAchievementDefinitionByAchievementId(const EOS_Achievements_CopyAchievementDefinitionByAchievementIdOptions* Options, EOS_Achievements_Definition** OutDefinition);
    };
}