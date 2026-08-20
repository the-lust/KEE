#pragma once
#include "common_includes.h"
#include "callback_manager.h"
#include "sdk/eos_progressionsnapshot.h"
#include "sdk/eos_progressionsnapshot_types.h"

namespace sdk {
    class EOSSDK_ProgressionSnapshot : public IRunCallback {
        std::mutex m_mutex;
        std::map<uint32_t, std::map<string,string>> m_active_snapshots;
        fifo_json   m_persisted;
        string m_save_path;
        void m_load();
        void m_save();
    public:
        EOSSDK_ProgressionSnapshot(); ~EOSSDK_ProgressionSnapshot();
        virtual bool CBRunFrame() override;
        virtual bool RunCallbacks(pFrameResult_t res) override;
        virtual void FreeCallback(pFrameResult_t res) override;
        EOS_EResult BeginSnapshot(const EOS_ProgressionSnapshot_BeginSnapshotOptions*, uint32_t*);
        EOS_EResult AddProgression(const EOS_ProgressionSnapshot_AddProgressionOptions*);
        void SubmitSnapshot(const EOS_ProgressionSnapshot_SubmitSnapshotOptions*, void*, EOS_ProgressionSnapshot_OnSubmitSnapshotCallback);
        EOS_EResult DeleteSnapshot(const EOS_ProgressionSnapshot_DeleteSnapshotOptions*);
    };
}
