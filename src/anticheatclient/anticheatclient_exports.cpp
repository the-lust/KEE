
#include "utils/common_includes.h"
#include "anticheatclient/eossdk_anticheatclient.h"

EOS_DECLARE_FUNC(EOS_NotificationId) EOS_AntiCheatClient_AddNotifyMessageToServer(EOS_HAntiCheatClient Handle, const EOS_AntiCheatClient_AddNotifyMessageToServerOptions* Options, void* ClientData, EOS_AntiCheatClient_OnMessageToServerCallback NotificationFn)
{
    auto* pInst = reinterpret_cast<sdk::EOSSDK_AntiCheatClient*>(Handle);
    EOS_NotificationId id = EOS_INVALID_NOTIFICATIONID;
    pInst->AddNotifyMessageToServer(Options, ClientData, NotificationFn, &id);
    return id;
}

EOS_DECLARE_FUNC(void) EOS_AntiCheatClient_RemoveNotifyMessageToServer(EOS_HAntiCheatClient Handle, EOS_NotificationId NotificationId)
{
    auto* pInst = reinterpret_cast<sdk::EOSSDK_AntiCheatClient*>(Handle);
    pInst->RemoveNotifyMessageToServer(NotificationId);
}

EOS_DECLARE_FUNC(EOS_NotificationId) EOS_AntiCheatClient_AddNotifyMessageToPeer(EOS_HAntiCheatClient Handle, const EOS_AntiCheatClient_AddNotifyMessageToPeerOptions* Options, void* ClientData, EOS_AntiCheatClient_OnMessageToPeerCallback NotificationFn)
{
    auto* pInst = reinterpret_cast<sdk::EOSSDK_AntiCheatClient*>(Handle);
    EOS_NotificationId id = EOS_INVALID_NOTIFICATIONID;
    pInst->AddNotifyMessageToPeer(Options, ClientData, NotificationFn, &id);
    return id;
}

EOS_DECLARE_FUNC(void) EOS_AntiCheatClient_RemoveNotifyMessageToPeer(EOS_HAntiCheatClient Handle, EOS_NotificationId NotificationId)
{
    auto* pInst = reinterpret_cast<sdk::EOSSDK_AntiCheatClient*>(Handle);
    pInst->RemoveNotifyMessageToPeer(NotificationId);
}

EOS_DECLARE_FUNC(EOS_NotificationId) EOS_AntiCheatClient_AddNotifyPeerActionRequired(EOS_HAntiCheatClient Handle, const EOS_AntiCheatClient_AddNotifyPeerActionRequiredOptions* Options, void* ClientData, EOS_AntiCheatClient_OnPeerActionRequiredCallback NotificationFn)
{
    auto* pInst = reinterpret_cast<sdk::EOSSDK_AntiCheatClient*>(Handle);
    EOS_NotificationId id = EOS_INVALID_NOTIFICATIONID;
    pInst->AddNotifyPeerActionRequired(Options, ClientData, NotificationFn, &id);
    return id;
}

EOS_DECLARE_FUNC(void) EOS_AntiCheatClient_RemoveNotifyPeerActionRequired(EOS_HAntiCheatClient Handle, EOS_NotificationId NotificationId)
{
    auto* pInst = reinterpret_cast<sdk::EOSSDK_AntiCheatClient*>(Handle);
    pInst->RemoveNotifyPeerActionRequired(NotificationId);
}

EOS_DECLARE_FUNC(EOS_NotificationId) EOS_AntiCheatClient_AddNotifyPeerAuthStatusChanged(EOS_HAntiCheatClient Handle, const EOS_AntiCheatClient_AddNotifyPeerAuthStatusChangedOptions* Options, void* ClientData, EOS_AntiCheatClient_OnPeerAuthStatusChangedCallback NotificationFn)
{
    auto* pInst = reinterpret_cast<sdk::EOSSDK_AntiCheatClient*>(Handle);
    EOS_NotificationId id = EOS_INVALID_NOTIFICATIONID;
    pInst->AddNotifyPeerAuthStatusChanged(Options, ClientData, NotificationFn, &id);
    return id;
}

EOS_DECLARE_FUNC(void) EOS_AntiCheatClient_RemoveNotifyPeerAuthStatusChanged(EOS_HAntiCheatClient Handle, EOS_NotificationId NotificationId)
{
    auto* pInst = reinterpret_cast<sdk::EOSSDK_AntiCheatClient*>(Handle);
    pInst->RemoveNotifyPeerAuthStatusChanged(NotificationId);
}

EOS_DECLARE_FUNC(EOS_NotificationId) EOS_AntiCheatClient_AddNotifyClientIntegrityViolated(EOS_HAntiCheatClient Handle, const EOS_AntiCheatClient_AddNotifyClientIntegrityViolatedOptions* Options, void* ClientData, EOS_AntiCheatClient_OnClientIntegrityViolatedCallback NotificationFn)
{
    auto* pInst = reinterpret_cast<sdk::EOSSDK_AntiCheatClient*>(Handle);
    if (pInst) {}
    return EOS_INVALID_NOTIFICATIONID;
}

EOS_DECLARE_FUNC(void) EOS_AntiCheatClient_RemoveNotifyClientIntegrityViolated(EOS_HAntiCheatClient Handle, EOS_NotificationId NotificationId)
{
}

EOS_DECLARE_FUNC(EOS_EResult) EOS_AntiCheatClient_BeginSession(EOS_HAntiCheatClient Handle, const EOS_AntiCheatClient_BeginSessionOptions* Options)
{
    auto* pInst = reinterpret_cast<sdk::EOSSDK_AntiCheatClient*>(Handle);
    return pInst->BeginSession(Options);
}

