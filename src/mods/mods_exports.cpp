#include "utils/common_includes.h"
#include "mods/eossdk_mods.h"
#include "eossdk_platform.h"

EOS_DECLARE_FUNC(void) EOS_Mods_InstallMod(EOS_HMods Handle, const EOS_Mods_InstallModOptions* Options, void* ClientData, const EOS_Mods_OnInstallModCallback CompletionDelegate)
{ GetEOS_Mods().InstallMod(Options, ClientData, CompletionDelegate); }

EOS_DECLARE_FUNC(void) EOS_Mods_UninstallMod(EOS_HMods Handle, const EOS_Mods_UninstallModOptions* Options, void* ClientData, const EOS_Mods_OnUninstallModCallback CompletionDelegate)
{ GetEOS_Mods().UninstallMod(Options, ClientData, CompletionDelegate); }

EOS_DECLARE_FUNC(void) EOS_Mods_UpdateMod(EOS_HMods Handle, const EOS_Mods_UpdateModOptions* Options, void* ClientData, const EOS_Mods_OnUpdateModCallback CompletionDelegate)
{ GetEOS_Mods().UpdateMod(Options, ClientData, CompletionDelegate); }

EOS_DECLARE_FUNC(void) EOS_Mods_EnumerateMods(EOS_HMods Handle, const EOS_Mods_EnumerateModsOptions* Options, void* ClientData, const EOS_Mods_OnEnumerateModsCallback CompletionDelegate)
{ GetEOS_Mods().EnumerateMods(Options, ClientData, CompletionDelegate); }

EOS_DECLARE_FUNC(EOS_EResult) EOS_Mods_CopyModInfo(EOS_HMods Handle, const EOS_Mods_CopyModInfoOptions* Options, EOS_Mods_ModInfo** OutModInfo)
{ return GetEOS_Mods().CopyModInfo(Options, OutModInfo); }

EOS_DECLARE_FUNC(void) EOS_Mods_ModInfo_Release(EOS_Mods_ModInfo* p) { delete p; }
