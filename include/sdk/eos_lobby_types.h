
#pragma once

#include "eos_common.h"
#include "eos_ui_types.h"

#pragma pack(push, 8)

EOS_EXTERN_C typedef struct EOS_LobbyHandle* EOS_HLobby;

EOS_EXTERN_C typedef struct EOS_LobbyModificationHandle* EOS_HLobbyModification;

EOS_EXTERN_C typedef struct EOS_LobbyDetailsHandle* EOS_HLobbyDetails;

EOS_EXTERN_C typedef struct EOS_LobbySearchHandle* EOS_HLobbySearch;

EOS_DECLARE_FUNC(void) EOS_LobbyModification_Release(EOS_HLobbyModification LobbyModificationHandle);

EOS_DECLARE_FUNC(void) EOS_LobbyDetails_Release(EOS_HLobbyDetails LobbyHandle);

EOS_DECLARE_FUNC(void) EOS_LobbySearch_Release(EOS_HLobbySearch LobbySearchHandle);

EOS_EXTERN_C typedef const char* EOS_LobbyId;

#define EOS_LOBBY_MAX_LOBBIES 16

#define EOS_LOBBY_MAX_LOBBY_MEMBERS 64

#define EOS_LOBBY_MAX_SEARCH_RESULTS 200

#define EOS_LOBBY_MIN_LOBBYIDOVERRIDE_LENGTH 4

#define EOS_LOBBY_MAX_LOBBYIDOVERRIDE_LENGTH 60

#define EOS_LOBBYMODIFICATION_MAX_ATTRIBUTES 64

#define EOS_LOBBYMODIFICATION_MAX_ATTRIBUTE_LENGTH 64

EOS_ENUM(EOS_ELobbyPermissionLevel,
	
	EOS_LPL_PUBLICADVERTISED = 0,
	
	EOS_LPL_JOINVIAPRESENCE = 1,
	
	EOS_LPL_INVITEONLY = 2
);

EOS_ENUM(EOS_ELobbyAttributeVisibility,
	
	EOS_LAT_PUBLIC = 0,
	
	EOS_LAT_PRIVATE = 1
);

EOS_ENUM(EOS_ELobbyMemberStatus,
	
	EOS_LMS_JOINED = 0,
	
	EOS_LMS_LEFT = 1,
	
	EOS_LMS_DISCONNECTED = 2,
	
	EOS_LMS_KICKED = 3,
	
	EOS_LMS_PROMOTED = 4,
	
	EOS_LMS_CLOSED = 5
);

EOS_ENUM(EOS_ELobbyRTCRoomJoinActionType,
	
	EOS_LRRJAT_AutomaticJoin = 0,
	
	EOS_LRRJAT_ManualJoin = 1
);

#define EOS_LOBBYDETAILS_INFO_API_LATEST 3

EOS_STRUCT(EOS_LobbyDetails_Info, (
	
	int32_t ApiVersion;
	
	EOS_LobbyId LobbyId;
	
	EOS_ProductUserId LobbyOwnerUserId;
	
	EOS_ELobbyPermissionLevel PermissionLevel;
	
	uint32_t AvailableSlots;
	
	uint32_t MaxMembers;
	
	EOS_Bool bAllowInvites;
	
	const char* BucketId;
	
	EOS_Bool bAllowHostMigration;
	
	EOS_Bool bRTCRoomEnabled;
	
	EOS_Bool bAllowJoinById;
	
	EOS_Bool bRejoinAfterKickRequiresInvite;
	
	EOS_Bool bPresenceEnabled;
	
	const uint32_t* AllowedPlatformIds;
	
	uint32_t AllowedPlatformIdsCount;
));

EOS_DECLARE_FUNC(void) EOS_LobbyDetails_Info_Release(EOS_LobbyDetails_Info* LobbyDetailsInfo);

#define EOS_LOBBY_LOCALRTCOPTIONS_API_LATEST 2

EOS_STRUCT(EOS_Lobby_LocalRTCOptions, (
	
	int32_t ApiVersion;
	
	uint32_t Flags;
	
	EOS_Bool bUseManualAudioInput;
	
	EOS_Bool bUseManualAudioOutput;
	
	EOS_Bool bLocalAudioDeviceInputStartsMuted;
	
	void* Reserved;
));

#define EOS_LOBBY_CREATELOBBY_API_LATEST 10

