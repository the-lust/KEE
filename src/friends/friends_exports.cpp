#include "friends/eossdk_friends.h"

using namespace sdk;

extern "C"
{
    EOS_DECLARE_FUNC(void) EOS_Friends_QueryFriends(EOS_HFriends Handle, const EOS_Friends_QueryFriendsOptions* Options, void* ClientData, const EOS_Friends_OnQueryFriendsCallback CompletionDelegate)
    {
        if (Handle == nullptr)
            return;

        auto pInst = reinterpret_cast<EOSSDK_Friends*>(Handle);
        pInst->QueryFriends(Options, ClientData, CompletionDelegate);
    }

    EOS_DECLARE_FUNC(int32_t) EOS_Friends_GetFriendsCount(EOS_HFriends Handle, const EOS_Friends_GetFriendsCountOptions* Options)
    {
        if (Handle == nullptr)
            return 0;

        auto pInst = reinterpret_cast<EOSSDK_Friends*>(Handle);
        return pInst->GetFriendsCount(Options);
    }

    EOS_DECLARE_FUNC(EOS_EpicAccountId) EOS_Friends_GetFriendAtIndex(EOS_HFriends Handle, const EOS_Friends_GetFriendAtIndexOptions* Options)
    {
        if (Handle == nullptr)
            return nullptr;

        auto pInst = reinterpret_cast<EOSSDK_Friends*>(Handle);
        return pInst->GetFriendAtIndex(Options);
    }

    EOS_DECLARE_FUNC(EOS_EFriendsStatus) EOS_Friends_GetStatus(EOS_HFriends Handle, const EOS_Friends_GetStatusOptions* Options)
    {
        if (Handle == nullptr)
            return EOS_EFriendsStatus::EOS_FS_NotFriends;

        auto pInst = reinterpret_cast<EOSSDK_Friends*>(Handle);
        return pInst->GetStatus(Options);
    }

    EOS_DECLARE_FUNC(EOS_NotificationId) EOS_Friends_AddNotifyFriendsUpdate(EOS_HFriends Handle, const EOS_Friends_AddNotifyFriendsUpdateOptions* Options, void* ClientData, const EOS_Friends_OnFriendsUpdateCallback FriendsUpdateHandler)
    {
        if (Handle == nullptr)
            return EOS_INVALID_NOTIFICATIONID;

        auto pInst = reinterpret_cast<EOSSDK_Friends*>(Handle);
        return pInst->AddNotifyFriendsUpdate(Options, ClientData, FriendsUpdateHandler);
    }

    EOS_DECLARE_FUNC(void) EOS_Friends_RemoveNotifyFriendsUpdate(EOS_HFriends Handle, EOS_NotificationId InId)
    {
        if (Handle == nullptr)
            return;

        auto pInst = reinterpret_cast<EOSSDK_Friends*>(Handle);
        pInst->RemoveNotifyFriendsUpdate(InId);
    }

    EOS_DECLARE_FUNC(void) EOS_Friends_AcceptInvite(EOS_HFriends Handle, const EOS_Friends_AcceptInviteOptions* Options, void* ClientData, const EOS_Friends_OnAcceptInviteCallback CompletionDelegate)
    {
        if (Handle == nullptr)
            return;

        auto pInst = reinterpret_cast<EOSSDK_Friends*>(Handle);
        pInst->AcceptInvite(Options, ClientData, CompletionDelegate);
    }

    EOS_DECLARE_FUNC(void) EOS_Friends_RejectInvite(EOS_HFriends Handle, const EOS_Friends_RejectInviteOptions* Options, void* ClientData, const EOS_Friends_OnRejectInviteCallback CompletionDelegate)
    {
        if (Handle == nullptr)
            return;

        auto pInst = reinterpret_cast<EOSSDK_Friends*>(Handle);
        pInst->RejectInvite(Options, ClientData, CompletionDelegate);
    }

    EOS_DECLARE_FUNC(void) EOS_Friends_SendInvite(EOS_HFriends Handle, const EOS_Friends_SendInviteOptions* Options, void* ClientData, const EOS_Friends_OnSendInviteCallback CompletionDelegate)
    {
        if (Handle == nullptr)
            return;

        auto pInst = reinterpret_cast<EOSSDK_Friends*>(Handle);
        pInst->SendInvite(Options, ClientData, CompletionDelegate);
    }

    EOS_DECLARE_FUNC(EOS_NotificationId) EOS_Friends_AddNotifyBlockedUsersUpdate(EOS_HFriends Handle, const EOS_Friends_AddNotifyBlockedUsersUpdateOptions* Options, void* ClientData, const EOS_Friends_OnBlockedUsersUpdateCallback BlockedUsersUpdateHandler)
    {
        if (Handle == nullptr)
            return EOS_INVALID_NOTIFICATIONID;

        auto pInst = reinterpret_cast<EOSSDK_Friends*>(Handle);
        return pInst->AddNotifyBlockedUsersUpdate(Options, ClientData, BlockedUsersUpdateHandler);
    }

    EOS_DECLARE_FUNC(void) EOS_Friends_RemoveNotifyBlockedUsersUpdate(EOS_HFriends Handle, EOS_NotificationId InId)
    {
        if (Handle == nullptr)
            return;

        auto pInst = reinterpret_cast<EOSSDK_Friends*>(Handle);
        pInst->RemoveNotifyBlockedUsersUpdate(InId);
    }

    EOS_DECLARE_FUNC(int32_t) EOS_Friends_GetBlockedUsersCount(EOS_HFriends Handle, const EOS_Friends_GetBlockedUsersCountOptions* Options)
    {
        if (Handle == nullptr)
            return 0;

        auto pInst = reinterpret_cast<EOSSDK_Friends*>(Handle);
        return pInst->GetBlockedUsersCount(Options);
    }

    EOS_DECLARE_FUNC(EOS_EpicAccountId) EOS_Friends_GetBlockedUserAtIndex(EOS_HFriends Handle, const EOS_Friends_GetBlockedUserAtIndexOptions* Options)
    {
        if (Handle == nullptr)
            return nullptr;

        auto pInst = reinterpret_cast<EOSSDK_Friends*>(Handle);
        return pInst->GetBlockedUserAtIndex(Options);
    }
}
