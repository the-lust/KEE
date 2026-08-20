
#include "utils/common_includes.h"
#include "integratedplatform/eossdk_integratedplatform.h"
#include "eossdk_platform.h"

EOS_DECLARE_FUNC(EOS_EResult) EOS_IntegratedPlatform_SetUserLoginStatus(EOS_HIntegratedPlatform Handle, const EOS_IntegratedPlatform_SetUserLoginStatusOptions* Options)
{
    return EOS_EResult::EOS_Success;
}

EOS_DECLARE_FUNC(EOS_NotificationId) EOS_IntegratedPlatform_AddNotifyUserLoginStatusChanged(EOS_HIntegratedPlatform Handle, const EOS_IntegratedPlatform_AddNotifyUserLoginStatusChangedOptions* Options, void* ClientData, const EOS_IntegratedPlatform_OnUserLoginStatusChangedCallback CallbackFunction)
{
    return EOS_INVALID_NOTIFICATIONID;
}

EOS_DECLARE_FUNC(void) EOS_IntegratedPlatform_RemoveNotifyUserLoginStatusChanged(EOS_HIntegratedPlatform Handle, EOS_NotificationId NotificationId)
{
}

EOS_DECLARE_FUNC(EOS_EResult) EOS_IntegratedPlatform_SetUserPreLogoutCallback(EOS_HIntegratedPlatform Handle, const EOS_IntegratedPlatform_SetUserPreLogoutCallbackOptions* Options, void* ClientData, EOS_IntegratedPlatform_OnUserPreLogoutCallback CallbackFunction)
{
    return EOS_EResult::EOS_Success;
}

EOS_DECLARE_FUNC(void) EOS_IntegratedPlatform_ClearUserPreLogoutCallback(EOS_HIntegratedPlatform Handle, const EOS_IntegratedPlatform_ClearUserPreLogoutCallbackOptions* Options)
{
}

EOS_DECLARE_FUNC(EOS_EResult) EOS_IntegratedPlatform_FinalizeDeferredUserLogout(EOS_HIntegratedPlatform Handle, const EOS_IntegratedPlatform_FinalizeDeferredUserLogoutOptions* Options)
{
    return EOS_EResult::EOS_Success;
}

EOS_DECLARE_FUNC(EOS_EResult) EOS_IntegratedPlatform_CreateIntegratedPlatformOptionsContainer(const EOS_IntegratedPlatform_CreateIntegratedPlatformOptionsContainerOptions* Options, EOS_HIntegratedPlatformOptionsContainer* OutIntegratedPlatformOptionsContainerHandle)
{
    if (OutIntegratedPlatformOptionsContainerHandle == nullptr) return EOS_EResult::EOS_InvalidParameters;
    *OutIntegratedPlatformOptionsContainerHandle = reinterpret_cast<EOS_HIntegratedPlatformOptionsContainer>(static_cast<uintptr_t>(1));
    return EOS_EResult::EOS_Success;
}

