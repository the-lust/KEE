#include "eossdk_sessions.h"

namespace sdk {

EOS_EResult EOSSDK_SessionModification::SetBucketId(const EOS_SessionModification_SetBucketIdOptions* Options)
{
    return EOS_EResult::EOS_Success;
}

EOS_EResult EOSSDK_SessionModification::SetHostAddress(const EOS_SessionModification_SetHostAddressOptions* Options)
{
    return EOS_EResult::EOS_Success;
}

EOS_EResult EOSSDK_SessionModification::SetPermissionLevel(const EOS_SessionModification_SetPermissionLevelOptions* Options)
{
    return EOS_EResult::EOS_Success;
}

EOS_EResult EOSSDK_SessionModification::SetJoinInProgressAllowed(const EOS_SessionModification_SetJoinInProgressAllowedOptions* Options)
{
    return EOS_EResult::EOS_Success;
}

EOS_EResult EOSSDK_SessionModification::SetMaxPlayers(const EOS_SessionModification_SetMaxPlayersOptions* Options)
{
    return EOS_EResult::EOS_Success;
}

EOS_EResult EOSSDK_SessionModification::SetInvitesAllowed(const EOS_SessionModification_SetInvitesAllowedOptions* Options)
{
    return EOS_EResult::EOS_Success;
}

EOS_EResult EOSSDK_SessionModification::AddAttribute(const EOS_SessionModification_AddAttributeOptions* Options)
{
    return EOS_EResult::EOS_Success;
}

EOS_EResult EOSSDK_SessionModification::RemoveAttribute(const EOS_SessionModification_RemoveAttributeOptions* Options)
{
    return EOS_EResult::EOS_Success;
}

void EOSSDK_SessionModification::Release()
{
    delete this;
}

}
