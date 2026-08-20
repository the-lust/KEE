#pragma once
#include "common_includes.h"
#include "callback_manager.h"
#include "network.h"

namespace sdk
{
    class EOSSDK_Leaderboards :
        public IRunCallback
    {
        string m_scores_filename;
        fifo_json   m_scores;              
        string m_active_leaderboard_id;

    public:
        EOSSDK_Leaderboards();
        ~EOSSDK_Leaderboards();

        virtual bool CBRunFrame();
        virtual bool RunCallbacks(pFrameResult_t res);
        virtual void FreeCallback(pFrameResult_t res);

        void        QueryLeaderboardDefinitions(const EOS_Leaderboards_QueryLeaderboardDefinitionsOptions*, void*, EOS_Leaderboards_OnQueryLeaderboardDefinitionsCompleteCallback);
        uint32_t    GetLeaderboardDefinitionCount(const EOS_Leaderboards_GetLeaderboardDefinitionCountOptions*);
        EOS_EResult CopyLeaderboardDefinitionByIndex(const EOS_Leaderboards_CopyLeaderboardDefinitionByIndexOptions*, EOS_Leaderboards_Definition**);
        EOS_EResult CopyLeaderboardDefinitionByLeaderboardId(const EOS_Leaderboards_CopyLeaderboardDefinitionByLeaderboardIdOptions*, EOS_Leaderboards_Definition**);
        void        QueryLeaderboardRanks(const EOS_Leaderboards_QueryLeaderboardRanksOptions*, void*, EOS_Leaderboards_OnQueryLeaderboardRanksCompleteCallback);
        uint32_t    GetLeaderboardRecordCount(const EOS_Leaderboards_GetLeaderboardRecordCountOptions*);
        EOS_EResult CopyLeaderboardRecordByIndex(const EOS_Leaderboards_CopyLeaderboardRecordByIndexOptions*, EOS_Leaderboards_LeaderboardRecord**);
        EOS_EResult CopyLeaderboardRecordByUserId(const EOS_Leaderboards_CopyLeaderboardRecordByUserIdOptions*, EOS_Leaderboards_LeaderboardRecord**);
        void        QueryLeaderboardUserScores(const EOS_Leaderboards_QueryLeaderboardUserScoresOptions*, void*, EOS_Leaderboards_OnQueryLeaderboardUserScoresCompleteCallback);
        uint32_t    GetLeaderboardUserScoreCount(const EOS_Leaderboards_GetLeaderboardUserScoreCountOptions*);
        EOS_EResult CopyLeaderboardUserScoreByIndex(const EOS_Leaderboards_CopyLeaderboardUserScoreByIndexOptions*, EOS_Leaderboards_LeaderboardUserScore**);
        EOS_EResult CopyLeaderboardUserScoreByUserId(const EOS_Leaderboards_CopyLeaderboardUserScoreByUserIdOptions*, EOS_Leaderboards_LeaderboardUserScore**);
    };
}
