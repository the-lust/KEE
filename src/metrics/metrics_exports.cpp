
#include "metrics/eossdk_metrics.h"

using namespace sdk;

extern "C"
{
    EOS_DECLARE_FUNC(EOS_EResult) EOS_Metrics_BeginPlayerSession(EOS_HMetrics Handle, const EOS_Metrics_BeginPlayerSessionOptions* Options)
    {
        if (Handle == nullptr)
            return EOS_EResult::EOS_InvalidParameters;
        auto pInst = reinterpret_cast<EOSSDK_Metrics*>(Handle);
        return pInst->BeginPlayerSession(Options);
    }

    EOS_DECLARE_FUNC(EOS_EResult) EOS_Metrics_EndPlayerSession(EOS_HMetrics Handle, const EOS_Metrics_EndPlayerSessionOptions* Options)
    {
        if (Handle == nullptr)
            return EOS_EResult::EOS_InvalidParameters;
        auto pInst = reinterpret_cast<EOSSDK_Metrics*>(Handle);
        return pInst->EndPlayerSession(Options);
    }
}
