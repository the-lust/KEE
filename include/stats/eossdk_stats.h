#pragma once
#include "common_includes.h"
#include "callback_manager.h"

namespace sdk
{
    class EOSSDK_Stats :
        public IRunCallback
    {
        string stats_filename;

        nlohmann::json m_stats;      
        fifo_json      m_stats_defs; 

    public:
        EOSSDK_Stats();
        ~EOSSDK_Stats();

        void save_stats();

        virtual bool CBRunFrame();
        virtual bool RunCallbacks(pFrameResult_t res);
        virtual void FreeCallback(pFrameResult_t res);

        void        IngestStat(const EOS_Stats_IngestStatOptions* Options, void* ClientData, const EOS_Stats_OnIngestStatCompleteCallback CompletionDelegate);
        void        QueryStats(const EOS_Stats_QueryStatsOptions* Options, void* ClientData, const EOS_Stats_OnQueryStatsCompleteCallback CompletionDelegate);
        uint32_t    GetStatsCount(const EOS_Stats_GetStatCountOptions* Options);
        EOS_EResult CopyStatByIndex(const EOS_Stats_CopyStatByIndexOptions* Options, EOS_Stats_Stat** OutStat);
        EOS_EResult CopyStatByName(const EOS_Stats_CopyStatByNameOptions* Options, EOS_Stats_Stat** OutStat);
    };
}
