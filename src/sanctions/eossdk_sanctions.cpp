
#include "sanctions/eossdk_sanctions.h"
#include "eossdk_platform.h"

namespace sdk {

EOSSDK_Sanctions::EOSSDK_Sanctions() { GetCB_Manager().register_callbacks(this); }
EOSSDK_Sanctions::~EOSSDK_Sanctions() { GetCB_Manager().unregister_callbacks(this); }

void EOSSDK_Sanctions::QueryActivePlayerSanctions(
    const EOS_Sanctions_QueryActivePlayerSanctionsOptions* Options,
    void* ClientData,
    EOS_Sanctions_OnQueryActivePlayerSanctionsCallback CompletionDelegate)
{
    if (!CompletionDelegate) return;
    pFrameResult_t res(new FrameResult);
    auto& cb = res->CreateCallback<EOS_Sanctions_QueryActivePlayerSanctionsCallbackInfo>(
        [CompletionDelegate](void* data) {
            CompletionDelegate(reinterpret_cast<EOS_Sanctions_QueryActivePlayerSanctionsCallbackInfo*>(data));
        });
    cb.ResultCode       = EOS_EResult::EOS_Success;
    cb.ClientData       = ClientData;
    cb.LocalUserId      = Options ? Options->LocalUserId : nullptr;
    cb.TargetUserId     = Options ? Options->TargetUserId : nullptr;
    SE_CB_END(this);
}

uint32_t EOSSDK_Sanctions::GetPlayerSanctionCount(const EOS_Sanctions_GetPlayerSanctionCountOptions* )
{ return 0; }

EOS_EResult EOSSDK_Sanctions::CopyPlayerSanctionByIndex(
    const EOS_Sanctions_CopyPlayerSanctionByIndexOptions* ,
    EOS_Sanctions_PlayerSanction** OutSanction)
{
    if (OutSanction) *OutSanction = nullptr;
    return EOS_EResult::EOS_NotFound;
}



} 
