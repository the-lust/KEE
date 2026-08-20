#include "lobby/eossdk_lobby.h"

using namespace sdk;

extern "C"
{
    EOS_DECLARE_FUNC(void) EOS_Lobby_CreateLobby(EOS_HLobby Handle, const EOS_Lobby_CreateLobbyOptions* Options, void* ClientData, const EOS_Lobby_OnCreateLobbyCallback CompletionDelegate)
    {
        if (Handle == nullptr)
            return;

        auto pInst = reinterpret_cast<EOSSDK_Lobby*>(Handle);
        pInst->CreateLobby(Options, ClientData, CompletionDelegate);
    }

    EOS_DECLARE_FUNC(void) EOS_Lobby_DestroyLobby(EOS_HLobby Handle, const EOS_Lobby_DestroyLobbyOptions* Options, void* ClientData, const EOS_Lobby_OnDestroyLobbyCallback CompletionDelegate)
    {
        if (Handle == nullptr)
            return;

        auto pInst = reinterpret_cast<EOSSDK_Lobby*>(Handle);
        pInst->DestroyLobby(Options, ClientData, CompletionDelegate);
    }

    EOS_DECLARE_FUNC(void) EOS_Lobby_JoinLobby(EOS_HLobby Handle, const EOS_Lobby_JoinLobbyOptions* Options, void* ClientData, const EOS_Lobby_OnJoinLobbyCallback CompletionDelegate)
    {
        if (Handle == nullptr)
            return;

        auto pInst = reinterpret_cast<EOSSDK_Lobby*>(Handle);
        pInst->JoinLobby(Options, ClientData, CompletionDelegate);
    }

    EOS_DECLARE_FUNC(void) EOS_Lobby_LeaveLobby(EOS_HLobby Handle, const EOS_Lobby_LeaveLobbyOptions* Options, void* ClientData, const EOS_Lobby_OnLeaveLobbyCallback CompletionDelegate)
    {
        if (Handle == nullptr)
            return;

        auto pInst = reinterpret_cast<EOSSDK_Lobby*>(Handle);
        pInst->LeaveLobby(Options, ClientData, CompletionDelegate);
    }

    EOS_DECLARE_FUNC(EOS_EResult) EOS_Lobby_UpdateLobbyModification(EOS_HLobby Handle, const EOS_Lobby_UpdateLobbyModificationOptions* Options, EOS_HLobbyModification* OutLobbyModificationHandle)
    {
        if (Handle == nullptr)
            return EOS_EResult::EOS_InvalidParameters;

        auto pInst = reinterpret_cast<EOSSDK_Lobby*>(Handle);
        return pInst->UpdateLobbyModification(Options, OutLobbyModificationHandle);
    }

    EOS_DECLARE_FUNC(void) EOS_Lobby_UpdateLobby(EOS_HLobby Handle, const EOS_Lobby_UpdateLobbyOptions* Options, void* ClientData, const EOS_Lobby_OnUpdateLobbyCallback CompletionDelegate)
    {
        if (Handle == nullptr)
            return;

        auto pInst = reinterpret_cast<EOSSDK_Lobby*>(Handle);
        pInst->UpdateLobby(Options, ClientData, CompletionDelegate);
    }

    EOS_DECLARE_FUNC(void) EOS_Lobby_PromoteMember(EOS_HLobby Handle, const EOS_Lobby_PromoteMemberOptions* Options, void* ClientData, const EOS_Lobby_OnPromoteMemberCallback CompletionDelegate)
    {
        if (Handle == nullptr)
            return;

        auto pInst = reinterpret_cast<EOSSDK_Lobby*>(Handle);
        pInst->PromoteMember(Options, ClientData, CompletionDelegate);
    }

    EOS_DECLARE_FUNC(void) EOS_Lobby_KickMember(EOS_HLobby Handle, const EOS_Lobby_KickMemberOptions* Options, void* ClientData, const EOS_Lobby_OnKickMemberCallback CompletionDelegate)
    {
        if (Handle == nullptr)
            return;

        auto pInst = reinterpret_cast<EOSSDK_Lobby*>(Handle);
        pInst->KickMember(Options, ClientData, CompletionDelegate);
    }

    EOS_DECLARE_FUNC(EOS_NotificationId) EOS_Lobby_AddNotifyLobbyUpdateReceived(EOS_HLobby Handle, const EOS_Lobby_AddNotifyLobbyUpdateReceivedOptions* Options, void* ClientData, const EOS_Lobby_OnLobbyUpdateReceivedCallback NotificationFn)
    {
        if (Handle == nullptr)
            return EOS_INVALID_NOTIFICATIONID;

        auto pInst = reinterpret_cast<EOSSDK_Lobby*>(Handle);
        return pInst->AddNotifyLobbyUpdateReceived(Options, ClientData, NotificationFn);
    }

    EOS_DECLARE_FUNC(void) EOS_Lobby_RemoveNotifyLobbyUpdateReceived(EOS_HLobby Handle, EOS_NotificationId InId)
    {
        if (Handle == nullptr)
            return;

        auto pInst = reinterpret_cast<EOSSDK_Lobby*>(Handle);
        pInst->RemoveNotifyLobbyUpdateReceived(InId);
    }

    EOS_DECLARE_FUNC(EOS_NotificationId) EOS_Lobby_AddNotifyLobbyMemberUpdateReceived(EOS_HLobby Handle, const EOS_Lobby_AddNotifyLobbyMemberUpdateReceivedOptions* Options, void* ClientData, const EOS_Lobby_OnLobbyMemberUpdateReceivedCallback NotificationFn)
    {
        if (Handle == nullptr)
            return EOS_INVALID_NOTIFICATIONID;

        auto pInst = reinterpret_cast<EOSSDK_Lobby*>(Handle);
        return pInst->AddNotifyLobbyMemberUpdateReceived(Options, ClientData, NotificationFn);
    }

    EOS_DECLARE_FUNC(void) EOS_Lobby_RemoveNotifyLobbyMemberUpdateReceived(EOS_HLobby Handle, EOS_NotificationId InId)
    {
        if (Handle == nullptr)
            return;

        auto pInst = reinterpret_cast<EOSSDK_Lobby*>(Handle);
        pInst->RemoveNotifyLobbyMemberUpdateReceived(InId);
    }

    EOS_DECLARE_FUNC(EOS_NotificationId) EOS_Lobby_AddNotifyLobbyMemberStatusReceived(EOS_HLobby Handle, const EOS_Lobby_AddNotifyLobbyMemberStatusReceivedOptions* Options, void* ClientData, const EOS_Lobby_OnLobbyMemberStatusReceivedCallback NotificationFn)
    {
        if (Handle == nullptr)
            return EOS_INVALID_NOTIFICATIONID;

        auto pInst = reinterpret_cast<EOSSDK_Lobby*>(Handle);
        return pInst->AddNotifyLobbyMemberStatusReceived(Options, ClientData, NotificationFn);
    }

    EOS_DECLARE_FUNC(void) EOS_Lobby_RemoveNotifyLobbyMemberStatusReceived(EOS_HLobby Handle, EOS_NotificationId InId)
    {
        if (Handle == nullptr)
            return;

        auto pInst = reinterpret_cast<EOSSDK_Lobby*>(Handle);
        pInst->RemoveNotifyLobbyMemberStatusReceived(InId);
    }

    EOS_DECLARE_FUNC(void) EOS_Lobby_SendInvite(EOS_HLobby Handle, const EOS_Lobby_SendInviteOptions* Options, void* ClientData, const EOS_Lobby_OnSendInviteCallback CompletionDelegate)
    {
        if (Handle == nullptr)
            return;

        auto pInst = reinterpret_cast<EOSSDK_Lobby*>(Handle);
        pInst->SendInvite(Options, ClientData, CompletionDelegate);
    }

    EOS_DECLARE_FUNC(void) EOS_Lobby_RejectInvite(EOS_HLobby Handle, const EOS_Lobby_RejectInviteOptions* Options, void* ClientData, const EOS_Lobby_OnRejectInviteCallback CompletionDelegate)
    {
        if (Handle == nullptr)
            return;

        auto pInst = reinterpret_cast<EOSSDK_Lobby*>(Handle);
        pInst->RejectInvite(Options, ClientData, CompletionDelegate);
    }

    EOS_DECLARE_FUNC(void) EOS_Lobby_QueryInvites(EOS_HLobby Handle, const EOS_Lobby_QueryInvitesOptions* Options, void* ClientData, const EOS_Lobby_OnQueryInvitesCallback CompletionDelegate)
    {
        if (Handle == nullptr)
            return;

        auto pInst = reinterpret_cast<EOSSDK_Lobby*>(Handle);
        pInst->QueryInvites(Options, ClientData, CompletionDelegate);
    }

    EOS_DECLARE_FUNC(uint32_t) EOS_Lobby_GetInviteCount(EOS_HLobby Handle, const EOS_Lobby_GetInviteCountOptions* Options)
    {
        if (Handle == nullptr)
            return 0;

        auto pInst = reinterpret_cast<EOSSDK_Lobby*>(Handle);
        return pInst->GetInviteCount(Options);
    }

    EOS_DECLARE_FUNC(EOS_EResult) EOS_Lobby_GetInviteIdByIndex(EOS_HLobby Handle, const EOS_Lobby_GetInviteIdByIndexOptions* Options, char* OutBuffer, int32_t* InOutBufferLength)
    {
        if (Handle == nullptr)
            return EOS_EResult::EOS_InvalidParameters;

        auto pInst = reinterpret_cast<EOSSDK_Lobby*>(Handle);
        return pInst->GetInviteIdByIndex(Options, OutBuffer, InOutBufferLength);
    }

    EOS_DECLARE_FUNC(EOS_EResult) EOS_Lobby_CreateLobbySearch(EOS_HLobby Handle, const EOS_Lobby_CreateLobbySearchOptions* Options, EOS_HLobbySearch* OutLobbySearchHandle)
    {
        if (Handle == nullptr)
            return EOS_EResult::EOS_InvalidParameters;

        auto pInst = reinterpret_cast<EOSSDK_Lobby*>(Handle);
        return pInst->CreateLobbySearch(Options, OutLobbySearchHandle);
    }

    EOS_DECLARE_FUNC(EOS_NotificationId) EOS_Lobby_AddNotifyLobbyInviteReceived(EOS_HLobby Handle, const EOS_Lobby_AddNotifyLobbyInviteReceivedOptions* Options, void* ClientData, const EOS_Lobby_OnLobbyInviteReceivedCallback NotificationFn)
    {
        if (Handle == nullptr)
            return EOS_INVALID_NOTIFICATIONID;

        auto pInst = reinterpret_cast<EOSSDK_Lobby*>(Handle);
        return pInst->AddNotifyLobbyInviteReceived(Options, ClientData, NotificationFn);
    }

    EOS_DECLARE_FUNC(void) EOS_Lobby_RemoveNotifyLobbyInviteReceived(EOS_HLobby Handle, EOS_NotificationId InId)
    {
        if (Handle == nullptr)
            return;

        auto pInst = reinterpret_cast<EOSSDK_Lobby*>(Handle);
        pInst->RemoveNotifyLobbyInviteReceived(InId);
    }

    EOS_DECLARE_FUNC(EOS_NotificationId) EOS_Lobby_AddNotifyLobbyInviteAccepted(EOS_HLobby Handle, const EOS_Lobby_AddNotifyLobbyInviteAcceptedOptions* Options, void* ClientData, const EOS_Lobby_OnLobbyInviteAcceptedCallback NotificationFn)
    {
        if (Handle == nullptr)
            return EOS_INVALID_NOTIFICATIONID;

        auto pInst = reinterpret_cast<EOSSDK_Lobby*>(Handle);
        return pInst->AddNotifyLobbyInviteAccepted(Options, ClientData, NotificationFn);
    }

    EOS_DECLARE_FUNC(void) EOS_Lobby_RemoveNotifyLobbyInviteAccepted(EOS_HLobby Handle, EOS_NotificationId InId)
    {
        if (Handle == nullptr)
            return;

        auto pInst = reinterpret_cast<EOSSDK_Lobby*>(Handle);
        pInst->RemoveNotifyLobbyInviteAccepted(InId);
    }

    EOS_DECLARE_FUNC(EOS_NotificationId) EOS_Lobby_AddNotifyJoinLobbyAccepted(EOS_HLobby Handle, const EOS_Lobby_AddNotifyJoinLobbyAcceptedOptions* Options, void* ClientData, const EOS_Lobby_OnJoinLobbyAcceptedCallback NotificationFn)
    {
        if (Handle == nullptr)
            return EOS_INVALID_NOTIFICATIONID;

        auto pInst = reinterpret_cast<EOSSDK_Lobby*>(Handle);
        return pInst->AddNotifyJoinLobbyAccepted(Options, ClientData, NotificationFn);
    }

    EOS_DECLARE_FUNC(void) EOS_Lobby_RemoveNotifyJoinLobbyAccepted(EOS_HLobby Handle, EOS_NotificationId InId)
    {
        if (Handle == nullptr)
            return;

        auto pInst = reinterpret_cast<EOSSDK_Lobby*>(Handle);
        pInst->RemoveNotifyJoinLobbyAccepted(InId);
    }

    EOS_DECLARE_FUNC(EOS_EResult) EOS_Lobby_CopyLobbyDetailsHandleByInviteId(EOS_HLobby Handle, const EOS_Lobby_CopyLobbyDetailsHandleByInviteIdOptions* Options, EOS_HLobbyDetails* OutLobbyDetailsHandle)
    {
        if (Handle == nullptr)
            return EOS_EResult::EOS_InvalidParameters;

        auto pInst = reinterpret_cast<EOSSDK_Lobby*>(Handle);
        return pInst->CopyLobbyDetailsHandleByInviteId(Options, OutLobbyDetailsHandle);
    }

    EOS_DECLARE_FUNC(EOS_EResult) EOS_Lobby_CopyLobbyDetailsHandleByUiEventId(EOS_HLobby Handle, const EOS_Lobby_CopyLobbyDetailsHandleByUiEventIdOptions* Options, EOS_HLobbyDetails* OutLobbyDetailsHandle)
    {
        if (Handle == nullptr)
            return EOS_EResult::EOS_InvalidParameters;

        auto pInst = reinterpret_cast<EOSSDK_Lobby*>(Handle);
        return pInst->CopyLobbyDetailsHandleByUiEventId(Options, OutLobbyDetailsHandle);
    }

    EOS_DECLARE_FUNC(EOS_EResult) EOS_Lobby_CopyLobbyDetailsHandle(EOS_HLobby Handle, const EOS_Lobby_CopyLobbyDetailsHandleOptions* Options, EOS_HLobbyDetails* OutLobbyDetailsHandle)
    {
        if (Handle == nullptr)
            return EOS_EResult::EOS_InvalidParameters;

        auto pInst = reinterpret_cast<EOSSDK_Lobby*>(Handle);
        return pInst->CopyLobbyDetailsHandle(Options, OutLobbyDetailsHandle);
    }

    EOS_DECLARE_FUNC(void) EOS_LobbySearch_Find(EOS_HLobbySearch Handle, const EOS_LobbySearch_FindOptions* Options, void* ClientData, const EOS_LobbySearch_OnFindCallback CompletionDelegate)
    {
        if (Handle == nullptr)
            return;

        auto pInst = reinterpret_cast<EOSSDK_LobbySearch*>(Handle);
        pInst->Find(Options, ClientData, CompletionDelegate);
    }

    EOS_DECLARE_FUNC(EOS_EResult) EOS_LobbySearch_SetLobbyId(EOS_HLobbySearch Handle, const EOS_LobbySearch_SetLobbyIdOptions* Options)
    {
        if (Handle == nullptr)
            return EOS_EResult::EOS_InvalidParameters;

        auto pInst = reinterpret_cast<EOSSDK_LobbySearch*>(Handle);
        return pInst->SetLobbyId(Options);
    }

    EOS_DECLARE_FUNC(EOS_EResult) EOS_LobbySearch_SetTargetUserId(EOS_HLobbySearch Handle, const EOS_LobbySearch_SetTargetUserIdOptions* Options)
    {
        if (Handle == nullptr)
            return EOS_EResult::EOS_InvalidParameters;

        auto pInst = reinterpret_cast<EOSSDK_LobbySearch*>(Handle);
        return pInst->SetTargetUserId(Options);
    }

    EOS_DECLARE_FUNC(EOS_EResult) EOS_LobbySearch_SetParameter(EOS_HLobbySearch Handle, const EOS_LobbySearch_SetParameterOptions* Options)
    {
        if (Handle == nullptr)
            return EOS_EResult::EOS_InvalidParameters;

        auto pInst = reinterpret_cast<EOSSDK_LobbySearch*>(Handle);
        return pInst->SetParameter(Options);
    }

    EOS_DECLARE_FUNC(EOS_EResult) EOS_LobbySearch_RemoveParameter(EOS_HLobbySearch Handle, const EOS_LobbySearch_RemoveParameterOptions* Options)
    {
        if (Handle == nullptr)
            return EOS_EResult::EOS_InvalidParameters;

        auto pInst = reinterpret_cast<EOSSDK_LobbySearch*>(Handle);
        return pInst->RemoveParameter(Options);
    }

    EOS_DECLARE_FUNC(EOS_EResult) EOS_LobbySearch_SetMaxResults(EOS_HLobbySearch Handle, const EOS_LobbySearch_SetMaxResultsOptions* Options)
    {
        if (Handle == nullptr)
            return EOS_EResult::EOS_InvalidParameters;

        auto pInst = reinterpret_cast<EOSSDK_LobbySearch*>(Handle);
        return pInst->SetMaxResults(Options);
    }

    EOS_DECLARE_FUNC(uint32_t) EOS_LobbySearch_GetSearchResultCount(EOS_HLobbySearch Handle, const EOS_LobbySearch_GetSearchResultCountOptions* Options)
    {
        if (Handle == nullptr)
            return 0;

        auto pInst = reinterpret_cast<EOSSDK_LobbySearch*>(Handle);
        return pInst->GetSearchResultCount(Options);
    }

    EOS_DECLARE_FUNC(EOS_EResult) EOS_LobbySearch_CopySearchResultByIndex(EOS_HLobbySearch Handle, const EOS_LobbySearch_CopySearchResultByIndexOptions* Options, EOS_HLobbyDetails* OutLobbyDetailsHandle)
    {
        if (Handle == nullptr)
            return EOS_EResult::EOS_InvalidParameters;

        auto pInst = reinterpret_cast<EOSSDK_LobbySearch*>(Handle);
        return pInst->CopySearchResultByIndex(Options, OutLobbyDetailsHandle);
    }

    EOS_DECLARE_FUNC(void) EOS_LobbySearch_Release(EOS_HLobbySearch LobbySearchHandle)
    {
        if (LobbySearchHandle == nullptr)
            return;

        auto pInst = reinterpret_cast<EOSSDK_LobbySearch*>(LobbySearchHandle);
        pInst->Release();
    }

    EOS_DECLARE_FUNC(void) EOS_Lobby_Attribute_Release(EOS_Lobby_Attribute* LobbyAttribute)
    {
        if (LobbyAttribute == nullptr)
            return;

        if (LobbyAttribute->Data->ValueType == EOS_AT_STRING)
            delete[] LobbyAttribute->Data->Value.AsUtf8;

        delete[] LobbyAttribute->Data->Key;
        delete LobbyAttribute->Data;
        delete LobbyAttribute;
    }

    EOS_DECLARE_FUNC(EOS_NotificationId) EOS_Lobby_AddNotifyLeaveLobbyRequested(EOS_HLobby Handle, const EOS_Lobby_AddNotifyLeaveLobbyRequestedOptions* Options, void* ClientData, const EOS_Lobby_OnLeaveLobbyRequestedCallback NotificationFn)
    { return static_cast<EOS_NotificationId>(1); }

    EOS_DECLARE_FUNC(void) EOS_Lobby_RemoveNotifyLeaveLobbyRequested(EOS_HLobby Handle, EOS_NotificationId InId) {}

    EOS_DECLARE_FUNC(EOS_NotificationId) EOS_Lobby_AddNotifyLobbyInviteRejected(EOS_HLobby Handle, const EOS_Lobby_AddNotifyLobbyInviteRejectedOptions* Options, void* ClientData, const EOS_Lobby_OnLobbyInviteRejectedCallback NotificationFn)
    { return static_cast<EOS_NotificationId>(1); }

    EOS_DECLARE_FUNC(void) EOS_Lobby_RemoveNotifyLobbyInviteRejected(EOS_HLobby Handle, EOS_NotificationId InId) {}

    EOS_DECLARE_FUNC(EOS_NotificationId) EOS_Lobby_AddNotifyRTCRoomConnectionChanged(EOS_HLobby Handle, const EOS_Lobby_AddNotifyRTCRoomConnectionChangedOptions* Options, void* ClientData, const EOS_Lobby_OnRTCRoomConnectionChangedCallback NotificationFn)
    { return static_cast<EOS_NotificationId>(1); }

    EOS_DECLARE_FUNC(void) EOS_Lobby_RemoveNotifyRTCRoomConnectionChanged(EOS_HLobby Handle, EOS_NotificationId InId) {}

    EOS_DECLARE_FUNC(EOS_NotificationId) EOS_Lobby_AddNotifySendLobbyNativeInviteRequested(EOS_HLobby Handle, const EOS_Lobby_AddNotifySendLobbyNativeInviteRequestedOptions* Options, void* ClientData, const EOS_Lobby_OnSendLobbyNativeInviteRequestedCallback NotificationFn)
    { return static_cast<EOS_NotificationId>(1); }

    EOS_DECLARE_FUNC(void) EOS_Lobby_RemoveNotifySendLobbyNativeInviteRequested(EOS_HLobby Handle, EOS_NotificationId InId) {}

    EOS_DECLARE_FUNC(EOS_EResult) EOS_Lobby_GetConnectString(EOS_HLobby Handle, const EOS_Lobby_GetConnectStringOptions* Options, char* OutBuffer, uint32_t* InOutBufferLength)
    { if (InOutBufferLength) *InOutBufferLength = 0; return EOS_EResult::EOS_NotImplemented; }

    EOS_DECLARE_FUNC(EOS_EResult) EOS_Lobby_GetRTCRoomName(EOS_HLobby Handle, const EOS_Lobby_GetRTCRoomNameOptions* Options, char* OutBuffer, uint32_t* InOutBufferLength)
    { if (InOutBufferLength) *InOutBufferLength = 0; return EOS_EResult::EOS_NotImplemented; }

    EOS_DECLARE_FUNC(void) EOS_Lobby_HardMuteMember(EOS_HLobby Handle, const EOS_Lobby_HardMuteMemberOptions* Options, void* ClientData, const EOS_Lobby_OnHardMuteMemberCallback CompletionDelegate)
    { if (CompletionDelegate) { EOS_Lobby_HardMuteMemberCallbackInfo info{}; info.ResultCode = EOS_EResult::EOS_Success; info.ClientData = ClientData; CompletionDelegate(&info); } }

    EOS_DECLARE_FUNC(EOS_EResult) EOS_Lobby_IsRTCRoomConnected(EOS_HLobby Handle, const EOS_Lobby_IsRTCRoomConnectedOptions* Options, EOS_Bool* bOutIsConnected)
    { if (bOutIsConnected) *bOutIsConnected = EOS_FALSE; return EOS_EResult::EOS_Success; }

    EOS_DECLARE_FUNC(void) EOS_Lobby_JoinLobbyById(EOS_HLobby Handle, const EOS_Lobby_JoinLobbyByIdOptions* Options, void* ClientData, const EOS_Lobby_OnJoinLobbyByIdCallback CompletionDelegate)
    { if (CompletionDelegate) { EOS_Lobby_JoinLobbyByIdCallbackInfo info{}; info.ResultCode = EOS_EResult::EOS_Success; info.ClientData = ClientData; CompletionDelegate(&info); } }

    EOS_DECLARE_FUNC(void) EOS_Lobby_JoinRTCRoom(EOS_HLobby Handle, const EOS_Lobby_JoinRTCRoomOptions* Options, void* ClientData, const EOS_Lobby_OnJoinRTCRoomCallback CompletionDelegate)
    { if (CompletionDelegate) { EOS_Lobby_JoinRTCRoomCallbackInfo info{}; info.ResultCode = EOS_EResult::EOS_Success; info.ClientData = ClientData; CompletionDelegate(&info); } }

    EOS_DECLARE_FUNC(void) EOS_Lobby_LeaveRTCRoom(EOS_HLobby Handle, const EOS_Lobby_LeaveRTCRoomOptions* Options, void* ClientData, const EOS_Lobby_OnLeaveRTCRoomCallback CompletionDelegate)
    { if (CompletionDelegate) { EOS_Lobby_LeaveRTCRoomCallbackInfo info{}; info.ResultCode = EOS_EResult::EOS_Success; info.ClientData = ClientData; CompletionDelegate(&info); } }

    EOS_DECLARE_FUNC(EOS_EResult) EOS_Lobby_ParseConnectString(EOS_HLobby Handle, const EOS_Lobby_ParseConnectStringOptions* Options, char* OutBuffer, uint32_t* InOutBufferLength)
    { if (InOutBufferLength) *InOutBufferLength = 0; return EOS_EResult::EOS_NotImplemented; }
}
