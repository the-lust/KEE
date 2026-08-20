#pragma once
#include "common_includes.h"
#include "callback_manager.h"
#include "sdk/eos_mods.h"
#include "sdk/eos_mods_types.h"
namespace sdk {
    class EOSSDK_Mods : public IRunCallback {
    public:
        EOSSDK_Mods(); ~EOSSDK_Mods();
        virtual bool CBRunFrame() override;
        virtual bool RunCallbacks(pFrameResult_t res) override;
        virtual void FreeCallback(pFrameResult_t res) override;
        void InstallMod(const EOS_Mods_InstallModOptions*, void*, EOS_Mods_OnInstallModCallback);
        void UninstallMod(const EOS_Mods_UninstallModOptions*, void*, EOS_Mods_OnUninstallModCallback);
        void UpdateMod(const EOS_Mods_UpdateModOptions*, void*, EOS_Mods_OnUpdateModCallback);
        void EnumerateMods(const EOS_Mods_EnumerateModsOptions*, void*, EOS_Mods_OnEnumerateModsCallback);
        EOS_EResult CopyModInfo(const EOS_Mods_CopyModInfoOptions*, EOS_Mods_ModInfo**);
    };
}
