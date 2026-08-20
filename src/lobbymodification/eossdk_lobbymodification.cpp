#include "eossdk_lobby.h"

namespace sdk {

EOSSDK_LobbyModification::EOSSDK_LobbyModification()
    : m_lobby_modified(false)
    , m_member_modified(false)
{
}

EOSSDK_LobbyModification::~EOSSDK_LobbyModification()
{
}

EOS_EResult EOSSDK_LobbyModification::SetPermissionLevel(const EOS_LobbyModification_SetPermissionLevelOptions* Options)
{
    return EOS_EResult::EOS_Success;
}

EOS_EResult EOSSDK_LobbyModification::SetMaxMembers(const EOS_LobbyModification_SetMaxMembersOptions* Options)
{
    return EOS_EResult::EOS_Success;
}

EOS_EResult EOSSDK_LobbyModification::AddAttribute(const EOS_LobbyModification_AddAttributeOptions* Options)
{
    return EOS_EResult::EOS_Success;
}

EOS_EResult EOSSDK_LobbyModification::RemoveAttribute(const EOS_LobbyModification_RemoveAttributeOptions* Options)
{
    return EOS_EResult::EOS_Success;
}

EOS_EResult EOSSDK_LobbyModification::AddMemberAttribute(const EOS_LobbyModification_AddMemberAttributeOptions* Options)
{
    return EOS_EResult::EOS_Success;
}

EOS_EResult EOSSDK_LobbyModification::RemoveMemberAttribute(const EOS_LobbyModification_RemoveMemberAttributeOptions* Options)
{
    return EOS_EResult::EOS_Success;
}

void EOSSDK_LobbyModification::Release()
{
    delete this;
}

}
