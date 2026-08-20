#pragma once

#include "common_includes.h"
#include "callback_manager.h"
#include "network.h"
#include "sdk/eos_custominvites.h"
#include "sdk/eos_custominvites_types.h"

namespace sdk
{
    class EOSSDK_CustomInvites : public IRunCallback, public IRunNetwork
    {
        std::mutex  m_mutex;
        string m_payload;

    public:
        EOSSDK_CustomInvites();
        ~EOSSDK_CustomInvites();

        virtual bool CBRunFrame()                       override;
        virtual bool RunCallbacks(pFrameResult_t res)   override;
        virtual void FreeCallback(pFrameResult_t res)   override;
        virtual bool RunNetwork(Network_Message_pb const& msg) override;

        bool on_custom_invite_received(Network_Message_pb const& msg,
                                       CustomInvite_Send_pb const& inv);

        EOS_EResult SetCustomInvite(const EOS_CustomInvites_SetCustomInviteOptions*);
        void        SendCustomInvite(const EOS_CustomInvites_SendCustomInviteOptions*, void*, EOS_CustomInvites_OnSendCustomInviteCallback);
        EOS_EResult FinalizeInvite(const EOS_CustomInvites_FinalizeInviteOptions*);

        EOS_NotificationId AddNotifyCustomInviteReceived(const EOS_CustomInvites_AddNotifyCustomInviteReceivedOptions*, void*, EOS_CustomInvites_OnCustomInviteReceivedCallback);
        void               RemoveNotifyCustomInviteReceived(EOS_NotificationId);

        EOS_NotificationId AddNotifyCustomInviteAccepted(const EOS_CustomInvites_AddNotifyCustomInviteAcceptedOptions*, void*, EOS_CustomInvites_OnCustomInviteAcceptedCallback);
        void               RemoveNotifyCustomInviteAccepted(EOS_NotificationId);

        EOS_NotificationId AddNotifyCustomInviteRejected(const EOS_CustomInvites_AddNotifyCustomInviteRejectedOptions*, void*, EOS_CustomInvites_OnCustomInviteRejectedCallback);
        void               RemoveNotifyCustomInviteRejected(EOS_NotificationId);
    };
}
