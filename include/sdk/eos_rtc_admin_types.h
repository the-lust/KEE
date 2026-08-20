

#pragma once

#include "eos_common.h"

#pragma pack(push, 8)

EOS_EXTERN_C typedef struct EOS_RTCAdminHandle* EOS_HRTCAdmin;

#define EOS_RTCADMIN_QUERYJOINROOMTOKEN_API_LATEST 2

EOS_STRUCT(EOS_RTCAdmin_QueryJoinRoomTokenOptions, (
	
	int32_t ApiVersion;
	
	EOS_ProductUserId LocalUserId;
	
	const char* RoomName;
	
	EOS_ProductUserId* TargetUserIds;
	
	uint32_t TargetUserIdsCount;
	
	const char** TargetUserIpAddresses;
));

EOS_STRUCT(EOS_RTCAdmin_QueryJoinRoomTokenCompleteCallbackInfo, (
	
	EOS_EResult ResultCode;
	
	void* ClientData;
	
	const char* RoomName;
	
	const char* ClientBaseUrl;
	
	uint32_t QueryId;
	
	uint32_t TokenCount;
));

EOS_DECLARE_CALLBACK(EOS_RTCAdmin_OnQueryJoinRoomTokenCompleteCallback, const EOS_RTCAdmin_QueryJoinRoomTokenCompleteCallbackInfo* Data);

#define EOS_RTCADMIN_USERTOKEN_API_LATEST 1

EOS_STRUCT(EOS_RTCAdmin_UserToken, (
	
	int32_t ApiVersion;
	
	EOS_ProductUserId ProductUserId;
	
	const char* Token;
));

#define EOS_RTCADMIN_COPYUSERTOKENBYINDEX_API_LATEST 2

EOS_STRUCT(EOS_RTCAdmin_CopyUserTokenByIndexOptions, (
	
	int32_t ApiVersion;
	
	uint32_t UserTokenIndex;
	
	uint32_t QueryId;
));

#define EOS_RTCADMIN_COPYUSERTOKENBYUSERID_API_LATEST 2

EOS_STRUCT(EOS_RTCAdmin_CopyUserTokenByUserIdOptions, (
	
	int32_t ApiVersion;
	
	EOS_ProductUserId TargetUserId;
	
	uint32_t QueryId;
));

EOS_DECLARE_FUNC(void) EOS_RTCAdmin_UserToken_Release(EOS_RTCAdmin_UserToken* UserToken);

#define EOS_RTCADMIN_KICK_API_LATEST 1

EOS_STRUCT(EOS_RTCAdmin_KickOptions, (
	
	int32_t ApiVersion;
	
	const char* RoomName;
	
	EOS_ProductUserId TargetUserId;
));

EOS_STRUCT(EOS_RTCAdmin_KickCompleteCallbackInfo, (
	
	EOS_EResult ResultCode;
	
	void* ClientData;
));

EOS_DECLARE_CALLBACK(EOS_RTCAdmin_OnKickCompleteCallback, const EOS_RTCAdmin_KickCompleteCallbackInfo* Data);

#define EOS_RTCADMIN_SETPARTICIPANTHARDMUTE_API_LATEST 1

EOS_STRUCT(EOS_RTCAdmin_SetParticipantHardMuteOptions, (
	
	int32_t ApiVersion;
	
	const char* RoomName;
	
	EOS_ProductUserId TargetUserId;
	
	EOS_Bool bMute;
));

EOS_STRUCT(EOS_RTCAdmin_SetParticipantHardMuteCompleteCallbackInfo, (
	
	EOS_EResult ResultCode;
	
	void* ClientData;
));

EOS_DECLARE_CALLBACK(EOS_RTCAdmin_OnSetParticipantHardMuteCompleteCallback, const EOS_RTCAdmin_SetParticipantHardMuteCompleteCallbackInfo* Data);

#pragma pack(pop)

