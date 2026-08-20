#include "sessions/eossdk_sessions.h"

using namespace sdk;

extern "C"
{
    EOS_DECLARE_FUNC(EOS_EResult) EOS_Sessions_CreateSessionModification(EOS_HSessions Handle, const EOS_Sessions_CreateSessionModificationOptions* Options, EOS_HSessionModification* OutSessionModificationHandle)
    {
        if (Handle == nullptr)
            return EOS_EResult::EOS_InvalidParameters;

        auto pInst = reinterpret_cast<EOSSDK_Sessions*>(Handle);
        return pInst->CreateSessionModification(Options, OutSessionModificationHandle);
    }

    EOS_DECLARE_FUNC(EOS_EResult) EOS_Sessions_UpdateSessionModification(EOS_HSessions Handle, const EOS_Sessions_UpdateSessionModificationOptions* Options, EOS_HSessionModification* OutSessionModificationHandle)
    {
        if (Handle == nullptr)
            return EOS_EResult::EOS_InvalidParameters;

        auto pInst = reinterpret_cast<EOSSDK_Sessions*>(Handle);
        return pInst->UpdateSessionModification(Options, OutSessionModificationHandle);
    }

    EOS_DECLARE_FUNC(void) EOS_Sessions_UpdateSession(EOS_HSessions Handle, const EOS_Sessions_UpdateSessionOptions* Options, void* ClientData, const EOS_Sessions_OnUpdateSessionCallback CompletionDelegate)
    {
        if (Handle == nullptr)
            return;

        auto pInst = reinterpret_cast<EOSSDK_Sessions*>(Handle);
        pInst->UpdateSession(Options, ClientData, CompletionDelegate);
    }

    EOS_DECLARE_FUNC(void) EOS_Sessions_DestroySession(EOS_HSessions Handle, const EOS_Sessions_DestroySessionOptions* Options, void* ClientData, const EOS_Sessions_OnDestroySessionCallback CompletionDelegate)
    {
        if (Handle == nullptr)
            return;

        auto pInst = reinterpret_cast<EOSSDK_Sessions*>(Handle);
        pInst->DestroySession(Options, ClientData, CompletionDelegate);
    }

    EOS_DECLARE_FUNC(void) EOS_Sessions_JoinSession(EOS_HSessions Handle, const EOS_Sessions_JoinSessionOptions* Options, void* ClientData, const EOS_Sessions_OnJoinSessionCallback CompletionDelegate)
    {
        if (Handle == nullptr)
            return;

        auto pInst = reinterpret_cast<EOSSDK_Sessions*>(Handle);
        pInst->JoinSession(Options, ClientData, CompletionDelegate);
    }

    EOS_DECLARE_FUNC(void) EOS_Sessions_StartSession(EOS_HSessions Handle, const EOS_Sessions_StartSessionOptions* Options, void* ClientData, const EOS_Sessions_OnStartSessionCallback CompletionDelegate)
    {
        if (Handle == nullptr)
            return;

        auto pInst = reinterpret_cast<EOSSDK_Sessions*>(Handle);
        pInst->StartSession(Options, ClientData, CompletionDelegate);
    }

    EOS_DECLARE_FUNC(void) EOS_Sessions_EndSession(EOS_HSessions Handle, const EOS_Sessions_EndSessionOptions* Options, void* ClientData, const EOS_Sessions_OnEndSessionCallback CompletionDelegate)
    {
        if (Handle == nullptr)
            return;

        auto pInst = reinterpret_cast<EOSSDK_Sessions*>(Handle);
        pInst->EndSession(Options, ClientData, CompletionDelegate);
    }

    EOS_DECLARE_FUNC(void) EOS_Sessions_RegisterPlayers(EOS_HSessions Handle, const EOS_Sessions_RegisterPlayersOptions* Options, void* ClientData, const EOS_Sessions_OnRegisterPlayersCallback CompletionDelegate)
    {
        if (Handle == nullptr)
            return;

        auto pInst = reinterpret_cast<EOSSDK_Sessions*>(Handle);
        pInst->RegisterPlayers(Options, ClientData, CompletionDelegate);
    }

    EOS_DECLARE_FUNC(void) EOS_Sessions_UnregisterPlayers(EOS_HSessions Handle, const EOS_Sessions_UnregisterPlayersOptions* Options, void* ClientData, const EOS_Sessions_OnUnregisterPlayersCallback CompletionDelegate)
    {
        if (Handle == nullptr)
            return;

        auto pInst = reinterpret_cast<EOSSDK_Sessions*>(Handle);
        pInst->UnregisterPlayers(Options, ClientData, CompletionDelegate);
    }

    EOS_DECLARE_FUNC(void) EOS_Sessions_SendInvite(EOS_HSessions Handle, const EOS_Sessions_SendInviteOptions* Options, void* ClientData, const EOS_Sessions_OnSendInviteCallback CompletionDelegate)
    {
        if (Handle == nullptr)
            return;

        auto pInst = reinterpret_cast<EOSSDK_Sessions*>(Handle);
        pInst->SendInvite(Options, ClientData, CompletionDelegate);
    }

    EOS_DECLARE_FUNC(void) EOS_Sessions_RejectInvite(EOS_HSessions Handle, const EOS_Sessions_RejectInviteOptions* Options, void* ClientData, const EOS_Sessions_OnRejectInviteCallback CompletionDelegate)
    {
        if (Handle == nullptr)
            return;

        auto pInst = reinterpret_cast<EOSSDK_Sessions*>(Handle);
        pInst->RejectInvite(Options, ClientData, CompletionDelegate);
    }

    EOS_DECLARE_FUNC(void) EOS_Sessions_QueryInvites(EOS_HSessions Handle, const EOS_Sessions_QueryInvitesOptions* Options, void* ClientData, const EOS_Sessions_OnQueryInvitesCallback CompletionDelegate)
    {
        if (Handle == nullptr)
            return;

        auto pInst = reinterpret_cast<EOSSDK_Sessions*>(Handle);
        pInst->QueryInvites(Options, ClientData, CompletionDelegate);
    }

    EOS_DECLARE_FUNC(uint32_t) EOS_Sessions_GetInviteCount(EOS_HSessions Handle, const EOS_Sessions_GetInviteCountOptions* Options)
    {
        if (Handle == nullptr)
            return 0;

        auto pInst = reinterpret_cast<EOSSDK_Sessions*>(Handle);
        return pInst->GetInviteCount(Options);
    }

    EOS_DECLARE_FUNC(EOS_EResult) EOS_Sessions_GetInviteIdByIndex(EOS_HSessions Handle, const EOS_Sessions_GetInviteIdByIndexOptions* Options, char* OutBuffer, int32_t* InOutBufferLength)
    {
        if (Handle == nullptr)
            return EOS_EResult::EOS_InvalidParameters;

        auto pInst = reinterpret_cast<EOSSDK_Sessions*>(Handle);
        return pInst->GetInviteIdByIndex(Options, OutBuffer, InOutBufferLength);
    }

    EOS_DECLARE_FUNC(EOS_EResult) EOS_Sessions_CreateSessionSearch(EOS_HSessions Handle, const EOS_Sessions_CreateSessionSearchOptions* Options, EOS_HSessionSearch* OutSessionSearchHandle)
    {
        if (Handle == nullptr)
            return EOS_EResult::EOS_InvalidParameters;

        auto pInst = reinterpret_cast<EOSSDK_Sessions*>(Handle);
        return pInst->CreateSessionSearch(Options, OutSessionSearchHandle);
    }

    EOS_DECLARE_FUNC(EOS_EResult) EOS_Sessions_CopyActiveSessionHandle(EOS_HSessions Handle, const EOS_Sessions_CopyActiveSessionHandleOptions* Options, EOS_HActiveSession* OutSessionHandle)
    {
        if (Handle == nullptr)
            return EOS_EResult::EOS_InvalidParameters;

        auto pInst = reinterpret_cast<EOSSDK_Sessions*>(Handle);
        return pInst->CopyActiveSessionHandle(Options, OutSessionHandle);
    }

    EOS_DECLARE_FUNC(EOS_NotificationId) EOS_Sessions_AddNotifySessionInviteReceived(EOS_HSessions Handle, const EOS_Sessions_AddNotifySessionInviteReceivedOptions* Options, void* ClientData, const EOS_Sessions_OnSessionInviteReceivedCallback NotificationFn)
    {    
        if (Handle == nullptr)
            return EOS_INVALID_NOTIFICATIONID;

        auto pInst = reinterpret_cast<EOSSDK_Sessions*>(Handle);
        return pInst->AddNotifySessionInviteReceived(Options, ClientData, NotificationFn);
    }

    EOS_DECLARE_FUNC(void) EOS_Sessions_RemoveNotifySessionInviteReceived(EOS_HSessions Handle, EOS_NotificationId InId)
    {
        if (Handle == nullptr)
            return;

        auto pInst = reinterpret_cast<EOSSDK_Sessions*>(Handle);
        pInst->RemoveNotifySessionInviteReceived(InId);
    }

    EOS_DECLARE_FUNC(EOS_NotificationId) EOS_Sessions_AddNotifySessionInviteAccepted(EOS_HSessions Handle, const EOS_Sessions_AddNotifySessionInviteAcceptedOptions* Options, void* ClientData, const EOS_Sessions_OnSessionInviteAcceptedCallback NotificationFn)
    {
        if (Handle == nullptr)
            return EOS_INVALID_NOTIFICATIONID;

        auto pInst = reinterpret_cast<EOSSDK_Sessions*>(Handle);
        return pInst->AddNotifySessionInviteAccepted(Options, ClientData, NotificationFn);
    }

    EOS_DECLARE_FUNC(void) EOS_Sessions_RemoveNotifySessionInviteAccepted(EOS_HSessions Handle, EOS_NotificationId InId)
    {
        if (Handle == nullptr)
            return;

        auto pInst = reinterpret_cast<EOSSDK_Sessions*>(Handle);
        pInst->RemoveNotifySessionInviteAccepted(InId);
    }

    EOS_DECLARE_FUNC(EOS_NotificationId) EOS_Sessions_AddNotifyJoinSessionAccepted(EOS_HSessions Handle, const EOS_Sessions_AddNotifyJoinSessionAcceptedOptions* Options, void* ClientData, const EOS_Sessions_OnJoinSessionAcceptedCallback NotificationFn)
    {
        if (Handle == nullptr)
            return EOS_INVALID_NOTIFICATIONID;

        auto pInst = reinterpret_cast<EOSSDK_Sessions*>(Handle);
        return pInst->AddNotifyJoinSessionAccepted(Options, ClientData, NotificationFn);
    }

    EOS_DECLARE_FUNC(void) EOS_Sessions_RemoveNotifyJoinSessionAccepted(EOS_HSessions Handle, EOS_NotificationId InId)
    {
        if (Handle == nullptr)
            return;

        auto pInst = reinterpret_cast<EOSSDK_Sessions*>(Handle);
        pInst->RemoveNotifyJoinSessionAccepted(InId);
    }

    EOS_DECLARE_FUNC(EOS_EResult) EOS_Sessions_CopySessionHandleByInviteId(EOS_HSessions Handle, const EOS_Sessions_CopySessionHandleByInviteIdOptions* Options, EOS_HSessionDetails* OutSessionHandle)
    {
        if (Handle == nullptr)
            return EOS_EResult::EOS_InvalidParameters;

        auto pInst = reinterpret_cast<EOSSDK_Sessions*>(Handle);
        return pInst->CopySessionHandleByInviteId(Options, OutSessionHandle);
    }

    EOS_DECLARE_FUNC(EOS_EResult) EOS_Sessions_CopySessionHandleByUiEventId(EOS_HSessions Handle, const EOS_Sessions_CopySessionHandleByUiEventIdOptions* Options, EOS_HSessionDetails* OutSessionHandle)
    {
        if (Handle == nullptr)
            return EOS_EResult::EOS_InvalidParameters;

        auto pInst = reinterpret_cast<EOSSDK_Sessions*>(Handle);
        return pInst->CopySessionHandleByUiEventId(Options, OutSessionHandle);
    }

    EOS_DECLARE_FUNC(EOS_EResult) EOS_Sessions_CopySessionHandleForPresence(EOS_HSessions Handle, const EOS_Sessions_CopySessionHandleForPresenceOptions* Options, EOS_HSessionDetails* OutSessionHandle)
    {
        if (Handle == nullptr)
            return EOS_EResult::EOS_InvalidParameters;

        auto pInst = reinterpret_cast<EOSSDK_Sessions*>(Handle);
        return pInst->CopySessionHandleForPresence(Options, OutSessionHandle);
    }

    EOS_DECLARE_FUNC(EOS_EResult) EOS_Sessions_IsUserInSession(EOS_HSessions Handle, const EOS_Sessions_IsUserInSessionOptions* Options)
    {
        if (Handle == nullptr)
            return EOS_EResult::EOS_InvalidParameters;

        auto pInst = reinterpret_cast<EOSSDK_Sessions*>(Handle);
        return pInst->IsUserInSession(Options);
    }

    EOS_DECLARE_FUNC(EOS_EResult) EOS_Sessions_DumpSessionState(EOS_HSessions Handle, const EOS_Sessions_DumpSessionStateOptions* Options)
    {
        if (Handle == nullptr)
            return EOS_EResult::EOS_InvalidParameters;

        auto pInst = reinterpret_cast<EOSSDK_Sessions*>(Handle);
        return pInst->DumpSessionState(Options);
    }

    EOS_DECLARE_FUNC(EOS_EResult) EOS_ActiveSession_CopyInfo(EOS_HActiveSession Handle, const EOS_ActiveSession_CopyInfoOptions* Options, EOS_ActiveSession_Info** OutActiveSessionInfo)
    {
        if (Handle == nullptr)
            return EOS_EResult::EOS_InvalidParameters;

        auto pInst = reinterpret_cast<EOSSDK_ActiveSession*>(Handle);
        return pInst->CopyInfo(Options, OutActiveSessionInfo);
    }

    EOS_DECLARE_FUNC(uint32_t) EOS_ActiveSession_GetRegisteredPlayerCount(EOS_HActiveSession Handle, const EOS_ActiveSession_GetRegisteredPlayerCountOptions* Options)
    {
        if (Handle == nullptr)
            return 0;

        auto pInst = reinterpret_cast<EOSSDK_ActiveSession*>(Handle);
        return pInst->GetRegisteredPlayerCount(Options);
    }

    EOS_DECLARE_FUNC(EOS_ProductUserId) EOS_ActiveSession_GetRegisteredPlayerByIndex(EOS_HActiveSession Handle, const EOS_ActiveSession_GetRegisteredPlayerByIndexOptions* Options)
    {
        if (Handle == nullptr)
            return nullptr;

        auto pInst = reinterpret_cast<EOSSDK_ActiveSession*>(Handle);
        return pInst->GetRegisteredPlayerByIndex(Options);
    }

    EOS_DECLARE_FUNC(EOS_EResult) EOS_SessionDetails_CopyInfo(EOS_HSessionDetails Handle, const EOS_SessionDetails_CopyInfoOptions* Options, EOS_SessionDetails_Info** OutSessionInfo)
    {
        if (Handle == nullptr)
            return EOS_EResult::EOS_InvalidParameters;

        auto pInst = reinterpret_cast<EOSSDK_SessionDetails*>(Handle);
        return pInst->CopyInfo(Options, OutSessionInfo);
    }

    EOS_DECLARE_FUNC(uint32_t) EOS_SessionDetails_GetSessionAttributeCount(EOS_HSessionDetails Handle, const EOS_SessionDetails_GetSessionAttributeCountOptions* Options)
    {
        if (Handle == nullptr)
            return 0;

        auto pInst = reinterpret_cast<EOSSDK_SessionDetails*>(Handle);
        return pInst->GetSessionAttributeCount(Options);
    }

    EOS_DECLARE_FUNC(EOS_EResult) EOS_SessionDetails_CopySessionAttributeByIndex(EOS_HSessionDetails Handle, const EOS_SessionDetails_CopySessionAttributeByIndexOptions* Options, EOS_SessionDetails_Attribute** OutSessionAttribute)
    {
        if (Handle == nullptr)
            return EOS_EResult::EOS_InvalidParameters;

        auto pInst = reinterpret_cast<EOSSDK_SessionDetails*>(Handle);
        return pInst->CopySessionAttributeByIndex(Options, OutSessionAttribute);
    }

    EOS_DECLARE_FUNC(EOS_EResult) EOS_SessionDetails_CopySessionAttributeByKey(EOS_HSessionDetails Handle, const EOS_SessionDetails_CopySessionAttributeByKeyOptions* Options, EOS_SessionDetails_Attribute** OutSessionAttribute)
    {
        if (Handle == nullptr)
            return EOS_EResult::EOS_InvalidParameters;

        auto pInst = reinterpret_cast<EOSSDK_SessionDetails*>(Handle);
        return pInst->CopySessionAttributeByKey(Options, OutSessionAttribute);
    }

    EOS_DECLARE_FUNC(EOS_EResult) EOS_SessionSearch_SetSessionId(EOS_HSessionSearch Handle, const EOS_SessionSearch_SetSessionIdOptions* Options)
    {
        if (Handle == nullptr)
            return EOS_EResult::EOS_InvalidParameters;

        auto pInst = reinterpret_cast<EOSSDK_SessionSearch*>(Handle);
        return pInst->SetSessionId(Options);
    }

    EOS_DECLARE_FUNC(EOS_EResult) EOS_SessionSearch_SetTargetUserId(EOS_HSessionSearch Handle, const EOS_SessionSearch_SetTargetUserIdOptions* Options)
    {
        if (Handle == nullptr)
            return EOS_EResult::EOS_InvalidParameters;

        auto pInst = reinterpret_cast<EOSSDK_SessionSearch*>(Handle);
        return pInst->SetTargetUserId(Options);
    }

    EOS_DECLARE_FUNC(EOS_EResult) EOS_SessionSearch_SetParameter(EOS_HSessionSearch Handle, const EOS_SessionSearch_SetParameterOptions* Options)
    {
        if (Handle == nullptr)
            return EOS_EResult::EOS_InvalidParameters;

        auto pInst = reinterpret_cast<EOSSDK_SessionSearch*>(Handle);
        return pInst->SetParameter(Options);
    }

    EOS_DECLARE_FUNC(EOS_EResult) EOS_SessionSearch_RemoveParameter(EOS_HSessionSearch Handle, const EOS_SessionSearch_RemoveParameterOptions* Options)
    {
        if (Handle == nullptr)
            return EOS_EResult::EOS_InvalidParameters;

        auto pInst = reinterpret_cast<EOSSDK_SessionSearch*>(Handle);
        return pInst->RemoveParameter(Options);
    }

    EOS_DECLARE_FUNC(EOS_EResult) EOS_SessionSearch_SetMaxResults(EOS_HSessionSearch Handle, const EOS_SessionSearch_SetMaxResultsOptions* Options)
    {
        if (Handle == nullptr)
            return EOS_EResult::EOS_InvalidParameters;

        auto pInst = reinterpret_cast<EOSSDK_SessionSearch*>(Handle);
        return pInst->SetMaxResults(Options);
    }

    EOS_DECLARE_FUNC(void) EOS_SessionSearch_Find(EOS_HSessionSearch Handle, const EOS_SessionSearch_FindOptions* Options, void* ClientData, const EOS_SessionSearch_OnFindCallback CompletionDelegate)
    {
        if (Handle == nullptr)
            return;

        auto pInst = reinterpret_cast<EOSSDK_SessionSearch*>(Handle);
        pInst->Find(Options, ClientData, CompletionDelegate);
    }

    EOS_DECLARE_FUNC(uint32_t) EOS_SessionSearch_GetSearchResultCount(EOS_HSessionSearch Handle, const EOS_SessionSearch_GetSearchResultCountOptions* Options)
    {
        if (Handle == nullptr)
            return 0;

        auto pInst = reinterpret_cast<EOSSDK_SessionSearch*>(Handle);
        return pInst->GetSearchResultCount(Options);
    }

    EOS_DECLARE_FUNC(EOS_EResult) EOS_SessionSearch_CopySearchResultByIndex(EOS_HSessionSearch Handle, const EOS_SessionSearch_CopySearchResultByIndexOptions* Options, EOS_HSessionDetails* OutSessionHandle)
    {
        if (Handle == nullptr)
            return EOS_EResult::EOS_InvalidParameters;

        auto pInst = reinterpret_cast<EOSSDK_SessionSearch*>(Handle);
        return pInst->CopySearchResultByIndex(Options, OutSessionHandle);
    }

    EOS_DECLARE_FUNC(void) EOS_ActiveSession_Release(EOS_HActiveSession Handle)
    {
        if (Handle == nullptr)
            return;

        auto pInst = reinterpret_cast<EOSSDK_ActiveSession*>(Handle);
        pInst->Release();
    }

    EOS_DECLARE_FUNC(void) EOS_SessionDetails_Release(EOS_HSessionDetails Handle)
    {
        if (Handle == nullptr)
            return;

        auto pInst = reinterpret_cast<EOSSDK_SessionDetails*>(Handle);
        pInst->Release();
    }

    EOS_DECLARE_FUNC(void) EOS_SessionSearch_Release(EOS_HSessionSearch Handle)
    {
        if (Handle == nullptr)
            return;

        auto pInst = reinterpret_cast<EOSSDK_SessionSearch*>(Handle);
        pInst->Release();
    }

    EOS_DECLARE_FUNC(void) EOS_SessionDetails_Attribute_Release(EOS_SessionDetails_Attribute* SessionAttribute)
    {
        if (SessionAttribute == nullptr)
            return;

        if(SessionAttribute->Data->ValueType == EOS_ESessionAttributeType::EOS_AT_STRING)
            delete[]SessionAttribute->Data->Value.AsUtf8;

        delete[]SessionAttribute->Data->Key;
        delete SessionAttribute->Data;
        delete SessionAttribute;
    }

    EOS_DECLARE_FUNC(void) EOS_SessionDetails_Info_Release(EOS_SessionDetails_Info* SessionInfo)
    {
        if (SessionInfo == nullptr)
            return;

        delete[] SessionInfo->Settings->BucketId;
        delete SessionInfo->Settings;
        delete[] SessionInfo->HostAddress;
        delete[] SessionInfo->SessionId;
        delete SessionInfo;
    }

    EOS_DECLARE_FUNC(void) EOS_ActiveSession_Info_Release(EOS_ActiveSession_Info* ActiveSessionInfo)
    {
        if (ActiveSessionInfo == nullptr)
            return;

        EOS_SessionDetails_Info_Release(const_cast<EOS_SessionDetails_Info*>(ActiveSessionInfo->SessionDetails));
        delete[]ActiveSessionInfo->SessionName;
        delete ActiveSessionInfo;
    }

    EOS_DECLARE_FUNC(EOS_NotificationId) EOS_Sessions_AddNotifyLeaveSessionRequested(EOS_HSessions Handle, const EOS_Sessions_AddNotifyLeaveSessionRequestedOptions* Options, void* ClientData, const EOS_Sessions_OnLeaveSessionRequestedCallback NotificationFn)
    { return static_cast<EOS_NotificationId>(1); }

    EOS_DECLARE_FUNC(void) EOS_Sessions_RemoveNotifyLeaveSessionRequested(EOS_HSessions Handle, EOS_NotificationId InId) {}

    EOS_DECLARE_FUNC(EOS_NotificationId) EOS_Sessions_AddNotifySendSessionNativeInviteRequested(EOS_HSessions Handle, const EOS_Sessions_AddNotifySendSessionNativeInviteRequestedOptions* Options, void* ClientData, const EOS_Sessions_OnSendSessionNativeInviteRequestedCallback NotificationFn)
    { return static_cast<EOS_NotificationId>(1); }

    EOS_DECLARE_FUNC(void) EOS_Sessions_RemoveNotifySendSessionNativeInviteRequested(EOS_HSessions Handle, EOS_NotificationId InId) {}

    EOS_DECLARE_FUNC(EOS_NotificationId) EOS_Sessions_AddNotifySessionInviteRejected(EOS_HSessions Handle, const EOS_Sessions_AddNotifySessionInviteRejectedOptions* Options, void* ClientData, const EOS_Sessions_OnSessionInviteRejectedCallback NotificationFn)
    { return static_cast<EOS_NotificationId>(1); }

    EOS_DECLARE_FUNC(void) EOS_Sessions_RemoveNotifySessionInviteRejected(EOS_HSessions Handle, EOS_NotificationId InId) {}
}
