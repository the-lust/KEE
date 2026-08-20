#include "utils/common_includes.h"
#include "progressionsnapshot/eossdk_progressionsnapshot.h"
#include "eossdk_platform.h"

EOS_DECLARE_FUNC(EOS_EResult) EOS_ProgressionSnapshot_BeginSnapshot(EOS_HProgressionSnapshot Handle, const EOS_ProgressionSnapshot_BeginSnapshotOptions* Options, uint32_t* OutSnapshotId)
{ return GetEOS_ProgressionSnapshot().BeginSnapshot(Options, OutSnapshotId); }

EOS_DECLARE_FUNC(EOS_EResult) EOS_ProgressionSnapshot_AddProgression(EOS_HProgressionSnapshot Handle, const EOS_ProgressionSnapshot_AddProgressionOptions* Options)
{ return GetEOS_ProgressionSnapshot().AddProgression(Options); }

EOS_DECLARE_FUNC(void) EOS_ProgressionSnapshot_SubmitSnapshot(EOS_HProgressionSnapshot Handle, const EOS_ProgressionSnapshot_SubmitSnapshotOptions* Options, void* ClientData, const EOS_ProgressionSnapshot_OnSubmitSnapshotCallback CompletionDelegate)
{ GetEOS_ProgressionSnapshot().SubmitSnapshot(Options, ClientData, CompletionDelegate); }

EOS_DECLARE_FUNC(void) EOS_ProgressionSnapshot_DeleteSnapshot(EOS_HProgressionSnapshot Handle, const EOS_ProgressionSnapshot_DeleteSnapshotOptions* Options, void* ClientData, const EOS_ProgressionSnapshot_OnDeleteSnapshotCallback CompletionDelegate)
{ if (CompletionDelegate) { EOS_ProgressionSnapshot_DeleteSnapshotCallbackInfo info{}; info.ResultCode = EOS_EResult::EOS_Success; info.ClientData = ClientData; CompletionDelegate(&info); } }

EOS_DECLARE_FUNC(EOS_EResult) EOS_ProgressionSnapshot_EndSnapshot(EOS_HProgressionSnapshot Handle, const EOS_ProgressionSnapshot_EndSnapshotOptions* Options)
{ return EOS_EResult::EOS_Success; }
