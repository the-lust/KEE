#pragma once

#include "common_includes.h"

namespace sdk
{
    class EOSSDK_Metrics
    {
    public:
        EOSSDK_Metrics();
        ~EOSSDK_Metrics();

        EOS_EResult BeginPlayerSession(const EOS_Metrics_BeginPlayerSessionOptions* Options);
        EOS_EResult EndPlayerSession(const EOS_Metrics_EndPlayerSessionOptions* Options);
    };
}