
#include "auth/eossdk_auth.h"
#include "eossdk_platform.h"
#include "helper_funcs.h"

extern "C" {

EOS_DECLARE_FUNC(EOS_EResult) EOS_Auth_CopyUserAuthTokenNew(EOS_HAuth Handle, const void* Options, EOS_EpicAccountId LocalUserId, EOS_Auth_Token** OutUserAuthToken)
{
    if (Handle == nullptr)
        return EOS_EResult::EOS_InvalidParameters;
    auto pInst = reinterpret_cast<EOSSDK_Auth*>(Handle);
    return pInst->CopyUserAuthToken((const EOS_Auth_CopyUserAuthTokenOptions*)Options, LocalUserId, OutUserAuthToken);
}

EOS_DECLARE_FUNC(EOS_EResult) EOS_Auth_CopyUserAuthTokenOld(EOS_HAuth Handle, EOS_EpicAccountId LocalUserId, EOS_Auth_Token** OutUserAuthToken)
{
    if (Handle == nullptr)
        return EOS_EResult::EOS_InvalidParameters;
    auto pInst = reinterpret_cast<EOSSDK_Auth*>(Handle);
    return pInst->CopyUserAuthTokenOld(LocalUserId, OutUserAuthToken);
}

EOS_DECLARE_FUNC(EOS_NotificationId) EOS_Auth_AddNotifyLoginStatusChangedNew(EOS_HAuth Handle, const void* Options, void* ClientData, const EOS_Auth_OnLoginStatusChangedCallback Notification)
{
    if (Handle == nullptr)
        return EOS_INVALID_NOTIFICATIONID;
    auto pInst = reinterpret_cast<EOSSDK_Auth*>(Handle);
    return pInst->AddNotifyLoginStatusChanged((const EOS_Auth_AddNotifyLoginStatusChangedOptions*)Options, ClientData, Notification);
}

EOS_DECLARE_FUNC(EOS_NotificationId) EOS_Auth_AddNotifyLoginStatusChangedOld(EOS_HAuth Handle, void* ClientData, const EOS_Auth_OnLoginStatusChangedCallback Notification)
{
    if (Handle == nullptr)
        return EOS_INVALID_NOTIFICATIONID;
    auto pInst = reinterpret_cast<EOSSDK_Auth*>(Handle);
    return pInst->AddNotifyLoginStatusChangedOld(ClientData, Notification);
}

}

extern "C" EOS_DECLARE_FUNC(EOS_EResult) CLANG_GCC_DONT_OPTIMIZE EOS_Auth_CopyUserAuthToken()
{
    return EOS_EResult::EOS_NotImplemented;
}

extern "C" EOS_DECLARE_FUNC(EOS_NotificationId) CLANG_GCC_DONT_OPTIMIZE EOS_Auth_AddNotifyLoginStatusChanged()
{
    return EOS_INVALID_NOTIFICATIONID;
}
