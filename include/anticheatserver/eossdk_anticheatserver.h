#pragma once

#include "common_includes.h"
#include "callback_manager.h"
#include "sdk/eos_anticheatserver.h"
#include "sdk/eos_anticheatserver_types.h"

namespace sdk
{
    class EOSSDK_AntiCheatServer : public IRunCallback
    {
    public:
        EOSSDK_AntiCheatServer();
        ~EOSSDK_AntiCheatServer();

        virtual bool CBRunFrame() override { return false; }
        virtual bool RunCallbacks(pFrameResult_t res) override { return false; }
        virtual void FreeCallback(pFrameResult_t res) override {}

        EOS_EResult AddNotifyMessageToClient(const EOS_AntiCheatServer_AddNotifyMessageToClientOptions* Options, void* ClientData, EOS_AntiCheatServer_OnMessageToClientCallback NotificationFn, EOS_NotificationId* OutNotificationId);
        void RemoveNotifyMessageToClient(EOS_NotificationId NotificationId);

        EOS_EResult AddNotifyClientActionRequired(const EOS_AntiCheatServer_AddNotifyClientActionRequiredOptions* Options, void* ClientData, EOS_AntiCheatServer_OnClientActionRequiredCallback NotificationFn, EOS_NotificationId* OutNotificationId);
        void RemoveNotifyClientActionRequired(EOS_NotificationId NotificationId);

        EOS_EResult AddNotifyClientAuthStatusChanged(const EOS_AntiCheatServer_AddNotifyClientAuthStatusChangedOptions* Options, void* ClientData, EOS_AntiCheatServer_OnClientAuthStatusChangedCallback NotificationFn, EOS_NotificationId* OutNotificationId);
        void RemoveNotifyClientAuthStatusChanged(EOS_NotificationId NotificationId);

        EOS_EResult BeginSession(const EOS_AntiCheatServer_BeginSessionOptions* Options);
        EOS_EResult EndSession(const EOS_AntiCheatServer_EndSessionOptions* Options);

        EOS_EResult ReceiveMessageFromClient(const EOS_AntiCheatServer_ReceiveMessageFromClientOptions* Options);
        EOS_EResult RegisterClient(const EOS_AntiCheatServer_RegisterClientOptions* Options);
        EOS_EResult UnregisterClient(const EOS_AntiCheatServer_UnregisterClientOptions* Options);

        EOS_EResult GetProtectMessageOutputLength(const EOS_AntiCheatServer_GetProtectMessageOutputLengthOptions* Options, uint32_t* OutBufferLengthBytes);
        EOS_EResult ProtectMessage(const EOS_AntiCheatServer_ProtectMessageOptions* Options, void* OutBuffer, uint32_t* InOutBufferLengthBytes);
        EOS_EResult UnprotectMessage(const EOS_AntiCheatServer_UnprotectMessageOptions* Options, void* OutBuffer, uint32_t* InOutBufferLengthBytes);
    };
}
