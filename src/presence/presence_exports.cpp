#include "presence/eossdk_presence.h"

using namespace sdk;

extern "C"
{
    EOS_DECLARE_FUNC(void) EOS_Presence_QueryPresence(EOS_HPresence Handle, const EOS_Presence_QueryPresenceOptions* Options, void* ClientData, const EOS_Presence_OnQueryPresenceCompleteCallback CompletionDelegate)
    {
        if (Handle == nullptr)
            return;

        auto pInst = reinterpret_cast<EOSSDK_Presence*>(Handle);
        pInst->QueryPresence(Options, ClientData, CompletionDelegate);
    }

    EOS_DECLARE_FUNC(EOS_Bool) EOS_Presence_HasPresence(EOS_HPresence Handle, const EOS_Presence_HasPresenceOptions* Options)
    {
        if (Handle == nullptr)
            return EOS_FALSE;

        auto pInst = reinterpret_cast<EOSSDK_Presence*>(Handle);
        return pInst->HasPresence(Options);
    }

    EOS_DECLARE_FUNC(EOS_EResult) EOS_Presence_CopyPresence(EOS_HPresence Handle, const EOS_Presence_CopyPresenceOptions* Options, EOS_Presence_Info** OutPresence)
    {
        if (Handle == nullptr)
            return EOS_EResult::EOS_InvalidParameters;

        auto pInst = reinterpret_cast<EOSSDK_Presence*>(Handle);
        return pInst->CopyPresence(Options, OutPresence);
    }

    EOS_DECLARE_FUNC(void) EOS_Presence_Info_Release(EOS_Presence_Info* PresenceInfo)
    {
        if (PresenceInfo == nullptr)
            return;

        delete[]PresenceInfo->RichText;
        delete[]PresenceInfo->ProductName;
        for (int32_t i = 0; i < PresenceInfo->RecordsCount; ++i)
        {
            delete[]PresenceInfo->Records[i].Key;
            delete[]PresenceInfo->Records[i].Value;
        }
        delete[]PresenceInfo->Records;
        delete PresenceInfo;
    }

    EOS_DECLARE_FUNC(EOS_EResult) EOS_Presence_CreatePresenceModification(EOS_HPresence Handle, const EOS_Presence_CreatePresenceModificationOptions* Options, EOS_HPresenceModification* OutPresenceModificationHandle)
    {
        if (Handle == nullptr)
            return EOS_EResult::EOS_InvalidParameters;

        auto pInst = reinterpret_cast<EOSSDK_Presence*>(Handle);
        return pInst->CreatePresenceModification(Options, OutPresenceModificationHandle);
    }

    EOS_DECLARE_FUNC(void) EOS_Presence_SetPresence(EOS_HPresence Handle, const EOS_Presence_SetPresenceOptions* Options, void* ClientData, const EOS_Presence_SetPresenceCompleteCallback CompletionDelegate)
    {
        if (Handle == nullptr)
            return;

        auto pInst = reinterpret_cast<EOSSDK_Presence*>(Handle);
        pInst->SetPresence(Options, ClientData, CompletionDelegate);
    }

    EOS_DECLARE_FUNC(EOS_NotificationId) EOS_Presence_AddNotifyOnPresenceChanged(EOS_HPresence Handle, const EOS_Presence_AddNotifyOnPresenceChangedOptions* Options, void* ClientData, const EOS_Presence_OnPresenceChangedCallback NotificationFn)
    {
        if (Handle == nullptr)
            return EOS_INVALID_NOTIFICATIONID;

        auto pInst = reinterpret_cast<EOSSDK_Presence*>(Handle);
        return pInst->AddNotifyOnPresenceChanged(Options, ClientData, NotificationFn);
    }

    EOS_DECLARE_FUNC(void) EOS_Presence_RemoveNotifyOnPresenceChanged(EOS_HPresence Handle, EOS_NotificationId InId)
    {
        if (Handle == nullptr)
            return;

        auto pInst = reinterpret_cast<EOSSDK_Presence*>(Handle);
        pInst->RemoveNotifyOnPresenceChanged(InId);
    }

    EOS_DECLARE_FUNC(EOS_NotificationId) EOS_Presence_AddNotifyJoinGameAccepted(EOS_HPresence Handle, const EOS_Presence_AddNotifyJoinGameAcceptedOptions* Options, void* ClientData, const EOS_Presence_OnJoinGameAcceptedCallback NotificationFn)
    {
        if (Handle == nullptr)
            return EOS_INVALID_NOTIFICATIONID;

        auto pInst = reinterpret_cast<EOSSDK_Presence*>(Handle);
        return pInst->AddNotifyJoinGameAccepted(Options, ClientData, NotificationFn);
    }

    EOS_DECLARE_FUNC(void) EOS_Presence_RemoveNotifyJoinGameAccepted(EOS_HPresence Handle, EOS_NotificationId InId)
    {
        if (Handle == nullptr)
            return;

        auto pInst = reinterpret_cast<EOSSDK_Presence*>(Handle);
        pInst->RemoveNotifyJoinGameAccepted(InId);
    }

    EOS_DECLARE_FUNC(EOS_EResult) EOS_Presence_GetJoinInfo(EOS_HPresence Handle, const EOS_Presence_GetJoinInfoOptions* Options, char* OutBuffer, int32_t* InOutBufferLength)
    {
        if (Handle == nullptr)
            return EOS_EResult::EOS_InvalidParameters;

        auto pInst = reinterpret_cast<EOSSDK_Presence*>(Handle);
        return pInst->GetJoinInfo(Options, OutBuffer, InOutBufferLength);
    }

    EOS_DECLARE_FUNC(EOS_Bool) EOS_Presence_GetInvisible(EOS_HPresence Handle, const void* Options)
    {
        (void)Options;
        return EOS_FALSE;
    }
}