EOS_STRUCT(EOS_Lobby_CreateLobbyOptions, (
	
	int32_t ApiVersion;
	
	EOS_ProductUserId LocalUserId;
	
	uint32_t MaxLobbyMembers;
	
	EOS_ELobbyPermissionLevel PermissionLevel;
	
	EOS_Bool bPresenceEnabled;
	
	EOS_Bool bAllowInvites;
	
	const char* BucketId;
	
	EOS_Bool bDisableHostMigration;
	
	EOS_Bool bEnableRTCRoom;
	
	const EOS_Lobby_LocalRTCOptions* LocalRTCOptions;
	
	EOS_LobbyId LobbyId;
	
	EOS_Bool bEnableJoinById;
	
	EOS_Bool bRejoinAfterKickRequiresInvite;
	
	const uint32_t* AllowedPlatformIds;
	
	uint32_t AllowedPlatformIdsCount;
	
	EOS_Bool bCrossplayOptOut;
	
	EOS_ELobbyRTCRoomJoinActionType RTCRoomJoinActionType;
));

EOS_STRUCT(EOS_Lobby_CreateLobbyCallbackInfo, (
	
	EOS_EResult ResultCode;
	
	void* ClientData;
	
	EOS_LobbyId LobbyId;
));

EOS_DECLARE_CALLBACK(EOS_Lobby_OnCreateLobbyCallback, const EOS_Lobby_CreateLobbyCallbackInfo* Data);

#define EOS_LOBBY_DESTROYLOBBY_API_LATEST 1

EOS_STRUCT(EOS_Lobby_DestroyLobbyOptions, (
	
	int32_t ApiVersion;
	
	EOS_ProductUserId LocalUserId;
	
	EOS_LobbyId LobbyId;
));

EOS_STRUCT(EOS_Lobby_DestroyLobbyCallbackInfo, (
	
	EOS_EResult ResultCode;
	
	void* ClientData;
	
	EOS_LobbyId LobbyId;
));

EOS_DECLARE_CALLBACK(EOS_Lobby_OnDestroyLobbyCallback, const EOS_Lobby_DestroyLobbyCallbackInfo* Data);

#define EOS_LOBBY_JOINLOBBY_API_LATEST 5

EOS_STRUCT(EOS_Lobby_JoinLobbyOptions, (
	
	int32_t ApiVersion;
	
	EOS_HLobbyDetails LobbyDetailsHandle;
	
	EOS_ProductUserId LocalUserId;
	
	EOS_Bool bPresenceEnabled;
	
	const EOS_Lobby_LocalRTCOptions* LocalRTCOptions;
	
	EOS_Bool bCrossplayOptOut;
	
	EOS_ELobbyRTCRoomJoinActionType RTCRoomJoinActionType;
));

EOS_STRUCT(EOS_Lobby_JoinLobbyCallbackInfo, (
	
	EOS_EResult ResultCode;
	
	void* ClientData;
	
	EOS_LobbyId LobbyId;
));

EOS_DECLARE_CALLBACK(EOS_Lobby_OnJoinLobbyCallback, const EOS_Lobby_JoinLobbyCallbackInfo* Data);

#define EOS_LOBBY_JOINLOBBYBYID_API_LATEST 3

EOS_STRUCT(EOS_Lobby_JoinLobbyByIdOptions, (
	
	int32_t ApiVersion;
	
	EOS_LobbyId LobbyId;
	
	EOS_ProductUserId LocalUserId;
	
	EOS_Bool bPresenceEnabled;
	
	const EOS_Lobby_LocalRTCOptions* LocalRTCOptions;
	
	EOS_Bool bCrossplayOptOut;
	
	EOS_ELobbyRTCRoomJoinActionType RTCRoomJoinActionType;
));

EOS_STRUCT(EOS_Lobby_JoinLobbyByIdCallbackInfo, (
	
	EOS_EResult ResultCode;
	
	void* ClientData;
	
	EOS_LobbyId LobbyId;
));

EOS_DECLARE_CALLBACK(EOS_Lobby_OnJoinLobbyByIdCallback, const EOS_Lobby_JoinLobbyByIdCallbackInfo* Data);

#define EOS_LOBBY_LEAVELOBBY_API_LATEST 1

EOS_STRUCT(EOS_Lobby_LeaveLobbyOptions, (
	
	int32_t ApiVersion;
	
	EOS_ProductUserId LocalUserId;
	
	EOS_LobbyId LobbyId;
));

EOS_STRUCT(EOS_Lobby_LeaveLobbyCallbackInfo, (
	
	EOS_EResult ResultCode;
	
	void* ClientData;
	
	EOS_LobbyId LobbyId;
));

EOS_DECLARE_CALLBACK(EOS_Lobby_OnLeaveLobbyCallback, const EOS_Lobby_LeaveLobbyCallbackInfo* Data);

#define EOS_LOBBY_UPDATELOBBYMODIFICATION_API_LATEST 1

