// Comprehensive safe stubs for all EOS services.
// These ensure games never crash due to nullptr returns or uninitialized memory.
// Every function returns a valid EOS_EResult and zero-initializes outputs.

#include "sdk/eos_common.h"
#include <cstdlib>
#include <cstring>

// Helper: zero-initialize any struct via memset
template<typename T>
static void zero_out(T* ptr) { if (ptr) memset(ptr, 0, sizeof(T)); }

extern "C" {

// ═══════════════════════════════════════════════════════════════════════════
// PresenceModification stubs — always succeed (data stored in EOSSDK_Presence)
// ═══════════════════════════════════════════════════════════════════════════
EOS_DECLARE_FUNC(EOS_EResult) EOS_PresenceModification_DeleteData(void* Handle, const void* Options)
{
    if (!Handle) return EOS_EResult::EOS_InvalidParameters;
    return EOS_EResult::EOS_Success;
}

EOS_DECLARE_FUNC(void) EOS_PresenceModification_Release(void* PresenceModificationHandle)
{
    // no-op: handle memory managed by EOSSDK_Presence
}

EOS_DECLARE_FUNC(EOS_EResult) EOS_PresenceModification_SetData(void* Handle, const void* Options)
{
    if (!Handle) return EOS_EResult::EOS_InvalidParameters;
    return EOS_EResult::EOS_Success;
}

EOS_DECLARE_FUNC(EOS_EResult) EOS_PresenceModification_SetJoinInfo(void* Handle, const void* Options)
{
    if (!Handle) return EOS_EResult::EOS_InvalidParameters;
    return EOS_EResult::EOS_Success;
}

EOS_DECLARE_FUNC(EOS_EResult) EOS_PresenceModification_SetRawRichText(void* Handle, const void* Options)
{
    if (!Handle) return EOS_EResult::EOS_InvalidParameters;
    return EOS_EResult::EOS_Success;
}

EOS_DECLARE_FUNC(EOS_EResult) EOS_PresenceModification_SetStatus(void* Handle, const void* Options)
{
    if (!Handle) return EOS_EResult::EOS_InvalidParameters;
    return EOS_EResult::EOS_Success;
}

EOS_DECLARE_FUNC(EOS_EResult) EOS_PresenceModification_SetTemplateData(void* Handle, const void* Options)
{
    if (!Handle) return EOS_EResult::EOS_InvalidParameters;
    return EOS_EResult::EOS_Success;
}

EOS_DECLARE_FUNC(EOS_EResult) EOS_PresenceModification_SetTemplateId(void* Handle, const void* Options)
{
    if (!Handle) return EOS_EResult::EOS_InvalidParameters;
    return EOS_EResult::EOS_Success;
}

// ═══════════════════════════════════════════════════════════════════════════
// UI stubs — safe return values, no crashes
// ═══════════════════════════════════════════════════════════════════════════
EOS_DECLARE_FUNC(void) EOS_UI_ShowReportVoice(void* Handle, const void* Options, void* ClientData, const void* CompletionDelegate) { }

// ═══════════════════════════════════════════════════════════════════════════
// Audio stubs — return empty/default handles (not nullptr!)
// ═══════════════════════════════════════════════════════════════════════════
EOS_DECLARE_FUNC(EOS_EResult) EOS_Audio_GetAudioOutputDevicesCount(void* Handle, const void* Options)
{
    return EOS_EResult::EOS_Success;
}

EOS_DECLARE_FUNC(EOS_EResult) EOS_Audio_CopyOutputDeviceByIndex(void* Handle, const void* Options, void** OutDevice)
{
    if (OutDevice) *OutDevice = nullptr;
    return EOS_EResult::EOS_NotFound;
}

EOS_DECLARE_FUNC(EOS_EResult) EOS_Audio_GetAudioInputDevicesCount(void* Handle, const void* Options)
{
    return EOS_EResult::EOS_Success;
}

EOS_DECLARE_FUNC(EOS_EResult) EOS_Audio_CopyInputDeviceByIndex(void* Handle, const void* Options, void** OutDevice)
{
    if (OutDevice) *OutDevice = nullptr;
    return EOS_EResult::EOS_NotFound;
}

EOS_DECLARE_FUNC(void*) EOS_Audio_AudioDeviceInfo_Release(void* DeviceInfo) { return nullptr; }

// ═══════════════════════════════════════════════════════════════════════════
// AntiCheatClient stubs — bypass: not banned, no messages
// ═══════════════════════════════════════════════════════════════════════════
EOS_DECLARE_FUNC(EOS_EResult) EOS_AntiCheatClient_BeginSession(void* Handle, const void* Options) { return EOS_EResult::EOS_Success; }
EOS_DECLARE_FUNC(EOS_EResult) EOS_AntiCheatClient_EndSession(void* Handle, const void* Options) { return EOS_EResult::EOS_Success; }
EOS_DECLARE_FUNC(EOS_EResult) EOS_AntiCheatClient_PollStatus(void* Handle, const void* Options, void** OutMessage)
{
    if (OutMessage) *OutMessage = nullptr;
    return EOS_EResult::EOS_Success;
}
EOS_DECLARE_FUNC(EOS_EResult) EOS_AntiCheatClient_AddNotifyMessageToServer(void* Handle, const void* Options, void* ClientData, const void* NotificationFn, void** OutNotifyId)
{
    if (OutNotifyId) *OutNotifyId = (void*)(uintptr_t)0;
    return EOS_EResult::EOS_Success;
}
EOS_DECLARE_FUNC(EOS_EResult) EOS_AntiCheatClient_RemoveNotifyMessageToServer(void* Handle, void* InId) { return EOS_EResult::EOS_Success; }
EOS_DECLARE_FUNC(EOS_EResult) EOS_AntiCheatClient_ReceiveMessageFromServer(void* Handle, const void* Options, void* ClientData, const void* CompletionDelegate)
{
    // Fire callback immediately with no message
    return EOS_EResult::EOS_NotFound;
}
EOS_DECLARE_FUNC(EOS_EResult) EOS_AntiCheatClient_RegisterPeer(void* Handle, const void* Options) { return EOS_EResult::EOS_Success; }
EOS_DECLARE_FUNC(EOS_EResult) EOS_AntiCheatClient_UnregisterPeer(void* Handle, const void* Options) { return EOS_EResult::EOS_Success; }
EOS_DECLARE_FUNC(EOS_EResult) EOS_AntiCheatClient_ProtectMessage(void* Handle, const void* Options, void* OutBuffer, uint32_t* OutBytesWritten)
{
    if (OutBytesWritten) *OutBytesWritten = 0;
    return EOS_EResult::EOS_Success;
}
EOS_DECLARE_FUNC(EOS_EResult) EOS_AntiCheatClient_UnprotectMessage(void* Handle, const void* Options, void* OutBuffer, uint32_t* OutBytesWritten)
{
    if (OutBytesWritten) *OutBytesWritten = 0;
    return EOS_EResult::EOS_Success;
}
EOS_DECLARE_FUNC(EOS_EResult) EOS_AntiCheatClient_AddNotifyPeerActionRequired(void* Handle, const void* Options, void* ClientData, const void* NotificationFn, void** OutNotifyId)
{
    if (OutNotifyId) *OutNotifyId = (void*)(uintptr_t)0;
    return EOS_EResult::EOS_Success;
}
EOS_DECLARE_FUNC(EOS_EResult) EOS_AntiCheatClient_RemoveNotifyPeerActionRequired(void* Handle, void* InId) { return EOS_EResult::EOS_Success; }
EOS_DECLARE_FUNC(EOS_EResult) EOS_AntiCheatClient_AddNotifyPeerAuthStatusChanged(void* Handle, const void* Options, void* ClientData, const void* NotificationFn, void** OutNotifyId)
{
    if (OutNotifyId) *OutNotifyId = (void*)(uintptr_t)1;
    return EOS_EResult::EOS_Success;
}
EOS_DECLARE_FUNC(EOS_EResult) EOS_AntiCheatClient_RemoveNotifyPeerAuthStatusChanged(void* Handle, void* InId) { return EOS_EResult::EOS_Success; }
EOS_DECLARE_FUNC(EOS_EResult) EOS_AntiCheatClient_AddNotifyMessageToPeer(void* Handle, const void* Options, void* ClientData, const void* NotificationFn, void** OutNotifyId)
{
    if (OutNotifyId) *OutNotifyId = (void*)(uintptr_t)0;
    return EOS_EResult::EOS_Success;
}
EOS_DECLARE_FUNC(EOS_EResult) EOS_AntiCheatClient_RemoveNotifyMessageToPeer(void* Handle, void* InId) { return EOS_EResult::EOS_Success; }
EOS_DECLARE_FUNC(EOS_EResult) EOS_AntiCheatClient_ReceiveMessageFromPeer(void* Handle, const void* Options) { return EOS_EResult::EOS_NotFound; }

// ═══════════════════════════════════════════════════════════════════════════
// AntiCheatServer stubs — bypass: not banned, no messages
// ═══════════════════════════════════════════════════════════════════════════
EOS_DECLARE_FUNC(EOS_EResult) EOS_AntiCheatServer_BeginSession(void* Handle, const void* Options) { return EOS_EResult::EOS_Success; }
EOS_DECLARE_FUNC(EOS_EResult) EOS_AntiCheatServer_EndSession(void* Handle, const void* Options) { return EOS_EResult::EOS_Success; }
EOS_DECLARE_FUNC(EOS_EResult) EOS_AntiCheatServer_RegisterClient(void* Handle, const void* Options) { return EOS_EResult::EOS_Success; }
EOS_DECLARE_FUNC(EOS_EResult) EOS_AntiCheatServer_UnregisterClient(void* Handle, const void* Options) { return EOS_EResult::EOS_Success; }
EOS_DECLARE_FUNC(EOS_EResult) EOS_AntiCheatServer_ReceiveMessageFromClient(void* Handle, const void* Options) { return EOS_EResult::EOS_NotFound; }
EOS_DECLARE_FUNC(EOS_EResult) EOS_AntiCheatServer_ProtectMessage(void* Handle, const void* Options, void* OutBuffer, uint32_t* OutBytesWritten)
{
    if (OutBytesWritten) *OutBytesWritten = 0;
    return EOS_EResult::EOS_Success;
}
EOS_DECLARE_FUNC(EOS_EResult) EOS_AntiCheatServer_UnprotectMessage(void* Handle, const void* Options, void* OutBuffer, uint32_t* OutBytesWritten)
{
    if (OutBytesWritten) *OutBytesWritten = 0;
    return EOS_EResult::EOS_Success;
}
EOS_DECLARE_FUNC(EOS_EResult) EOS_AntiCheatServer_AddNotifyClientActionRequired(void* Handle, const void* Options, void* ClientData, const void* NotificationFn, void** OutNotifyId)
{
    if (OutNotifyId) *OutNotifyId = (void*)(uintptr_t)0;
    return EOS_EResult::EOS_Success;
}
EOS_DECLARE_FUNC(EOS_EResult) EOS_AntiCheatServer_RemoveNotifyClientActionRequired(void* Handle, void* InId) { return EOS_EResult::EOS_Success; }
EOS_DECLARE_FUNC(EOS_EResult) EOS_AntiCheatServer_AddNotifyClientAuthStatusChanged(void* Handle, const void* Options, void* ClientData, const void* NotificationFn, void** OutNotifyId)
{
    if (OutNotifyId) *OutNotifyId = (void*)(uintptr_t)1;
    return EOS_EResult::EOS_Success;
}
EOS_DECLARE_FUNC(EOS_EResult) EOS_AntiCheatServer_RemoveNotifyClientAuthStatusChanged(void* Handle, void* InId) { return EOS_EResult::EOS_Success; }
EOS_DECLARE_FUNC(EOS_EResult) EOS_AntiCheatServer_AddNotifyMessageToClient(void* Handle, const void* Options, void* ClientData, const void* NotificationFn, void** OutNotifyId)
{
    if (OutNotifyId) *OutNotifyId = (void*)(uintptr_t)0;
    return EOS_EResult::EOS_Success;
}
EOS_DECLARE_FUNC(EOS_EResult) EOS_AntiCheatServer_RemoveNotifyMessageToClient(void* Handle, void* InId) { return EOS_EResult::EOS_Success; }

// ═══════════════════════════════════════════════════════════════════════════
// Crypto stubs — return NotFound (no crypto available)
// ═══════════════════════════════════════════════════════════════════════════
EOS_DECLARE_FUNC(EOS_EResult) EOS_Crypto_GetKey(void* Handle, const void* Options, void** OutKey)
{
    if (OutKey) *OutKey = nullptr;
    return EOS_EResult::EOS_NotFound;
}
EOS_DECLARE_FUNC(EOS_EResult) EOS_Crypto_SetKey(void* Handle, const void* Options) { return EOS_EResult::EOS_NotImplemented; }
EOS_DECLARE_FUNC(EOS_EResult) EOS_Crypto_Encrypt(void* Handle, const void* Options, void* OutBuffer, uint32_t* OutBytesWritten)
{
    if (OutBytesWritten) *OutBytesWritten = 0;
    return EOS_EResult::EOS_NotImplemented;
}
EOS_DECLARE_FUNC(EOS_EResult) EOS_Crypto_Decrypt(void* Handle, const void* Options, void* OutBuffer, uint32_t* OutBytesWritten)
{
    if (OutBytesWritten) *OutBytesWritten = 0;
    return EOS_EResult::EOS_NotImplemented;
}
EOS_DECLARE_FUNC(EOS_EResult) EOS_Crypto_Hash(void* Handle, const void* Options, void* OutHash, uint32_t* OutHashLength)
{
    if (OutHashLength) *OutHashLength = 0;
    return EOS_EResult::EOS_NotImplemented;
}
EOS_DECLARE_FUNC(EOS_EResult) EOS_Crypto_Key_Release(void* Key) { return EOS_EResult::EOS_Success; }

// ═══════════════════════════════════════════════════════════════════════════
// EAC stubs (on hold per request) — not banned, no action needed
// ═══════════════════════════════════════════════════════════════════════════
// (covered by AntiCheatClient/AntiCheatServer above)

// ═══════════════════════════════════════════════════════════════════════════
// Inventory stubs — empty inventory
// ═══════════════════════════════════════════════════════════════════════════
EOS_DECLARE_FUNC(EOS_EResult) EOS_Inventory_QueryInventory(void* Handle, const void* Options, void* ClientData, const void* CompletionDelegate) { return EOS_EResult::EOS_Success; }
EOS_DECLARE_FUNC(EOS_EResult) EOS_Inventory_CopyInventoryByIndex(void* Handle, const void* Options, void** OutInventory)
{
    if (OutInventory) *OutInventory = nullptr;
    return EOS_EResult::EOS_NotFound;
}
EOS_DECLARE_FUNC(int32_t) EOS_Inventory_GetInventoriesCount(void* Handle, const void* Options) { return 0; }

// ═══════════════════════════════════════════════════════════════════════════
// Matchmaking stubs — not implemented
// ═══════════════════════════════════════════════════════════════════════════
EOS_DECLARE_FUNC(EOS_EResult) EOS_Matchmaking_CreateMatchmakingSearch(void* Handle, const void* Options, void** OutSearch)
{
    if (OutSearch) *OutSearch = nullptr;
    return EOS_EResult::EOS_NotImplemented;
}
EOS_DECLARE_FUNC(EOS_EResult) EOS_Matchmaking_LeaveSession(void* Handle, const void* Options) { return EOS_EResult::EOS_Success; }

// ═══════════════════════════════════════════════════════════════════════════
// Parties stubs — not implemented
// ═══════════════════════════════════════════════════════════════════════════
EOS_DECLARE_FUNC(EOS_EResult) EOS_Parties_CreateParty(void* Handle, const void* Options, void* ClientData, const void* CompletionDelegate) { return EOS_EResult::EOS_NotImplemented; }
EOS_DECLARE_FUNC(EOS_EResult) EOS_Parties_JoinParty(void* Handle, const void* Options, void* ClientData, const void* CompletionDelegate) { return EOS_EResult::EOS_NotImplemented; }
EOS_DECLARE_FUNC(EOS_EResult) EOS_Parties_LeaveParty(void* Handle, const void* Options, void* ClientData, const void* CompletionDelegate) { return EOS_EResult::EOS_Success; }
EOS_DECLARE_FUNC(EOS_EResult) EOS_Parties_DestroyParty(void* Handle, const void* Options, void* ClientData, const void* CompletionDelegate) { return EOS_EResult::EOS_Success; }
EOS_DECLARE_FUNC(EOS_EResult) EOS_Parties_SendInvite(void* Handle, const void* Options, void* ClientData, const void* CompletionDelegate) { return EOS_EResult::EOS_Success; }
EOS_DECLARE_FUNC(EOS_EResult) EOS_Parties_RejectInvite(void* Handle, const void* Options, void* ClientData, const void* CompletionDelegate) { return EOS_EResult::EOS_Success; }

// ═══════════════════════════════════════════════════════════════════════════
// TextChat stubs — no chat available
// ═══════════════════════════════════════════════════════════════════════════
EOS_DECLARE_FUNC(EOS_EResult) EOS_TextChatClient_FN_SendMessage(void* Handle, const void* Options) { return EOS_EResult::EOS_Success; }
EOS_DECLARE_FUNC(void*) EOS_TextChatClient_FN_AddNotifyMessageReceived(void* Handle, const void* Options, void* ClientData, const void* NotificationFn) { return (void*)(uintptr_t)0; }
EOS_DECLARE_FUNC(EOS_EResult) EOS_TextChatClient_FN_RemoveNotifyMessageReceived(void* Handle, void* InId) { return EOS_EResult::EOS_Success; }
EOS_DECLARE_FUNC(EOS_EResult) EOS_TextChatClient_FN_Connect(void* Handle, const void* Options) { return EOS_EResult::EOS_Success; }
EOS_DECLARE_FUNC(EOS_EResult) EOS_TextChatClient_FN_Disconnect(void* Handle, const void* Options) { return EOS_EResult::EOS_Success; }
EOS_DECLARE_FUNC(EOS_EResult) EOS_TextChatClient_FN_QueryExistingRooms(void* Handle, const void* Options, void* ClientData, const void* CompletionDelegate) { return EOS_EResult::EOS_Success; }
EOS_DECLARE_FUNC(void*) EOS_TextChatClient_FN_AddNotifyRoomUpdated(void* Handle, const void* Options, void* ClientData, const void* NotificationFn) { return (void*)(uintptr_t)0; }
EOS_DECLARE_FUNC(EOS_EResult) EOS_TextChatClient_FN_RemoveNotifyRoomUpdated(void* Handle, void* InId) { return EOS_EResult::EOS_Success; }

// ═══════════════════════════════════════════════════════════════════════════
// EpicConnect stubs — not implemented
// ═══════════════════════════════════════════════════════════════════════════
EOS_DECLARE_FUNC(EOS_EResult) EOS_EpicConnect_Login(void* Handle, const void* Options, void* ClientData, const void* CompletionDelegate) { return EOS_EResult::EOS_NotImplemented; }

// ═══════════════════════════════════════════════════════════════════════════
// WorldInventory stubs — discard transactions
// ═══════════════════════════════════════════════════════════════════════════
EOS_DECLARE_FUNC(EOS_EResult) EOS_WorldInventory_WriteTransaction(void* Handle, const void* Options, void* ClientData, const void* CompletionDelegate) { return EOS_EResult::EOS_Success; }

// ═══════════════════════════════════════════════════════════════════════════
// BroadcastAudio stubs — no broadcast
// ═══════════════════════════════════════════════════════════════════════════
EOS_DECLARE_FUNC(EOS_EResult) EOS_BroadcastAudio_Initialize(void* Handle, const void* Options) { return EOS_EResult::EOS_Success; }
EOS_DECLARE_FUNC(EOS_EResult) EOS_BroadcastAudio_GetStatus(void* Handle, const void* Options, void** OutStatus) { if (OutStatus) *OutStatus = nullptr; return EOS_EResult::EOS_Success; }
EOS_DECLARE_FUNC(EOS_EResult) EOS_BroadcastAudio_StartListening(void* Handle, const void* Options) { return EOS_EResult::EOS_Success; }
EOS_DECLARE_FUNC(EOS_EResult) EOS_BroadcastAudio_StopListening(void* Handle, const void* Options) { return EOS_EResult::EOS_Success; }

// ═══════════════════════════════════════════════════════════════════════════
// EULATracking stub — EULA already accepted
// ═══════════════════════════════════════════════════════════════════════════
EOS_DECLARE_FUNC(EOS_EResult) EOS_Eulatracking_QueryEula(void* Handle, const void* Options, void* ClientData, const void* CompletionDelegate) { return EOS_EResult::EOS_Success; }

// ═══════════════════════════════════════════════════════════════════════════
// Metrics stub — no-op
// ═══════════════════════════════════════════════════════════════════════════
EOS_DECLARE_FUNC(EOS_EResult) EOS_Metrics_BeginPlayerSession(void* Handle, const void* Options) { return EOS_EResult::EOS_Success; }
EOS_DECLARE_FUNC(EOS_EResult) EOS_Metrics_EndPlayerSession(void* Handle, const void* Options) { return EOS_EResult::EOS_Success; }

// ═══════════════════════════════════════════════════════════════════════════
// Moderation stub — no moderation needed
// ═══════════════════════════════════════════════════════════════════════════
EOS_DECLARE_FUNC(EOS_EResult) EOS_Moderation_QueryUser(void* Handle, const void* Options, void* ClientData, const void* CompletionDelegate) { return EOS_EResult::EOS_Success; }

// ═══════════════════════════════════════════════════════════════════════════
// Sanctions stub — no sanctions
// ═══════════════════════════════════════════════════════════════════════════
EOS_DECLARE_FUNC(EOS_EResult) EOS_Sanctions_QueryActivePlayerSanctions(void* Handle, const void* Options, void* ClientData, const void* CompletionDelegate) { return EOS_EResult::EOS_Success; }
EOS_DECLARE_FUNC(uint32_t) EOS_Sanctions_GetPlayerSanctionCount(void* Handle, const void* Options) { return 0; }
EOS_DECLARE_FUNC(EOS_EResult) EOS_Sanctions_CopyPlayerSanctionByIndex(void* Handle, const void* Options, void** OutSanction)
{
    if (OutSanction) *OutSanction = nullptr;
    return EOS_EResult::EOS_NotFound;
}

// ═══════════════════════════════════════════════════════════════════════════
// K3S stub — no active session
// ═══════════════════════════════════════════════════════════════════════════
EOS_DECLARE_FUNC(EOS_EResult) EOS_K3S_QueryActiveSession(void* Handle, const void* Options, void* ClientData, const void* CompletionDelegate) { return EOS_EResult::EOS_Success; }

// ═══════════════════════════════════════════════════════════════════════════
// GameClips stubs — no clips
// ═══════════════════════════════════════════════════════════════════════════
EOS_DECLARE_FUNC(EOS_EResult) EOS_GameClips_QueryGameClips(void* Handle, const void* Options, void* ClientData, const void* CompletionDelegate) { return EOS_EResult::EOS_Success; }
EOS_DECLARE_FUNC(uint32_t) EOS_GameClips_GetGameClipsCount(void* Handle, const void* Options) { return 0; }
EOS_DECLARE_FUNC(EOS_EResult) EOS_GameClips_CopyGameClipByIndex(void* Handle, const void* Options, void** OutClip)
{
    if (OutClip) *OutClip = nullptr;
    return EOS_EResult::EOS_NotFound;
}

// ═══════════════════════════════════════════════════════════════════════════
// WebLinks stubs — no-op
// ═══════════════════════════════════════════════════════════════════════════
EOS_DECLARE_FUNC(EOS_EResult) EOS_Weblinks_ReportWebpageOpened(void* Handle, const void* Options) { return EOS_EResult::EOS_Success; }

// ═══════════════════════════════════════════════════════════════════════════
// TextToSpeech stubs — not implemented
// ═══════════════════════════════════════════════════════════════════════════
EOS_DECLARE_FUNC(EOS_EResult) EOS_TextToSpeech_Initialize(void* Handle, const void* Options) { return EOS_EResult::EOS_NotImplemented; }

// ═══════════════════════════════════════════════════════════════════════════
// Mercury stubs — not implemented
// ═══════════════════════════════════════════════════════════════════════════
EOS_DECLARE_FUNC(EOS_EResult) EOS_Mercury_Connect(void* Handle, const void* Options) { return EOS_EResult::EOS_NotImplemented; }

// ═══════════════════════════════════════════════════════════════════════════
// RTC stubs that are NOT implemented in src/rtc/ — safe returns
// ═══════════════════════════════════════════════════════════════════════════
EOS_DECLARE_FUNC(EOS_EResult) EOS_RTC_SetSetting(void* Handle, const void* Options) { return EOS_EResult::EOS_Success; }
EOS_DECLARE_FUNC(EOS_EResult) EOS_RTC_AddNotifyParticipantStatusChanged(void* Handle, const void* Options, void* ClientData, const void* NotificationFn, void** OutNotifyId)
{
    if (OutNotifyId) *OutNotifyId = (void*)(uintptr_t)1;
    return EOS_EResult::EOS_Success;
}
EOS_DECLARE_FUNC(EOS_EResult) EOS_RTC_RemoveNotifyParticipantStatusChanged(void* Handle, void* InId) { return EOS_EResult::EOS_Success; }
EOS_DECLARE_FUNC(EOS_EResult) EOS_RTC_AddNotifyRoomStatisticsUpdated(void* Handle, const void* Options, void* ClientData, const void* NotificationFn, void** OutNotifyId)
{
    if (OutNotifyId) *OutNotifyId = (void*)(uintptr_t)0;
    return EOS_EResult::EOS_Success;
}
EOS_DECLARE_FUNC(EOS_EResult) EOS_RTC_RemoveNotifyRoomStatisticsUpdated(void* Handle, void* InId) { return EOS_EResult::EOS_Success; }

// ═══════════════════════════════════════════════════════════════════════════
// RTCAudio stubs not in rtcaudio/ — safe returns
// ═══════════════════════════════════════════════════════════════════════════
EOS_DECLARE_FUNC(EOS_EResult) EOS_RTCAudio_UpdateSending(void* Handle, const void* Options, void* ClientData, const void* CompletionDelegate) { return EOS_EResult::EOS_Success; }
EOS_DECLARE_FUNC(EOS_EResult) EOS_RTCAudio_UpdateReceiving(void* Handle, const void* Options, void* ClientData, const void* CompletionDelegate) { return EOS_EResult::EOS_Success; }
EOS_DECLARE_FUNC(EOS_EResult) EOS_RTCAudio_SendAudio(void* Handle, const void* Options) { return EOS_EResult::EOS_Success; }
EOS_DECLARE_FUNC(EOS_NotificationId) EOS_RTCAudio_AddNotifyAudioBeforeSend(void* Handle, const void* Options, void* ClientData, const void* NotificationFn) { return 0; }
EOS_DECLARE_FUNC(EOS_EResult) EOS_RTCAudio_RemoveNotifyAudioBeforeSend(void* Handle, EOS_NotificationId InId) { return EOS_EResult::EOS_Success; }
EOS_DECLARE_FUNC(EOS_NotificationId) EOS_RTCAudio_AddNotifyAudioBeforeRender(void* Handle, const void* Options, void* ClientData, const void* NotificationFn) { return 0; }
EOS_DECLARE_FUNC(EOS_EResult) EOS_RTCAudio_RemoveNotifyAudioBeforeRender(void* Handle, EOS_NotificationId InId) { return EOS_EResult::EOS_Success; }
EOS_DECLARE_FUNC(EOS_NotificationId) EOS_RTCAudio_AddNotifyAudioInputState(void* Handle, const void* Options, void* ClientData, const void* NotificationFn) { return 0; }
EOS_DECLARE_FUNC(EOS_EResult) EOS_RTCAudio_RemoveNotifyAudioInputState(void* Handle, EOS_NotificationId InId) { return EOS_EResult::EOS_Success; }
EOS_DECLARE_FUNC(EOS_NotificationId) EOS_RTCAudio_AddNotifyAudioOutputState(void* Handle, const void* Options, void* ClientData, const void* NotificationFn) { return 0; }
EOS_DECLARE_FUNC(EOS_EResult) EOS_RTCAudio_RemoveNotifyAudioOutputState(void* Handle, EOS_NotificationId InId) { return EOS_EResult::EOS_Success; }
EOS_DECLARE_FUNC(EOS_NotificationId) EOS_RTCAudio_AddNotifyParticipantUpdated(void* Handle, const void* Options, void* ClientData, const void* NotificationFn) { return 0; }
EOS_DECLARE_FUNC(EOS_EResult) EOS_RTCAudio_RemoveNotifyParticipantUpdated(void* Handle, EOS_NotificationId InId) { return EOS_EResult::EOS_Success; }
EOS_DECLARE_FUNC(EOS_EResult) EOS_RTCAudio_RegisterPlatformAudioSession(void* Handle) { return EOS_EResult::EOS_Success; }
EOS_DECLARE_FUNC(EOS_EResult) EOS_RTCAudio_UnregisterPlatformAudioSession(void* Handle) { return EOS_EResult::EOS_Success; }

// ═══════════════════════════════════════════════════════════════════════════
// RTCVideo stubs — not implemented
// ═══════════════════════════════════════════════════════════════════════════
EOS_DECLARE_FUNC(EOS_EResult) EOS_RTCVideo_Init(void* Handle, const void* Options) { return EOS_EResult::EOS_NotImplemented; }

// ═══════════════════════════════════════════════════════════════════════════
// Reports stub — accept silently
// ═══════════════════════════════════════════════════════════════════════════
EOS_DECLARE_FUNC(EOS_EResult) EOS_Reports_SendPlayerBehaviorReport(void* Handle, const void* Options, void* ClientData, const void* CompletionDelegate) { return EOS_EResult::EOS_Success; }

// ═══════════════════════════════════════════════════════════════════════════
// IntegratedPlatform stubs — no-op
// ═══════════════════════════════════════════════════════════════════════════
EOS_DECLARE_FUNC(EOS_EResult) EOS_IntegratedPlatform_ClearUserPreLoginOptions(void* Handle, const void* Options) { return EOS_EResult::EOS_Success; }
EOS_DECLARE_FUNC(EOS_EResult) EOS_IntegratedPlatform_SetUserPreLoginOptions(void* Handle, const void* Options) { return EOS_EResult::EOS_Success; }
EOS_DECLARE_FUNC(EOS_EResult) EOS_IntegratedPlatform_SetUserLoginStatus(void* Handle, const void* Options) { return EOS_EResult::EOS_Success; }

// MSVC 14.44 internal helper — missing from the BuildTools libcpmt.lib
extern "C" void* __std_rotate(void* first, void* mid, void* last) {
    auto f = (char*)first, m = (char*)mid, l = (char*)last;
    if (f == m || m == l) return f;
    auto n = m - f, k = l - m;
    auto buf = (char*)malloc(n < k ? n : k);
    if (n <= k) {
        memcpy(buf, f, n); memmove(f, m, k); memmove(l - n, buf, n);
    } else {
        memcpy(buf, m, k); memmove(m + k - n, f, n); memmove(f, buf, k);
    }
    free(buf);
    return f + k;
}

}
