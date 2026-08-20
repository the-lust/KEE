#include "utils/common_includes.h"

EOS_DECLARE_FUNC(const char*) EOS_EApplicationStatus_ToString(EOS_EApplicationStatus ApplicationStatus)
{
    switch (ApplicationStatus)
    {
        case EOS_EApplicationStatus::EOS_AS_BackgroundConstrained:    return "BackgroundConstrained";
        case EOS_EApplicationStatus::EOS_AS_BackgroundUnconstrained:  return "BackgroundUnconstrained";
        case EOS_EApplicationStatus::EOS_AS_BackgroundSuspended:      return "BackgroundSuspended";
        case EOS_EApplicationStatus::EOS_AS_Foreground:               return "Foreground";
        default:                                                       return "Unknown";
    }
}