EOS_STRUCT(EOS_Lobby_UpdateLobbyModificationOptions, (
	
	int32_t ApiVersion;
	
	EOS_ProductUserId LocalUserId;
	
	EOS_LobbyId LobbyId;
));

#define EOS_LOBBY_UPDATELOBBY_API_LATEST 1

EOS_STRUCT(EOS_Lobby_UpdateLobbyOptions, (
	
	int32_t ApiVersion;
	
	EOS_HLobbyModification LobbyModificationHandle;
));

EOS_STRUCT(EOS_Lobby_UpdateLobbyCallbackInfo, (
	
	EOS_EResult ResultCode;
	
	void* ClientData;
	
	EOS_LobbyId LobbyId;
));

EOS_DECLARE_CALLBACK(EOS_Lobby_OnUpdateLobbyCallback, const EOS_Lobby_UpdateLobbyCallbackInfo* Data);

#define EOS_LOBBY_JOINRTCROOM_API_LATEST 1

EOS_STRUCT(EOS_Lobby_JoinRTCRoomOptions, (
	
	int32_t ApiVersion;
	
	EOS_LobbyId LobbyId;
	
	EOS_ProductUserId LocalUserId;
	
	const EOS_Lobby_LocalRTCOptions* LocalRTCOptions;
));

EOS_STRUCT(EOS_Lobby_JoinRTCRoomCallbackInfo, (
	
	EOS_EResult ResultCode;
	
	void* ClientData;
	
	EOS_LobbyId LobbyId;
));

EOS_DECLARE_CALLBACK(EOS_Lobby_OnJoinRTCRoomCallback, const EOS_Lobby_JoinRTCRoomCallbackInfo* Data);

#define EOS_LOBBY_LEAVERTCROOM_API_LATEST 1

EOS_STRUCT(EOS_Lobby_LeaveRTCRoomOptions, (
	
	int32_t ApiVersion;
	
	EOS_LobbyId LobbyId;
	
	EOS_ProductUserId LocalUserId;
));

EOS_STRUCT(EOS_Lobby_LeaveRTCRoomCallbackInfo, (
	
	EOS_EResult ResultCode;
	
	void* ClientData;
	
	EOS_LobbyId LobbyId;
));

EOS_DECLARE_CALLBACK(EOS_Lobby_OnLeaveRTCRoomCallback, const EOS_Lobby_LeaveRTCRoomCallbackInfo* Data);

#define EOS_LOBBY_PROMOTEMEMBER_API_LATEST 1

EOS_STRUCT(EOS_Lobby_PromoteMemberOptions, (
	
	int32_t ApiVersion;
	
	EOS_LobbyId LobbyId;
	
	EOS_ProductUserId LocalUserId;
	
	EOS_ProductUserId TargetUserId;
));

EOS_STRUCT(EOS_Lobby_PromoteMemberCallbackInfo, (
	
	EOS_EResult ResultCode;
	
	void* ClientData;
	
	EOS_LobbyId LobbyId;
));

EOS_DECLARE_CALLBACK(EOS_Lobby_OnPromoteMemberCallback, const EOS_Lobby_PromoteMemberCallbackInfo* Data);

#define EOS_LOBBY_KICKMEMBER_API_LATEST 1

EOS_STRUCT(EOS_Lobby_KickMemberOptions, (
	
	int32_t ApiVersion;
	
	EOS_LobbyId LobbyId;
	
	EOS_ProductUserId LocalUserId;
	
	EOS_ProductUserId TargetUserId;
));

EOS_STRUCT(EOS_Lobby_KickMemberCallbackInfo, (
	
	EOS_EResult ResultCode;
	
	void* ClientData;
	
	EOS_LobbyId LobbyId;
));

EOS_DECLARE_CALLBACK(EOS_Lobby_OnKickMemberCallback, const EOS_Lobby_KickMemberCallbackInfo* Data);

#define EOS_LOBBY_HARDMUTEMEMBER_API_LATEST 1

EOS_STRUCT(EOS_Lobby_HardMuteMemberOptions, (
	
	int32_t ApiVersion;
	
	EOS_LobbyId LobbyId;
	
	EOS_ProductUserId LocalUserId;
	
	EOS_ProductUserId TargetUserId;
	
	EOS_Bool bHardMute;
));

EOS_STRUCT(EOS_Lobby_HardMuteMemberCallbackInfo, (
	
	EOS_EResult ResultCode;
	
	void* ClientData;
	
	EOS_LobbyId LobbyId;
	
	EOS_ProductUserId TargetUserId;
));

EOS_DECLARE_CALLBACK(EOS_Lobby_OnHardMuteMemberCallback, const EOS_Lobby_HardMuteMemberCallbackInfo* Data);

