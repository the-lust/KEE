
#include "reports/eossdk_reports.h"
#include "eossdk_platform.h"

namespace sdk {

EOSSDK_Reports::EOSSDK_Reports() { GetCB_Manager().register_callbacks(this); }
EOSSDK_Reports::~EOSSDK_Reports() { GetCB_Manager().unregister_callbacks(this); }

void EOSSDK_Reports::SendPlayerBehaviorReport(
    const EOS_Reports_SendPlayerBehaviorReportOptions* Options,
    void* ClientData,
    EOS_Reports_OnSendPlayerBehaviorReportCompleteCallback CompletionDelegate)
{
    if (!CompletionDelegate) return;
    pFrameResult_t res(new FrameResult);
    auto& cb = res->CreateCallback<EOS_Reports_SendPlayerBehaviorReportCompleteCallbackInfo>(
        [CompletionDelegate](void* data) {
            CompletionDelegate(reinterpret_cast<EOS_Reports_SendPlayerBehaviorReportCompleteCallbackInfo*>(data));
        });
    cb.ResultCode = EOS_EResult::EOS_Success;
    cb.ClientData = ClientData;
    SE_CB_END(this);
}

bool EOSSDK_Reports::CBRunFrame()  { return false; }
bool EOSSDK_Reports::RunCallbacks(pFrameResult_t res) { res->GetCallbackMsg().cb_func(res->GetFuncParam()); return true; }
void EOSSDK_Reports::FreeCallback(pFrameResult_t) {}

} 
