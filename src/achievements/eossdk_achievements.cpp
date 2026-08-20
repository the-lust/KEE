#include "eossdk_achievements.h"
#include "eossdk_platform.h"
#include "settings.h"
#include "overlay/OverlayManager.h"

extern "C" void EOS_CALL EOS_Achievements_DefinitionV2_Release(EOS_Achievements_DefinitionV2* AchievementDefinition);

namespace sdk
{

EOSSDK_Achievements::EOSSDK_Achievements()
{
    
    string kee_settings = Settings::Inst().kee_settings_path;
    string saves = Settings::Inst().save_path;

    achievements_filename = FileManager::join(saves, "achievements.json");
    achievements_db_filename = FileManager::join(kee_settings, "achievements.json");

    FileManager::load_json(achievements_filename, m_achievements);

    string stats_filename = FileManager::join(saves, "stats.json");
    FileManager::load_json(stats_filename, m_stats_cache);

    if (std::filesystem::exists(achievements_db_filename))
    {
        FileManager::load_json(achievements_db_filename, m_achievements_db);
    }
    else
    {
        
        string gbe_achievements = "steam_settings/achievements.json";
        if (std::filesystem::exists(gbe_achievements))
        {
            fifo_json gbe_data;
            FileManager::load_json(gbe_achievements, gbe_data);

            if (gbe_data.is_array())
            {
                for (auto& item : gbe_data)
                {
                    string id = item.value("name", "");
                    if (!id.empty())
                    {
                        m_achievements_db[id]["achievement_id"] = id;
                        m_achievements_db[id]["unlocked_display_name"] = item.value("display_name", id);
                        m_achievements_db[id]["unlocked_description"] = item.value("description", "");
                        m_achievements_db[id]["locked_display_name"] = item.value("display_name", id);
                        m_achievements_db[id]["locked_description"] = item.value("description", "");
                        m_achievements_db[id]["hidden_description"] = item.value("description", "");
                        m_achievements_db[id]["flavor_text"] = item.value("description", "");
                        m_achievements_db[id]["is_hidden"] = item.value("hidden", false);
                        m_achievements_db[id]["unlocked_icon_url"] = "steam_settings/achievements/" + id + ".png";
                        m_achievements_db[id]["locked_icon_url"] = "steam_settings/achievements/" + id + ".png";
                    }

                }
            }
        }
    }
}

EOSSDK_Achievements::~EOSSDK_Achievements()
{

    GetCB_Manager().remove_all_notifications(this);
}

EOS_EResult EOSSDK_Achievements::copy_definition(typename decltype(m_achievements_db)::iterator it, EOS_Achievements_Definition** OutDefinition)
{
    EOS_Achievements_Definition* ach = new EOS_Achievements_Definition;
    memset(ach, 0, sizeof(*ach));

    ach->ApiVersion = EOS_ACHIEVEMENTS_DEFINITION_API_LATEST;

    try
    {
        ach->AchievementId = it.value()["achievement_id"].get_ref<string&>().c_str();
    }
    catch (...)
    {
        APP_LOG(Log::LogLevel::ERR, "Error in achievement definition: Invalid achievement_id");
        goto achievement_error;
    }
    try
    {
        ach->DisplayName = it.value()["unlocked_display_name"].get_ref<string&>().c_str();
    }
    catch (...)
    {
        APP_LOG(Log::LogLevel::ERR, "Error in achievement definition: Invalid unlocked_display_name for %s", ach->AchievementId);
        goto achievement_error;
    }
    try
    {
        ach->Description = it.value()["unlocked_description"].get_ref<string&>().c_str();
    }
    catch (...)
    {
        APP_LOG(Log::LogLevel::ERR, "Error in achievement definition: Invalid unlocked_description for %s", ach->AchievementId);
        goto achievement_error;
    }
    try
    {
        ach->LockedDisplayName = it.value()["locked_display_name"].get_ref<string&>().c_str();
    }
    catch (...)
    {
        APP_LOG(Log::LogLevel::ERR, "Error in achievement definition: Invalid locked_display_name for %s", ach->AchievementId);
        goto achievement_error;
    }
    try
    {
        ach->LockedDescription = it.value()["locked_description"].get_ref<string&>().c_str();
    }
    catch (...)
    {
        APP_LOG(Log::LogLevel::ERR, "Error in achievement definition: Invalid locked_description for %s", ach->AchievementId);
        goto achievement_error;
    }
    try
    {
        try
        {
            ach->HiddenDescription = it.value()["hidden_description"].get_ref<string&>().c_str();
        }
        catch (...)
        {
            APP_LOG(Log::LogLevel::INFO, "No \"hidden_description\" in achievements_db, falling back to \"locked_description\"");
            ach->HiddenDescription = it.value()["locked_description"].get_ref<string&>().c_str();
        }
    }
    catch (...)
    {
        APP_LOG(Log::LogLevel::ERR, "Error in achievement definition: Invalid hidden_description or locked_description for %s", ach->AchievementId);
        goto achievement_error;
    }
    try
    {
        try
        {
            ach->CompletionDescription = it.value()["completion_description"].get_ref<string&>().c_str();
        }
        catch (...)
        {
            APP_LOG(Log::LogLevel::INFO, "No \"completion_description\" in achievements_db, falling back to \"flavor_text\"");
            ach->CompletionDescription = it.value()["flavor_text"].get_ref<string&>().c_str();
        }
    }
    catch (...)
    {
        APP_LOG(Log::LogLevel::ERR, "Error in achievement definition: Invalid completion_description for %s", ach->AchievementId);
        goto achievement_error;
    }
    try
    {
        ach->UnlockedIconId = it.value()["unlocked_icon_url"].get_ref<string&>().c_str();
    }
    catch (...)
    {
        APP_LOG(Log::LogLevel::ERR, "Error in achievement definition: Invalid unlocked_icon_url for %s", ach->AchievementId);
        goto achievement_error;
    }
    try
    {
        ach->LockedIconId = it.value()["locked_icon_url"].get_ref<string&>().c_str();
    }
    catch (...)
    {
        APP_LOG(Log::LogLevel::ERR, "Error in achievement definition: Invalid locked_icon_url for %s", ach->AchievementId);
        goto achievement_error;
    }
    try
    {
        ach->bIsHidden = (bool)it.value()["is_hidden"];
    }
    catch (...)
    {
        APP_LOG(Log::LogLevel::ERR, "Error in achievement definition: Invalid is_hidden for %s", ach->AchievementId);
        goto achievement_error;
    }
    try
    {
        auto& stats_thresholds = it.value()["stats_thresholds"];
        ach->StatThresholdsCount = stats_thresholds.size();
        EOS_Achievements_StatThresholds* stats = new EOS_Achievements_StatThresholds[ach->StatThresholdsCount];
        memset(stats, 0, sizeof(*stats) * ach->StatThresholdsCount);
        int i = 0;
        for (auto stat_it = stats_thresholds.begin(); stat_it != stats_thresholds.end(); ++stat_it, ++i)
        {
            stats[i].ApiVersion = EOS_ACHIEVEMENTS_STATTHRESHOLD_API_LATEST;
            try
            {
                stats[i].Name = stat_it.value()["name"].get_ref<string&>().c_str();
            }
            catch (...)
            {
                APP_LOG(Log::LogLevel::ERR, "Error in achievement definition: Invalid stats_thresholds name for %s", ach->AchievementId);
                goto achievement_error;
            }
            try
            {
                stats[i].Threshold = stat_it.value()["threshold"];
            }
            catch (...)
            {
                APP_LOG(Log::LogLevel::ERR, "Error in achievement definition: Invalid stats_thresholds[\"%s\"][\"threshold\"] for %s", stat_it.key().c_str(), ach->AchievementId);
                goto achievement_error;
            }
        }
        ach->StatThresholds = stats;
    }
    catch (...)
    {
        APP_LOG(Log::LogLevel::ERR, "Error in achievement definition: Invalid stats_thresholds for %s", ach->AchievementId);
        goto achievement_error;
    }

    *OutDefinition = ach;
    return EOS_EResult::EOS_Success;

achievement_error:
    EOS_Achievements_Definition_Release(ach);

    *OutDefinition = nullptr;
    return EOS_EResult::EOS_UnexpectedError;
}

EOS_EResult EOSSDK_Achievements::copy_definition_v2(typename decltype(m_achievements_db)::iterator it, EOS_Achievements_DefinitionV2** OutDefinition)
{
    EOS_Achievements_DefinitionV2* ach = new EOS_Achievements_DefinitionV2;
    memset(ach, 0, sizeof(*ach));
    ach->ApiVersion = EOS_ACHIEVEMENTS_DEFINITION_API_LATEST;

    try
    {
        ach->AchievementId = it.value()["achievement_id"].get_ref<string&>().c_str();
    }
    catch (...)
    {
        APP_LOG(Log::LogLevel::ERR, "Error in achievement definition: Invalid Key");
        goto achievement_error;
    }
    try
    {
        ach->UnlockedDisplayName = it.value()["unlocked_display_name"].get_ref<string&>().c_str();
    }
    catch (...)
    {
        APP_LOG(Log::LogLevel::ERR, "Error in achievement definition: Invalid unlocked_display_name for %s", it.key().c_str());
        goto achievement_error;
    }
    try
    {
        ach->UnlockedDescription = it.value()["unlocked_description"].get_ref<string&>().c_str();
    }
    catch (...)
    {
        APP_LOG(Log::LogLevel::ERR, "Error in achievement definition: Invalid unlocked_description for %s", it.key().c_str());
        goto achievement_error;
    }
    try
    {
        ach->LockedDisplayName = it.value()["locked_display_name"].get_ref<string&>().c_str();
    }
    catch (...)
    {
        APP_LOG(Log::LogLevel::ERR, "Error in achievement definition: Invalid locked_display_name for %s", it.key().c_str());
        goto achievement_error;
    }
    try
    {
        ach->LockedDescription = it.value()["locked_description"].get_ref<string&>().c_str();
    }
    catch (...)
    {
        APP_LOG(Log::LogLevel::ERR, "Error in achievement definition: Invalid locked_description for %s", it.key().c_str());
        goto achievement_error;
    }
    try
    {
        ach->FlavorText = it.value()["flavor_text"].get_ref<string&>().c_str();
    }
    catch (...)
    {
        APP_LOG(Log::LogLevel::ERR, "Error in achievement definition: Invalid flavor_text for %s", it.key().c_str());
        goto achievement_error;
    }
    try
    {
        ach->UnlockedIconURL = it.value()["unlocked_icon_url"].get_ref<string&>().c_str();
    }
    catch (...)
    {
        APP_LOG(Log::LogLevel::ERR, "Error in achievement definition: Invalid unlocked_icon_url for %s", it.key().c_str());
        goto achievement_error;
    }
    try
    {
        ach->LockedIconURL = it.value()["locked_icon_url"].get_ref<string&>().c_str();
    }
    catch (...)
    {
        APP_LOG(Log::LogLevel::ERR, "Error in achievement definition: Invalid locked_icon_url for %s", it.key().c_str());
        goto achievement_error;
    }
    try
    {
        ach->bIsHidden = (bool)it.value()["is_hidden"];
    }
    catch (...)
    {
        APP_LOG(Log::LogLevel::ERR, "Error in achievement definition: Invalid is_hidden for %s", it.key().c_str());
        goto achievement_error;
    }
    try
    {
        auto& stats_thresholds = it.value()["stats_thresholds"];
        ach->StatThresholdsCount = stats_thresholds.size();
        EOS_Achievements_StatThresholds* stats = new EOS_Achievements_StatThresholds[ach->StatThresholdsCount];
        memset(stats, 0, sizeof(*stats) * ach->StatThresholdsCount);
        int i = 0;
        for (auto stat_it = stats_thresholds.begin(); stat_it != stats_thresholds.end(); ++stat_it, ++i)
        {
            stats[i].ApiVersion = EOS_ACHIEVEMENTS_STATTHRESHOLD_API_LATEST;
            try
            {
                stats[i].Name = stat_it.value()["name"].get_ref<string&>().c_str();
            }
            catch (...)
            {
                APP_LOG(Log::LogLevel::ERR, "Error in achievement definition: Invalid stats_thresholds name for %s", it.key().c_str());
                goto achievement_error;
            }
            try
            {
                stats[i].Threshold = stat_it.value()["threshold"];
            }
            catch (...)
            {
                APP_LOG(Log::LogLevel::ERR, "Error in achievement definition: Invalid stats_thresholds[\"%s\"][\"threshold\"] for %s", stat_it.key().c_str(), it.key().c_str());
                goto achievement_error;
            }
        }
        ach->StatThresholds = stats;
    }
    catch (...)
    {
        APP_LOG(Log::LogLevel::ERR, "Error in achievement definition: Invalid stats_thresholds for %s", it.key().c_str());
        goto achievement_error;
    }

    *OutDefinition = ach;
    return EOS_EResult::EOS_Success;

achievement_error:
    EOS_Achievements_DefinitionV2_Release(ach);

    *OutDefinition = nullptr;
    return EOS_EResult::EOS_UnexpectedError;
}

EOS_EResult EOSSDK_Achievements::copy_unlocked_achievement(typename decltype(m_unlocked_achievements)::iterator it, EOS_Achievements_UnlockedAchievement** OutAchievement)
{
    EOS_Achievements_UnlockedAchievement* ach = new EOS_Achievements_UnlockedAchievement;
    ach->ApiVersion = EOS_ACHIEVEMENTS_UNLOCKACHIEVEMENTS_API_LATEST;

    try
    {
        ach->AchievementId = (*it->second)["achievement_id"].get_ref<string&>().c_str();
    }
    catch (...)
    {
        APP_LOG(Log::LogLevel::ERR, "Error in unlocked achievement definition: Invalid achievement_id");
        goto achievement_error;
    }
    
    try
    {
        ach->UnlockTime = (*it->second)["unlock_time"].get<int64_t>();
    }
    catch (...)
    {
        APP_LOG(Log::LogLevel::ERR, "Error in unlocked achievement definition: Invalid unlock time");
        goto achievement_error;
    }

    *OutAchievement = ach;
    return EOS_EResult::EOS_Success;

achievement_error:
    delete ach;
    *OutAchievement = nullptr;
    return EOS_EResult::EOS_UnexpectedError;
}

EOS_EResult EOSSDK_Achievements::copy_player_achievement(typename decltype(m_achievements)::iterator it, EOS_Achievements_PlayerAchievement** OutAchievement)
{
    EOS_Achievements_PlayerAchievement* ach = new EOS_Achievements_PlayerAchievement;
    memset(ach, 0, sizeof(*ach));

    ach->ApiVersion = EOS_ACHIEVEMENTS_PLAYERACHIEVEMENT_API_LATEST;

    try
    {
        ach->AchievementId = it.value()["achievement_id"].get_ref<string&>().c_str();
    }
    catch (...)
    {
        APP_LOG(Log::LogLevel::ERR, "Error in player achievement: Invalid achievement_id");
        goto achievement_error;
    }
    try
    {
        ach->DisplayName = it.value()["display_name"].get_ref<string&>().c_str();
    }
    catch (...)
    {
        APP_LOG(Log::LogLevel::ERR, "Error in player achievement: Invalid display_name for %s", it.key().c_str());
        goto achievement_error;
    }
    try
    {
        ach->Description = it.value()["description"].get_ref<string&>().c_str();
    }
    catch (...)
    {
        APP_LOG(Log::LogLevel::ERR, "Error in player achievement: Invalid description for %s", it.key().c_str());
        goto achievement_error;
    }
    try
    {
        ach->IconURL = it.value()["icon_url"].get_ref<string&>().c_str();
    }
    catch (...)
    {
        APP_LOG(Log::LogLevel::ERR, "Error in player achievement: Invalid icon_url for %s", it.key().c_str());
        goto achievement_error;
    }
    try
    {
        ach->FlavorText = it.value()["flavor_text"].get_ref<string&>().c_str();
    }
    catch (...)
    {
        APP_LOG(Log::LogLevel::ERR, "Error in player achievement: Invalid flavor_text for %s", it.key().c_str());
        goto achievement_error;
    }
    try
    {
        ach->Progress = it.value()["progress"];
    }
    catch (...)
    {
        APP_LOG(Log::LogLevel::ERR, "Error in player achievement: Invalid progress for %s", it.key().c_str());
        goto achievement_error;
    }
    try
    {
        ach->UnlockTime = it.value()["unlock_time"];
    }
    catch (...)
    {
        APP_LOG(Log::LogLevel::ERR, "Error in player achievement: Invalid unlock_time for %s", it.key().c_str());
        goto achievement_error;
    }
    try
    {
        auto& stat_info = it.value()["stat_info"];
        ach->StatInfoCount = stat_info.size();
        EOS_Achievements_PlayerStatInfo* stats = new EOS_Achievements_PlayerStatInfo[ach->StatInfoCount];
        memset(stats, 0, sizeof(*stats) * ach->StatInfoCount);
        int i = 0;
        for (auto stat_it = stat_info.begin(); stat_it != stat_info.end(); ++stat_it, ++i)
        {
            stats[i].ApiVersion = EOS_ACHIEVEMENTS_PLAYERSTATINFO_API_LATEST;
            try
            {
                stats[i].Name = stat_it.value()["name"].get_ref<string&>().c_str();
            }
            catch (...)
            {
                APP_LOG(Log::LogLevel::ERR, "Error in player achievement: Invalid stat_info name for %s", it.key().c_str());
                goto achievement_error;
            }
            try
            {
                stats[i].CurrentValue = stat_it.value()["current_value"];
            }
            catch (...)
            {
                APP_LOG(Log::LogLevel::ERR, "Error in player achievement: Invalid stat_info[\"%s\"][\"current_value\"] for %s", stats[i].Name, it.key().c_str());
                goto achievement_error;
            }
            try
            {
                stats[i].ThresholdValue = stat_it.value()["threshold_value"];
            }
            catch (...)
            {
                APP_LOG(Log::LogLevel::ERR, "Error in player achievement: Invalid stat_info[\"%s\"][\"threshold_value\"] for %s", stats[i].Name, it.key().c_str());
                goto achievement_error;
            }
        }
        ach->StatInfo = stats;
    }
    catch (...)
    {
        APP_LOG(Log::LogLevel::ERR, "Error in achievement definition: Invalid stats_thresholds for %s", it.key().c_str());
        goto achievement_error;
    }

    *OutAchievement = ach;
    return EOS_EResult::EOS_Success;

achievement_error:
    EOS_Achievements_PlayerAchievement_Release(ach);

    *OutAchievement = nullptr;
    return EOS_EResult::EOS_UnexpectedError;
}

void EOSSDK_Achievements::QueryDefinitions(const EOS_Achievements_QueryDefinitionsOptions* Options, void* ClientData, const EOS_Achievements_OnQueryDefinitionsCompleteCallback CompletionDelegate)
{
    TRACE_FUNC();

    SE_CB_BEGIN(EOS_Achievements_OnQueryDefinitionsCompleteCallbackInfo, CompletionDelegate);

    if (Options == nullptr)
    {
        info.ResultCode = EOS_EResult::EOS_InvalidParameters;
    }
    else
    {
        info.ResultCode = EOS_EResult::EOS_Success;
    }

    SE_CB_END(this);
}

uint32_t EOSSDK_Achievements::GetAchievementDefinitionCount(const EOS_Achievements_GetAchievementDefinitionCountOptions* Options)
{
    TRACE_FUNC();

    return m_achievements_db.size();
}

EOS_EResult EOSSDK_Achievements::CopyAchievementDefinitionV2ByIndex(const EOS_Achievements_CopyAchievementDefinitionV2ByIndexOptions* Options, EOS_Achievements_DefinitionV2** OutDefinition)
{
    TRACE_FUNC();

    if (Options == nullptr || Options->AchievementIndex >= m_achievements_db.size())
    {
        *OutDefinition = nullptr;
        return EOS_EResult::EOS_InvalidParameters;
    }

    auto it = m_achievements_db.begin();
    std::advance(it, Options->AchievementIndex);

    return copy_definition_v2(it, OutDefinition);
}

EOS_EResult EOSSDK_Achievements::CopyAchievementDefinitionV2ByAchievementId(const EOS_Achievements_CopyAchievementDefinitionV2ByAchievementIdOptions* Options, EOS_Achievements_DefinitionV2** OutDefinition)
{
    TRACE_FUNC();

    if (Options == nullptr || Options->AchievementId == nullptr)
    {
        *OutDefinition = nullptr;
        return EOS_EResult::EOS_InvalidParameters;
    }

    auto it = m_achievements_db.find(Options->AchievementId);
    if (it == m_achievements_db.end())
    {
        *OutDefinition = nullptr;
        return EOS_EResult::EOS_NotFound;
    }

    return copy_definition_v2(it, OutDefinition);
}

void EOSSDK_Achievements::QueryPlayerAchievements(const EOS_Achievements_QueryPlayerAchievementsOptions* Options, void* ClientData, const EOS_Achievements_OnQueryPlayerAchievementsCompleteCallback CompletionDelegate)
{
    TRACE_FUNC();
    lock _(GLOBAL_MUTEX);

    SE_CB_BEGIN(EOS_Achievements_OnQueryPlayerAchievementsCompleteCallbackInfo, CompletionDelegate);
    info.LocalUserId = Options->LocalUserId;

    if (Options == nullptr || Options->TargetUserId == nullptr || Options->TargetUserId != GetEOS_Connect().get_myself()->first)
    {
        info.ResultCode = EOS_EResult::EOS_InvalidParameters;
    }
    else
    {
        info.ResultCode = EOS_EResult::EOS_Success;
        info.TargetUserId = Options->TargetUserId;
        info.LocalUserId = Options->LocalUserId;
    }

    SE_CB_END(this);
}

uint32_t EOSSDK_Achievements::GetPlayerAchievementCount(const EOS_Achievements_GetPlayerAchievementCountOptions* Options)
{
    TRACE_FUNC();
    lock _(GLOBAL_MUTEX);

    if (Options == nullptr || Options->UserId == nullptr || Options->UserId != GetEOS_Connect().get_myself()->first)
        return 0;

    return m_achievements.size();
}

EOS_EResult EOSSDK_Achievements::CopyPlayerAchievementByIndex(const EOS_Achievements_CopyPlayerAchievementByIndexOptions* Options, EOS_Achievements_PlayerAchievement** OutAchievement)
{
    TRACE_FUNC();
    lock _(GLOBAL_MUTEX);

    if (Options == nullptr || Options->TargetUserId == nullptr || Options->AchievementIndex >= m_achievements.size() || Options->TargetUserId != GetEOS_Connect().get_myself()->first)
    {
        *OutAchievement = nullptr;
        return EOS_EResult::EOS_InvalidParameters;
    }

    auto it = m_achievements.begin();
    std::advance(it, Options->AchievementIndex);

    return copy_player_achievement(it, OutAchievement);
}

EOS_EResult EOSSDK_Achievements::CopyPlayerAchievementByAchievementId(const EOS_Achievements_CopyPlayerAchievementByAchievementIdOptions* Options, EOS_Achievements_PlayerAchievement** OutAchievement)
{
    TRACE_FUNC();
    lock _(GLOBAL_MUTEX);

    if (Options == nullptr || Options->TargetUserId == nullptr || Options->TargetUserId != GetEOS_Connect().get_myself()->first)
    {
        *OutAchievement = nullptr;
        return EOS_EResult::EOS_InvalidParameters;
    }

    auto it = m_achievements.find(Options->AchievementId);
    if (it == m_achievements.end())
    {
        *OutAchievement = nullptr;
        return EOS_EResult::EOS_NotFound;
    }

    return copy_player_achievement(it, OutAchievement);
}

void EOSSDK_Achievements::UnlockAchievements(const EOS_Achievements_UnlockAchievementsOptions* Options, void* ClientData, const EOS_Achievements_OnUnlockAchievementsCompleteCallback CompletionDelegate)
{
    TRACE_FUNC();
    lock _(GLOBAL_MUTEX);

    SE_CB_BEGIN(EOS_Achievements_OnUnlockAchievementsCompleteCallbackInfo, CompletionDelegate);
    info.UserId = Options->UserId;

    if (Options == nullptr || Options->UserId == nullptr || (Options->AchievementsCount && Options->AchievementIds == nullptr))
    {
        info.AchievementsCount = 0;
        info.ResultCode = EOS_EResult::EOS_InvalidParameters;
    }
    else if (Options->UserId != GetEOS_Connect().get_myself()->first)
    {
        info.AchievementsCount = 0;
        info.ResultCode = EOS_EResult::EOS_UnexpectedError;
    }
    else
    {
        for (int i = 0; i < Options->AchievementsCount; ++i)
        {
            auto it_db = m_achievements_db.find(Options->AchievementIds[i]);
            if (it_db != m_achievements_db.end())
            {
                m_achievements[Options->AchievementIds[i]]["unlock_time"] = static_cast<int64_t>(clock::now().time_since_epoch().count());

                string title = it_db.value().value("unlocked_display_name", Options->AchievementIds[i]);
                string desc = it_db.value().value("unlocked_description", "Achievement Unlocked!");
                OverlayManager::Inst().PushNotification(title, desc, "", true);
            }

        }

        FileManager::save_json(achievements_filename, m_achievements);

        info.AchievementsCount = Options->AchievementsCount;
        info.ResultCode = EOS_EResult::EOS_Success;
    }

    SE_CB_END(this);
}

uint32_t EOSSDK_Achievements::GetUnlockedAchievementCount(const EOS_Achievements_GetUnlockedAchievementCountOptions* Options)
{
    TRACE_FUNC();
    lock _(GLOBAL_MUTEX);

    if (Options == nullptr || Options->UserId != GetEOS_Connect().get_myself()->first)
        return 0;

    m_unlocked_achievements.clear();
    for (auto it = m_achievements.begin(); it != m_achievements.end(); ++it)
    {
        try
        {
            if (it.value()["unlock_time"] != EOS_ACHIEVEMENTS_ACHIEVEMENT_UNLOCKTIME_UNDEFINED)
            {
                m_unlocked_achievements[it.key()] = &it.value();
            }
        }
        catch (...)
        {}
    }
    
    return m_unlocked_achievements.size();
}

EOS_EResult EOSSDK_Achievements::CopyUnlockedAchievementByIndex(const EOS_Achievements_CopyUnlockedAchievementByIndexOptions* Options, EOS_Achievements_UnlockedAchievement** OutAchievement)
{
    TRACE_FUNC();
    lock _(GLOBAL_MUTEX);

    if (Options == nullptr || Options->UserId != GetEOS_Connect().get_myself()->first || Options->AchievementIndex >= m_unlocked_achievements.size())
    {
        *OutAchievement = nullptr;
        return EOS_EResult::EOS_InvalidParameters;
    }

    auto it = m_unlocked_achievements.begin();
    std::advance(it, Options->AchievementIndex);
    
    return copy_unlocked_achievement(it, OutAchievement);
}

EOS_EResult EOSSDK_Achievements::CopyUnlockedAchievementByAchievementId(const EOS_Achievements_CopyUnlockedAchievementByAchievementIdOptions* Options, EOS_Achievements_UnlockedAchievement** OutAchievement)
{
    TRACE_FUNC();
    lock _(GLOBAL_MUTEX);

    if (Options == nullptr || Options->UserId != GetEOS_Connect().get_myself()->first || Options->AchievementId == nullptr)
    {
        *OutAchievement = nullptr;
        return EOS_EResult::EOS_InvalidParameters;
    }

    auto it = m_unlocked_achievements.find(Options->AchievementId);

    return copy_unlocked_achievement(it, OutAchievement);
}

EOS_NotificationId EOSSDK_Achievements::AddNotifyAchievementsUnlocked(const EOS_Achievements_AddNotifyAchievementsUnlockedOptions* Options, void* ClientData, const EOS_Achievements_OnAchievementsUnlockedCallback NotificationFn)
{
    TRACE_FUNC();

    if (Options == nullptr || NotificationFn == nullptr)
        return EOS_INVALID_NOTIFICATIONID;

    pFrameResult_t res(new FrameResult);
    EOS_Achievements_OnAchievementsUnlockedCallbackInfo& oauci = res->CreateCallback<EOS_Achievements_OnAchievementsUnlockedCallbackInfo>(CallbackFunc(NotificationFn));
    oauci.ClientData = ClientData;
    oauci.UserId = GetEOS_Connect().get_myself()->first;
    oauci.AchievementIds = nullptr;
    oauci.AchievementsCount = 0;

    return GetCB_Manager().add_notification(this, res);
}

EOS_NotificationId EOSSDK_Achievements::AddNotifyAchievementsUnlockedV2(const EOS_Achievements_AddNotifyAchievementsUnlockedV2Options* Options, void* ClientData, const EOS_Achievements_OnAchievementsUnlockedCallbackV2 NotificationFn)
{
    TRACE_FUNC();

    if (Options == nullptr || NotificationFn == nullptr)
        return EOS_INVALID_NOTIFICATIONID;

    pFrameResult_t res(new FrameResult);
    EOS_Achievements_OnAchievementsUnlockedCallbackV2Info& oauci = res->CreateCallback<EOS_Achievements_OnAchievementsUnlockedCallbackV2Info>(CallbackFunc(NotificationFn));
    oauci.ClientData = ClientData;
    oauci.UserId = GetEOS_Connect().get_myself()->first;
    oauci.UnlockTime = 0;
    oauci.AchievementId = nullptr;

    return GetCB_Manager().add_notification(this, res);
}

void EOSSDK_Achievements::RemoveNotifyAchievementsUnlocked(EOS_NotificationId InId)
{
    TRACE_FUNC();

    GetCB_Manager().remove_notification(this, InId);
}

EOS_EResult EOSSDK_Achievements::CopyAchievementDefinitionByIndex(const EOS_Achievements_CopyAchievementDefinitionByIndexOptions* Options, EOS_Achievements_Definition** OutDefinition)
{
    TRACE_FUNC();

    if (Options == nullptr || Options->AchievementIndex >= m_achievements_db.size())
    {
        *OutDefinition = nullptr;
        return EOS_EResult::EOS_InvalidParameters;
    }

    auto it = m_achievements_db.begin();
    std::advance(it, Options->AchievementIndex);

    return copy_definition(it, OutDefinition);
}

EOS_EResult EOSSDK_Achievements::CopyAchievementDefinitionByAchievementId(const EOS_Achievements_CopyAchievementDefinitionByAchievementIdOptions* Options, EOS_Achievements_Definition** OutDefinition)
{
    TRACE_FUNC();

    if (Options == nullptr || Options->AchievementId == nullptr)
    {
        *OutDefinition = nullptr;
        return EOS_EResult::EOS_InvalidParameters;
    }

    auto it = m_achievements_db.find(Options->AchievementId);
    if (it == m_achievements_db.end())
    {
        *OutDefinition = nullptr;
        return EOS_EResult::EOS_NotFound;
    }

    return copy_definition(it, OutDefinition);
}

void EOSSDK_Achievements::check_stat_thresholds(const string& stat_name, int32_t new_value)
{
    lock _(GLOBAL_MUTEX);

    m_stats_cache[stat_name] = new_value;

    for (auto& [ach_id, ach_def] : m_achievements_db.items())
    {
        
        if (m_achievements.contains(ach_id))
        {
            int64_t unlock_time = m_achievements[ach_id].value("unlock_time",
                static_cast<int64_t>(EOS_ACHIEVEMENTS_ACHIEVEMENT_UNLOCKTIME_UNDEFINED));
            if (unlock_time != static_cast<int64_t>(EOS_ACHIEVEMENTS_ACHIEVEMENT_UNLOCKTIME_UNDEFINED))
                continue;
        }

        if (!ach_def.contains("stats_thresholds")) continue;

        bool all_met = true;
        for (auto& [s_name, threshold_obj] : ach_def["stats_thresholds"].items())
        {
            int32_t threshold = threshold_obj.is_number()
                ? threshold_obj.get<int32_t>()
                : threshold_obj.value("threshold", INT32_MAX);

            int32_t cur = 0;
            if (s_name == stat_name)
                cur = new_value;
            else if (m_stats_cache.contains(s_name) && m_stats_cache[s_name].is_number())
                cur = m_stats_cache[s_name].get<int32_t>();

            if (cur < threshold) { all_met = false; break; }
        }

        if (all_met)
        {
            auto now_ms = static_cast<int64_t>(
                std::chrono::duration_cast<std::chrono::milliseconds>(
                    std::chrono::system_clock::now().time_since_epoch()).count());

            m_achievements[ach_id]["unlock_time"] = now_ms;
            m_unlocked_achievements[ach_id] = &m_achievements[ach_id];

            string title = ach_def.value("unlocked_display_name", ach_id);
            string desc  = ach_def.value("unlocked_description", "Achievement Unlocked!");
            OverlayManager::Inst().PushNotification(title, desc, "", true);

            APP_LOG(Log::LogLevel::INFO,
                "Auto-unlocked achievement [%s] after stat %s=%d",
                ach_id.c_str(), stat_name.c_str(), new_value);

            FileManager::save_json(achievements_filename, m_achievements);
        }
    }
}

bool EOSSDK_Achievements::CBRunFrame()
{
    return false;
}

bool EOSSDK_Achievements::RunCallbacks(pFrameResult_t res)
{
    return false;
}

void EOSSDK_Achievements::FreeCallback(pFrameResult_t res)
{
    lock _(GLOBAL_MUTEX);

    switch (res->ICallback())
    {

        case EOS_Achievements_OnAchievementsUnlockedCallbackInfo::k_iCallback:
        {
            EOS_Achievements_OnAchievementsUnlockedCallbackInfo& callback = res->GetCallback<EOS_Achievements_OnAchievementsUnlockedCallbackInfo>();

            delete[]callback.AchievementIds;
        }

        case EOS_Achievements_OnAchievementsUnlockedCallbackV2Info::k_iCallback:
        {
            EOS_Achievements_OnAchievementsUnlockedCallbackV2Info& callback = res->GetCallback<EOS_Achievements_OnAchievementsUnlockedCallbackV2Info>();

        }
        break;
    }
}

}