
#pragma once

#include "eos_common.h"

#pragma pack(push, 8)

EOS_EXTERN_C typedef struct EOS_FriendsHandle* EOS_HFriends;

#define EOS_FRIENDS_QUERYFRIENDS_API_LATEST 1

EOS_STRUCT(EOS_Friends_QueryFriendsOptions, (
	
	int32_t ApiVersion;
	
	EOS_EpicAccountId LocalUserId;
));

EOS_STRUCT(EOS_Friends_QueryFriendsCallbackInfo, (
	
	EOS_EResult ResultCode;
	
	void* ClientData;
	
	EOS_EpicAccountId LocalUserId;
));

EOS_DECLARE_CALLBACK(EOS_Friends_OnQueryFriendsCallback, const EOS_Friends_QueryFriendsCallbackInfo* Data);

#define EOS_FRIENDS_SENDINVITE_API_LATEST 1

EOS_STRUCT(EOS_Friends_SendInviteOptions, (
	
	int32_t ApiVersion;
	
	EOS_EpicAccountId LocalUserId;
	
	EOS_EpicAccountId TargetUserId;
));

EOS_STRUCT(EOS_Friends_SendInviteCallbackInfo, (
	
	EOS_EResult ResultCode;
	
	void* ClientData;
	
	EOS_EpicAccountId LocalUserId;
	
	EOS_EpicAccountId TargetUserId;
));

EOS_DECLARE_CALLBACK(EOS_Friends_OnSendInviteCallback, const EOS_Friends_SendInviteCallbackInfo* Data);

#define EOS_FRIENDS_ACCEPTINVITE_API_LATEST 1

EOS_STRUCT(EOS_Friends_AcceptInviteOptions, (
	
	int32_t ApiVersion;
	
	EOS_EpicAccountId LocalUserId;
	
	EOS_EpicAccountId TargetUserId;
));

EOS_STRUCT(EOS_Friends_AcceptInviteCallbackInfo, (
	
	EOS_EResult ResultCode;
	
	void* ClientData;
	
	EOS_EpicAccountId LocalUserId;
	
	EOS_EpicAccountId TargetUserId;
));

EOS_DECLARE_CALLBACK(EOS_Friends_OnAcceptInviteCallback, const EOS_Friends_AcceptInviteCallbackInfo* Data);

#define EOS_FRIENDS_REJECTINVITE_API_LATEST 1

EOS_STRUCT(EOS_Friends_RejectInviteOptions, (
	
	int32_t ApiVersion;
	
	EOS_EpicAccountId LocalUserId;
	
	EOS_EpicAccountId TargetUserId;
));

EOS_STRUCT(EOS_Friends_RejectInviteCallbackInfo, (
	
	EOS_EResult ResultCode;
	
	void* ClientData;
	
	EOS_EpicAccountId LocalUserId;
	
	EOS_EpicAccountId TargetUserId;
));

EOS_DECLARE_CALLBACK(EOS_Friends_OnRejectInviteCallback, const EOS_Friends_RejectInviteCallbackInfo* Data);

#define EOS_FRIENDS_GETFRIENDSCOUNT_API_LATEST 1

EOS_STRUCT(EOS_Friends_GetFriendsCountOptions, (
	
	int32_t ApiVersion;
	
	EOS_EpicAccountId LocalUserId;
));

#define EOS_FRIENDS_GETFRIENDATINDEX_API_LATEST 1

EOS_STRUCT(EOS_Friends_GetFriendAtIndexOptions, (
	
	int32_t ApiVersion;
	
	EOS_EpicAccountId LocalUserId;
	
	int32_t Index;
));

#define EOS_FRIENDS_GETSTATUS_API_LATEST 1

EOS_ENUM(EOS_EFriendsStatus,
	
	EOS_FS_NotFriends = 0,

	EOS_FS_InviteSent = 1,

	EOS_FS_InviteReceived = 2,

	EOS_FS_Friends = 3
);

EOS_STRUCT(EOS_Friends_GetStatusOptions, (
	
	int32_t ApiVersion;
	
	EOS_EpicAccountId LocalUserId;
	
	EOS_EpicAccountId TargetUserId;
));

#define EOS_FRIENDS_ADDNOTIFYFRIENDSUPDATE_API_LATEST 1

EOS_STRUCT(EOS_Friends_AddNotifyFriendsUpdateOptions, (
	
	int32_t ApiVersion;
));

EOS_STRUCT(EOS_Friends_OnFriendsUpdateInfo, (
	
	void* ClientData;
	
	EOS_EpicAccountId LocalUserId;
	
	EOS_EpicAccountId TargetUserId;
	
	EOS_EFriendsStatus PreviousStatus;
	
	EOS_EFriendsStatus CurrentStatus;
));

EOS_DECLARE_CALLBACK(EOS_Friends_OnFriendsUpdateCallback, const EOS_Friends_OnFriendsUpdateInfo* Data);

#define EOS_FRIENDS_GETBLOCKEDUSERSCOUNT_API_LATEST 1

EOS_STRUCT(EOS_Friends_GetBlockedUsersCountOptions, (
	
	int32_t ApiVersion;
	
	EOS_EpicAccountId LocalUserId;
));

#define EOS_FRIENDS_GETBLOCKEDUSERATINDEX_API_LATEST 1

EOS_STRUCT(EOS_Friends_GetBlockedUserAtIndexOptions, (
	
	int32_t ApiVersion;
	
	EOS_EpicAccountId LocalUserId;
	
	int32_t Index;
));

#define EOS_FRIENDS_ADDNOTIFYBLOCKEDUSERSUPDATE_API_LATEST 1

EOS_STRUCT(EOS_Friends_AddNotifyBlockedUsersUpdateOptions, (
	
	int32_t ApiVersion;
));

EOS_STRUCT(EOS_Friends_OnBlockedUsersUpdateInfo, (
	
	void* ClientData;
	
	EOS_EpicAccountId LocalUserId;
	
	EOS_EpicAccountId TargetUserId;
	
	EOS_Bool bBlocked;
));

EOS_DECLARE_CALLBACK(EOS_Friends_OnBlockedUsersUpdateCallback, const EOS_Friends_OnBlockedUsersUpdateInfo* Data);

#pragma pack(pop)
