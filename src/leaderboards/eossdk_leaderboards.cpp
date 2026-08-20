#include "eossdk_leaderboards.h"
#include "eossdk_platform.h"

namespace sdk
{

EOSSDK_Leaderboards::EOSSDK_Leaderboards()
{
    m_scores_filename = FileManager::join(Settings::Inst().save_path, "leaderboards.json");
    FileManager::load_json(m_scores_filename, m_scores);
}

EOSSDK_Leaderboards::~EOSSDK_Leaderboards()
{
    FileManager::save_json(m_scores_filename, m_scores);
}

void EOSSDK_Leaderboards::QueryLeaderboardDefinitions(const EOS_Leaderboards_QueryLeaderboardDefinitionsOptions* Options, void* ClientData, const EOS_Leaderboards_OnQueryLeaderboardDefinitionsCompleteCallback CompletionDelegate)
{
    TRACE_FUNC();

    SE_CB_BEGIN(EOS_Leaderboards_OnQueryLeaderboardDefinitionsCompleteCallbackInfo, CompletionDelegate);

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

uint32_t EOSSDK_Leaderboards::GetLeaderboardDefinitionCount(const EOS_Leaderboards_GetLeaderboardDefinitionCountOptions* Options)
{
    TRACE_FUNC();

    if (Options == nullptr)
        return 0;

    return 0;
}

EOS_EResult EOSSDK_Leaderboards::CopyLeaderboardDefinitionByIndex(const EOS_Leaderboards_CopyLeaderboardDefinitionByIndexOptions* Options, EOS_Leaderboards_Definition** OutLeaderboardDefinition)
{
    TRACE_FUNC();

    if (Options == nullptr || OutLeaderboardDefinition == nullptr)
    {
        set_nullptr(OutLeaderboardDefinition);
        return EOS_EResult::EOS_InvalidParameters;
    }

    OutLeaderboardDefinition = nullptr;
    return EOS_EResult::EOS_NotFound;
}

EOS_EResult EOSSDK_Leaderboards::CopyLeaderboardDefinitionByLeaderboardId(const EOS_Leaderboards_CopyLeaderboardDefinitionByLeaderboardIdOptions* Options, EOS_Leaderboards_Definition** OutLeaderboardDefinition)
{
    TRACE_FUNC();

    if (Options == nullptr || Options->LeaderboardId == nullptr || OutLeaderboardDefinition == nullptr)
    {
        set_nullptr(OutLeaderboardDefinition);
        return EOS_EResult::EOS_InvalidParameters;
    }

    OutLeaderboardDefinition = nullptr;
    return EOS_EResult::EOS_NotFound;
}

void EOSSDK_Leaderboards::QueryLeaderboardRanks(const EOS_Leaderboards_QueryLeaderboardRanksOptions* Options, void* ClientData, const EOS_Leaderboards_OnQueryLeaderboardRanksCompleteCallback CompletionDelegate)
{
    TRACE_FUNC();

    SE_CB_BEGIN(EOS_Leaderboards_OnQueryLeaderboardRanksCompleteCallbackInfo, CompletionDelegate);

    if (Options == nullptr || Options->LeaderboardId == nullptr)
    {
        info.ResultCode = EOS_EResult::EOS_InvalidParameters;
    }
    else
    {
        
        string lb_id = Options->LeaderboardId;
        string uid   = Settings::Inst().userid->to_string();
        if (!m_scores.contains(lb_id) || !m_scores[lb_id].contains(uid))
        {
            
            fifo_json stats_data;
            string stats_file = FileManager::join(Settings::Inst().save_path, "stats.json");
            FileManager::load_json(stats_file, stats_data);
            int32_t score = 0;
            if (stats_data.contains(lb_id) && stats_data[lb_id].is_number())
                score = stats_data[lb_id].get<int32_t>();
            m_scores[lb_id][uid] = score;
            FileManager::save_json(m_scores_filename, m_scores);
        }
        m_active_leaderboard_id = lb_id;
        info.ResultCode = EOS_EResult::EOS_Success;
    }

    SE_CB_END(this);
}

uint32_t EOSSDK_Leaderboards::GetLeaderboardRecordCount(const EOS_Leaderboards_GetLeaderboardRecordCountOptions* Options)
{
    TRACE_FUNC();

    if (Options == nullptr)
    {
        return 0;
    }

    return 0;
}

EOS_EResult EOSSDK_Leaderboards::CopyLeaderboardRecordByIndex(const EOS_Leaderboards_CopyLeaderboardRecordByIndexOptions* Options, EOS_Leaderboards_LeaderboardRecord** OutLeaderboardRecord)
{
    TRACE_FUNC();

    if (Options == nullptr || OutLeaderboardRecord == nullptr)
    {
        set_nullptr(OutLeaderboardRecord);
        return EOS_EResult::EOS_InvalidParameters;
    }

    OutLeaderboardRecord = nullptr;
    return EOS_EResult::EOS_NotFound;
}

EOS_EResult EOSSDK_Leaderboards::CopyLeaderboardRecordByUserId(const EOS_Leaderboards_CopyLeaderboardRecordByUserIdOptions* Options, EOS_Leaderboards_LeaderboardRecord** OutLeaderboardRecord)
{
    TRACE_FUNC();

    if (Options == nullptr || Options->UserId == nullptr || OutLeaderboardRecord == nullptr)
    {
        set_nullptr(OutLeaderboardRecord);
        return EOS_EResult::EOS_InvalidParameters;
    }

    OutLeaderboardRecord = nullptr;
    return EOS_EResult::EOS_NotFound;
}

void EOSSDK_Leaderboards::QueryLeaderboardUserScores(const EOS_Leaderboards_QueryLeaderboardUserScoresOptions* Options, void* ClientData, const EOS_Leaderboards_OnQueryLeaderboardUserScoresCompleteCallback CompletionDelegate)
{
    TRACE_FUNC();

    SE_CB_BEGIN(EOS_Leaderboards_OnQueryLeaderboardUserScoresCompleteCallbackInfo, CompletionDelegate);

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

uint32_t EOSSDK_Leaderboards::GetLeaderboardUserScoreCount(const EOS_Leaderboards_GetLeaderboardUserScoreCountOptions* Options)
{
    TRACE_FUNC();

    if (Options == nullptr)
        return 0;

    return 0;
}

EOS_EResult EOSSDK_Leaderboards::CopyLeaderboardUserScoreByIndex(const EOS_Leaderboards_CopyLeaderboardUserScoreByIndexOptions* Options, EOS_Leaderboards_LeaderboardUserScore** OutLeaderboardUserScore)
{
    TRACE_FUNC();

    if (Options == nullptr || OutLeaderboardUserScore == nullptr)
    {
        set_nullptr(OutLeaderboardUserScore);
        return EOS_EResult::EOS_InvalidParameters;
    }

    OutLeaderboardUserScore = nullptr;
    return EOS_EResult::EOS_NotFound;
}

EOS_EResult EOSSDK_Leaderboards::CopyLeaderboardUserScoreByUserId(const EOS_Leaderboards_CopyLeaderboardUserScoreByUserIdOptions* Options, EOS_Leaderboards_LeaderboardUserScore** OutLeaderboardUserScore)
{
    TRACE_FUNC();

    if (Options == nullptr || Options->StatName == nullptr || OutLeaderboardUserScore == nullptr)
    {
        set_nullptr(OutLeaderboardUserScore);
        return EOS_EResult::EOS_InvalidParameters;
    }

    OutLeaderboardUserScore = nullptr;
    return EOS_EResult::EOS_NotFound;
}

bool EOSSDK_Leaderboards::CBRunFrame()
{
    return false;
}

bool EOSSDK_Leaderboards::RunCallbacks(pFrameResult_t res)
{
    lock _(GLOBAL_MUTEX);

    return res->done;
}

void EOSSDK_Leaderboards::FreeCallback(pFrameResult_t res)
{
    lock _(GLOBAL_MUTEX);

    {

    }
}

}
