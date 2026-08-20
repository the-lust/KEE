#include "utils/compat_types.h"
#include "ui/eossdk_ui.h"

using namespace sdk;

extern "C"
{
    EOS_DECLARE_FUNC(void) EOS_UI_ShowFriends(EOS_HUI Handle, const EOS_UI_ShowFriendsOptions* Options, void* ClientData, const EOS_UI_OnShowFriendsCallback CompletionDelegate)
    {
        if (Handle == nullptr)
            return;

        auto pInst = reinterpret_cast<EOSSDK_UI*>(Handle);
        pInst->ShowFriends(Options, ClientData, CompletionDelegate);
    }

    EOS_DECLARE_FUNC(void) EOS_UI_HideFriends(EOS_HUI Handle, const EOS_UI_HideFriendsOptions* Options, void* ClientData, const EOS_UI_OnHideFriendsCallback CompletionDelegate)
    {
        if (Handle == nullptr)
            return;

        auto pInst = reinterpret_cast<EOSSDK_UI*>(Handle);
        pInst->HideFriends(Options, ClientData, CompletionDelegate);
    }

    EOS_DECLARE_FUNC(EOS_Bool) EOS_UI_GetFriendsVisible(EOS_HUI Handle, const EOS_UI_GetFriendsVisibleOptions* Options)
    {
        if (Handle == nullptr)
            return EOS_FALSE;

        auto pInst = reinterpret_cast<EOSSDK_UI*>(Handle);
        return pInst->GetFriendsVisible(Options);
    }

    EOS_DECLARE_FUNC(EOS_NotificationId) EOS_UI_AddNotifyDisplaySettingsUpdated(EOS_HUI Handle, const EOS_UI_AddNotifyDisplaySettingsUpdatedOptions* Options, void* ClientData, const EOS_UI_OnDisplaySettingsUpdatedCallback NotificationFn)
    {
        if (Handle == nullptr)
            return EOS_INVALID_NOTIFICATIONID;

        auto pInst = reinterpret_cast<EOSSDK_UI*>(Handle);
        return pInst->AddNotifyDisplaySettingsUpdated(Options, ClientData, NotificationFn);
    }

    EOS_DECLARE_FUNC(void) EOS_UI_RemoveNotifyDisplaySettingsUpdated(EOS_HUI Handle, EOS_NotificationId Id)
    {
        if (Handle == nullptr)
            return;

        auto pInst = reinterpret_cast<EOSSDK_UI*>(Handle);
        pInst->RemoveNotifyDisplaySettingsUpdated(Id);
    }

    EOS_DECLARE_FUNC(EOS_EResult) EOS_UI_SetToggleFriendsKey(EOS_HUI Handle, const EOS_UI_SetToggleFriendsKeyOptions* Options)
    {
        if (Handle == nullptr)
            return EOS_EResult::EOS_InvalidParameters;

        auto pInst = reinterpret_cast<EOSSDK_UI*>(Handle);
        return pInst->SetToggleFriendsKey(Options);
    }

    EOS_DECLARE_FUNC(EOS_UI_EKeyCombination) EOS_UI_GetToggleFriendsKey(EOS_HUI Handle, const EOS_UI_GetToggleFriendsKeyOptions* Options)
    {
        if (Handle == nullptr)
            return EOS_UI_EKeyCombination::EOS_UIK_None;

        auto pInst = reinterpret_cast<EOSSDK_UI*>(Handle);
        return pInst->GetToggleFriendsKey(Options);
    }

    EOS_DECLARE_FUNC(EOS_Bool) EOS_UI_IsValidKeyCombination(EOS_HUI Handle, EOS_UI_EKeyCombination KeyCombination)
    {
        if (Handle == nullptr)
            return EOS_FALSE;

        auto pInst = reinterpret_cast<EOSSDK_UI*>(Handle);
        return pInst->IsValidKeyCombination(KeyCombination);
    }

    EOS_DECLARE_FUNC(EOS_EResult) EOS_UI_SetDisplayPreference(EOS_HUI Handle, const EOS_UI_SetDisplayPreferenceOptions* Options)
    {
        if (Handle == nullptr)
            return EOS_EResult::EOS_InvalidParameters;

        auto pInst = reinterpret_cast<EOSSDK_UI*>(Handle);
        return pInst->SetDisplayPreference(Options);
    }

    EOS_DECLARE_FUNC(EOS_UI_ENotificationLocation) EOS_UI_GetNotificationLocationPreference(EOS_HUI Handle)
    {
        if (Handle == nullptr)
            return EOS_UI_ENotificationLocation::EOS_UNL_TopRight;

        auto pInst = reinterpret_cast<EOSSDK_UI*>(Handle);
        return pInst->GetNotificationLocationPreference();
    }

    EOS_DECLARE_FUNC(EOS_EResult) EOS_UI_AcknowledgeEventId(EOS_HUI Handle, const EOS_UI_AcknowledgeEventIdOptions* Options)
    {
        if (Handle == nullptr)
            return EOS_EResult::EOS_InvalidParameters;

        auto pInst = reinterpret_cast<EOSSDK_UI*>(Handle);
        return pInst->AcknowledgeEventId(Options);
    }

    EOS_DECLARE_FUNC(EOS_NotificationId) EOS_UI_AddNotifyFocusChanged(EOS_HUI Handle, const EOS_UI_AddNotifyFocusChangedOptions* Options, void* ClientData, const EOS_UI_OnFocusChangedCallback NotificationFn)
    { return static_cast<EOS_NotificationId>(1); }

    EOS_DECLARE_FUNC(void) EOS_UI_RemoveNotifyFocusChanged(EOS_HUI Handle, EOS_NotificationId Id) {}

    EOS_DECLARE_FUNC(EOS_NotificationId) EOS_UI_AddNotifyLoseFocusRequested(EOS_HUI Handle, const EOS_UI_AddNotifyLoseFocusRequestedOptions* Options, void* ClientData, const EOS_UI_OnLoseFocusRequestedCallback NotificationFn)
    { return static_cast<EOS_NotificationId>(1); }

    EOS_DECLARE_FUNC(void) EOS_UI_RemoveNotifyLoseFocusRequested(EOS_HUI Handle, EOS_NotificationId Id) {}

    EOS_DECLARE_FUNC(EOS_NotificationId) EOS_UI_AddNotifyMemoryMonitor(EOS_HUI Handle, const EOS_UI_AddNotifyMemoryMonitorOptions* Options, void* ClientData, const EOS_UI_OnMemoryMonitorCallback NotificationFn)
    { return static_cast<EOS_NotificationId>(1); }

    EOS_DECLARE_FUNC(void) EOS_UI_RemoveNotifyMemoryMonitor(EOS_HUI Handle, EOS_NotificationId Id) {}

    EOS_DECLARE_FUNC(EOS_NotificationId) EOS_UI_AddNotifyOnScreenKeyboardRequested(EOS_HUI Handle, const EOS_UI_AddNotifyOnScreenKeyboardRequestedOptions* Options, void* ClientData, const EOS_UI_OnScreenKeyboardRequestedCallback NotificationFn)
    { return static_cast<EOS_NotificationId>(1); }

    EOS_DECLARE_FUNC(void) EOS_UI_RemoveNotifyOnScreenKeyboardRequested(EOS_HUI Handle, EOS_NotificationId Id) {}

    EOS_DECLARE_FUNC(EOS_EResult) EOS_UI_ConfigureOnScreenKeyboard(EOS_HUI Handle, const EOS_UI_ConfigureOnScreenKeyboardOptions* Options)
    { return EOS_EResult::EOS_Success; }

    EOS_DECLARE_FUNC(EOS_Bool) EOS_UI_GetFriendsExclusiveInput(EOS_HUI Handle, const EOS_UI_GetFriendsExclusiveInputOptions* Options)
    { return EOS_FALSE; }

    EOS_DECLARE_FUNC(EOS_UI_EKeyCombination) EOS_UI_GetToggleFriendsButton(EOS_HUI Handle, const EOS_UI_GetToggleFriendsButtonOptions* Options)
    { return EOS_UIK_None; }

    EOS_DECLARE_FUNC(EOS_Bool) EOS_UI_IsSocialOverlayPaused(EOS_HUI Handle)
    { return EOS_FALSE; }

    EOS_DECLARE_FUNC(EOS_Bool) EOS_UI_IsValidButtonCombination(EOS_HUI Handle, EOS_UI_EKeyCombination ButtonCombination)
    { return EOS_TRUE; }

    EOS_DECLARE_FUNC(EOS_EResult) EOS_UI_PauseSocialOverlay(EOS_HUI Handle, const EOS_UI_PauseSocialOverlayOptions* Options)
    { return EOS_EResult::EOS_Success; }

    EOS_DECLARE_FUNC(EOS_EResult) EOS_UI_PrePresent(EOS_HUI Handle, const EOS_UI_PrePresentOptions* Options)
    { return EOS_EResult::EOS_Success; }

    EOS_DECLARE_FUNC(EOS_EResult) EOS_UI_PromptVoiceTerms(EOS_HUI Handle, const EOS_UI_PromptVoiceTermsOptions* Options)
    { return EOS_EResult::EOS_Success; }

    EOS_DECLARE_FUNC(void) EOS_UI_RegisterExternalFont(EOS_HUI Handle, const EOS_UI_RegisterExternalFontOptions* Options) {}

    EOS_DECLARE_FUNC(EOS_EResult) EOS_UI_ReportInputState(EOS_HUI Handle, const EOS_UI_ReportInputStateOptions* Options)
    { return EOS_EResult::EOS_Success; }

    EOS_DECLARE_FUNC(EOS_EResult) EOS_UI_SetFocus(EOS_HUI Handle, const EOS_UI_SetFocusOptions* Options)
    { return EOS_EResult::EOS_Success; }

    EOS_DECLARE_FUNC(EOS_EResult) EOS_UI_SetMode(EOS_HUI Handle, const EOS_UI_SetModeOptions* Options)
    { return EOS_EResult::EOS_Success; }

    EOS_DECLARE_FUNC(EOS_EResult) EOS_UI_SetToggleFriendsButton(EOS_HUI Handle, const EOS_UI_SetToggleFriendsButtonOptions* Options)
    { return EOS_EResult::EOS_Success; }

    EOS_DECLARE_FUNC(void) EOS_UI_ShowBlockPlayer(EOS_HUI Handle, const EOS_UI_ShowBlockPlayerOptions* Options, void* ClientData, const EOS_UI_OnShowBlockPlayerCallback CompletionDelegate)
    { if (CompletionDelegate) { EOS_UI_OnShowBlockPlayerCallbackInfo info{}; info.ResultCode = EOS_EResult::EOS_Success; info.ClientData = ClientData; CompletionDelegate(&info); } }

    EOS_DECLARE_FUNC(void) EOS_UI_ShowNativeProfile(EOS_HUI Handle, const EOS_UI_ShowNativeProfileOptions* Options, void* ClientData, const EOS_UI_OnShowNativeProfileCallback CompletionDelegate)
    { if (CompletionDelegate) { EOS_UI_ShowNativeProfileCallbackInfo info{}; info.ResultCode = EOS_EResult::EOS_Success; info.ClientData = ClientData; CompletionDelegate(&info); } }

    EOS_DECLARE_FUNC(void) EOS_UI_ShowReportPlayer(EOS_HUI Handle, const EOS_UI_ShowReportPlayerOptions* Options, void* ClientData, const EOS_UI_OnShowReportPlayerCallback CompletionDelegate)
    { if (CompletionDelegate) { EOS_UI_OnShowReportPlayerCallbackInfo info{}; info.ResultCode = EOS_EResult::EOS_Success; info.ClientData = ClientData; CompletionDelegate(&info); } }

    EOS_DECLARE_FUNC(EOS_EResult) EOS_UI_UpdateElements(EOS_HUI Handle, const EOS_UI_UpdateElementsOptions* Options)
    { return EOS_EResult::EOS_Success; }
}
