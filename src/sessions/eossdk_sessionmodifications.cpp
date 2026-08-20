#include "eossdk_sessions.h"
#include "eossdk_platform.h"
#include "eos_client_api.h"
#include "settings.h"

namespace sdk
{

EOS_EResult EOSSDK_SessionModification::SetBucketId(const EOS_SessionModification_SetBucketIdOptions* Options)
{
    TRACE_FUNC();
    std::lock_guard<std::mutex> lk(m_local_mutex);
    
    if (Options == nullptr || Options->BucketId == nullptr)
        return EOS_EResult::EOS_InvalidParameters;

    m_infos.set_bucket_id(Options->BucketId);

    return EOS_EResult::EOS_Success;
}

EOS_EResult EOSSDK_SessionModification::SetHostAddress(const EOS_SessionModification_SetHostAddressOptions* Options)
{
    TRACE_FUNC();
    std::lock_guard<std::mutex> lk(m_local_mutex);
    
    if (Options == nullptr || Options->HostAddress == nullptr)
        return EOS_EResult::EOS_InvalidParameters;

    m_infos.set_host_address(Options->HostAddress);

    return EOS_EResult::EOS_Success;
}

EOS_EResult EOSSDK_SessionModification::SetPermissionLevel(const EOS_SessionModification_SetPermissionLevelOptions* Options)
{
    TRACE_FUNC();
    std::lock_guard<std::mutex> lk(m_local_mutex);
    
    if (Options == nullptr)
        return EOS_EResult::EOS_InvalidParameters;

    m_infos.set_permission_level(static_cast<int32_t>(Options->PermissionLevel));

    return EOS_EResult::EOS_Success;
}

EOS_EResult EOSSDK_SessionModification::SetJoinInProgressAllowed(const EOS_SessionModification_SetJoinInProgressAllowedOptions* Options)
{
    TRACE_FUNC();
    std::lock_guard<std::mutex> lk(m_local_mutex);
    
    if (Options == nullptr)
        return EOS_EResult::EOS_InvalidParameters;

    m_infos.set_join_in_progress_allowed(Options->bAllowJoinInProgress);

    return EOS_EResult::EOS_Success;
}

EOS_EResult EOSSDK_SessionModification::SetMaxPlayers(const EOS_SessionModification_SetMaxPlayersOptions* Options)
{
    TRACE_FUNC();
    std::lock_guard<std::mutex> lk(m_local_mutex);
    
    if (Options == nullptr)
        return EOS_EResult::EOS_InvalidParameters;

    m_infos.set_max_players(Options->MaxPlayers);

    return EOS_EResult::EOS_Success;
}

EOS_EResult EOSSDK_SessionModification::SetInvitesAllowed(const EOS_SessionModification_SetInvitesAllowedOptions* Options)
{
    TRACE_FUNC();
    std::lock_guard<std::mutex> lk(m_local_mutex);
    
    if (Options == nullptr)
        return EOS_EResult::EOS_InvalidParameters;

    m_infos.set_invites_allowed(Options->bInvitesAllowed);

    return EOS_EResult::EOS_Success;
}

EOS_EResult EOSSDK_SessionModification::AddAttribute(const EOS_SessionModification_AddAttributeOptions* Options)
{
    TRACE_FUNC();
    std::lock_guard<std::mutex> lk(m_local_mutex);
    
    if (Options == nullptr || Options->SessionAttribute == nullptr || Options->SessionAttribute->Key == nullptr ||
        (Options->SessionAttribute->ValueType == EOS_ESessionAttributeType::EOS_AT_STRING && Options->SessionAttribute->Value.AsUtf8 == nullptr))
        return EOS_EResult::EOS_InvalidParameters;

    if(strlen(Options->SessionAttribute->Key) > EOS_SESSIONMODIFICATION_MAX_SESSION_ATTRIBUTE_LENGTH)
        return EOS_EResult::EOS_InvalidParameters;

    auto it = m_infos.attributes().find(Options->SessionAttribute->Key);
    
    if(it != m_infos.attributes().end() && m_infos.attributes_size() >= EOS_SESSIONMODIFICATION_MAX_SESSION_ATTRIBUTES)
        return EOS_EResult::EOS_InvalidParameters;

    auto& attribute = (*m_infos.mutable_attributes())[Options->SessionAttribute->Key];
    attribute.set_advertisement_type(static_cast<int32_t>(Options->AdvertisementType));

    switch (Options->SessionAttribute->ValueType)
    {
        case EOS_ESessionAttributeType::EOS_AT_BOOLEAN: 
            APP_LOG(Log::LogLevel::DEBUG, "%s = %s", Options->SessionAttribute->Key, (Options->SessionAttribute->Value.AsBool == EOS_TRUE ? "EOS_TRUE" : "EOS_FALSE"));
            attribute.mutable_value()->set_b(Options->SessionAttribute->Value.AsBool);
            break;

        case EOS_ESessionAttributeType::EOS_AT_DOUBLE :
            APP_LOG(Log::LogLevel::DEBUG, "%s = %f", Options->SessionAttribute->Key, Options->SessionAttribute->Value.AsDouble);
            attribute.mutable_value()->set_d(Options->SessionAttribute->Value.AsDouble);
            break;

        case EOS_ESessionAttributeType::EOS_AT_INT64  :
            APP_LOG(Log::LogLevel::DEBUG, "%s = %lld", Options->SessionAttribute->Key, Options->SessionAttribute->Value.AsInt64);
            attribute.mutable_value()->set_i(Options->SessionAttribute->Value.AsInt64);
            break;

        case EOS_ESessionAttributeType::EOS_AT_STRING :
            APP_LOG(Log::LogLevel::DEBUG, "%s = %s", Options->SessionAttribute->Key, Options->SessionAttribute->Value.AsUtf8);
            attribute.mutable_value()->set_s(Options->SessionAttribute->Value.AsUtf8);
            break;

        default                                       :
            return EOS_EResult::EOS_InvalidParameters;
    }

    return EOS_EResult::EOS_Success;
}

EOS_EResult EOSSDK_SessionModification::RemoveAttribute(const EOS_SessionModification_RemoveAttributeOptions* Options)
{
    TRACE_FUNC();
    std::lock_guard<std::mutex> lk(m_local_mutex);
    
    if (Options == nullptr || Options->Key == nullptr)
        return EOS_EResult::EOS_InvalidParameters;

    auto &attributes = *m_infos.mutable_attributes();
    auto it = attributes.find(Options->Key);
    if (it != attributes.end())
    {
        APP_LOG(Log::LogLevel::DEBUG, "%s", Options->Key);
        attributes.erase(it);
    }

    return EOS_EResult::EOS_Success;
}

void EOSSDK_SessionModification::Release()
{
    TRACE_FUNC();

    delete this;
}

}