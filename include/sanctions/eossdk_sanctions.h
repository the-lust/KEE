#pragma once

#include "common_includes.h"
#include "callback_manager.h"
#include "sdk/eos_sanctions.h"
#include "sdk/eos_sanctions_types.h"

namespace sdk
{
    class EOSSDK_Sanctions : public IRunCallback
    {
    public:
        EOSSDK_Sanctions();
        ~EOSSDK_Sanctions();

        virtual bool CBRunFrame() override { return false; }
        virtual bool RunCallbacks(pFrameResult_t res) override { return false; }
        virtual void FreeCallback(pFrameResult_t res) override {}

        void QueryActivePlayerSanctions(const EOS_Sanctions_QueryActivePlayerSanctionsOptions* Options, void* ClientData, EOS_Sanctions_OnQueryActivePlayerSanctionsCallback CompletionDelegate);
        uint32_t GetPlayerSanctionCount(const EOS_Sanctions_GetPlayerSanctionCountOptions* Options);
        EOS_EResult CopyPlayerSanctionByIndex(const EOS_Sanctions_CopyPlayerSanctionByIndexOptions* Options, EOS_Sanctions_PlayerSanction** OutSanction);
    };
}
