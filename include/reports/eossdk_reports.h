#pragma once
#include "common_includes.h"
#include "callback_manager.h"
#include "sdk/eos_reports.h"
#include "sdk/eos_reports_types.h"

namespace sdk {
    class EOSSDK_Reports : public IRunCallback {
    public:
        EOSSDK_Reports();
        ~EOSSDK_Reports();
        virtual bool CBRunFrame() override;
        virtual bool RunCallbacks(pFrameResult_t res) override;
        virtual void FreeCallback(pFrameResult_t res) override;
        void SendPlayerBehaviorReport(const EOS_Reports_SendPlayerBehaviorReportOptions*, void*, EOS_Reports_OnSendPlayerBehaviorReportCompleteCallback);
    };
}
