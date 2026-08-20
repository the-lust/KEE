#include "eossdk_metrics.h"

namespace sdk
{

EOSSDK_Metrics::EOSSDK_Metrics()
{}

EOSSDK_Metrics::~EOSSDK_Metrics()
{}

EOS_EResult EOSSDK_Metrics::BeginPlayerSession(const EOS_Metrics_BeginPlayerSessionOptions* Options)
{
    TRACE_FUNC();

    return EOS_EResult::EOS_Success;
}

EOS_EResult EOSSDK_Metrics::EndPlayerSession(const EOS_Metrics_EndPlayerSessionOptions* Options)
{
    TRACE_FUNC();

    return EOS_EResult::EOS_Success;
}

}