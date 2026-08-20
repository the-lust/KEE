#include "eossdk_stats.h"
#include "eossdk_platform.h"
#include "achievements/eossdk_achievements.h"

namespace sdk
{

EOSSDK_Stats::EOSSDK_Stats()
{
    string kee_settings = Settings::Inst().kee_settings_path;
    string saves = Settings::Inst().save_path;

    stats_filename = FileManager::join(saves, "stats.json");

    FileManager::load_json(stats_filename, m_stats);

    string defs_file = FileManager::join(kee_settings, "achievements_db.json");
    if (std::filesystem::exists(defs_file))
    {
        fifo_json db;
        FileManager::load_json(defs_file, db);
        if (db.contains("stats"))
            m_stats_defs = db["stats"];
    }
    else
    {
        defs_file = FileManager::join(kee_settings, "stats.json");
        if (std::filesystem::exists(defs_file))
            FileManager::load_json(defs_file, m_stats_defs);
    }

    if (m_stats.empty())
    {
        string gbe_stats = "steam_settings/stats.json";
        if (std::filesystem::exists(gbe_stats))
        {
            nlohmann::json gbe_data;
            FileManager::load_json(gbe_stats, gbe_data);
            
            for (auto& [key, val] : gbe_data.items())
            {
                if (!m_stats.contains(key))
                {
                    int32_t def = 0;
                    if (val.contains("default") && val["default"].is_number())
                        def = val["default"].get<int32_t>();
                    m_stats[key] = def;
                }
                
                if (val.contains("aggregation") && !m_stats_defs.contains(key))
                    m_stats_defs[key]["aggregation"] = val["aggregation"];
            }
        }
    }
}

EOSSDK_Stats::~EOSSDK_Stats()
{}

void EOSSDK_Stats::save_stats()
{
    FileManager::save_json(stats_filename, m_stats);
}

void EOSSDK_Stats::IngestStat(const EOS_Stats_IngestStatOptions* Options, void* ClientData, const EOS_Stats_OnIngestStatCompleteCallback CompletionDelegate)
{
    TRACE_FUNC();

    SE_CB_BEGIN(EOS_Stats_IngestStatCompleteCallbackInfo, CompletionDelegate);
    if (Options == nullptr)
    {
        info.ResultCode = EOS_EResult::EOS_InvalidParameters;
    }
    else
    {
        info.TargetUserId = Options->TargetUserId;
        info.LocalUserId = Options->LocalUserId;

        if (Options->StatsCount > EOS_STATS_MAX_INGEST_STATS || Options->Stats == nullptr)
        {
            info.ResultCode = EOS_EResult::EOS_InvalidParameters;
        }
        else
        {
            for (uint32_t i = 0; i < Options->StatsCount; ++i)
            {
                auto& stat = Options->Stats[i];
                if (!stat.StatName) continue;

                const string name = stat.StatName;
                const int32_t amount   = stat.IngestAmount;

                string agg = "SUM";
                if (m_stats_defs.contains(name) && m_stats_defs[name].contains("aggregation"))
                    agg = m_stats_defs[name]["aggregation"].get<string>();

                int32_t current = 0;
                if (m_stats.contains(name) && m_stats[name].is_number())
                    current = m_stats[name].get<int32_t>();

                int32_t new_val = current;
                if      (agg == "SUM"     || agg == "ADD") new_val = current + amount;
                else if (agg == "MAX"                     ) new_val = std::max(current, amount);
                else if (agg == "MIN"                     ) new_val = std::min(current, amount);
                else if (agg == "REPLACE" || agg == "SET" ) new_val = amount;
                else                                        new_val = current + amount; 

                m_stats[name] = new_val;
                
                GetEOS_Achievements().check_stat_thresholds(name, new_val);
            }

            save_stats();
            info.ResultCode = EOS_EResult::EOS_Success;
        }
    }

    SE_CB_END(this);
}

void EOSSDK_Stats::QueryStats(const EOS_Stats_QueryStatsOptions* Options, void* ClientData, const EOS_Stats_OnQueryStatsCompleteCallback CompletionDelegate)
{
    TRACE_FUNC();

    SE_CB_BEGIN(EOS_Stats_OnQueryStatsCompleteCallbackInfo, CompletionDelegate);
    if (Options == nullptr)
    {
        info.ResultCode = EOS_EResult::EOS_InvalidParameters;
    }
    else
    {
        info.ResultCode = EOS_EResult::EOS_Success;
    }
    info.LocalUserId = Options->LocalUserId;
    info.TargetUserId = Options->TargetUserId;

    SE_CB_END(this);
}

uint32_t EOSSDK_Stats::GetStatsCount(const EOS_Stats_GetStatCountOptions* Options)
{
    TRACE_FUNC();

    if (Options == nullptr)
        return 0;

    return m_stats.size();
}

EOS_EResult EOSSDK_Stats::CopyStatByIndex(const EOS_Stats_CopyStatByIndexOptions* Options, EOS_Stats_Stat** OutStat)
{
    TRACE_FUNC();

    if (Options == nullptr || Options->StatIndex > m_stats.size() || OutStat == nullptr)
    {
        set_nullptr(OutStat);
        return EOS_EResult::EOS_InvalidParameters;
    }

    auto it = m_stats.begin();
    std::advance(it, Options->StatIndex);

    EOS_Stats_Stat* stat = new EOS_Stats_Stat();
    stat->ApiVersion = EOS_STATS_STAT_API_LATEST;
    stat->StartTime = EOS_STATS_TIME_UNDEFINED;
    stat->EndTime = EOS_STATS_TIME_UNDEFINED;
    {
        size_t len = it.key().length() + 1;
        char* str = new char[len];
        strncpy(str, it.key().c_str(), len);
        stat->Name = str;
    }
    try
    {
        stat->Value = it.value();
    }
    catch(...)
    {
        stat->Value = 0;
        it.value() = 0;
    }

    *OutStat = stat;
    return EOS_EResult::EOS_Success;
}

EOS_EResult EOSSDK_Stats::CopyStatByName(const EOS_Stats_CopyStatByNameOptions* Options, EOS_Stats_Stat** OutStat)
{
    TRACE_FUNC();

    if (Options == nullptr || Options->Name == nullptr || OutStat == nullptr)
    {
        set_nullptr(OutStat);
        return EOS_EResult::EOS_InvalidParameters;
    }

    auto it = m_stats.find(Options->Name);
    if (it == m_stats.end())
    {
        set_nullptr(OutStat);
        return EOS_EResult::EOS_InvalidParameters;
    }

    EOS_Stats_Stat* stat = new EOS_Stats_Stat();
    stat->ApiVersion = EOS_STATS_STAT_API_LATEST;
    stat->StartTime = EOS_STATS_TIME_UNDEFINED;
    stat->EndTime = EOS_STATS_TIME_UNDEFINED;
    {
        size_t len = it.key().length() + 1;
        char* str = new char[len];
        strncpy(str, it.key().c_str(), len);
        stat->Name = str;
    }
    try
    {
        stat->Value = it.value();
    }
    catch (...)
    {
        stat->Value = 0;
        it.value() = 0;
    }

    *OutStat = stat;
    return EOS_EResult::EOS_Success;
}

bool EOSSDK_Stats::CBRunFrame()
{
    return false;
}

bool EOSSDK_Stats::RunCallbacks(pFrameResult_t res)
{
    lock _(GLOBAL_MUTEX);

    return res->done;
}

void EOSSDK_Stats::FreeCallback(pFrameResult_t res)
{
    lock _(GLOBAL_MUTEX);

    {

    }
}

}