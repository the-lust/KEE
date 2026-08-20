
#include "achievements/eossdk_achievements.h"

using namespace sdk;

extern "C"
{
    EOS_DECLARE_FUNC(void) EOS_Achievements_QueryDefinitions(EOS_HAchievements Handle, const EOS_Achievements_QueryDefinitionsOptions* Options, void* ClientData, const EOS_Achievements_OnQueryDefinitionsCompleteCallback CompletionDelegate)
    {
        if (Handle == nullptr)
            return;
        auto pInst = reinterpret_cast<EOSSDK_Achievements*>(Handle);
        pInst->QueryDefinitions(Options, ClientData, CompletionDelegate);
    }

    EOS_DECLARE_FUNC(uint32_t) EOS_Achievements_GetAchievementDefinitionCount(EOS_HAchievements Handle, const EOS_Achievements_GetAchievementDefinitionCountOptions* Options)
    {
        if (Handle == nullptr)
            return 0;
        auto pInst = reinterpret_cast<EOSSDK_Achievements*>(Handle);
        return pInst->GetAchievementDefinitionCount(Options);
    }

    EOS_DECLARE_FUNC(EOS_EResult) EOS_Achievements_CopyAchievementDefinitionByIndex(EOS_HAchievements Handle, const EOS_Achievements_CopyAchievementDefinitionByIndexOptions* Options, EOS_Achievements_Definition** OutDefinition)
    {
        if (Handle == nullptr)
            return EOS_EResult::EOS_InvalidParameters;
        auto pInst = reinterpret_cast<EOSSDK_Achievements*>(Handle);
        return pInst->CopyAchievementDefinitionByIndex(Options, OutDefinition);
    }

    EOS_DECLARE_FUNC(EOS_EResult) EOS_Achievements_CopyAchievementDefinitionByAchievementId(EOS_HAchievements Handle, const EOS_Achievements_CopyAchievementDefinitionByAchievementIdOptions* Options, EOS_Achievements_Definition** OutDefinition)
    {
        if (Handle == nullptr)
            return EOS_EResult::EOS_InvalidParameters;
        auto pInst = reinterpret_cast<EOSSDK_Achievements*>(Handle);
        return pInst->CopyAchievementDefinitionByAchievementId(Options, OutDefinition);
    }

    EOS_DECLARE_FUNC(EOS_EResult) EOS_Achievements_CopyAchievementDefinitionV2ByIndex(EOS_HAchievements Handle, const EOS_Achievements_CopyAchievementDefinitionV2ByIndexOptions* Options, EOS_Achievements_DefinitionV2** OutDefinition)
    {
        if (Handle == nullptr)
            return EOS_EResult::EOS_InvalidParameters;
        auto pInst = reinterpret_cast<EOSSDK_Achievements*>(Handle);
        return pInst->CopyAchievementDefinitionV2ByIndex(Options, OutDefinition);
    }

    EOS_DECLARE_FUNC(EOS_EResult) EOS_Achievements_CopyAchievementDefinitionV2ByAchievementId(EOS_HAchievements Handle, const EOS_Achievements_CopyAchievementDefinitionV2ByAchievementIdOptions* Options, EOS_Achievements_DefinitionV2** OutDefinition)
    {
        if (Handle == nullptr)
            return EOS_EResult::EOS_InvalidParameters;
        auto pInst = reinterpret_cast<EOSSDK_Achievements*>(Handle);
        return pInst->CopyAchievementDefinitionV2ByAchievementId(Options, OutDefinition);
    }

    EOS_DECLARE_FUNC(void) EOS_Achievements_QueryPlayerAchievements(EOS_HAchievements Handle, const EOS_Achievements_QueryPlayerAchievementsOptions* Options, void* ClientData, const EOS_Achievements_OnQueryPlayerAchievementsCompleteCallback CompletionDelegate)
    {
        if (Handle == nullptr)
            return;
        auto pInst = reinterpret_cast<EOSSDK_Achievements*>(Handle);
        pInst->QueryPlayerAchievements(Options, ClientData, CompletionDelegate);
    }

    EOS_DECLARE_FUNC(uint32_t) EOS_Achievements_GetPlayerAchievementCount(EOS_HAchievements Handle, const EOS_Achievements_GetPlayerAchievementCountOptions* Options)
    {
        if (Handle == nullptr)
            return 0;
        auto pInst = reinterpret_cast<EOSSDK_Achievements*>(Handle);
        return pInst->GetPlayerAchievementCount(Options);
    }

    EOS_DECLARE_FUNC(EOS_EResult) EOS_Achievements_CopyPlayerAchievementByIndex(EOS_HAchievements Handle, const EOS_Achievements_CopyPlayerAchievementByIndexOptions* Options, EOS_Achievements_PlayerAchievement** OutAchievement)
    {
        if (Handle == nullptr)
            return EOS_EResult::EOS_InvalidParameters;
        auto pInst = reinterpret_cast<EOSSDK_Achievements*>(Handle);
        return pInst->CopyPlayerAchievementByIndex(Options, OutAchievement);
    }

    EOS_DECLARE_FUNC(EOS_EResult) EOS_Achievements_CopyPlayerAchievementByAchievementId(EOS_HAchievements Handle, const EOS_Achievements_CopyPlayerAchievementByAchievementIdOptions* Options, EOS_Achievements_PlayerAchievement** OutAchievement)
    {
        if (Handle == nullptr)
            return EOS_EResult::EOS_InvalidParameters;
        auto pInst = reinterpret_cast<EOSSDK_Achievements*>(Handle);
        return pInst->CopyPlayerAchievementByAchievementId(Options, OutAchievement);
    }

    EOS_DECLARE_FUNC(void) EOS_Achievements_UnlockAchievements(EOS_HAchievements Handle, const EOS_Achievements_UnlockAchievementsOptions* Options, void* ClientData, const EOS_Achievements_OnUnlockAchievementsCompleteCallback CompletionDelegate)
    {
        if (Handle == nullptr)
            return;
        auto pInst = reinterpret_cast<EOSSDK_Achievements*>(Handle);
        pInst->UnlockAchievements(Options, ClientData, CompletionDelegate);
    }

    EOS_DECLARE_FUNC(uint32_t) EOS_Achievements_GetUnlockedAchievementCount(EOS_HAchievements Handle, const EOS_Achievements_GetUnlockedAchievementCountOptions* Options)
    {
        if (Handle == nullptr)
            return 0;
        auto pInst = reinterpret_cast<EOSSDK_Achievements*>(Handle);
        return pInst->GetUnlockedAchievementCount(Options);
    }

    EOS_DECLARE_FUNC(EOS_EResult) EOS_Achievements_CopyUnlockedAchievementByIndex(EOS_HAchievements Handle, const EOS_Achievements_CopyUnlockedAchievementByIndexOptions* Options, EOS_Achievements_UnlockedAchievement** OutAchievement)
    {
        if (Handle == nullptr)
            return EOS_EResult::EOS_InvalidParameters;
        auto pInst = reinterpret_cast<EOSSDK_Achievements*>(Handle);
        return pInst->CopyUnlockedAchievementByIndex(Options, OutAchievement);
    }

    EOS_DECLARE_FUNC(EOS_EResult) EOS_Achievements_CopyUnlockedAchievementByAchievementId(EOS_HAchievements Handle, const EOS_Achievements_CopyUnlockedAchievementByAchievementIdOptions* Options, EOS_Achievements_UnlockedAchievement** OutAchievement)
    {
        if (Handle == nullptr)
            return EOS_EResult::EOS_InvalidParameters;
        auto pInst = reinterpret_cast<EOSSDK_Achievements*>(Handle);
        return pInst->CopyUnlockedAchievementByAchievementId(Options, OutAchievement);
    }

    EOS_DECLARE_FUNC(EOS_NotificationId) EOS_Achievements_AddNotifyAchievementsUnlocked(EOS_HAchievements Handle, const EOS_Achievements_AddNotifyAchievementsUnlockedOptions* Options, void* ClientData, const EOS_Achievements_OnAchievementsUnlockedCallback NotificationFn)
    {
        if (Handle == nullptr)
            return EOS_INVALID_NOTIFICATIONID;
        auto pInst = reinterpret_cast<EOSSDK_Achievements*>(Handle);
        return pInst->AddNotifyAchievementsUnlocked(Options, ClientData, NotificationFn);
    }

    EOS_DECLARE_FUNC(EOS_NotificationId) EOS_Achievements_AddNotifyAchievementsUnlockedV2(EOS_HAchievements Handle, const EOS_Achievements_AddNotifyAchievementsUnlockedV2Options* Options, void* ClientData, const EOS_Achievements_OnAchievementsUnlockedCallbackV2 NotificationFn)
    {
        if (Handle == nullptr)
            return EOS_INVALID_NOTIFICATIONID;
        auto pInst = reinterpret_cast<EOSSDK_Achievements*>(Handle);
        return pInst->AddNotifyAchievementsUnlockedV2(Options, ClientData, NotificationFn);
    }

    EOS_DECLARE_FUNC(void) EOS_Achievements_RemoveNotifyAchievementsUnlocked(EOS_HAchievements Handle, EOS_NotificationId InId)
    {
        if (Handle == nullptr)
            return;
        auto pInst = reinterpret_cast<EOSSDK_Achievements*>(Handle);
        pInst->RemoveNotifyAchievementsUnlocked(InId);
    }

    EOS_DECLARE_FUNC(void) EOS_Achievements_Definition_Release(EOS_Achievements_Definition* AchievementDefinition)
    {
        if (AchievementDefinition == nullptr)
            return;
        delete AchievementDefinition;
    }

    EOS_DECLARE_FUNC(void) EOS_Achievements_DefinitionV2_Release(EOS_Achievements_DefinitionV2* AchievementDefinition)
    {
        if (AchievementDefinition == nullptr)
            return;
        delete AchievementDefinition;
    }

    EOS_DECLARE_FUNC(void) EOS_Achievements_PlayerAchievement_Release(EOS_Achievements_PlayerAchievement* Achievement)
    {
        if (Achievement == nullptr)
            return;
        delete Achievement;
    }

    EOS_DECLARE_FUNC(void) EOS_Achievements_UnlockedAchievement_Release(EOS_Achievements_UnlockedAchievement* Achievement)
    {
        if (Achievement == nullptr)
            return;
        delete Achievement;
    }
}