#define EOS_LOBBY_ADDNOTIFYLOBBYUPDATERECEIVED_API_LATEST 1

EOS_STRUCT(EOS_Lobby_AddNotifyLobbyUpdateReceivedOptions, (
	
	int32_t ApiVersion;
));

EOS_STRUCT(EOS_Lobby_LobbyUpdateReceivedCallbackInfo, (
	
	void* ClientData;
	
	EOS_LobbyId LobbyId;
));

EOS_DECLARE_CALLBACK(EOS_Lobby_OnLobbyUpdateReceivedCallback, const EOS_Lobby_LobbyUpdateReceivedCallbackInfo* Data);

#define EOS_LOBBY_ADDNOTIFYLOBBYMEMBERUPDATERECEIVED_API_LATEST 1

EOS_STRUCT(EOS_Lobby_AddNotifyLobbyMemberUpdateReceivedOptions, (
	
	int32_t ApiVersion;
));

EOS_STRUCT(EOS_Lobby_LobbyMemberUpdateReceivedCallbackInfo, (
	
	void* ClientData;
	
	EOS_LobbyId LobbyId;
	
	EOS_ProductUserId TargetUserId;
));

EOS_DECLARE_CALLBACK(EOS_Lobby_OnLobbyMemberUpdateReceivedCallback, const EOS_Lobby_LobbyMemberUpdateReceivedCallbackInfo* Data);

#define EOS_LOBBY_ADDNOTIFYLOBBYMEMBERSTATUSRECEIVED_API_LATEST 1

EOS_STRUCT(EOS_Lobby_AddNotifyLobbyMemberStatusReceivedOptions, (
	
	int32_t ApiVersion;
));

EOS_STRUCT(EOS_Lobby_LobbyMemberStatusReceivedCallbackInfo, (
	
	void* ClientData;
	
	EOS_LobbyId LobbyId;
	
	EOS_ProductUserId TargetUserId;
	
	EOS_ELobbyMemberStatus CurrentStatus;
));

EOS_DECLARE_CALLBACK(EOS_Lobby_OnLobbyMemberStatusReceivedCallback, const EOS_Lobby_LobbyMemberStatusReceivedCallbackInfo* Data);

#define EOS_LOBBY_INVITEID_MAX_LENGTH 64

#define EOS_LOBBY_ADDNOTIFYLOBBYINVITERECEIVED_API_LATEST 1

EOS_STRUCT(EOS_Lobby_AddNotifyLobbyInviteReceivedOptions, (
	
	int32_t ApiVersion;
));

EOS_STRUCT(EOS_Lobby_LobbyInviteReceivedCallbackInfo, (
	
	void* ClientData;
	
	const char* InviteId;
	
	EOS_ProductUserId LocalUserId;
	
	EOS_ProductUserId TargetUserId;
));

EOS_DECLARE_CALLBACK(EOS_Lobby_OnLobbyInviteReceivedCallback, const EOS_Lobby_LobbyInviteReceivedCallbackInfo* Data);

#define EOS_LOBBY_ADDNOTIFYLOBBYINVITEACCEPTED_API_LATEST 1

EOS_STRUCT(EOS_Lobby_AddNotifyLobbyInviteAcceptedOptions, (
	
	int32_t ApiVersion;
));

EOS_STRUCT(EOS_Lobby_LobbyInviteAcceptedCallbackInfo, (
	
	void* ClientData;
	
	const char* InviteId;
	
	EOS_ProductUserId LocalUserId;
	
	EOS_ProductUserId TargetUserId;
	
	EOS_LobbyId LobbyId;
));

EOS_DECLARE_CALLBACK(EOS_Lobby_OnLobbyInviteAcceptedCallback, const EOS_Lobby_LobbyInviteAcceptedCallbackInfo* Data);

#define EOS_LOBBY_ADDNOTIFYJOINLOBBYACCEPTED_API_LATEST 1

EOS_STRUCT(EOS_Lobby_AddNotifyJoinLobbyAcceptedOptions, (
	
	int32_t ApiVersion;
));

#define EOS_LOBBY_ADDNOTIFYLOBBYINVITEREJECTED_API_LATEST 1

EOS_STRUCT(EOS_Lobby_AddNotifyLobbyInviteRejectedOptions, (
	
	int32_t ApiVersion;
));

EOS_STRUCT(EOS_Lobby_LobbyInviteRejectedCallbackInfo, (
	
	void* ClientData;
	
	const char* InviteId;
	
	EOS_ProductUserId LocalUserId;
	
	EOS_ProductUserId TargetUserId;
	
	EOS_LobbyId LobbyId;
));

