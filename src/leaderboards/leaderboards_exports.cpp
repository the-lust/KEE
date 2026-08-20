#include "leaderboards/eossdk_leaderboards.h"

using namespace sdk;

extern "C"
{
    EOS_DECLARE_FUNC(void) EOS_Leaderboards_QueryLeaderboardDefinitions(EOS_HLeaderboards Handle, const EOS_Leaderboards_QueryLeaderboardDefinitionsOptions* Options, void* ClientData, const EOS_Leaderboards_OnQueryLeaderboardDefinitionsCompleteCallback CompletionDelegate)
    {
        if (Handle == nullptr)
            return;

        auto pInst = reinterpret_cast<EOSSDK_Leaderboards*>(Handle);
        pInst->QueryLeaderboardDefinitions(Options, ClientData, CompletionDelegate);
    }

    EOS_DECLARE_FUNC(uint32_t) EOS_Leaderboards_GetLeaderboardDefinitionCount(EOS_HLeaderboards Handle, const EOS_Leaderboards_GetLeaderboardDefinitionCountOptions* Options)
    {
        if (Handle == nullptr)
            return 0;

        auto pInst = reinterpret_cast<EOSSDK_Leaderboards*>(Handle);
        return pInst->GetLeaderboardDefinitionCount(Options);
    }

    EOS_DECLARE_FUNC(EOS_EResult) EOS_Leaderboards_CopyLeaderboardDefinitionByIndex(EOS_HLeaderboards Handle, const EOS_Leaderboards_CopyLeaderboardDefinitionByIndexOptions* Options, EOS_Leaderboards_Definition** OutLeaderboardDefinition)
    {
        if (Handle == nullptr)
            return EOS_EResult::EOS_InvalidParameters;

        auto pInst = reinterpret_cast<EOSSDK_Leaderboards*>(Handle);
        return pInst->CopyLeaderboardDefinitionByIndex(Options, OutLeaderboardDefinition);
    }

    EOS_DECLARE_FUNC(EOS_EResult) EOS_Leaderboards_CopyLeaderboardDefinitionByLeaderboardId(EOS_HLeaderboards Handle, const EOS_Leaderboards_CopyLeaderboardDefinitionByLeaderboardIdOptions* Options, EOS_Leaderboards_Definition** OutLeaderboardDefinition)
    {
        if (Handle == nullptr)
            return EOS_EResult::EOS_InvalidParameters;

        auto pInst = reinterpret_cast<EOSSDK_Leaderboards*>(Handle);
        return pInst->CopyLeaderboardDefinitionByLeaderboardId(Options, OutLeaderboardDefinition);
    }

    EOS_DECLARE_FUNC(void) EOS_Leaderboards_Definition_Release(EOS_Leaderboards_Definition* LeaderboardDefinition)
    {
        if (LeaderboardDefinition == nullptr)
            return;

        delete[]LeaderboardDefinition->LeaderboardId;
        delete[]LeaderboardDefinition->StatName;
        delete LeaderboardDefinition;
    }

    EOS_DECLARE_FUNC(void) EOS_Leaderboards_LeaderboardDefinition_Release(EOS_Leaderboards_Definition* LeaderboardDefinition)
    {
        EOS_Leaderboards_Definition_Release(LeaderboardDefinition);
    }

    EOS_DECLARE_FUNC(EOS_EResult) EOS_Leaderboards_CreateLeaderboard(EOS_HLeaderboards Handle, const void* Options)
    { return EOS_EResult::EOS_Success; }

    EOS_DECLARE_FUNC(EOS_EResult) EOS_Leaderboards_RemoveUser(EOS_HLeaderboards Handle, const void* Options, void* ClientData, const void* CompletionDelegate)
    { return EOS_EResult::EOS_Success; }

    EOS_DECLARE_FUNC(EOS_EResult) EOS_Leaderboards_Reset(EOS_HLeaderboards Handle, const void* Options)
    { return EOS_EResult::EOS_Success; }

    EOS_DECLARE_FUNC(void) EOS_Leaderboards_QueryLeaderboardRanks(EOS_HLeaderboards Handle, const EOS_Leaderboards_QueryLeaderboardRanksOptions* Options, void* ClientData, const EOS_Leaderboards_OnQueryLeaderboardRanksCompleteCallback CompletionDelegate)
    {
        if (Handle == nullptr)
            return;

        auto pInst = reinterpret_cast<EOSSDK_Leaderboards*>(Handle);
        pInst->QueryLeaderboardRanks(Options, ClientData, CompletionDelegate);
    }

    EOS_DECLARE_FUNC(uint32_t) EOS_Leaderboards_GetLeaderboardRecordCount(EOS_HLeaderboards Handle, const EOS_Leaderboards_GetLeaderboardRecordCountOptions* Options)
    {
        if (Handle == nullptr)
            return 0;

        auto pInst = reinterpret_cast<EOSSDK_Leaderboards*>(Handle);
        return pInst->GetLeaderboardRecordCount(Options);
    }

    EOS_DECLARE_FUNC(EOS_EResult) EOS_Leaderboards_CopyLeaderboardRecordByIndex(EOS_HLeaderboards Handle, const EOS_Leaderboards_CopyLeaderboardRecordByIndexOptions* Options, EOS_Leaderboards_LeaderboardRecord** OutLeaderboardRecord)
    {
        if (Handle == nullptr)
            return EOS_EResult::EOS_InvalidParameters;

        auto pInst = reinterpret_cast<EOSSDK_Leaderboards*>(Handle);
        return pInst->CopyLeaderboardRecordByIndex(Options, OutLeaderboardRecord);
    }

    EOS_DECLARE_FUNC(EOS_EResult) EOS_Leaderboards_CopyLeaderboardRecordByUserId(EOS_HLeaderboards Handle, const EOS_Leaderboards_CopyLeaderboardRecordByUserIdOptions* Options, EOS_Leaderboards_LeaderboardRecord** OutLeaderboardRecord)
    {
        if (Handle == nullptr)
            return EOS_EResult::EOS_InvalidParameters;

        auto pInst = reinterpret_cast<EOSSDK_Leaderboards*>(Handle);
        return pInst->CopyLeaderboardRecordByUserId(Options, OutLeaderboardRecord);
    }

    EOS_DECLARE_FUNC(void) EOS_Leaderboards_LeaderboardRecord_Release(EOS_Leaderboards_LeaderboardRecord* LeaderboardRecord)
    {
        if (LeaderboardRecord == nullptr)
            return;

        delete LeaderboardRecord;
    }

    EOS_DECLARE_FUNC(void) EOS_Leaderboards_QueryLeaderboardUserScores(EOS_HLeaderboards Handle, const EOS_Leaderboards_QueryLeaderboardUserScoresOptions* Options, void* ClientData, const EOS_Leaderboards_OnQueryLeaderboardUserScoresCompleteCallback CompletionDelegate)
    {
        if (Handle == nullptr)
            return;

        auto pInst = reinterpret_cast<EOSSDK_Leaderboards*>(Handle);
        pInst->QueryLeaderboardUserScores(Options, ClientData, CompletionDelegate);
    }

    EOS_DECLARE_FUNC(uint32_t) EOS_Leaderboards_GetLeaderboardUserScoreCount(EOS_HLeaderboards Handle, const EOS_Leaderboards_GetLeaderboardUserScoreCountOptions* Options)
    {
        if (Handle == nullptr)
            return 0;

        auto pInst = reinterpret_cast<EOSSDK_Leaderboards*>(Handle);
        return pInst->GetLeaderboardUserScoreCount(Options);
    }

    EOS_DECLARE_FUNC(EOS_EResult) EOS_Leaderboards_CopyLeaderboardUserScoreByIndex(EOS_HLeaderboards Handle, const EOS_Leaderboards_CopyLeaderboardUserScoreByIndexOptions* Options, EOS_Leaderboards_LeaderboardUserScore** OutLeaderboardUserScore)
    {
        if (Handle == nullptr)
            return EOS_EResult::EOS_InvalidParameters;

        auto pInst = reinterpret_cast<EOSSDK_Leaderboards*>(Handle);
        return pInst->CopyLeaderboardUserScoreByIndex(Options, OutLeaderboardUserScore);
    }

    EOS_DECLARE_FUNC(EOS_EResult) EOS_Leaderboards_CopyLeaderboardUserScoreByUserId(EOS_HLeaderboards Handle, const EOS_Leaderboards_CopyLeaderboardUserScoreByUserIdOptions* Options, EOS_Leaderboards_LeaderboardUserScore** OutLeaderboardUserScore)
    {
        if (Handle == nullptr)
            return EOS_EResult::EOS_InvalidParameters;

        auto pInst = reinterpret_cast<EOSSDK_Leaderboards*>(Handle);
        return pInst->CopyLeaderboardUserScoreByUserId(Options, OutLeaderboardUserScore);
    }

    EOS_DECLARE_FUNC(void) EOS_Leaderboards_LeaderboardUserScore_Release(EOS_Leaderboards_LeaderboardUserScore* LeaderboardUserScore)
    {
        if (LeaderboardUserScore == nullptr)
            return;

        delete LeaderboardUserScore;
    }
}
