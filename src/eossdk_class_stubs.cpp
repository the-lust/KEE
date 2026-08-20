#include "eossdk_connect.h"
#include "eossdk_custominvites.h"
#include "eossdk_friends.h"
#include "eossdk_lobby.h"
#include "eossdk_p2p.h"
#include "eossdk_presence.h"
#include "eossdk_sessions.h"
#include "eossdk_userinfo.h"
#include "eossdk_achievements.h"
#include "eossdk_progressionsnapshot.h"
#include "eossdk_rtc.h"
#include "eossdk_rtcadmin.h"
#include "eos_client_api.h"

#ifdef NETWORK_PROTOBUF_AVAILABLE
#include "network.h"
#endif

// Stubs for services excluded from build when protobuf is NOT available.
// When protobuf IS available, real implementation files provide these.

#ifndef NETWORK_PROTOBUF_AVAILABLE

#include <cstdint>

// ── EOSSDK_Connect (stub) ────────────────────────────────────────────────
namespace sdk {
void EOSSDK_Connect::Login(const EOS_Connect_LoginOptions* Options, void* ClientData, const EOS_Connect_OnLoginCallback CompletionDelegate) {}
void EOSSDK_Connect::CreateUser(const EOS_Connect_CreateUserOptions* Options, void* ClientData, const EOS_Connect_OnCreateUserCallback CompletionDelegate) {}
void EOSSDK_Connect::LinkAccount(const EOS_Connect_LinkAccountOptions* Options, void* ClientData, const EOS_Connect_OnLinkAccountCallback CompletionDelegate) {}
void EOSSDK_Connect::UnlinkAccount(const EOS_Connect_UnlinkAccountOptions* Options, void* ClientData, const EOS_Connect_OnUnlinkAccountCallback CompletionDelegate) {}
void EOSSDK_Connect::CreateDeviceId(const EOS_Connect_CreateDeviceIdOptions* Options, void* ClientData, const EOS_Connect_OnCreateDeviceIdCallback CompletionDelegate) {}
void EOSSDK_Connect::DeleteDeviceId(const EOS_Connect_DeleteDeviceIdOptions* Options, void* ClientData, const EOS_Connect_OnDeleteDeviceIdCallback CompletionDelegate) {}
void EOSSDK_Connect::TransferDeviceIdAccount(const EOS_Connect_TransferDeviceIdAccountOptions* Options, void* ClientData, const EOS_Connect_OnTransferDeviceIdAccountCallback CompletionDelegate) {}
void EOSSDK_Connect::QueryExternalAccountMappings(const EOS_Connect_QueryExternalAccountMappingsOptions* Options, void* ClientData, const EOS_Connect_OnQueryExternalAccountMappingsCallback CompletionDelegate) {}
void EOSSDK_Connect::QueryProductUserIdMappings(const EOS_Connect_QueryProductUserIdMappingsOptions* Options, void* ClientData, const EOS_Connect_OnQueryProductUserIdMappingsCallback CompletionDelegate) {}
EOS_ProductUserId EOSSDK_Connect::GetExternalAccountMapping(const EOS_Connect_GetExternalAccountMappingsOptions* Options) { return GetInvalidProductUserId(); }
EOS_EResult EOSSDK_Connect::GetProductUserIdMapping(const EOS_Connect_GetProductUserIdMappingOptions* Options, char* OutBuffer, int32_t* InOutBufferLength) { return EOS_EResult::EOS_NotFound; }
int32_t EOSSDK_Connect::GetLoggedInUsersCount() { return 0; }
EOS_ProductUserId EOSSDK_Connect::GetLoggedInUserByIndex(int32_t Index) { return GetInvalidProductUserId(); }
EOS_ELoginStatus EOSSDK_Connect::GetLoginStatus(EOS_ProductUserId LocalUserId) { return EOS_ELoginStatus::EOS_LS_NotLoggedIn; }
EOS_NotificationId EOSSDK_Connect::AddNotifyAuthExpiration(const EOS_Connect_AddNotifyAuthExpirationOptions* Options, void* ClientData, const EOS_Connect_OnAuthExpirationCallback NotificationFn) { return 1; }
void EOSSDK_Connect::RemoveNotifyAuthExpiration(EOS_NotificationId InId) {}
EOS_NotificationId EOSSDK_Connect::AddNotifyLoginStatusChanged(const EOS_Connect_AddNotifyLoginStatusChangedOptions* Options, void* ClientData, const EOS_Connect_OnLoginStatusChangedCallback NotificationFn) { return 1; }
void EOSSDK_Connect::RemoveNotifyLoginStatusChanged(EOS_NotificationId InId) {}
uint32_t EOSSDK_Connect::GetProductUserExternalAccountCount(const EOS_Connect_GetProductUserExternalAccountCountOptions* Options) { return 0; }
EOS_EResult EOSSDK_Connect::CopyProductUserExternalAccountByIndex(const EOS_Connect_CopyProductUserExternalAccountByIndexOptions* Options, EOS_Connect_ExternalAccountInfo** OutExternalAccountInfo) { return EOS_EResult::EOS_NotFound; }
EOS_EResult EOSSDK_Connect::CopyProductUserExternalAccountByAccountType(const EOS_Connect_CopyProductUserExternalAccountByAccountTypeOptions* Options, EOS_Connect_ExternalAccountInfo** OutExternalAccountInfo) { return EOS_EResult::EOS_NotFound; }
EOS_EResult EOSSDK_Connect::CopyProductUserExternalAccountByAccountId(const EOS_Connect_CopyProductUserExternalAccountByAccountIdOptions* Options, EOS_Connect_ExternalAccountInfo** OutExternalAccountInfo) { return EOS_EResult::EOS_NotFound; }
EOS_EResult EOSSDK_Connect::CopyProductUserInfo(const EOS_Connect_CopyProductUserInfoOptions* Options, EOS_Connect_ExternalAccountInfo** OutExternalAccountInfo) { return EOS_EResult::EOS_NotFound; }
EOS_EResult EOSSDK_Connect::CopyIdToken(const EOS_Connect_CopyIdTokenOptions* Options, EOS_Connect_IdToken** OutIdToken) { return EOS_EResult::EOS_NotFound; }
}

