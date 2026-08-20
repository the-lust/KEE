
#include "anticheatserver/eossdk_anticheatserver.h"

namespace sdk
{
    EOSSDK_AntiCheatServer::EOSSDK_AntiCheatServer() {}
    EOSSDK_AntiCheatServer::~EOSSDK_AntiCheatServer() {}

    EOS_EResult EOSSDK_AntiCheatServer::AddNotifyMessageToClient(const EOS_AntiCheatServer_AddNotifyMessageToClientOptions* Options, void* ClientData, EOS_AntiCheatServer_OnMessageToClientCallback NotificationFn, EOS_NotificationId* OutNotificationId)
    {
        if (OutNotificationId) *OutNotificationId = 1;
        return EOS_EResult::EOS_Success;
    }

    void EOSSDK_AntiCheatServer::RemoveNotifyMessageToClient(EOS_NotificationId NotificationId) {}

    EOS_EResult EOSSDK_AntiCheatServer::AddNotifyClientActionRequired(const EOS_AntiCheatServer_AddNotifyClientActionRequiredOptions* Options, void* ClientData, EOS_AntiCheatServer_OnClientActionRequiredCallback NotificationFn, EOS_NotificationId* OutNotificationId)
    {
        if (OutNotificationId) *OutNotificationId = 1;
        return EOS_EResult::EOS_Success;
    }

    void EOSSDK_AntiCheatServer::RemoveNotifyClientActionRequired(EOS_NotificationId NotificationId) {}

    EOS_EResult EOSSDK_AntiCheatServer::AddNotifyClientAuthStatusChanged(const EOS_AntiCheatServer_AddNotifyClientAuthStatusChangedOptions* Options, void* ClientData, EOS_AntiCheatServer_OnClientAuthStatusChangedCallback NotificationFn, EOS_NotificationId* OutNotificationId)
    {
        if (OutNotificationId) *OutNotificationId = 1;
        return EOS_EResult::EOS_Success;
    }

    void EOSSDK_AntiCheatServer::RemoveNotifyClientAuthStatusChanged(EOS_NotificationId NotificationId) {}

    EOS_EResult EOSSDK_AntiCheatServer::BeginSession(const EOS_AntiCheatServer_BeginSessionOptions* Options)
    {
        return EOS_EResult::EOS_Success;
    }

    EOS_EResult EOSSDK_AntiCheatServer::EndSession(const EOS_AntiCheatServer_EndSessionOptions* Options)
    {
        return EOS_EResult::EOS_Success;
    }

    EOS_EResult EOSSDK_AntiCheatServer::ReceiveMessageFromClient(const EOS_AntiCheatServer_ReceiveMessageFromClientOptions* Options)
    {
        return EOS_EResult::EOS_Success;
    }

    EOS_EResult EOSSDK_AntiCheatServer::RegisterClient(const EOS_AntiCheatServer_RegisterClientOptions* Options)
    {
        return EOS_EResult::EOS_Success;
    }

    EOS_EResult EOSSDK_AntiCheatServer::UnregisterClient(const EOS_AntiCheatServer_UnregisterClientOptions* Options)
    {
        return EOS_EResult::EOS_Success;
    }

    EOS_EResult EOSSDK_AntiCheatServer::GetProtectMessageOutputLength(const EOS_AntiCheatServer_GetProtectMessageOutputLengthOptions* Options, uint32_t* OutBufferLengthBytes)
    {
        if (OutBufferLengthBytes) *OutBufferLengthBytes = Options->DataLengthBytes;
        return EOS_EResult::EOS_Success;
    }

    EOS_EResult EOSSDK_AntiCheatServer::ProtectMessage(const EOS_AntiCheatServer_ProtectMessageOptions* Options, void* OutBuffer, uint32_t* InOutBufferLengthBytes)
    {
        if (OutBuffer && Options->Data && InOutBufferLengthBytes && *InOutBufferLengthBytes >= Options->DataLengthBytes)
        {
            memcpy(OutBuffer, Options->Data, Options->DataLengthBytes);
            *InOutBufferLengthBytes = Options->DataLengthBytes;
            return EOS_EResult::EOS_Success;
        }
        return EOS_EResult::EOS_InvalidParameters;
    }

    EOS_EResult EOSSDK_AntiCheatServer::UnprotectMessage(const EOS_AntiCheatServer_UnprotectMessageOptions* Options, void* OutBuffer, uint32_t* InOutBufferLengthBytes)
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