EOS_DECLARE_FUNC(EOS_EResult) EOS_AntiCheatClient_EndSession(EOS_HAntiCheatClient Handle, const EOS_AntiCheatClient_EndSessionOptions* Options)
{
    auto* pInst = reinterpret_cast<sdk::EOSSDK_AntiCheatClient*>(Handle);
    return pInst->EndSession(Options);
}

EOS_DECLARE_FUNC(EOS_EResult) EOS_AntiCheatClient_PollStatus(EOS_HAntiCheatClient Handle, const EOS_AntiCheatClient_PollStatusOptions* Options, EOS_EAntiCheatClientViolationType* OutViolationType, char* OutMessage)
{
    auto* pInst = reinterpret_cast<sdk::EOSSDK_AntiCheatClient*>(Handle);
    return pInst->PollStatus(Options, OutViolationType, OutMessage);
}

EOS_DECLARE_FUNC(EOS_EResult) EOS_AntiCheatClient_Reserved01(EOS_HAntiCheatClient Handle, const EOS_AntiCheatClient_Reserved01Options* Options, int32_t* OutValue)
{
    if (OutValue) *OutValue = 0;
    return EOS_EResult::EOS_Success;
}

EOS_DECLARE_FUNC(EOS_EResult) EOS_AntiCheatClient_Reserved02(EOS_HAntiCheatClient Handle, const EOS_AntiCheatClient_Reserved02Options* Options)
{
    return EOS_EResult::EOS_Success;
}

EOS_DECLARE_FUNC(EOS_EResult) EOS_AntiCheatClient_GetModuleBuildId(EOS_HAntiCheatClient Handle, const EOS_AntiCheatClient_GetModuleBuildIdOptions* Options, uint32_t* OutModuleBuildId)
{
    if (OutModuleBuildId) *OutModuleBuildId = 1;
    return EOS_EResult::EOS_Success;
}

EOS_DECLARE_FUNC(EOS_EResult) EOS_AntiCheatClient_AddExternalIntegrityCatalog(EOS_HAntiCheatClient Handle, const EOS_AntiCheatClient_AddExternalIntegrityCatalogOptions* Options)
{
    return EOS_EResult::EOS_Success;
}

EOS_DECLARE_FUNC(EOS_EResult) EOS_AntiCheatClient_ReceiveMessageFromServer(EOS_HAntiCheatClient Handle, const EOS_AntiCheatClient_ReceiveMessageFromServerOptions* Options)
{
    auto* pInst = reinterpret_cast<sdk::EOSSDK_AntiCheatClient*>(Handle);
    return pInst->ReceiveMessageFromServer(Options);
}

EOS_DECLARE_FUNC(EOS_EResult) EOS_AntiCheatClient_GetProtectMessageOutputLength(EOS_HAntiCheatClient Handle, const EOS_AntiCheatClient_GetProtectMessageOutputLengthOptions* Options, uint32_t* OutBufferSizeBytes)
{
    auto* pInst = reinterpret_cast<sdk::EOSSDK_AntiCheatClient*>(Handle);
    return pInst->GetProtectMessageOutputLength(Options, OutBufferSizeBytes);
}

EOS_DECLARE_FUNC(EOS_EResult) EOS_AntiCheatClient_ProtectMessage(EOS_HAntiCheatClient Handle, const EOS_AntiCheatClient_ProtectMessageOptions* Options, void* OutBuffer, uint32_t* OutBytesWritten)
{
    auto* pInst = reinterpret_cast<sdk::EOSSDK_AntiCheatClient*>(Handle);
    return pInst->ProtectMessage(Options, OutBuffer, OutBytesWritten);
}

EOS_DECLARE_FUNC(EOS_EResult) EOS_AntiCheatClient_UnprotectMessage(EOS_HAntiCheatClient Handle, const EOS_AntiCheatClient_UnprotectMessageOptions* Options, void* OutBuffer, uint32_t* OutBytesWritten)
{
    auto* pInst = reinterpret_cast<sdk::EOSSDK_AntiCheatClient*>(Handle);
    return pInst->UnprotectMessage(Options, OutBuffer, OutBytesWritten);
}

EOS_DECLARE_FUNC(EOS_EResult) EOS_AntiCheatClient_RegisterPeer(EOS_HAntiCheatClient Handle, const EOS_AntiCheatClient_RegisterPeerOptions* Options)
{
    auto* pInst = reinterpret_cast<sdk::EOSSDK_AntiCheatClient*>(Handle);
    return pInst->RegisterPeer(Options);
}

EOS_DECLARE_FUNC(EOS_EResult) EOS_AntiCheatClient_UnregisterPeer(EOS_HAntiCheatClient Handle, const EOS_AntiCheatClient_UnregisterPeerOptions* Options)
{
    auto* pInst = reinterpret_cast<sdk::EOSSDK_AntiCheatClient*>(Handle);
    return pInst->UnregisterPeer(Options);
}

EOS_DECLARE_FUNC(EOS_EResult) EOS_AntiCheatClient_ReceiveMessageFromPeer(EOS_HAntiCheatClient Handle, const EOS_AntiCheatClient_ReceiveMessageFromPeerOptions* Options)
{
    auto* pInst = reinterpret_cast<sdk::EOSSDK_AntiCheatClient*>(Handle);
    return pInst->ReceiveMessageFromPeer(Options);
}
