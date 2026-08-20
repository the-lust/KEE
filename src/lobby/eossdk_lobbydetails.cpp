#include "eossdk_lobby.h"
#include "eos_client_api.h"

namespace sdk
{

EOSSDK_LobbyDetails::EOSSDK_LobbyDetails()
{
}

EOSSDK_LobbyDetails::~EOSSDK_LobbyDetails()
{
}

EOS_ProductUserId EOSSDK_LobbyDetails::GetLobbyOwner(const EOS_LobbyDetails_GetLobbyOwnerOptions* Options)
{
    return GetInvalidProductUserId();
}

EOS_EResult EOSSDK_LobbyDetails::CopyInfo(const EOS_LobbyDetails_CopyInfoOptions* Options, EOS_LobbyDetails_Info** OutLobbyDetailsInfo)
{
    *OutLobbyDetailsInfo = nullptr;
    return EOS_EResult::EOS_InvalidParameters;
}

uint32_t EOSSDK_LobbyDetails::GetAttributeCount(const EOS_LobbyDetails_GetAttributeCountOptions* Options)
{
    return 0;
}

EOS_EResult EOSSDK_LobbyDetails::CopyAttributeByIndex(const EOS_LobbyDetails_CopyAttributeByIndexOptions* Options, EOS_Lobby_Attribute** OutAttribute)
{
    *OutAttribute = nullptr;
    return EOS_EResult::EOS_NotFound;
}

EOS_EResult EOSSDK_LobbyDetails::CopyAttributeByKey(const EOS_LobbyDetails_CopyAttributeByKeyOptions* Options, EOS_Lobby_Attribute** OutAttribute)
{
    *OutAttribute = nullptr;
    return EOS_EResult::EOS_NotFound;
}

uint32_t EOSSDK_LobbyDetails::GetMemberCount(const EOS_LobbyDetails_GetMemberCountOptions* Options)
{
    return 0;
}

EOS_ProductUserId EOSSDK_LobbyDetails::GetMemberByIndex(const EOS_LobbyDetails_GetMemberByIndexOptions* Options)
{
    return GetInvalidProductUserId();
}

uint32_t EOSSDK_LobbyDetails::GetMemberAttributeCount(const EOS_LobbyDetails_GetMemberAttributeCountOptions* Options)
{
    return 0;
}

EOS_EResult EOSSDK_LobbyDetails::CopyMemberAttributeByIndex(const EOS_LobbyDetails_CopyMemberAttributeByIndexOptions* Options, EOS_Lobby_Attribute** OutAttribute)
{
    *OutAttribute = nullptr;
    return EOS_EResult::EOS_NotFound;
}

EOS_EResult EOSSDK_LobbyDetails::CopyMemberAttributeByKey(const EOS_LobbyDetails_CopyMemberAttributeByKeyOptions* Options, EOS_Lobby_Attribute** OutAttribute)
{
    *OutAttribute = nullptr;
    return EOS_EResult::EOS_NotFound;
}

void EOSSDK_LobbyDetails::Release()
{
    delete this;
}

}
