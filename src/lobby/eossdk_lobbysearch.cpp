#include "eossdk_lobby.h"
#include "eossdk_platform.h"
#include "eos_client_api.h"
#include "settings.h"

namespace sdk
{

std::atomic<uint64_t> EOSSDK_LobbySearch::search_id(0);

EOSSDK_LobbySearch::EOSSDK_LobbySearch() :
    m_released(false),
    m_max_results(0)
{
}

EOSSDK_LobbySearch::~EOSSDK_LobbySearch()
{
}

bool EOSSDK_LobbySearch::released()
{
    return m_released;
}

bool EOSSDK_LobbySearch::send_lobbies_search(Lobbies_Search_pb* search)
{
    return false;
}

bool EOSSDK_LobbySearch::on_lobbies_search_response(Network_Message_pb const& msg, Lobbies_Search_response_pb const& resp)
{
    return false;
}

bool EOSSDK_LobbySearch::CBRunFrame()
{
    return false;
}

bool EOSSDK_LobbySearch::RunNetwork(Network_Message_pb const& msg)
{
    return false;
}

bool EOSSDK_LobbySearch::RunCallbacks(pFrameResult_t res)
{
    return false;
}

void EOSSDK_LobbySearch::FreeCallback(pFrameResult_t res)
{
}

void EOSSDK_LobbySearch::Find(const EOS_LobbySearch_FindOptions* Options, void* ClientData, const EOS_LobbySearch_OnFindCallback CompletionDelegate)
{
    SE_CB_BEGIN(EOS_LobbySearch_FindCallbackInfo, CompletionDelegate)
    {
        info.ResultCode = EOS_EResult::EOS_Success;
    }
    SE_CB_END(this)
}

EOS_EResult EOSSDK_LobbySearch::SetLobbyId(const EOS_LobbySearch_SetLobbyIdOptions* Options)
{
    return EOS_EResult::EOS_Success;
}

EOS_EResult EOSSDK_LobbySearch::SetTargetUserId(const EOS_LobbySearch_SetTargetUserIdOptions* Options)
{
    return EOS_EResult::EOS_Success;
}

EOS_EResult EOSSDK_LobbySearch::SetParameter(const EOS_LobbySearch_SetParameterOptions* Options)
{
    return EOS_EResult::EOS_Success;
}

EOS_EResult EOSSDK_LobbySearch::RemoveParameter(const EOS_LobbySearch_RemoveParameterOptions* Options)
{
    return EOS_EResult::EOS_Success;
}

EOS_EResult EOSSDK_LobbySearch::SetMaxResults(const EOS_LobbySearch_SetMaxResultsOptions* Options)
{
    return EOS_EResult::EOS_Success;
}

uint32_t EOSSDK_LobbySearch::GetSearchResultCount(const EOS_LobbySearch_GetSearchResultCountOptions* Options)
{
    return 0;
}

EOS_EResult EOSSDK_LobbySearch::CopySearchResultByIndex(const EOS_LobbySearch_CopySearchResultByIndexOptions* Options, EOS_HLobbyDetails* OutLobbyDetailsHandle)
{
    *OutLobbyDetailsHandle = nullptr;
    return EOS_EResult::EOS_NotFound;
}

void EOSSDK_LobbySearch::Release()
{
    m_released = true;
    delete this;
}

}
