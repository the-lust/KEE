
#include "utils/common_includes.h"
#include "anticheatserver/eossdk_anticheatserver.h"

EOS_DECLARE_FUNC(EOS_NotificationId) EOS_AntiCheatServer_AddNotifyMessageToClient(EOS_HAntiCheatServer Handle, const EOS_AntiCheatServer_AddNotifyMessageToClientOptions* Options, void* ClientData, EOS_AntiCheatServer_OnMessageToClientCallback NotificationFn)
{
    auto* pInst = reinterpret_cast<sdk::EOSSDK_AntiCheatServer*>(Handle);
    EOS_NotificationId id = EOS_INVALID_NOTIFICATIONID;
    pInst->AddNotifyMessageToClient(Options, ClientData, NotificationFn, &id);
    return id;
}

EOS_DECLARE_FUNC(void) EOS_AntiCheatServer_RemoveNotifyMessageToClient(EOS_HAntiCheatServer Handle, EOS_NotificationId NotificationId)
{
    auto* pInst = reinterpret_cast<sdk::EOSSDK_AntiCheatServer*>(Handle);
    pInst->RemoveNotifyMessageToClient(NotificationId);
}

EOS_DECLARE_FUNC(EOS_NotificationId) EOS_AntiCheatServer_AddNotifyClientActionRequired(EOS_HAntiCheatServer Handle, const EOS_AntiCheatServer_AddNotifyClientActionRequiredOptions* Options, void* ClientData, EOS_AntiCheatServer_OnClientActionRequiredCallback NotificationFn)
{
    auto* pInst = reinterpret_cast<sdk::EOSSDK_AntiCheatServer*>(Handle);
    EOS_NotificationId id = EOS_INVALID_NOTIFICATIONID;
    pInst->AddNotifyClientActionRequired(Options, ClientData, NotificationFn, &id);
    return id;
}

EOS_DECLARE_FUNC(void) EOS_AntiCheatServer_RemoveNotifyClientActionRequired(EOS_HAntiCheatServer Handle, EOS_NotificationId NotificationId)
{
    auto* pInst = reinterpret_cast<sdk::EOSSDK_AntiCheatServer*>(Handle);
    pInst->RemoveNotifyClientActionRequired(NotificationId);
}

EOS_DECLARE_FUNC(EOS_NotificationId) EOS_AntiCheatServer_AddNotifyClientAuthStatusChanged(EOS_HAntiCheatServer Handle, const EOS_AntiCheatServer_AddNotifyClientAuthStatusChangedOptions* Options, void* ClientData, EOS_AntiCheatServer_OnClientAuthStatusChangedCallback NotificationFn)
{
    auto* pInst = reinterpret_cast<sdk::EOSSDK_AntiCheatServer*>(Handle);
    EOS_NotificationId id = EOS_INVALID_NOTIFICATIONID;
    pInst->AddNotifyClientAuthStatusChanged(Options, ClientData, NotificationFn, &id);
    return id;
}

EOS_DECLARE_FUNC(void) EOS_AntiCheatServer_RemoveNotifyClientAuthStatusChanged(EOS_HAntiCheatServer Handle, EOS_NotificationId NotificationId)
{
    auto* pInst = reinterpret_cast<sdk::EOSSDK_AntiCheatServer*>(Handle);
    pInst->RemoveNotifyClientAuthStatusChanged(NotificationId);
}

EOS_DECLARE_FUNC(EOS_EResult) EOS_AntiCheatServer_BeginSession(EOS_HAntiCheatServer Handle, const EOS_AntiCheatServer_BeginSessionOptions* Options)
{
    auto* pInst = reinterpret_cast<sdk::EOSSDK_AntiCheatServer*>(Handle);
    return pInst->BeginSession(Options);
}

EOS_DECLARE_FUNC(EOS_EResult) EOS_AntiCheatServer_EndSession(EOS_HAntiCheatServer Handle, const EOS_AntiCheatServer_EndSessionOptions* Options)
{
    auto* pInst = reinterpret_cast<sdk::EOSSDK_AntiCheatServer*>(Handle);
    return pInst->EndSession(Options);
}

EOS_DECLARE_FUNC(EOS_EResult) EOS_AntiCheatServer_RegisterClient(EOS_HAntiCheatServer Handle, const EOS_AntiCheatServer_RegisterClientOptions* Options)
{
    auto* pInst = reinterpret_cast<sdk::EOSSDK_AntiCheatServer*>(Handle);
    return pInst->RegisterClient(Options);
}

EOS_DECLARE_FUNC(EOS_EResult) EOS_AntiCheatServer_UnregisterClient(EOS_HAntiCheatServer Handle, const EOS_AntiCheatServer_UnregisterClientOptions* Options)
{
    auto* pInst = reinterpret_cast<sdk::EOSSDK_AntiCheatServer*>(Handle);
    return pInst->UnregisterClient(Options);
}

EOS_DECLARE_FUNC(EOS_EResult) EOS_AntiCheatServer_ReceiveMessageFromClient(EOS_HAntiCheatServer Handle, const EOS_AntiCheatServer_ReceiveMessageFromClientOptions* Options)
{
    auto* pInst = reinterpret_cast<sdk::EOSSDK_AntiCheatServer*>(Handle);
    return pInst->ReceiveMessageFromClient(Options);
}

EOS_DECLARE_FUNC(EOS_EResult) EOS_AntiCheatServer_SetClientDetails(EOS_HAntiCheatServer Handle, const EOS_AntiCheatCommon_SetClientDetailsOptions* Options)
{
    return EOS_EResult::EOS_Success;
}

