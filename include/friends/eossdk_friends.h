#pragma once

#include "common_includes.h"
#include "callback_manager.h"
#include "network.h"

namespace sdk
{
    class EOSSDK_Friends :
        public IRunCallback,
        public IRunNetwork
    {
        std::set<EOS_EpicAccountId> m_friends;
        std::set<EOS_EpicAccountId> m_persisted_friends; 
        string                 m_friends_filename;

    public:
        EOSSDK_Friends();
        ~EOSSDK_Friends();

        virtual bool CBRunFrame()                             override;
        virtual bool RunCallbacks(pFrameResult_t res)         override;
        virtual void FreeCallback(pFrameResult_t res)         override;
        virtual bool RunNetwork(Network_Message_pb const& msg) override;

        bool on_friends_invite(Network_Message_pb const& msg, Friends_Invite_pb const& inv);

        void QueryFriends(const EOS_Friends_QueryFriendsOptions* Options, void* ClientData, const EOS_Friends_OnQueryFriendsCallback CompletionDelegate);
        void SendInvite(const EOS_Friends_SendInviteOptions* Options, void* ClientData, const EOS_Friends_OnSendInviteCallback CompletionDelegate);
        void AcceptInvite(const EOS_Friends_AcceptInviteOptions* Options, void* ClientData, const EOS_Friends_OnAcceptInviteCallback CompletionDelegate);
        void RejectInvite(const EOS_Friends_RejectInviteOptions* Options, void* ClientData, const EOS_Friends_OnRejectInviteCallback CompletionDelegate);
        int32_t GetFriendsCount(const EOS_Friends_GetFriendsCountOptions* Options);
        EOS_EpicAccountId GetFriendAtIndex(const EOS_Friends_GetFriendAtIndexOptions* Options);
        EOS_EFriendsStatus GetStatus(const EOS_Friends_GetStatusOptions* Options);
        EOS_NotificationId AddNotifyFriendsUpdate(const EOS_Friends_AddNotifyFriendsUpdateOptions* Options, void* ClientData, const EOS_Friends_OnFriendsUpdateCallback FriendsUpdateHandler);
        void RemoveNotifyFriendsUpdate(EOS_NotificationId NotificationId);

        EOS_NotificationId AddNotifyBlockedUsersUpdate(const EOS_Friends_AddNotifyBlockedUsersUpdateOptions* Options, void* ClientData, const EOS_Friends_OnBlockedUsersUpdateCallback BlockedUsersUpdateHandler);
        void RemoveNotifyBlockedUsersUpdate(EOS_NotificationId NotificationId);
        int32_t GetBlockedUsersCount(const EOS_Friends_GetBlockedUsersCountOptions* Options);
        EOS_EpicAccountId GetBlockedUserAtIndex(const EOS_Friends_GetBlockedUserAtIndexOptions* Options);
    };
}