EOS_DECLARE_CALLBACK(EOS_Lobby_OnLobbyInviteRejectedCallback, const EOS_Lobby_LobbyInviteRejectedCallbackInfo* Data);

EOS_STRUCT(EOS_Lobby_JoinLobbyAcceptedCallbackInfo, (
	
	void* ClientData;
	
	EOS_ProductUserId LocalUserId;
	
	EOS_UI_EventId UiEventId;
));

EOS_DECLARE_CALLBACK(EOS_Lobby_OnJoinLobbyAcceptedCallback, const EOS_Lobby_JoinLobbyAcceptedCallbackInfo* Data);

#define EOS_LOBBY_ADDNOTIFYSENDLOBBYNATIVEINVITEREQUESTED_API_LATEST 1

EOS_STRUCT(EOS_Lobby_AddNotifySendLobbyNativeInviteRequestedOptions, (
	
	int32_t ApiVersion;
));

EOS_STRUCT(EOS_Lobby_SendLobbyNativeInviteRequestedCallbackInfo, (
	
	void* ClientData;
	
	EOS_UI_EventId UiEventId;
	
	EOS_ProductUserId LocalUserId;
	
	EOS_IntegratedPlatformType TargetNativeAccountType;
	
	const char* TargetUserNativeAccountId;
	
	EOS_LobbyId LobbyId;
));

EOS_DECLARE_CALLBACK(EOS_Lobby_OnSendLobbyNativeInviteRequestedCallback, const EOS_Lobby_SendLobbyNativeInviteRequestedCallbackInfo* Data);

#define EOS_LOBBY_COPYLOBBYDETAILSHANDLEBYINVITEID_API_LATEST 1

EOS_STRUCT(EOS_Lobby_CopyLobbyDetailsHandleByInviteIdOptions, (
	
	int32_t ApiVersion;
	
	const char* InviteId;
));

#define EOS_LOBBY_COPYLOBBYDETAILSHANDLEBYUIEVENTID_API_LATEST 1

EOS_STRUCT(EOS_Lobby_CopyLobbyDetailsHandleByUiEventIdOptions, (
	
	int32_t ApiVersion;
	
	EOS_UI_EventId UiEventId;
));

#define EOS_LOBBY_CREATELOBBYSEARCH_API_LATEST 1

EOS_STRUCT(EOS_Lobby_CreateLobbySearchOptions, (
	
	int32_t ApiVersion;
	
	uint32_t MaxResults;
));

#define EOS_LOBBY_SENDINVITE_API_LATEST 1

EOS_STRUCT(EOS_Lobby_SendInviteOptions, (
	
	int32_t ApiVersion;
	
	EOS_LobbyId LobbyId;
	
	EOS_ProductUserId LocalUserId;
	
	EOS_ProductUserId TargetUserId;
));

EOS_STRUCT(EOS_Lobby_SendInviteCallbackInfo, (
	
	EOS_EResult ResultCode;
	
	void* ClientData;
	
	EOS_LobbyId LobbyId;
));

EOS_DECLARE_CALLBACK(EOS_Lobby_OnSendInviteCallback, const EOS_Lobby_SendInviteCallbackInfo* Data);

#define EOS_LOBBY_REJECTINVITE_API_LATEST 1

EOS_STRUCT(EOS_Lobby_RejectInviteOptions, (
	
	int32_t ApiVersion;
	
	const char* InviteId;
	
	EOS_ProductUserId LocalUserId;
));

EOS_STRUCT(EOS_Lobby_RejectInviteCallbackInfo, (
	
	EOS_EResult ResultCode;
	
	void* ClientData;
	
	const char* InviteId;
));

EOS_DECLARE_CALLBACK(EOS_Lobby_OnRejectInviteCallback, const EOS_Lobby_RejectInviteCallbackInfo* Data);

#define EOS_LOBBY_QUERYINVITES_API_LATEST 1

EOS_STRUCT(EOS_Lobby_QueryInvitesOptions, (
	
	int32_t ApiVersion;
	
	EOS_ProductUserId LocalUserId;
));

EOS_STRUCT(EOS_Lobby_QueryInvitesCallbackInfo, (
	
	EOS_EResult ResultCode;
	
	void* ClientData;
	
	EOS_ProductUserId LocalUserId;
));

EOS_DECLARE_CALLBACK(EOS_Lobby_OnQueryInvitesCallback, const EOS_Lobby_QueryInvitesCallbackInfo* Data);

#define EOS_LOBBY_GETINVITECOUNT_API_LATEST 1