EOS_DECLARE_FUNC(EOS_EResult) EOS_AntiCheatServer_SetGameSessionId(EOS_HAntiCheatServer Handle, const EOS_AntiCheatCommon_SetGameSessionIdOptions* Options)
{
    return EOS_EResult::EOS_Success;
}

EOS_DECLARE_FUNC(EOS_EResult) EOS_AntiCheatServer_SetClientNetworkState(EOS_HAntiCheatServer Handle, const EOS_AntiCheatServer_SetClientNetworkStateOptions* Options)
{
    return EOS_EResult::EOS_Success;
}

EOS_DECLARE_FUNC(EOS_EResult) EOS_AntiCheatServer_GetProtectMessageOutputLength(EOS_HAntiCheatServer Handle, const EOS_AntiCheatServer_GetProtectMessageOutputLengthOptions* Options, uint32_t* OutBufferSizeBytes)
{
    auto* pInst = reinterpret_cast<sdk::EOSSDK_AntiCheatServer*>(Handle);
    return pInst->GetProtectMessageOutputLength(Options, OutBufferSizeBytes);
}

EOS_DECLARE_FUNC(EOS_EResult) EOS_AntiCheatServer_ProtectMessage(EOS_HAntiCheatServer Handle, const EOS_AntiCheatServer_ProtectMessageOptions* Options, void* OutBuffer, uint32_t* OutBytesWritten)
{
    auto* pInst = reinterpret_cast<sdk::EOSSDK_AntiCheatServer*>(Handle);
    return pInst->ProtectMessage(Options, OutBuffer, OutBytesWritten);
}

EOS_DECLARE_FUNC(EOS_EResult) EOS_AntiCheatServer_UnprotectMessage(EOS_HAntiCheatServer Handle, const EOS_AntiCheatServer_UnprotectMessageOptions* Options, void* OutBuffer, uint32_t* OutBytesWritten)
{
    auto* pInst = reinterpret_cast<sdk::EOSSDK_AntiCheatServer*>(Handle);
    return pInst->UnprotectMessage(Options, OutBuffer, OutBytesWritten);
}

EOS_DECLARE_FUNC(EOS_EResult) EOS_AntiCheatServer_RegisterEvent(EOS_HAntiCheatServer Handle, const EOS_AntiCheatCommon_RegisterEventOptions* Options)
{
    return EOS_EResult::EOS_Success;
}

EOS_DECLARE_FUNC(EOS_EResult) EOS_AntiCheatServer_LogEvent(EOS_HAntiCheatServer Handle, const EOS_AntiCheatCommon_LogEventOptions* Options)
{
    return EOS_EResult::EOS_Success;
}

EOS_DECLARE_FUNC(EOS_EResult) EOS_AntiCheatServer_LogGameRoundStart(EOS_HAntiCheatServer Handle, const EOS_AntiCheatCommon_LogGameRoundStartOptions* Options)
{
    return EOS_EResult::EOS_Success;
}

EOS_DECLARE_FUNC(EOS_EResult) EOS_AntiCheatServer_LogGameRoundEnd(EOS_HAntiCheatServer Handle, const EOS_AntiCheatCommon_LogGameRoundEndOptions* Options)
{
    return EOS_EResult::EOS_Success;
}

EOS_DECLARE_FUNC(EOS_EResult) EOS_AntiCheatServer_LogPlayerSpawn(EOS_HAntiCheatServer Handle, const EOS_AntiCheatCommon_LogPlayerSpawnOptions* Options)
{
    return EOS_EResult::EOS_Success;
}

EOS_DECLARE_FUNC(EOS_EResult) EOS_AntiCheatServer_LogPlayerDespawn(EOS_HAntiCheatServer Handle, const EOS_AntiCheatCommon_LogPlayerDespawnOptions* Options)
{
    return EOS_EResult::EOS_Success;
}

EOS_DECLARE_FUNC(EOS_EResult) EOS_AntiCheatServer_LogPlayerRevive(EOS_HAntiCheatServer Handle, const EOS_AntiCheatCommon_LogPlayerReviveOptions* Options)
{
    return EOS_EResult::EOS_Success;
}

EOS_DECLARE_FUNC(EOS_EResult) EOS_AntiCheatServer_LogPlayerTick(EOS_HAntiCheatServer Handle, const EOS_AntiCheatCommon_LogPlayerTickOptions* Options)
{
    return EOS_EResult::EOS_Success;
}

EOS_DECLARE_FUNC(EOS_EResult) EOS_AntiCheatServer_LogPlayerUseWeapon(EOS_HAntiCheatServer Handle, const EOS_AntiCheatCommon_LogPlayerUseWeaponOptions* Options)
{
    return EOS_EResult::EOS_Success;
}

EOS_DECLARE_FUNC(EOS_EResult) EOS_AntiCheatServer_LogPlayerUseAbility(EOS_HAntiCheatServer Handle, const EOS_AntiCheatCommon_LogPlayerUseAbilityOptions* Options)
{
    return EOS_EResult::EOS_Success;
}

EOS_DECLARE_FUNC(EOS_EResult) EOS_AntiCheatServer_LogPlayerTakeDamage(EOS_HAntiCheatServer Handle, const EOS_AntiCheatCommon_LogPlayerTakeDamageOptions* Options)
{
    return EOS_EResult::EOS_Success;
}
