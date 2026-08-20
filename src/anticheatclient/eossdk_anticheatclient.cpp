#include "anticheatclient/eossdk_anticheatclient.h"

namespace sdk
{
    EOSSDK_AntiCheatClient::EOSSDK_AntiCheatClient() {}
    EOSSDK_AntiCheatClient::~EOSSDK_AntiCheatClient() {}

    EOS_EResult EOSSDK_AntiCheatClient::AddNotifyMessageToServer(const EOS_AntiCheatClient_AddNotifyMessageToServerOptions* Options, void* ClientData, EOS_AntiCheatClient_OnMessageToServerCallback NotificationFn, EOS_NotificationId* OutNotificationId)
    {
        if (OutNotificationId) *OutNotificationId = 1;
        return EOS_EResult::EOS_Success;
    }

    void EOSSDK_AntiCheatClient::RemoveNotifyMessageToServer(EOS_NotificationId NotificationId) {}

    EOS_EResult EOSSDK_AntiCheatClient::AddNotifyMessageToPeer(const EOS_AntiCheatClient_AddNotifyMessageToPeerOptions* Options, void* ClientData, EOS_AntiCheatClient_OnMessageToPeerCallback NotificationFn, EOS_NotificationId* OutNotificationId)
    {
        if (OutNotificationId) *OutNotificationId = 1;
        return EOS_EResult::EOS_Success;
    }

    void EOSSDK_AntiCheatClient::RemoveNotifyMessageToPeer(EOS_NotificationId NotificationId) {}

    EOS_EResult EOSSDK_AntiCheatClient::AddNotifyPeerActionRequired(const EOS_AntiCheatClient_AddNotifyPeerActionRequiredOptions* Options, void* ClientData, EOS_AntiCheatClient_OnPeerActionRequiredCallback NotificationFn, EOS_NotificationId* OutNotificationId)
    {
        if (OutNotificationId) *OutNotificationId = 1;
        return EOS_EResult::EOS_Success;
    }

    void EOSSDK_AntiCheatClient::RemoveNotifyPeerActionRequired(EOS_NotificationId NotificationId) {}

    EOS_EResult EOSSDK_AntiCheatClient::AddNotifyPeerAuthStatusChanged(const EOS_AntiCheatClient_AddNotifyPeerAuthStatusChangedOptions* Options, void* ClientData, EOS_AntiCheatClient_OnPeerAuthStatusChangedCallback NotificationFn, EOS_NotificationId* OutNotificationId)
    {
        if (OutNotificationId) *OutNotificationId = 1;
        return EOS_EResult::EOS_Success;
    }

    void EOSSDK_AntiCheatClient::RemoveNotifyPeerAuthStatusChanged(EOS_NotificationId NotificationId) {}

    EOS_EResult EOSSDK_AntiCheatClient::BeginSession(const EOS_AntiCheatClient_BeginSessionOptions* Options)
    {
        return EOS_EResult::EOS_Success;
    }

    EOS_EResult EOSSDK_AntiCheatClient::EndSession(const EOS_AntiCheatClient_EndSessionOptions* Options)
    {
        return EOS_EResult::EOS_Success;
    }

    EOS_EResult EOSSDK_AntiCheatClient::PollStatus(const EOS_AntiCheatClient_PollStatusOptions* Options, EOS_EAntiCheatClientViolationType* OutViolationType, char* OutMessage)
    {
        if (OutViolationType) *OutViolationType = EOS_EAntiCheatClientViolationType::EOS_ACCVT_None;
        return EOS_EResult::EOS_Success;
    }

    EOS_EResult EOSSDK_AntiCheatClient::ReceiveMessageFromServer(const EOS_AntiCheatClient_ReceiveMessageFromServerOptions* Options)
    {
        return EOS_EResult::EOS_Success;
    }

    EOS_EResult EOSSDK_AntiCheatClient::ReceiveMessageFromPeer(const EOS_AntiCheatClient_ReceiveMessageFromPeerOptions* Options)
    {
        return EOS_EResult::EOS_Success;
    }

    EOS_EResult EOSSDK_AntiCheatClient::RegisterPeer(const EOS_AntiCheatClient_RegisterPeerOptions* Options)
    {
        return EOS_EResult::EOS_Success;
    }

    EOS_EResult EOSSDK_AntiCheatClient::UnregisterPeer(const EOS_AntiCheatClient_UnregisterPeerOptions* Options)
    {
        return EOS_EResult::EOS_Success;
    }

    EOS_EResult EOSSDK_AntiCheatClient::GetProtectMessageOutputLength(const EOS_AntiCheatClient_GetProtectMessageOutputLengthOptions* Options, uint32_t* OutBufferLengthBytes)
    {
        if (OutBufferLengthBytes) *OutBufferLengthBytes = Options->DataLengthBytes;
        return EOS_EResult::EOS_Success;
    }

    EOS_EResult EOSSDK_AntiCheatClient::ProtectMessage(const EOS_AntiCheatClient_ProtectMessageOptions* Options, void* OutBuffer, uint32_t* InOutBufferLengthBytes)
    {
        if (OutBuffer && Options->Data && InOutBufferLengthBytes && *InOutBufferLengthBytes >= Options->DataLengthBytes)
        {
            memcpy(OutBuffer, Options->Data, Options->DataLengthBytes);
            *InOutBufferLengthBytes = Options->DataLengthBytes;
            return EOS_EResult::EOS_Success;
        }
        return EOS_EResult::EOS_InvalidParameters;
    }

    EOS_EResult EOSSDK_AntiCheatClient::UnprotectMessage(const EOS_AntiCheatClient_UnprotectMessageOptions* Options, void* OutBuffer, uint32_t* InOutBufferLengthBytes)
    {
        if (OutBuffer && Options->Data && InOutBufferLengthBytes && *InOutBufferLengthBytes >= Options->DataLengthBytes)
        {
            memcpy(OutBuffer, Options->Data, Options->DataLengthBytes);
            *InOutBufferLengthBytes = Options->DataLengthBytes;
            return EOS_EResult::EOS_Success;
        }
        return EOS_EResult::EOS_InvalidParameters;
    }
}
