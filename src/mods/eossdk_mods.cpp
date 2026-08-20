
#include "mods/eossdk_mods.h"
#include "eossdk_platform.h"

namespace sdk {

EOSSDK_Mods::EOSSDK_Mods() { GetCB_Manager().register_callbacks(this); }
EOSSDK_Mods::~EOSSDK_Mods() { GetCB_Manager().unregister_callbacks(this); }

template<typename TInfo, typename TDelegate>
static void fire_success(pFrameResult_t& res, void* ClientData, TDelegate delegate)
{
    auto& cb = res->CreateCallback<TInfo>([delegate](void* data) {
        delegate(reinterpret_cast<TInfo*>(data));
    });
    cb.ResultCode = EOS_EResult::EOS_Success;
    cb.ClientData = ClientData;
    res->done = true;
}

void EOSSDK_Mods::InstallMod(const EOS_Mods_InstallModOptions* Options, void* ClientData, EOS_Mods_OnInstallModCallback CompletionDelegate)
{
    if (!CompletionDelegate) return;
    pFrameResult_t res(new FrameResult);
    auto& cb = res->CreateCallback<EOS_Mods_InstallModCallbackInfo>([CompletionDelegate](void* data){ CompletionDelegate(reinterpret_cast<EOS_Mods_InstallModCallbackInfo*>(data)); });
    cb.ResultCode = EOS_EResult::EOS_Success;
    cb.ClientData = ClientData;
    cb.LocalUserId = Options ? Options->LocalUserId : nullptr;
    cb.Mod = nullptr;
    SE_CB_END(this);
}

void EOSSDK_Mods::UninstallMod(const EOS_Mods_UninstallModOptions* Options, void* ClientData, EOS_Mods_OnUninstallModCallback CompletionDelegate)
{
    if (!CompletionDelegate) return;
    pFrameResult_t res(new FrameResult);
    auto& cb = res->CreateCallback<EOS_Mods_UninstallModCallbackInfo>([CompletionDelegate](void* data){ CompletionDelegate(reinterpret_cast<EOS_Mods_UninstallModCallbackInfo*>(data)); });
    cb.ResultCode = EOS_EResult::EOS_Success;
    cb.ClientData = ClientData;
    cb.LocalUserId = Options ? Options->LocalUserId : nullptr;
    cb.Mod = nullptr;
    SE_CB_END(this);
}

void EOSSDK_Mods::UpdateMod(const EOS_Mods_UpdateModOptions* Options, void* ClientData, EOS_Mods_OnUpdateModCallback CompletionDelegate)
{
    if (!CompletionDelegate) return;
    pFrameResult_t res(new FrameResult);
    auto& cb = res->CreateCallback<EOS_Mods_UpdateModCallbackInfo>([CompletionDelegate](void* data){ CompletionDelegate(reinterpret_cast<EOS_Mods_UpdateModCallbackInfo*>(data)); });
    cb.ResultCode = EOS_EResult::EOS_Success;
    cb.ClientData = ClientData;
    cb.LocalUserId = Options ? Options->LocalUserId : nullptr;
    cb.Mod = nullptr;
    SE_CB_END(this);
}

void EOSSDK_Mods::EnumerateMods(const EOS_Mods_EnumerateModsOptions* Options, void* ClientData, EOS_Mods_OnEnumerateModsCallback CompletionDelegate)
{
    if (!CompletionDelegate) return;
    pFrameResult_t res(new FrameResult);
    auto& cb = res->CreateCallback<EOS_Mods_EnumerateModsCallbackInfo>([CompletionDelegate](void* data){ CompletionDelegate(reinterpret_cast<EOS_Mods_EnumerateModsCallbackInfo*>(data)); });
    cb.ResultCode  = EOS_EResult::EOS_Success;
    cb.ClientData  = ClientData;
    cb.LocalUserId = Options ? Options->LocalUserId : nullptr;
    cb.Type        = Options ? Options->Type : EOS_EModEnumerationType::EOS_MET_INSTALLED;
    SE_CB_END(this);
}

EOS_EResult EOSSDK_Mods::CopyModInfo(const EOS_Mods_CopyModInfoOptions* , EOS_Mods_ModInfo** OutModInfo)
{
    if (!OutModInfo) return EOS_EResult::EOS_InvalidParameters;
    
    auto* mi = new EOS_Mods_ModInfo{};
    mi->ApiVersion = EOS_MODS_COPYMODINFO_API_LATEST;
    mi->Mods       = nullptr;
    mi->ModsCount  = 0;
    mi->Type       = EOS_EModEnumerationType::EOS_MET_INSTALLED;
    *OutModInfo = mi;
    return EOS_EResult::EOS_Success;
}

bool EOSSDK_Mods::CBRunFrame()  { return false; }
bool EOSSDK_Mods::RunCallbacks(pFrameResult_t res) { res->GetCallbackMsg().cb_func(res->GetFuncParam()); return true; }
void EOSSDK_Mods::FreeCallback(pFrameResult_t) {}

} 