// ── Network stubs (no-op, no protobuf dependency) ─────────────────────────
Network::Network() {}
Network::~Network() {}
void Network::advertise_peer_id(peer_t const&) {}
void Network::remove_advertise_peer_id(peer_t const&) {}
void Network::advertise(bool) {}
void Network::set_default_channel(peer_t, channel_t) {}
void Network::register_listener(IRunNetwork*, channel_t, Network_Message_pb::MessagesCase) {}
void Network::unregister_listener(IRunNetwork*, channel_t, Network_Message_pb::MessagesCase) {}
bool Network::TCPSendTo(Network_Message_pb&) { return false; }
bool Network::SendBroadcast(Network_Message_pb&) { return false; }
bool Network::UDPSendTo(Network_Message_pb&) { return false; }

// ── Service class ctors/dtors (non-protobuf builds) ───────────────────────
namespace sdk {
EOSSDK_Connect::EOSSDK_Connect() {}
EOSSDK_Connect::~EOSSDK_Connect() {}
EOSSDK_Presence::EOSSDK_Presence() {}
EOSSDK_Presence::~EOSSDK_Presence() {}
void EOSSDK_Presence::setup_myself() {}
EOSSDK_ProgressionSnapshot::EOSSDK_ProgressionSnapshot() {}
EOSSDK_ProgressionSnapshot::~EOSSDK_ProgressionSnapshot() {}

// ── IRunCallback / IRunNetwork virtual stubs (non-protobuf builds) ────────
bool EOSSDK_Connect::CBRunFrame() { return false; }
bool EOSSDK_Connect::RunNetwork(Network_Message_pb const&) { return false; }
bool EOSSDK_Connect::RunCallbacks(pFrameResult_t) { return false; }
void EOSSDK_Connect::FreeCallback(pFrameResult_t) {}

bool EOSSDK_Presence::CBRunFrame() { return false; }
bool EOSSDK_Presence::RunNetwork(Network_Message_pb const&) { return false; }
bool EOSSDK_Presence::RunCallbacks(pFrameResult_t) { return false; }
void EOSSDK_Presence::FreeCallback(pFrameResult_t) {}

bool EOSSDK_ProgressionSnapshot::CBRunFrame() { return false; }
bool EOSSDK_ProgressionSnapshot::RunCallbacks(pFrameResult_t) { return false; }
void EOSSDK_ProgressionSnapshot::FreeCallback(pFrameResult_t) {}

// ── Cross-class stubs (non-protobuf builds) ───────────────────────────────
bool EOSSDK_Lobby::on_peer_disconnect(Network_Message_pb const&, Network_Peer_Disconnect_pb const&) { return false; }
bool EOSSDK_P2P::on_peer_connect(Network_Message_pb const&, Network_Peer_Connect_pb const&) { return false; }
bool EOSSDK_P2P::on_peer_disconnect(Network_Message_pb const&, Network_Peer_Disconnect_pb const&) { return false; }
bool EOSSDK_Sessions::on_peer_disconnect(Network_Message_pb const&, Network_Peer_Disconnect_pb const&) { return false; }
}

#endif // !NETWORK_PROTOBUF_AVAILABLE
