#include "eossdk_sessions.h"
#include "eossdk_platform.h"
#include "eos_client_api.h"
#include "settings.h"

namespace sdk
{

EOS_EResult EOSSDK_ActiveSession::CopyInfo(const EOS_ActiveSession_CopyInfoOptions* Options, EOS_ActiveSession_Info** OutActiveSessionInfo)
{
    *OutActiveSessionInfo = nullptr;
    return EOS_EResult::EOS_InvalidParameters;
}

uint32_t EOSSDK_ActiveSession::GetRegisteredPlayerCount(const EOS_ActiveSession_GetRegisteredPlayerCountOptions* Options)
{
    return 0;
}

EOS_ProductUserId EOSSDK_ActiveSession::GetRegisteredPlayerByIndex(const EOS_ActiveSession_GetRegisteredPlayerByIndexOptions* Options)
{
    return GetInvalidProductUserId();
}

void EOSSDK_ActiveSession::Release()
{
    delete this;
}

}
