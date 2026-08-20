#include "utils/common_includes.h"
#include "sanctions/eossdk_sanctions.h"
#include "eossdk_platform.h"

EOS_DECLARE_FUNC(void) EOS_Sanctions_QueryActivePlayerSanctions(EOS_HSanctions Handle, const EOS_Sanctions_QueryActivePlayerSanctionsOptions* Options, void* ClientData, const EOS_Sanctions_OnQueryActivePlayerSanctionsCallback CompletionDelegate)
{ GetEOS_Sanctions().QueryActivePlayerSanctions(Options, ClientData, CompletionDelegate); }

EOS_DECLARE_FUNC(uint32_t) EOS_Sanctions_GetPlayerSanctionCount(EOS_HSanctions Handle, const EOS_Sanctions_GetPlayerSanctionCountOptions* Options)
{ return GetEOS_Sanctions().GetPlayerSanctionCount(Options); }

EOS_DECLARE_FUNC(EOS_EResult) EOS_Sanctions_CopyPlayerSanctionByIndex(EOS_HSanctions Handle, const EOS_Sanctions_CopyPlayerSanctionByIndexOptions* Options, EOS_Sanctions_PlayerSanction** OutSanction)
{ return GetEOS_Sanctions().CopyPlayerSanctionByIndex(Options, OutSanction); }

EOS_DECLARE_FUNC(void) EOS_Sanctions_PlayerSanction_Release(EOS_Sanctions_PlayerSanction* p) { delete p; }

EOS_DECLARE_FUNC(void) EOS_Sanctions_CreatePlayerSanctionAppeal(EOS_HSanctions Handle, const EOS_Sanctions_CreatePlayerSanctionAppealOptions* Options, void* ClientData, const EOS_Sanctions_CreatePlayerSanctionAppealCallback CompletionDelegate)
{
    if (CompletionDelegate) {
        EOS_Sanctions_CreatePlayerSanctionAppealCallbackInfo info{};
        info.ResultCode = EOS_EResult::EOS_NotImplemented;
        info.ClientData = ClientData;
        CompletionDelegate(&info);
    }
}
