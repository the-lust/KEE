
#include "progressionsnapshot/eossdk_progressionsnapshot.h"
#include "eossdk_platform.h"
#include "settings.h"

namespace sdk {

static uint32_t s_next_snapshot_id = 1;

EOSSDK_ProgressionSnapshot::EOSSDK_ProgressionSnapshot()
{
    GetCB_Manager().register_callbacks(this);
    m_load();
}

EOSSDK_ProgressionSnapshot::~EOSSDK_ProgressionSnapshot()
{
    GetCB_Manager().unregister_callbacks(this);
}

void EOSSDK_ProgressionSnapshot::m_load()
{
    m_save_path = Settings::Inst().save_path + "/progression_snapshot.json";
    try {
        FileManager::load_json(m_save_path, m_persisted);
    } catch (...) {}
}

void EOSSDK_ProgressionSnapshot::m_save()
{
    try { FileManager::save_json(m_save_path, m_persisted); } catch (...) {}
}

EOS_EResult EOSSDK_ProgressionSnapshot::BeginSnapshot(
    const EOS_ProgressionSnapshot_BeginSnapshotOptions* Options,
    uint32_t* OutSnapshotId)
{
    if (!Options || !OutSnapshotId) return EOS_EResult::EOS_InvalidParameters;
    std::lock_guard<std::mutex> lk(m_mutex);
    uint32_t id = s_next_snapshot_id++;
    m_active_snapshots[id] = {};
    *OutSnapshotId = id;
    return EOS_EResult::EOS_Success;
}

EOS_EResult EOSSDK_ProgressionSnapshot::AddProgression(
    const EOS_ProgressionSnapshot_AddProgressionOptions* Options)
{
    if (!Options || !Options->Key || !Options->Value) return EOS_EResult::EOS_InvalidParameters;
    std::lock_guard<std::mutex> lk(m_mutex);
    auto it = m_active_snapshots.find(Options->SnapshotId);
    if (it == m_active_snapshots.end()) return EOS_EResult::EOS_NotFound;
    it->second[Options->Key] = Options->Value;
    return EOS_EResult::EOS_Success;
}

void EOSSDK_ProgressionSnapshot::SubmitSnapshot(
    const EOS_ProgressionSnapshot_SubmitSnapshotOptions* Options,
    void* ClientData,
    EOS_ProgressionSnapshot_OnSubmitSnapshotCallback CompletionDelegate)
{
    if (!CompletionDelegate) return;

    EOS_EResult result = EOS_EResult::EOS_Success;
    if (Options) {
        std::lock_guard<std::mutex> lk(m_mutex);
        auto it = m_active_snapshots.find(Options->SnapshotId);
        if (it != m_active_snapshots.end()) {
            
            for (auto& [k, v] : it->second)
                m_persisted[k] = v;
            m_save();
        } else {
            result = EOS_EResult::EOS_NotFound;
        }
    }

    pFrameResult_t res(new FrameResult);
    auto& cb = res->CreateCallback<EOS_ProgressionSnapshot_SubmitSnapshotCallbackInfo>(
        [CompletionDelegate](void* data) {
            CompletionDelegate(reinterpret_cast<EOS_ProgressionSnapshot_SubmitSnapshotCallbackInfo*>(data));
        });
    cb.ResultCode  = result;
    cb.ClientData  = ClientData;
    cb.SnapshotId  = Options ? Options->SnapshotId : 0;
    SE_CB_END(this);
}

EOS_EResult EOSSDK_ProgressionSnapshot::DeleteSnapshot(
    const EOS_ProgressionSnapshot_DeleteSnapshotOptions* Options)
{
    if (!Options) return EOS_EResult::EOS_InvalidParameters;
    std::lock_guard<std::mutex> lk(m_mutex);
    m_active_snapshots.clear();
    return EOS_EResult::EOS_Success;
}

bool EOSSDK_ProgressionSnapshot::CBRunFrame()  { return false; }
bool EOSSDK_ProgressionSnapshot::RunCallbacks(pFrameResult_t res) { res->GetCallbackMsg().cb_func(res->GetFuncParam()); return true; }
void EOSSDK_ProgressionSnapshot::FreeCallback(pFrameResult_t) {}

} 
