#include "eossdk_lobby.h"
#include "eossdk_platform.h"

namespace sdk
{

EOSSDK_LobbyModification::EOSSDK_LobbyModification():
    m_lobby_modified(false),
    m_member_modified(false)
{}

EOSSDK_LobbyModification::~EOSSDK_LobbyModification()
{}

EOS_EResult EOSSDK_LobbyModification::SetPermissionLevel(const EOS_LobbyModification_SetPermissionLevelOptions* Options)
{
    TRACE_FUNC();
    std::lock_guard<std::mutex> lk(m_local_mutex);

    if (Options == nullptr)
        return EOS_EResult::EOS_InvalidParameters;

    m_infos.set_permission_level(static_cast<int>(Options->PermissionLevel));

    m_lobby_modified = true;
    return EOS_EResult::EOS_Success;
}

EOS_EResult EOSSDK_LobbyModification::SetMaxMembers(const EOS_LobbyModification_SetMaxMembersOptions* Options)
{
    TRACE_FUNC();
    std::lock_guard<std::mutex> lk(m_local_mutex);

    if (Options == nullptr || Options->MaxMembers < m_infos.members_size() || Options->MaxMembers > EOS_LOBBY_MAX_LOBBY_MEMBERS)
        return EOS_EResult::EOS_InvalidParameters;

    m_infos.set_max_lobby_member(Options->MaxMembers);

    m_lobby_modified = true;
    return EOS_EResult::EOS_Success;
}

EOS_EResult EOSSDK_LobbyModification::AddAttribute(const EOS_LobbyModification_AddAttributeOptions* Options)
{
    TRACE_FUNC();
    std::lock_guard<std::mutex> lk(m_local_mutex);
    
    if (Options == nullptr || Options->Attribute == nullptr || Options->Attribute->Key == nullptr || 
        (Options->Attribute->ValueType == EOS_ELobbyAttributeType::EOS_AT_STRING && Options->Attribute->Value.AsUtf8 == nullptr))
        return EOS_EResult::EOS_InvalidParameters;

    if (strlen(Options->Attribute->Key) > EOS_LOBBYMODIFICATION_MAX_ATTRIBUTE_LENGTH)
        return EOS_EResult::EOS_InvalidParameters;

    auto it = m_infos.attributes().find(Options->Attribute->Key);
    
    if (it != m_infos.attributes().end() && m_infos.attributes_size() >= EOS_LOBBYMODIFICATION_MAX_ATTRIBUTES)
        return EOS_EResult::EOS_InvalidParameters;

    auto& attribute = (*m_infos.mutable_attributes())[Options->Attribute->Key];
    attribute.set_visibility_type(static_cast<int>(Options->Visibility));
    
    switch (Options->Attribute->ValueType)
    {
        case EOS_ESessionAttributeType::EOS_AT_BOOLEAN: attribute.mutable_value()->set_b(Options->Attribute->Value.AsBool); break;
        case EOS_ESessionAttributeType::EOS_AT_DOUBLE : attribute.mutable_value()->set_d(Options->Attribute->Value.AsDouble); break;
        case EOS_ESessionAttributeType::EOS_AT_INT64  : attribute.mutable_value()->set_i(Options->Attribute->Value.AsInt64); break;
        case EOS_ESessionAttributeType::EOS_AT_STRING : attribute.mutable_value()->set_s(Options->Attribute->Value.AsUtf8); break;
        default                                       : return EOS_EResult::EOS_InvalidParameters;
    }

    m_lobby_modified = true;
    return EOS_EResult::EOS_Success;
}

EOS_EResult EOSSDK_LobbyModification::RemoveAttribute(const EOS_LobbyModification_RemoveAttributeOptions* Options)
{
    TRACE_FUNC();
    std::lock_guard<std::mutex> lk(m_local_mutex);

    if (Options == nullptr || Options->Key == nullptr)
        return EOS_EResult::EOS_InvalidParameters;

    auto& attributes = (*m_infos.mutable_attributes());
    auto it = attributes.find(Options->Key);
    if (it != attributes.end())
        attributes.erase(it);

    m_lobby_modified = true;
    return EOS_EResult::EOS_Success;
}

EOS_EResult EOSSDK_LobbyModification::AddMemberAttribute(const EOS_LobbyModification_AddMemberAttributeOptions* Options)
{
    TRACE_FUNC();
    std::lock_guard<std::mutex> lk(m_local_mutex);

    if (Options == nullptr || Options->Attribute == nullptr || Options->Attribute->Key == nullptr || *Options->Attribute->Key == '\0' ||
        (Options->Attribute->ValueType == EOS_ELobbyAttributeType::EOS_AT_STRING && Options->Attribute->Value.AsUtf8 == nullptr))
        return EOS_EResult::EOS_InvalidParameters;

    if (strlen(Options->Attribute->Key) > EOS_LOBBYMODIFICATION_MAX_ATTRIBUTE_LENGTH)
        return EOS_EResult::EOS_InvalidParameters;

    auto& member_attributes = (*m_infos.mutable_members())[GetEOS_Connect().get_myself()->first->to_string()];

    auto it = member_attributes.attributes().find(Options->Attribute->Key);
    
    if (it != member_attributes.attributes().end() && member_attributes.attributes_size() >= EOS_LOBBYMODIFICATION_MAX_ATTRIBUTES)
        return EOS_EResult::EOS_InvalidParameters;

    auto& attribute = (*member_attributes.mutable_attributes())[Options->Attribute->Key];
        
    attribute.set_visibility_type(static_cast<int>(Options->Visibility));
    
    switch (Options->Attribute->ValueType)
    {
        case EOS_ESessionAttributeType::EOS_AT_BOOLEAN: attribute.mutable_value()->set_b(Options->Attribute->Value.AsBool); break;
        case EOS_ESessionAttributeType::EOS_AT_DOUBLE : attribute.mutable_value()->set_d(Options->Attribute->Value.AsDouble); break;
        case EOS_ESessionAttributeType::EOS_AT_INT64  : attribute.mutable_value()->set_i(Options->Attribute->Value.AsInt64); break;
        case EOS_ESessionAttributeType::EOS_AT_STRING : attribute.mutable_value()->set_s(Options->Attribute->Value.AsUtf8); break;
        default                                       : return EOS_EResult::EOS_InvalidParameters;
    }

    m_member_modified = true;
    return EOS_EResult::EOS_Success;
}

EOS_EResult EOSSDK_LobbyModification::RemoveMemberAttribute(const EOS_LobbyModification_RemoveMemberAttributeOptions* Options)
{
    TRACE_FUNC();
    std::lock_guard<std::mutex> lk(m_local_mutex);

    if (Options == nullptr || Options->Key == nullptr || *Options->Key == '\0')
        return EOS_EResult::EOS_InvalidParameters;

    auto& member_attributes = (*m_infos.mutable_members())[GetEOS_Connect().get_myself()->first->to_string()];
    auto& attributes = (*member_attributes.mutable_attributes());

    auto it = attributes.find(Options->Key);
    if (it != attributes.end())
        attributes.erase(it);

    m_member_modified = true;
    return EOS_EResult::EOS_Success;
}

void EOSSDK_LobbyModification::Release()
{
    TRACE_FUNC();

    delete this;
}

}