EOS_STRUCT(EOS_Lobby_GetInviteCountOptions, (
	
	int32_t ApiVersion;
	
	EOS_ProductUserId LocalUserId;
));

#define EOS_LOBBY_GETINVITEIDBYINDEX_API_LATEST 1

EOS_STRUCT(EOS_Lobby_GetInviteIdByIndexOptions, (
	
	int32_t ApiVersion;
	
	EOS_ProductUserId LocalUserId;
	
	uint32_t Index;
));

#define EOS_LOBBY_COPYLOBBYDETAILSHANDLE_API_LATEST 1

EOS_STRUCT(EOS_Lobby_CopyLobbyDetailsHandleOptions, (
	
	int32_t ApiVersion;
	
	EOS_LobbyId LobbyId;
	
	EOS_ProductUserId LocalUserId;
));

#define EOS_LOBBY_GETRTCROOMNAME_API_LATEST 1

EOS_STRUCT(EOS_Lobby_GetRTCRoomNameOptions, (
	
	int32_t ApiVersion;
	
	EOS_LobbyId LobbyId;
	
	EOS_ProductUserId LocalUserId;
));

#define EOS_LOBBY_ISRTCROOMCONNECTED_API_LATEST 1

EOS_STRUCT(EOS_Lobby_IsRTCRoomConnectedOptions, (
	
	int32_t ApiVersion;
	
	EOS_LobbyId LobbyId;
	
	EOS_ProductUserId LocalUserId;
));

#define EOS_LOBBY_ADDNOTIFYRTCROOMCONNECTIONCHANGED_API_LATEST 2

EOS_STRUCT(EOS_Lobby_AddNotifyRTCRoomConnectionChangedOptions, (
	
	int32_t ApiVersion;
	
	EOS_LobbyId LobbyId_DEPRECATED;
	
	EOS_ProductUserId LocalUserId_DEPRECATED;
));

EOS_STRUCT(EOS_Lobby_RTCRoomConnectionChangedCallbackInfo, (
	
	void* ClientData;
	
	EOS_LobbyId LobbyId;
	
	EOS_ProductUserId LocalUserId;
	
	EOS_Bool bIsConnected;
	
	EOS_EResult DisconnectReason;
));

EOS_DECLARE_CALLBACK(EOS_Lobby_OnRTCRoomConnectionChangedCallback, const EOS_Lobby_RTCRoomConnectionChangedCallbackInfo* Data);

#define EOS_LOBBY_SEARCH_BUCKET_ID "bucket"

#define EOS_LOBBY_SEARCH_MINCURRENTMEMBERS "mincurrentmembers"

#define EOS_LOBBY_SEARCH_MINSLOTSAVAILABLE "minslotsavailable"

#define EOS_LOBBY_ATTRIBUTEDATA_API_LATEST 1

EOS_STRUCT(EOS_Lobby_AttributeData, (
	
	int32_t ApiVersion;
	
	const char* Key;
	
	union
	{
		
		int64_t AsInt64;
		
		double AsDouble;
		
		EOS_Bool AsBool;
		
		const char* AsUtf8;
	} Value;

	EOS_ELobbyAttributeType ValueType;
));

#define EOS_LOBBY_ATTRIBUTE_API_LATEST 1

EOS_STRUCT(EOS_Lobby_Attribute, (
	
	int32_t ApiVersion;
	
	EOS_Lobby_AttributeData* Data;
	
	EOS_ELobbyAttributeVisibility Visibility;
));

EOS_DECLARE_FUNC(void) EOS_Lobby_Attribute_Release(EOS_Lobby_Attribute* LobbyAttribute);

#define EOS_LOBBY_GETCONNECTSTRING_API_LATEST 1

#define EOS_LOBBY_GETCONNECTSTRING_BUFFER_SIZE 256

EOS_STRUCT(EOS_Lobby_GetConnectStringOptions, (
	
	int32_t ApiVersion;
	
	EOS_ProductUserId LocalUserId;
	
	EOS_LobbyId LobbyId;
));

#define EOS_LOBBY_PARSECONNECTSTRING_API_LATEST 1

#define EOS_LOBBY_PARSECONNECTSTRING_BUFFER_SIZE 256

EOS_STRUCT(EOS_Lobby_ParseConnectStringOptions, (
	
	int32_t ApiVersion;
	
	const char* ConnectString;
));

#define EOS_LOBBYMODIFICATION_SETBUCKETID_API_LATEST 1

EOS_STRUCT(EOS_LobbyModification_SetBucketIdOptions, (
	
	int32_t ApiVersion;
	
	const char* BucketId;
));

#define EOS_LOBBYMODIFICATION_SETPERMISSIONLEVEL_API_LATEST 1

