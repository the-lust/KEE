#pragma once

#include "common_includes.h"
#include "callback_manager.h"

namespace sdk
{
    class EOSSDK_UI :
        public IRunCallback
    {
    public:
        EOSSDK_UI();
        ~EOSSDK_UI();

        virtual bool CBRunFrame();
        virtual bool RunCallbacks(pFrameResult_t res);
        virtual void FreeCallback(pFrameResult_t res);

        void                         ShowFriends(const EOS_UI_ShowFriendsOptions* Options, void* ClientData, const EOS_UI_OnShowFriendsCallback CompletionDelegate);
        void                         HideFriends(const EOS_UI_HideFriendsOptions* Options, void* ClientData, const EOS_UI_OnHideFriendsCallback CompletionDelegate);
        EOS_Bool                     GetFriendsVisible(const EOS_UI_GetFriendsVisibleOptions* Options);
        EOS_NotificationId           AddNotifyDisplaySettingsUpdated(const EOS_UI_AddNotifyDisplaySettingsUpdatedOptions* Options, void* ClientData, const EOS_UI_OnDisplaySettingsUpdatedCallback NotificationFn);
        void                         RemoveNotifyDisplaySettingsUpdated(EOS_NotificationId Id);
        EOS_EResult                  SetToggleFriendsKey(const EOS_UI_SetToggleFriendsKeyOptions* Options);
        EOS_UI_EKeyCombination       GetToggleFriendsKey(const EOS_UI_GetToggleFriendsKeyOptions* Options);
        EOS_Bool                     IsValidKeyCombination(EOS_UI_EKeyCombination KeyCombination);
        EOS_EResult                  SetDisplayPreference(const EOS_UI_SetDisplayPreferenceOptions* Options);
        EOS_UI_ENotificationLocation GetNotificationLocationPreference();
        EOS_EResult                  AcknowledgeEventId(const EOS_UI_AcknowledgeEventIdOptions* Options);
    };
}
