#include "eossdk_sessions.h"
#include "eossdk_platform.h"
#include "eos_client_api.h"
#include "settings.h"

namespace sdk
{

std::atomic<uint64_t> EOSSDK_SessionSearch::search_id(0);

EOSSDK_SessionSearch::EOSSDK_SessionSearch() :
    m_released(false)
{
}

EOSSDK_SessionSearch::~EOSSDK_SessionSearch()
{
}

bool EOSSDK_SessionSearch::released()
{
    return m_released;
}

bool EOSSDK_SessionSearch::send_sessions_search(Sessions_Search_pb* search)
{
    return false;
}

bool EOSSDK_SessionSearch::on_sessions_search_response(Network_Message_pb const& msg, Sessions_Search_response_pb const& resp)
{
    return false;
}

bool EOSSDK_SessionSearch::CBRunFrame()
{
    return false;
}

bool EOSSDK_SessionSearch::RunNetwork(Network_Message_pb const& msg)
{
    return false;
}

bool EOSSDK_SessionSearch::RunCallbacks(pFrameResult_t res)
{
    return false;
}

void EOSSDK_SessionSearch::FreeCallback(pFrameResult_t res)
{
}

EOS_EResult EOSSDK_SessionSearch::SetSessionId(const EOS_SessionSearch_SetSessionIdOptions* Options)
{
    return EOS_EResult::EOS_Success;
}

EOS_EResult EOSSDK_SessionSearch::SetTargetUserId(const EOS_SessionSearch_SetTargetUserIdOptions* Options)
{
    return EOS_EResult::EOS_Success;
}

EOS_EResult EOSSDK_SessionSearch::SetParameter(const EOS_SessionSearch_SetParameterOptions* Options)
{
    return EOS_EResult::EOS_Success;
}

EOS_EResult EOSSDK_SessionSearch::RemoveParameter(const EOS_SessionSearch_RemoveParameterOptions* Options)
{
    return EOS_EResult::EOS_Success;
}

EOS_EResult EOSSDK_SessionSearch::SetMaxResults(const EOS_SessionSearch_SetMaxResultsOptions* Options)
{
    return EOS_EResult::EOS_Success;
}

void EOSSDK_SessionSearch::Find(const EOS_SessionSearch_FindOptions* Options, void* ClientData, const EOS_SessionSearch_OnFindCallback CompletionDelegate)
{
    SE_CB_BEGIN(EOS_SessionSearch_FindCallbackInfo, CompletionDelegate)
    {
        info.ResultCode = EOS_EResult::EOS_Success;
    }
    SE_CB_END(this)
}

uint32_t EOSSDK_SessionSearch::GetSearchResultCount(const EOS_SessionSearch_GetSearchResultCountOptions* Options)
{
    return 0;
}

EOS_EResult EOSSDK_SessionSearch::CopySearchResultByIndex(const EOS_SessionSearch_CopySearchResultByIndexOptions* Options, EOS_HSessionDetails* OutSessionHandle)
{
    *OutSessionHandle = nullptr;
    return EOS_EResult::EOS_NotFound;
}

void EOSSDK_SessionSearch::Release()
{
    m_released = true;
    delete this;
}

}
