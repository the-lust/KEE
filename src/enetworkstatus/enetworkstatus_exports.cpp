#include "utils/common_includes.h"

EOS_DECLARE_FUNC(const char*) EOS_ENetworkStatus_ToString(EOS_ENetworkStatus NetworkStatus)
{
    switch (NetworkStatus)
    {
        case EOS_ENetworkStatus::EOS_NS_Disabled:    return "Disabled";
        case EOS_ENetworkStatus::EOS_NS_Offline:     return "Offline";
        case EOS_ENetworkStatus::EOS_NS_Online:      return "Online";
        default:                                     return "Unknown";
    }
}

