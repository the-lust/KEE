
#include "utils/common_includes.h"
#include "eossdk_platform.h"
#include "settings.h"

// Parties API is not in SDK 1.19.0.7 -- all stubs

EOS_DECLARE_FUNC(void) EOS_Parties_CreateParty(void*, const void*, void*, const void*)
{
}

EOS_DECLARE_FUNC(void) EOS_Parties_DestroyParty(void*, const void*, void*, const void*)
{
}

EOS_DECLARE_FUNC(void) EOS_Parties_SendInvite(void*, const void*, void*, const void*)
{
}

EOS_DECLARE_FUNC(void) EOS_Parties_RejectInvite(void*, const void*, void*, const void*)
{
}

EOS_DECLARE_FUNC(void) EOS_Parties_QueryPartyInvites(void*, const void*, void*, const void*)
{
}

EOS_DECLARE_FUNC(uint32_t) EOS_Parties_GetPartyInviteCount(void*, const void*) { return 0; }

EOS_DECLARE_FUNC(int) EOS_Parties_CopyPartyInviteByIndex(void*, const void*, void**)
{ return (int)EOS_EResult::EOS_NotFound; }

EOS_DECLARE_FUNC(int) EOS_Parties_GetCurrentPartyId(void*, const void*, void**)
{ return (int)EOS_EResult::EOS_NotFound; }

EOS_DECLARE_FUNC(int) EOS_Parties_GetPartyMemberCount(void*, const void*, uint32_t* Out)
{ if (Out) *Out = 0; return (int)EOS_EResult::EOS_Success; }

EOS_DECLARE_FUNC(int) EOS_Parties_GetPartyMemberByIndex(void*, const void*, void**)
{ return (int)EOS_EResult::EOS_NotFound; }

EOS_DECLARE_FUNC(void) EOS_Parties_UpdateParty(void*, const void*, void*, const void*)
{
}

EOS_DECLARE_FUNC(int) EOS_Parties_CreatePartyModification(void*, const void*, void** Out)
{ if (Out) *Out = (void*)(uintptr_t)1; return (int)EOS_EResult::EOS_Success; }

EOS_DECLARE_FUNC(int) EOS_PartyModification_SetMaxMembers(void*, const void*)
{ return (int)EOS_EResult::EOS_Success; }

EOS_DECLARE_FUNC(int) EOS_PartyModification_SetInvitePermissions(void*, const void*)
{ return (int)EOS_EResult::EOS_Success; }

EOS_DECLARE_FUNC(int) EOS_PartyModification_AddMemberAttribute(void*, const void*)
{ return (int)EOS_EResult::EOS_Success; }

EOS_DECLARE_FUNC(int) EOS_PartyModification_RemoveMemberAttribute(void*, const void*)
{ return (int)EOS_EResult::EOS_Success; }

EOS_DECLARE_FUNC(int) EOS_PartyModification_SetAttribute(void*, const void*)
{ return (int)EOS_EResult::EOS_Success; }

EOS_DECLARE_FUNC(int) EOS_PartyModification_RemoveAttribute(void*, const void*)
{ return (int)EOS_EResult::EOS_Success; }

EOS_DECLARE_FUNC(void) EOS_PartyModification_Release(void*) {}
EOS_DECLARE_FUNC(void) EOS_PartyInvite_Release(void*) {}
EOS_DECLARE_FUNC(void) EOS_PartyDetails_Release(void*) {}

EOS_DECLARE_FUNC(EOS_NotificationId) EOS_Parties_AddNotifyPartyUpdateReceived(void*, const void*, void*, const void*)
{ return (EOS_NotificationId)1; }

EOS_DECLARE_FUNC(void) EOS_Parties_RemoveNotifyPartyUpdateReceived(void*, EOS_NotificationId) {}

EOS_DECLARE_FUNC(EOS_NotificationId) EOS_Parties_AddNotifyPartyDataReceived(void*, const void*, void*, const void*)
{ return (EOS_NotificationId)1; }

EOS_DECLARE_FUNC(void) EOS_Parties_RemoveNotifyPartyDataReceived(void*, EOS_NotificationId) {}

EOS_DECLARE_FUNC(EOS_NotificationId) EOS_Parties_AddNotifyPartyInviteReceived(void*, const void*, void*, const void*)
{ return (EOS_NotificationId)1; }

EOS_DECLARE_FUNC(void) EOS_Parties_RemoveNotifyPartyInviteReceived(void*, EOS_NotificationId) {}

EOS_DECLARE_FUNC(EOS_NotificationId) EOS_Parties_AddNotifyPartyInviteAccepted(void*, const void*, void*, const void*)
{ return (EOS_NotificationId)1; }

EOS_DECLARE_FUNC(void) EOS_Parties_RemoveNotifyPartyInviteAccepted(void*, EOS_NotificationId) {}

EOS_DECLARE_FUNC(EOS_NotificationId) EOS_Parties_AddNotifyPartyInviteRejected(void*, const void*, void*, const void*)
{ return (EOS_NotificationId)1; }

EOS_DECLARE_FUNC(void) EOS_Parties_RemoveNotifyPartyInviteRejected(void*, EOS_NotificationId) {}

EOS_DECLARE_FUNC(EOS_NotificationId) EOS_Parties_AddNotifyPartyMemberStatusReceived(void*, const void*, void*, const void*)
{ return (EOS_NotificationId)1; }

EOS_DECLARE_FUNC(void) EOS_Parties_RemoveNotifyPartyMemberStatusReceived(void*, EOS_NotificationId) {}

EOS_DECLARE_FUNC(EOS_NotificationId) EOS_Parties_AddNotifyPartyMemberConnectionStatusChanged(void*, const void*, void*, const void*)
{ return (EOS_NotificationId)1; }

EOS_DECLARE_FUNC(void) EOS_Parties_RemoveNotifyPartyMemberConnectionStatusChanged(void*, EOS_NotificationId) {}

EOS_DECLARE_FUNC(EOS_NotificationId) EOS_Parties_AddNotifyMemberJoined(void*, const void*, void*, const void*)
{ return (EOS_NotificationId)1; }

EOS_DECLARE_FUNC(void) EOS_Parties_RemoveNotifyMemberJoined(void*, EOS_NotificationId) {}

EOS_DECLARE_FUNC(EOS_NotificationId) EOS_Parties_AddNotifyMemberLeft(void*, const void*, void*, const void*)
{ return (EOS_NotificationId)1; }

EOS_DECLARE_FUNC(void) EOS_Parties_RemoveNotifyMemberLeft(void*, EOS_NotificationId) {}

EOS_DECLARE_FUNC(EOS_NotificationId) EOS_Parties_AddNotifyUserRefreshPartyInfo(void*, const void*, void*, const void*)
{ return (EOS_NotificationId)1; }

EOS_DECLARE_FUNC(void) EOS_Parties_RemoveNotifyUserRefreshPartyInfo(void*, EOS_NotificationId) {}

EOS_DECLARE_FUNC(int) EOS_Parties_CopyUserPartyInfoByUserId(void*, const void*, void** Out)
{ if (Out) *Out = nullptr; return (int)EOS_EResult::EOS_NotFound; }

EOS_DECLARE_FUNC(void) EOS_Parties_UserPartyInfo_Release(void*) {}
