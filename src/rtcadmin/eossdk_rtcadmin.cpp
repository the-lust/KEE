
#include "rtcadmin/eossdk_rtcadmin.h"
#include "eossdk_platform.h"
#include "settings.h"

namespace sdk {

EOSSDK_RTCAdmin::EOSSDK_RTCAdmin()
{
    GetCB_Manager().register_callbacks(this);
}

EOSSDK_RTCAdmin::~EOSSDK_RTCAdmin()
{
    GetCB_Manager().unregister_callbacks(this);
}

void EOSSDK_RTCAdmin::QueryJoinRoomToken(
    const EOS_RTCAdmin_QueryJoinRoomTokenOptions* Options,
    void* ClientData,
    EOS_RTCAdmin_OnQueryJoinRoomTokenCompleteCallback CompletionDelegate)
{
    if (!CompletionDelegate) return;

    pFrameResult_t res(new FrameResult);
    auto& cb = res->CreateCallback<EOS_RTCAdmin_QueryJoinRoomTokenCompleteCallbackInfo>(
        [CompletionDelegate](void* data) {
            CompletionDelegate(reinterpret_cast<EOS_RTCAdmin_QueryJoinRoomTokenCompleteCallbackInfo*>(data));
        });
    cb.ResultCode = EOS_EResult::EOS_Success;
    cb.ClientData = ClientData;
    cb.RoomName   = Options ? Options->RoomName : nullptr;
    cb.ClientBaseUrl = nullptr;
    cb.QueryId    = 0;
    cb.TokenCount = 0;
    SE_CB_END(this);
}

EOS_EResult EOSSDK_RTCAdmin::CopyUserTokenByIndex(
    const EOS_RTCAdmin_CopyUserTokenByIndexOptions* Options,
    EOS_RTCAdmin_UserToken** OutUserToken)
{
    if (!OutUserToken) return EOS_EResult::EOS_InvalidParameters;
    *OutUserToken = nullptr;
    return EOS_EResult::EOS_NotFound;
}

EOS_EResult EOSSDK_RTCAdmin::CopyUserTokenByUserId(
    const EOS_RTCAdmin_CopyUserTokenByUserIdOptions* Options,
    EOS_RTCAdmin_UserToken** OutUserToken)
{
    if (!OutUserToken) return EOS_EResult::EOS_InvalidParameters;
    *OutUserToken = nullptr;
    return EOS_EResult::EOS_NotFound;
}

void EOSSDK_RTCAdmin::Kick(
    const EOS_RTCAdmin_KickOptions*,
    void* ClientData,
    EOS_RTCAdmin_OnKickCompleteCallback CompletionDelegate)
{
    if (!CompletionDelegate) return;
    EOS_RTCAdmin_KickCompleteCallbackInfo info{};
    info.ResultCode = EOS_EResult::EOS_Success;
    info.ClientData = ClientData;
    CompletionDelegate(&info);
}

void EOSSDK_RTCAdmin::SetParticipantHardMute(
    const EOS_RTCAdmin_SetParticipantHardMuteOptions*,
    void* ClientData,
    EOS_RTCAdmin_OnSetParticipantHardMuteCompleteCallback CompletionDelegate)
{
    if (!CompletionDelegate) return;
    EOS_RTCAdmin_SetParticipantHardMuteCompleteCallbackInfo info{};
    info.ResultCode = EOS_EResult::EOS_Success;
    info.ClientData = ClientData;
    CompletionDelegate(&info);
}

bool EOSSDK_RTCAdmin::CBRunFrame()  { return false; }
bool EOSSDK_RTCAdmin::RunCallbacks(pFrameResult_t res)
{
    (void)res;
    return false;
}
void EOSSDK_RTCAdmin::FreeCallback(pFrameResult_t) {}

} 
