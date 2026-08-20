#include "eossdk_sessions.h"
#include "eossdk_platform.h"
#include "eos_client_api.h"
#include "settings.h"

namespace sdk
{

EOS_EResult EOSSDK_SessionDetails::CopyInfo(const EOS_SessionDetails_CopyInfoOptions* Options, EOS_SessionDetails_Info** OutSessionInfo)
{
    *OutSessionInfo = nullptr;
    return EOS_EResult::EOS_InvalidParameters;
}

uint32_t EOSSDK_SessionDetails::GetSessionAttributeCount(const EOS_SessionDetails_GetSessionAttributeCountOptions* Options)
{
    return 0;
}

EOS_EResult EOSSDK_SessionDetails::CopySessionAttributeByIndex(const EOS_SessionDetails_CopySessionAttributeByIndexOptions* Options, EOS_SessionDetails_Attribute** OutSessionAttribute)
{
    *OutSessionAttribute = nullptr;
    return EOS_EResult::EOS_NotFound;
}

EOS_EResult EOSSDK_SessionDetails::CopySessionAttributeByKey(const EOS_SessionDetails_CopySessionAttributeByKeyOptions* Options, EOS_SessionDetails_Attribute** OutSessionAttribute)
{
    *OutSessionAttribute = nullptr;
    return EOS_EResult::EOS_NotFound;
}

void EOSSDK_SessionDetails::Release()
{
    delete this;
}

}
