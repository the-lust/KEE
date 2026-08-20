#include "utils/common_includes.h"
#include "rtcadmin/eossdk_rtcadmin.h"
#include "eossdk_platform.h"

EOS_DECLARE_FUNC(void) EOS_RTCAdmin_QueryJoinRoomToken(EOS_HRTCAdmin Handle, const EOS_RTCAdmin_QueryJoinRoomTokenOptions* Options, void* ClientData, const EOS_RTCAdmin_OnQueryJoinRoomTokenCompleteCallback CompletionDelegate)
{ if (Handle) GetEOS_RTCAdmin().QueryJoinRoomToken(Options, ClientData, CompletionDelegate); }

EOS_DECLARE_FUNC(EOS_EResult) EOS_RTCAdmin_CopyUserTokenByIndex(EOS_HRTCAdmin Handle, const EOS_RTCAdmin_CopyUserTokenByIndexOptions* Options, EOS_RTCAdmin_UserToken** OutUserToken)
{ if (!Handle) return EOS_EResult::EOS_InvalidParameters; return GetEOS_RTCAdmin().CopyUserTokenByIndex(Options, OutUserToken); }

EOS_DECLARE_FUNC(EOS_EResult) EOS_RTCAdmin_CopyUserTokenByUserId(EOS_HRTCAdmin Handle, const EOS_RTCAdmin_CopyUserTokenByUserIdOptions* Options, EOS_RTCAdmin_UserToken** OutUserToken)
{ if (!Handle) return EOS_EResult::EOS_InvalidParameters; return GetEOS_RTCAdmin().CopyUserTokenByUserId(Options, OutUserToken); }

EOS_DECLARE_FUNC(void) EOS_RTCAdmin_Kick(EOS_HRTCAdmin Handle, const EOS_RTCAdmin_KickOptions* Options, void* ClientData, const EOS_RTCAdmin_OnKickCompleteCallback CompletionDelegate)
{ if (Handle) GetEOS_RTCAdmin().Kick(Options, ClientData, CompletionDelegate); }

EOS_DECLARE_FUNC(void) EOS_RTCAdmin_SetParticipantHardMute(EOS_HRTCAdmin Handle, const EOS_RTCAdmin_SetParticipantHardMuteOptions* Options, void* ClientData, const EOS_RTCAdmin_OnSetParticipantHardMuteCompleteCallback CompletionDelegate)
{ if (Handle) GetEOS_RTCAdmin().SetParticipantHardMute(Options, ClientData, CompletionDelegate); }

EOS_DECLARE_FUNC(void) EOS_RTCAdmin_UserToken_Release(EOS_RTCAdmin_UserToken* p) { delete p; }