EOS_STRUCT(EOS_LobbyModification_SetPermissionLevelOptions, (
	
	int32_t ApiVersion;
	
	EOS_ELobbyPermissionLevel PermissionLevel;
));

#define EOS_LOBBYMODIFICATION_SETMAXMEMBERS_API_LATEST 1

EOS_STRUCT(EOS_LobbyModification_SetMaxMembersOptions, (
	
	int32_t ApiVersion;
	
	uint32_t MaxMembers;
));

#define EOS_LOBBYMODIFICATION_SETINVITESALLOWED_API_LATEST 1

EOS_STRUCT(EOS_LobbyModification_SetInvitesAllowedOptions, (
	
	int32_t ApiVersion;
	
	EOS_Bool bInvitesAllowed;
));

#define EOS_LOBBYMODIFICATION_ADDATTRIBUTE_API_LATEST 2

EOS_STRUCT(EOS_LobbyModification_AddAttributeOptions, (
	
	int32_t ApiVersion;
	
	const EOS_Lobby_AttributeData* Attribute;
	
	EOS_ELobbyAttributeVisibility Visibility;
));

#define EOS_LOBBYMODIFICATION_REMOVEATTRIBUTE_API_LATEST 1

EOS_STRUCT(EOS_LobbyModification_RemoveAttributeOptions, (
	
	int32_t ApiVersion;
	
	const char* Key;
));

#define EOS_LOBBYMODIFICATION_ADDMEMBERATTRIBUTE_API_LATEST 2

EOS_STRUCT(EOS_LobbyModification_AddMemberAttributeOptions, (
	
	int32_t ApiVersion;
	
	const EOS_Lobby_AttributeData* Attribute;
	
	EOS_ELobbyAttributeVisibility Visibility;
));

#define EOS_LOBBYMODIFICATION_REMOVEMEMBERATTRIBUTE_API_LATEST 1

EOS_STRUCT(EOS_LobbyModification_RemoveMemberAttributeOptions, (
	
	int32_t ApiVersion;
	
	const char* Key;
));

#define EOS_LOBBYMODIFICATION_SETALLOWEDPLATFORMIDS_API_LATEST 1

EOS_STRUCT(EOS_LobbyModification_SetAllowedPlatformIdsOptions, (
	
	int32_t ApiVersion;
	
	const uint32_t* AllowedPlatformIds;
	
	uint32_t AllowedPlatformIdsCount;
));

#define EOS_LOBBYDETAILS_GETLOBBYOWNER_API_LATEST 1

EOS_STRUCT(EOS_LobbyDetails_GetLobbyOwnerOptions, (
	
	int32_t ApiVersion;
));

#define EOS_LOBBYDETAILS_COPYINFO_API_LATEST 1

EOS_STRUCT(EOS_LobbyDetails_CopyInfoOptions, (
	
	int32_t ApiVersion;
));

#define EOS_LOBBYDETAILS_GETATTRIBUTECOUNT_API_LATEST 1

EOS_STRUCT(EOS_LobbyDetails_GetAttributeCountOptions, (
	
	int32_t ApiVersion;
));

#define EOS_LOBBYDETAILS_COPYATTRIBUTEBYINDEX_API_LATEST 1

EOS_STRUCT(EOS_LobbyDetails_CopyAttributeByIndexOptions, (
	
	int32_t ApiVersion;
	
	uint32_t AttrIndex;
));

#define EOS_LOBBYDETAILS_COPYATTRIBUTEBYKEY_API_LATEST 1

EOS_STRUCT(EOS_LobbyDetails_CopyAttributeByKeyOptions, (
	
	int32_t ApiVersion;
	
	const char* AttrKey;
));

#define EOS_LOBBYDETAILS_GETMEMBERATTRIBUTECOUNT_API_LATEST 1

EOS_STRUCT(EOS_LobbyDetails_GetMemberAttributeCountOptions, (
	
	int32_t ApiVersion;
	
	EOS_ProductUserId TargetUserId;
));

#define EOS_LOBBYDETAILS_COPYMEMBERATTRIBUTEBYINDEX_API_LATEST 1

EOS_STRUCT(EOS_LobbyDetails_CopyMemberAttributeByIndexOptions, (
	
	int32_t ApiVersion;
	
	EOS_ProductUserId TargetUserId;
	
	uint32_t AttrIndex;
));

#define EOS_LOBBYDETAILS_COPYMEMBERATTRIBUTEBYKEY_API_LATEST 1

EOS_STRUCT(EOS_LobbyDetails_CopyMemberAttributeByKeyOptions, (
	
	int32_t ApiVersion;
	
	EOS_ProductUserId TargetUserId;
	
	const char* AttrKey;
));

