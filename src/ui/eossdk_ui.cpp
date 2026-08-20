#include "eossdk_ui.h"
#include "eossdk_platform.h"

namespace sdk
{

EOSSDK_UI::EOSSDK_UI()
{}

EOSSDK_UI::~EOSSDK_UI()
{}

void EOSSDK_UI::ShowFriends(const EOS_UI_ShowFriendsOptions* Options, void* ClientData, const EOS_UI_OnShowFriendsCallback CompletionDelegate)
{
    TRACE_FUNC();

    SE_CB_BEGIN(EOS_UI_ShowFriendsCallbackInfo, CompletionDelegate);
    info.LocalUserId = Options->LocalUserId;
    info.ResultCode = EOS_EResult::EOS_Success;

    SE_CB_END(this);
}

void EOSSDK_UI::HideFriends(const EOS_UI_HideFriendsOptions* Options, void* ClientData, const EOS_UI_OnHideFriendsCallback CompletionDelegate)
{
    TRACE_FUNC();

    SE_CB_BEGIN(EOS_UI_HideFriendsCallbackInfo, CompletionDelegate);
    info.LocalUserId = Options->LocalUserId;
    info.ResultCode = EOS_EResult::EOS_Success;

    SE_CB_END(this);
}

EOS_Bool EOSSDK_UI::GetFriendsVisible(const EOS_UI_GetFriendsVisibleOptions* Options)
{
    TRACE_FUNC();

    if (Options == nullptr)
        return EOS_FALSE;

    return EOS_FALSE;
}

EOS_NotificationId EOSSDK_UI::AddNotifyDisplaySettingsUpdated(const EOS_UI_AddNotifyDisplaySettingsUpdatedOptions* Options, void* ClientData, const EOS_UI_OnDisplaySettingsUpdatedCallback NotificationFn)
{
    TRACE_FUNC();

    if (Options == nullptr || NotificationFn == nullptr)
        return EOS_INVALID_NOTIFICATIONID;

    pFrameResult_t res(new FrameResult);
    EOS_UI_OnDisplaySettingsUpdatedCallbackInfo& odsuci = res->CreateCallback<EOS_UI_OnDisplaySettingsUpdatedCallbackInfo>((CallbackFunc)NotificationFn);
    odsuci.ClientData = ClientData;
    odsuci.bIsExclusiveInput = false;
    odsuci.bIsVisible = false;

    return GetCB_Manager().add_notification(this, res);
}

void EOSSDK_UI::RemoveNotifyDisplaySettingsUpdated(EOS_NotificationId Id)
{
    TRACE_FUNC();

    GetCB_Manager().remove_notification(this, Id);
}

EOS_EResult EOSSDK_UI::SetToggleFriendsKey(const EOS_UI_SetToggleFriendsKeyOptions* Options)
{
    TRACE_FUNC();

    if (Options == nullptr || !IsValidKeyCombination(Options->KeyCombination))
        return EOS_EResult::EOS_InvalidParameters;

    return EOS_EResult::EOS_Success;
}

EOS_UI_EKeyCombination EOSSDK_UI::GetToggleFriendsKey(const EOS_UI_GetToggleFriendsKeyOptions* Options)
{
    TRACE_FUNC();

    return static_cast<EOS_UI_EKeyCombination>(EOS_UI_EKeyCombination::EOS_UIK_ModifierShift | EOS_UI_EKeyCombination::EOS_UIK_F2);
}

EOS_Bool EOSSDK_UI::IsValidKeyCombination(EOS_UI_EKeyCombination KeyCombination)
{
    TRACE_FUNC();

    return (KeyCombination & EOS_UI_EKeyCombination::EOS_UIK_ValidModifierMask) == KeyCombination ? EOS_FALSE : EOS_TRUE;
}

EOS_EResult EOSSDK_UI::SetDisplayPreference(const EOS_UI_SetDisplayPreferenceOptions* Options)
{
    TRACE_FUNC();

    if (Options == nullptr)
        return EOS_EResult::EOS_InvalidParameters;

    return EOS_EResult::EOS_InvalidParameters;
}

EOS_UI_ENotificationLocation EOSSDK_UI::GetNotificationLocationPreference()
{
    TRACE_FUNC();

    return EOS_UI_ENotificationLocation::EOS_UNL_BottomLeft;
}

EOS_EResult EOSSDK_UI::AcknowledgeEventId(const EOS_UI_AcknowledgeEventIdOptions* Options)
{
    TRACE_FUNC();

    if (Options == nullptr)
        return EOS_EResult::EOS_InvalidParameters;

    return EOS_EResult::EOS_Success;
}

bool EOSSDK_UI::CBRunFrame()
{
    return false;
}

bool EOSSDK_UI::RunCallbacks(pFrameResult_t res)
{
    lock _(GLOBAL_MUTEX);

    return res->done;
}

void EOSSDK_UI::FreeCallback(pFrameResult_t res)
{
    lock _(GLOBAL_MUTEX);

    {

    }
}

}