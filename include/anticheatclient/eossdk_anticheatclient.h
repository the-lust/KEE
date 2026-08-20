#pragma once

#include "common_includes.h"
#include "callback_manager.h"
#include "sdk/eos_anticheatclient.h"
#include "sdk/eos_anticheatclient_types.h"

namespace sdk
{
    class EOSSDK_AntiCheatClient : public IRunCallback
    {
    public:
        EOSSDK_AntiCheatClient();
        ~EOSSDK_AntiCheatClient();

        virtual bool CBRunFrame() override { return false; }
        virtual bool RunCallbacks(pFrameResult_t res) override { return false; }
        virtual void FreeCallback(pFrameResult_t res) override {}

        EOS_EResult AddNotifyMessageToServer(const EOS_AntiCheatClient_AddNotifyMessageToServerOptions* Options, void* ClientData, EOS_AntiCheatClient_OnMessageToServerCallback NotificationFn, EOS_NotificationId* OutNotificationId);
        void RemoveNotifyMessageToServer(EOS_NotificationId NotificationId);
        
        EOS_EResult AddNotifyMessageToPeer(const EOS_AntiCheatClient_AddNotifyMessageToPeerOptions* Options, void* ClientData, EOS_AntiCheatClient_OnMessageToPeerCallback NotificationFn, EOS_NotificationId* OutNotificationId);
        void RemoveNotifyMessageToPeer(EOS_NotificationId NotificationId);

        EOS_EResult AddNotifyPeerActionRequired(const EOS_AntiCheatClient_AddNotifyPeerActionRequiredOptions* Options, void* ClientData, EOS_AntiCheatClient_OnPeerActionRequiredCallback NotificationFn, EOS_NotificationId* OutNotificationId);
        void RemoveNotifyPeerActionRequired(EOS_NotificationId NotificationId);

        EOS_EResult AddNotifyPeerAuthStatusChanged(const EOS_AntiCheatClient_AddNotifyPeerAuthStatusChangedOptions* Options, void* ClientData, EOS_AntiCheatClient_OnPeerAuthStatusChangedCallback NotificationFn, EOS_NotificationId* OutNotificationId);
        void RemoveNotifyPeerAuthStatusChanged(EOS_NotificationId NotificationId);

        EOS_EResult BeginSession(const EOS_AntiCheatClient_BeginSessionOptions* Options);
        EOS_EResult EndSession(const EOS_AntiCheatClient_EndSessionOptions* Options);

        EOS_EResult PollStatus(const EOS_AntiCheatClient_PollStatusOptions* Options, EOS_EAntiCheatClientViolationType* OutViolationType, char* OutMessage);
        EOS_EResult ReceiveMessageFromServer(const EOS_AntiCheatClient_ReceiveMessageFromServerOptions* Options);
        EOS_EResult ReceiveMessageFromPeer(const EOS_AntiCheatClient_ReceiveMessageFromPeerOptions* Options);

        EOS_EResult RegisterPeer(const EOS_AntiCheatClient_RegisterPeerOptions* Options);
        EOS_EResult UnregisterPeer(const EOS_AntiCheatClient_UnregisterPeerOptions* Options);

        EOS_EResult GetProtectMessageOutputLength(const EOS_AntiCheatClient_GetProtectMessageOutputLengthOptions* Options, uint32_t* OutBufferLengthBytes);
        EOS_EResult ProtectMessage(const EOS_AntiCheatClient_ProtectMessageOptions* Options, void* OutBuffer, uint32_t* InOutBufferLengthBytes);
        EOS_EResult UnprotectMessage(const EOS_AntiCheatClient_UnprotectMessageOptions* Options, void* OutBuffer, uint32_t* InOutBufferLengthBytes);
    };
}