#define EOS_LOBBYDETAILS_GETMEMBERCOUNT_API_LATEST 1

EOS_STRUCT(EOS_LobbyDetails_GetMemberCountOptions, (
	
	int32_t ApiVersion;
));

#define EOS_LOBBYDETAILS_GETMEMBERBYINDEX_API_LATEST 1

EOS_STRUCT(EOS_LobbyDetails_GetMemberByIndexOptions, (
	
	int32_t ApiVersion;
	
	uint32_t MemberIndex;
));

#define EOS_LOBBYSEARCH_FIND_API_LATEST 1

EOS_STRUCT(EOS_LobbySearch_FindOptions, (
	
	int32_t ApiVersion;
	
	EOS_ProductUserId LocalUserId;
));

EOS_STRUCT(EOS_LobbySearch_FindCallbackInfo, (
	
	EOS_EResult ResultCode;
	
	void* ClientData;
));

EOS_DECLARE_CALLBACK(EOS_LobbySearch_OnFindCallback, const EOS_LobbySearch_FindCallbackInfo* Data);

#define EOS_LOBBYSEARCH_SETLOBBYID_API_LATEST 1

EOS_STRUCT(EOS_LobbySearch_SetLobbyIdOptions, (
	
	int32_t ApiVersion;
	
	EOS_LobbyId LobbyId;
));

#define EOS_LOBBYSEARCH_SETTARGETUSERID_API_LATEST 1

EOS_STRUCT(EOS_LobbySearch_SetTargetUserIdOptions, (
	
	int32_t ApiVersion;
	
	EOS_ProductUserId TargetUserId;
));

#define EOS_LOBBYSEARCH_SETPARAMETER_API_LATEST 1

EOS_STRUCT(EOS_LobbySearch_SetParameterOptions, (
	
	int32_t ApiVersion;
	
	const EOS_Lobby_AttributeData* Parameter;
	
	EOS_EComparisonOp ComparisonOp;
));

#define EOS_LOBBYSEARCH_REMOVEPARAMETER_API_LATEST 1

EOS_STRUCT(EOS_LobbySearch_RemoveParameterOptions, (
	
	int32_t ApiVersion;
	
	const char* Key;
	
	EOS_EComparisonOp ComparisonOp;
));

#define EOS_LOBBYSEARCH_SETMAXRESULTS_API_LATEST 1

EOS_STRUCT(EOS_LobbySearch_SetMaxResultsOptions, (
	
	int32_t ApiVersion;
	
	uint32_t MaxResults;
));

#define EOS_LOBBYSEARCH_GETSEARCHRESULTCOUNT_API_LATEST 1

EOS_STRUCT(EOS_LobbySearch_GetSearchResultCountOptions, (
	
	int32_t ApiVersion;
));

#define EOS_LOBBYSEARCH_COPYSEARCHRESULTBYINDEX_API_LATEST 1

EOS_STRUCT(EOS_LobbySearch_CopySearchResultByIndexOptions, (
	
	int32_t ApiVersion;
	
	uint32_t LobbyIndex;
));

#define EOS_LOBBYDETAILS_COPYMEMBERINFO_API_LATEST 1

EOS_STRUCT(EOS_LobbyDetails_CopyMemberInfoOptions, (
	
	int32_t ApiVersion;
	
	EOS_ProductUserId TargetUserId;
));

#define EOS_LOBBYDETAILS_MEMBERINFO_API_LATEST 1

EOS_STRUCT(EOS_LobbyDetails_MemberInfo, (
	
	int32_t ApiVersion;
	
	EOS_ProductUserId UserId;
	
	EOS_OnlinePlatformType Platform;
	
	EOS_Bool bAllowsCrossplay;
));

EOS_DECLARE_FUNC(void) EOS_LobbyDetails_MemberInfo_Release(EOS_LobbyDetails_MemberInfo* LobbyDetailsMemberInfo);

#define EOS_LOBBY_ADDNOTIFYLEAVELOBBYREQUESTED_API_LATEST 1

EOS_STRUCT(EOS_Lobby_AddNotifyLeaveLobbyRequestedOptions, (
	
	int32_t ApiVersion;
));

EOS_STRUCT(EOS_Lobby_LeaveLobbyRequestedCallbackInfo, (
	
	void* ClientData;
	
	EOS_ProductUserId LocalUserId;
	
	EOS_LobbyId LobbyId;
));

EOS_DECLARE_CALLBACK(EOS_Lobby_OnLeaveLobbyRequestedCallback, const EOS_Lobby_LeaveLobbyRequestedCallbackInfo* Data);

#pragma pack(pop)
