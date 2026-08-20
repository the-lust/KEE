
#include "stats/eossdk_stats.h"

using namespace sdk;

extern "C"
{
    EOS_DECLARE_FUNC(void) EOS_Stats_IngestStat(EOS_HStats Handle, const EOS_Stats_IngestStatOptions* Options, void* ClientData, const EOS_Stats_OnIngestStatCompleteCallback CompletionDelegate)
    {
        if (Handle == nullptr)
            return;
        auto pInst = reinterpret_cast<EOSSDK_Stats*>(Handle);
        pInst->IngestStat(Options, ClientData, CompletionDelegate);
    }

    EOS_DECLARE_FUNC(void) EOS_Stats_QueryStats(EOS_HStats Handle, const EOS_Stats_QueryStatsOptions* Options, void* ClientData, const EOS_Stats_OnQueryStatsCompleteCallback CompletionDelegate)
    {
        if (Handle == nullptr)
            return;
        auto pInst = reinterpret_cast<EOSSDK_Stats*>(Handle);
        pInst->QueryStats(Options, ClientData, CompletionDelegate);
    }

    EOS_DECLARE_FUNC(uint32_t) EOS_Stats_GetStatsCount(EOS_HStats Handle, const EOS_Stats_GetStatCountOptions* Options)
    {
        if (Handle == nullptr)
            return 0;
        auto pInst = reinterpret_cast<EOSSDK_Stats*>(Handle);
        return pInst->GetStatsCount(Options);
    }

    EOS_DECLARE_FUNC(EOS_EResult) EOS_Stats_CopyStatByIndex(EOS_HStats Handle, const EOS_Stats_CopyStatByIndexOptions* Options, EOS_Stats_Stat** OutStat)
    {
        if (Handle == nullptr)
            return EOS_EResult::EOS_InvalidParameters;
        auto pInst = reinterpret_cast<EOSSDK_Stats*>(Handle);
        return pInst->CopyStatByIndex(Options, OutStat);
    }

    EOS_DECLARE_FUNC(EOS_EResult) EOS_Stats_CopyStatByName(EOS_HStats Handle, const EOS_Stats_CopyStatByNameOptions* Options, EOS_Stats_Stat** OutStat)
    {
        if (Handle == nullptr)
            return EOS_EResult::EOS_InvalidParameters;
        auto pInst = reinterpret_cast<EOSSDK_Stats*>(Handle);
        return pInst->CopyStatByName(Options, OutStat);
    }

    EOS_DECLARE_FUNC(void) EOS_Stats_Stat_Release(EOS_Stats_Stat* Stat)
    {
        if (Stat == nullptr)
            return;
        delete[] Stat->Name;
        delete Stat;
    }
}
