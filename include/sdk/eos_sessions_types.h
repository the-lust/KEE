

#pragma once

#include "eos_common.h"
#include "eos_ui_types.h"

#pragma pack(push, 8)

EOS_EXTERN_C typedef struct EOS_SessionsHandle* EOS_HSessions;

EOS_EXTERN_C typedef struct EOS_SessionModificationHandle* EOS_HSessionModification;

EOS_EXTERN_C typedef struct EOS_ActiveSessionHandle* EOS_HActiveSession;

EOS_EXTERN_C typedef struct EOS_SessionDetailsHandle* EOS_HSessionDetails;

EOS_EXTERN_C typedef struct EOS_SessionSearchHandle* EOS_HSessionSearch;

EOS_DECLARE_FUNC(void) EOS_SessionModification_Release(EOS_HSessionModification SessionModificationHandle);

EOS_DECLARE_FUNC(void) EOS_ActiveSession_Release(EOS_HActiveSession ActiveSessionHandle);

EOS_DECLARE_FUNC(void) EOS_SessionDetails_Release(EOS_HSessionDetails SessionHandle);

EOS_DECLARE_FUNC(void) EOS_SessionSearch_Release(EOS_HSessionSearch SessionSearchHandle);

EOS_ENUM(EOS_EOnlineSessionState,
	
	EOS_OSS_NoSession = 0,
	
	EOS_OSS_Creating = 1,
	
	EOS_OSS_Pending = 2,
	
	EOS_OSS_Starting = 3,
	
	EOS_OSS_InProgress = 4,
	
	EOS_OSS_Ending = 5,
	
	EOS_OSS_Ended = 6,
	
	EOS_OSS_Destroying = 7
);

EOS_ENUM(EOS_ESessionAttributeAdvertisementType,
	
	EOS_SAAT_DontAdvertise = 0,
	
	EOS_SAAT_Advertise = 1
);

#define EOS_SAT_Boolean EOS_AT_BOOLEAN
#define EOS_SAT_Int64	EOS_AT_INT64
#define EOS_SAT_Double	EOS_AT_DOUBLE
#define EOS_SAT_String	EOS_AT_STRING

#define EOS_OCO_EQUAL				EOS_CO_EQUAL
#define EOS_OCO_NOTEQUAL			EOS_CO_NOTEQUAL
#define EOS_OCO_GREATERTHAN			EOS_CO_GREATERTHAN
#define EOS_OCO_GREATERTHANOREQUAL	EOS_CO_GREATERTHANOREQUAL
#define EOS_OCO_LESSTHAN			EOS_CO_LESSTHAN
#define EOS_OCO_LESSTHANOREQUAL		EOS_CO_LESSTHANOREQUAL
#define EOS_OCO_DISTANCE			EOS_CO_DISTANCE
#define EOS_OCO_ANYOF				EOS_CO_ANYOF
#define EOS_OCO_NOTANYOF			EOS_CO_NOTANYOF

#define EOS_SESSIONMODIFICATION_MAX_SESSION_ATTRIBUTES 64

#define EOS_SESSIONMODIFICATION_MAX_SESSION_ATTRIBUTE_LENGTH 64

#define EOS_SESSIONMODIFICATION_MIN_SESSIONIDOVERRIDE_LENGTH 16

#define EOS_SESSIONMODIFICATION_MAX_SESSIONIDOVERRIDE_LENGTH 64

#define EOS_SESSIONS_CREATESESSIONMODIFICATION_API_LATEST 5

EOS_STRUCT(EOS_Sessions_CreateSessionModificationOptions, (
	
	int32_t ApiVersion;
	
	const char* SessionName;
	
	const char* BucketId;
	
	uint32_t MaxPlayers;
	
	EOS_ProductUserId LocalUserId;
	
	EOS_Bool bPresenceEnabled;
	
	const char* SessionId;
	
	EOS_Bool bSanctionsEnabled;
	
	const uint32_t* AllowedPlatformIds;
	
	uint32_t AllowedPlatformIdsCount;
));

#define EOS_SESSIONS_UPDATESESSIONMODIFICATION_API_LATEST 1

EOS_STRUCT(EOS_Sessions_UpdateSessionModificationOptions, (
	
	int32_t ApiVersion;
	
	const char* SessionName;
));

#define EOS_SESSIONS_INVITEID_MAX_LENGTH 64

#define EOS_SESSIONS_SENDINVITE_API_LATEST 1

EOS_STRUCT(EOS_Sessions_SendInviteOptions, (
	
	int32_t ApiVersion;
	
	const char* SessionName;
	
	EOS_ProductUserId LocalUserId;
	
	EOS_ProductUserId TargetUserId;
));

EOS_STRUCT(EOS_Sessions_SendInviteCallbackInfo, (
	
	EOS_EResult ResultCode;
	
	void* ClientData;
));

EOS_DECLARE_CALLBACK(EOS_Sessions_OnSendInviteCallback, const EOS_Sessions_SendInviteCallbackInfo* Data);

#define EOS_SESSIONS_REJECTINVITE_API_LATEST 1

EOS_STRUCT(EOS_Sessions_RejectInviteOptions, (
	
	int32_t ApiVersion;
	
	EOS_ProductUserId LocalUserId;
	
	const char* InviteId;
));

EOS_STRUCT(EOS_Sessions_RejectInviteCallbackInfo, (
	
	EOS_EResult ResultCode;
	
	void* ClientData;
));

EOS_DECLARE_CALLBACK(EOS_Sessions_OnRejectInviteCallback, const EOS_Sessions_RejectInviteCallbackInfo* Data);

#define EOS_SESSIONS_QUERYINVITES_API_LATEST 1

EOS_STRUCT(EOS_Sessions_QueryInvitesOptions, (
	
	int32_t ApiVersion;
	
	EOS_ProductUserId LocalUserId;
));

EOS_STRUCT(EOS_Sessions_QueryInvitesCallbackInfo, (
	
	EOS_EResult ResultCode;
	
	void* ClientData;
	
	EOS_ProductUserId LocalUserId;
));

EOS_DECLARE_CALLBACK(EOS_Sessions_OnQueryInvitesCallback, const EOS_Sessions_QueryInvitesCallbackInfo* Data);

#define EOS_SESSIONS_GETINVITECOUNT_API_LATEST 1

EOS_STRUCT(EOS_Sessions_GetInviteCountOptions, (
	
	int32_t ApiVersion;
	
	EOS_ProductUserId LocalUserId;
));

#define EOS_SESSIONS_GETINVITEIDBYINDEX_API_LATEST 1

EOS_STRUCT(EOS_Sessions_GetInviteIdByIndexOptions, (
	
	int32_t ApiVersion;
	
	EOS_ProductUserId LocalUserId;
	
	uint32_t Index;
));

#define EOS_SESSIONS_CREATESESSIONSEARCH_API_LATEST 1

EOS_STRUCT(EOS_Sessions_CreateSessionSearchOptions, (
	
	int32_t ApiVersion;
	
	uint32_t MaxSearchResults;
));

#define EOS_SESSIONS_UPDATESESSION_API_LATEST 1

EOS_STRUCT(EOS_Sessions_UpdateSessionOptions, (
	
	int32_t ApiVersion;
	
	EOS_HSessionModification SessionModificationHandle;
));

EOS_STRUCT(EOS_Sessions_UpdateSessionCallbackInfo, (
	
	EOS_EResult ResultCode;
	
	void* ClientData;
	
	const char* SessionName;
	
	const char* SessionId;
));

EOS_DECLARE_CALLBACK(EOS_Sessions_OnUpdateSessionCallback, const EOS_Sessions_UpdateSessionCallbackInfo* Data);

#define EOS_SESSIONS_DESTROYSESSION_API_LATEST 1

EOS_STRUCT(EOS_Sessions_DestroySessionOptions, (
	
	int32_t ApiVersion;
	
	const char* SessionName;
));

EOS_STRUCT(EOS_Sessions_DestroySessionCallbackInfo, (
	
	EOS_EResult ResultCode;
	
	void* ClientData;
));

EOS_DECLARE_CALLBACK(EOS_Sessions_OnDestroySessionCallback, const EOS_Sessions_DestroySessionCallbackInfo* Data);

#define EOS_SESSIONS_JOINSESSION_API_LATEST 2

EOS_STRUCT(EOS_Sessions_JoinSessionOptions, (
	
	int32_t ApiVersion;
	
	const char* SessionName;
	
	EOS_HSessionDetails SessionHandle;
	
	EOS_ProductUserId LocalUserId;
	
	EOS_Bool bPresenceEnabled;
));

EOS_STRUCT(EOS_Sessions_JoinSessionCallbackInfo, (
	
	EOS_EResult ResultCode;
	
	void* ClientData;
));

EOS_DECLARE_CALLBACK(EOS_Sessions_OnJoinSessionCallback, const EOS_Sessions_JoinSessionCallbackInfo* Data);

#define EOS_SESSIONS_STARTSESSION_API_LATEST 1

EOS_STRUCT(EOS_Sessions_StartSessionOptions, (
	
	int32_t ApiVersion;
	
	const char* SessionName;
));

EOS_STRUCT(EOS_Sessions_StartSessionCallbackInfo, (
	
	EOS_EResult ResultCode;
	
	void* ClientData;
));

EOS_DECLARE_CALLBACK(EOS_Sessions_OnStartSessionCallback, const EOS_Sessions_StartSessionCallbackInfo* Data);

#define EOS_SESSIONS_ENDSESSION_API_LATEST 1

EOS_STRUCT(EOS_Sessions_EndSessionOptions, (
	
	int32_t ApiVersion;
	
	const char* SessionName;
));

EOS_STRUCT(EOS_Sessions_EndSessionCallbackInfo, (
	
	EOS_EResult ResultCode;
	
	void* ClientData;
));

EOS_DECLARE_CALLBACK(EOS_Sessions_OnEndSessionCallback, const EOS_Sessions_EndSessionCallbackInfo* Data);

#define EOS_SESSIONS_REGISTERPLAYERS_API_LATEST 3

EOS_STRUCT(EOS_Sessions_RegisterPlayersOptions, (
	
	int32_t ApiVersion;
	
	const char* SessionName;
	
	EOS_ProductUserId* PlayersToRegister;
	
	uint32_t PlayersToRegisterCount;
));

EOS_STRUCT(EOS_Sessions_RegisterPlayersCallbackInfo, (
	
	EOS_EResult ResultCode;
	
	void* ClientData;
	
	EOS_ProductUserId* RegisteredPlayers;
	
	uint32_t RegisteredPlayersCount;
	
	EOS_ProductUserId* SanctionedPlayers;
	
	uint32_t SanctionedPlayersCount;
));

EOS_DECLARE_CALLBACK(EOS_Sessions_OnRegisterPlayersCallback, const EOS_Sessions_RegisterPlayersCallbackInfo* Data);

#define EOS_SESSIONS_UNREGISTERPLAYERS_API_LATEST 2

EOS_STRUCT(EOS_Sessions_UnregisterPlayersOptions, (
	
	int32_t ApiVersion;
	
	const char* SessionName;
	
	EOS_ProductUserId* PlayersToUnregister;
	
	uint32_t PlayersToUnregisterCount;
));

EOS_STRUCT(EOS_Sessions_UnregisterPlayersCallbackInfo, (
	
	EOS_EResult ResultCode;
	
	void* ClientData;
	
	EOS_ProductUserId* UnregisteredPlayers;
	
	uint32_t UnregisteredPlayersCount;
));

EOS_DECLARE_CALLBACK(EOS_Sessions_OnUnregisterPlayersCallback, const EOS_Sessions_UnregisterPlayersCallbackInfo* Data);

#define EOS_SESSIONMODIFICATION_SETBUCKETID_API_LATEST 1

EOS_STRUCT(EOS_SessionModification_SetBucketIdOptions, (
	
	int32_t ApiVersion;
	
	const char* BucketId;
));

#define EOS_SESSIONMODIFICATION_SETHOSTADDRESS_API_LATEST 1

EOS_STRUCT(EOS_SessionModification_SetHostAddressOptions, (
	
	int32_t ApiVersion;
	
	const char* HostAddress;
));

EOS_ENUM(EOS_EOnlineSessionPermissionLevel,
	
	EOS_OSPF_PublicAdvertised = 0,
	
	EOS_OSPF_JoinViaPresence = 1,
	
	EOS_OSPF_InviteOnly = 2
);

#define EOS_SESSIONMODIFICATION_SETPERMISSIONLEVEL_API_LATEST 1

EOS_STRUCT(EOS_SessionModification_SetPermissionLevelOptions, (
	
	int32_t ApiVersion;
	
	EOS_EOnlineSessionPermissionLevel PermissionLevel;
));

#define EOS_SESSIONMODIFICATION_SETJOININPROGRESSALLOWED_API_LATEST 1

EOS_STRUCT(EOS_SessionModification_SetJoinInProgressAllowedOptions, (
	
	int32_t ApiVersion;
	
	EOS_Bool bAllowJoinInProgress;
));

#define EOS_SESSIONS_MAXREGISTEREDPLAYERS 1000

#define EOS_SESSIONMODIFICATION_SETMAXPLAYERS_API_LATEST 1

EOS_STRUCT(EOS_SessionModification_SetMaxPlayersOptions, (
	
	int32_t ApiVersion;
	
	uint32_t MaxPlayers;
));

#define EOS_SESSIONMODIFICATION_SETINVITESALLOWED_API_LATEST 1

EOS_STRUCT(EOS_SessionModification_SetInvitesAllowedOptions, (
	
	int32_t ApiVersion;
	
	EOS_Bool bInvitesAllowed;
));

#define EOS_SESSIONMODIFICATION_SETALLOWEDPLATFORMIDS_API_LATEST 1

EOS_STRUCT(EOS_SessionModification_SetAllowedPlatformIdsOptions, (
	
	int32_t ApiVersion;
	
	const uint32_t* AllowedPlatformIds;
	
	uint32_t AllowedPlatformIdsCount;
));

#define EOS_SESSIONS_SEARCH_BUCKET_ID "bucket"

#define EOS_SESSIONS_SEARCH_EMPTY_SERVERS_ONLY "emptyonly"

#define EOS_SESSIONS_SEARCH_NONEMPTY_SERVERS_ONLY "nonemptyonly"

#define EOS_SESSIONS_SEARCH_MINSLOTSAVAILABLE "minslotsavailable"

#define EOS_SESSIONS_ATTRIBUTEDATA_API_LATEST 1

#define EOS_SESSIONS_SESSIONATTRIBUTEDATA_API_LATEST EOS_SESSIONS_ATTRIBUTEDATA_API_LATEST

EOS_STRUCT(EOS_Sessions_AttributeData, (
	
	int32_t ApiVersion;
	
	const char* Key;
	
	union
	{
		
		int64_t AsInt64;
		
		double AsDouble;
		
		EOS_Bool AsBool;
		
		const char* AsUtf8;
	} Value;

	EOS_ESessionAttributeType ValueType;
));

#define EOS_ACTIVESESSION_COPYINFO_API_LATEST 1

EOS_STRUCT(EOS_ActiveSession_CopyInfoOptions, (
	
	int32_t ApiVersion;
));

#define EOS_ACTIVESESSION_GETREGISTEREDPLAYERCOUNT_API_LATEST 1

EOS_STRUCT(EOS_ActiveSession_GetRegisteredPlayerCountOptions, (
	
	int32_t ApiVersion;
));

#define EOS_ACTIVESESSION_GETREGISTEREDPLAYERBYINDEX_API_LATEST 1

EOS_STRUCT(EOS_ActiveSession_GetRegisteredPlayerByIndexOptions, (
	
	int32_t ApiVersion;
	
	uint32_t PlayerIndex;
));

#define EOS_SESSIONDETAILS_ATTRIBUTE_API_LATEST 1

#define EOS_SESSIONS_SESSIONATTRIBUTE_API_LATEST EOS_SESSIONDETAILS_ATTRIBUTE_API_LATEST

EOS_STRUCT(EOS_SessionDetails_Attribute, (
	
	int32_t ApiVersion;
	
	EOS_Sessions_AttributeData* Data;
	
	EOS_ESessionAttributeAdvertisementType AdvertisementType;
));

EOS_DECLARE_FUNC(void) EOS_SessionDetails_Attribute_Release(EOS_SessionDetails_Attribute* SessionAttribute);

#define EOS_SESSIONMODIFICATION_ADDATTRIBUTE_API_LATEST 2

EOS_STRUCT(EOS_SessionModification_AddAttributeOptions, (
	
	int32_t ApiVersion;
	
	const EOS_Sessions_AttributeData* SessionAttribute;
	
	EOS_ESessionAttributeAdvertisementType AdvertisementType;
));

#define EOS_SESSIONMODIFICATION_REMOVEATTRIBUTE_API_LATEST 1

EOS_STRUCT(EOS_SessionModification_RemoveAttributeOptions, (
	
	int32_t ApiVersion;
	
	const char* Key;
));

#define EOS_SESSIONS_MAX_SEARCH_RESULTS 200

#define EOS_SESSIONSEARCH_SETMAXSEARCHRESULTS_API_LATEST 1

EOS_STRUCT(EOS_SessionSearch_SetMaxResultsOptions, (
	
	int32_t ApiVersion;
	
	uint32_t MaxSearchResults;
));

#define EOS_SESSIONSEARCH_FIND_API_LATEST 2

EOS_STRUCT(EOS_SessionSearch_FindOptions, (
	
	int32_t ApiVersion;
	
	EOS_ProductUserId LocalUserId;
));

EOS_STRUCT(EOS_SessionSearch_FindCallbackInfo, (
	
	EOS_EResult ResultCode;
	
	void* ClientData;
));

EOS_DECLARE_CALLBACK(EOS_SessionSearch_OnFindCallback, const EOS_SessionSearch_FindCallbackInfo* Data);

#define EOS_SESSIONSEARCH_GETSEARCHRESULTCOUNT_API_LATEST 1

EOS_STRUCT(EOS_SessionSearch_GetSearchResultCountOptions, (
	
	int32_t ApiVersion;
));

#define EOS_SESSIONSEARCH_COPYSEARCHRESULTBYINDEX_API_LATEST 1

EOS_STRUCT(EOS_SessionSearch_CopySearchResultByIndexOptions, (
	
	int32_t ApiVersion;
	
	uint32_t SessionIndex;
));

#define EOS_SESSIONSEARCH_SETSESSIONID_API_LATEST 1

EOS_STRUCT(EOS_SessionSearch_SetSessionIdOptions, (
	
	int32_t ApiVersion;
	
	const char* SessionId;
));

#define EOS_SESSIONSEARCH_SETTARGETUSERID_API_LATEST 1

EOS_STRUCT(EOS_SessionSearch_SetTargetUserIdOptions, (
	
	int32_t ApiVersion;
	
	EOS_ProductUserId TargetUserId;
));

#define EOS_SESSIONSEARCH_SETPARAMETER_API_LATEST 1

EOS_STRUCT(EOS_SessionSearch_SetParameterOptions, (
	
	int32_t ApiVersion;
	
	const EOS_Sessions_AttributeData* Parameter;
	
	EOS_EOnlineComparisonOp ComparisonOp;
));

#define EOS_SESSIONSEARCH_REMOVEPARAMETER_API_LATEST 1

EOS_STRUCT(EOS_SessionSearch_RemoveParameterOptions, (
	
	int32_t ApiVersion;
	
	const char* Key;
	
	EOS_EOnlineComparisonOp ComparisonOp;
));

#define EOS_SESSIONDETAILS_SETTINGS_API_LATEST 4

EOS_STRUCT(EOS_SessionDetails_Settings, (
	
	int32_t ApiVersion;
	
	const char* BucketId;
	
	uint32_t NumPublicConnections;
	
	EOS_Bool bAllowJoinInProgress;
	
	EOS_EOnlineSessionPermissionLevel PermissionLevel;
	
	EOS_Bool bInvitesAllowed;
	
	EOS_Bool bSanctionsEnabled;
	
	const uint32_t* AllowedPlatformIds;
	
	uint32_t AllowedPlatformIdsCount;
));

#define EOS_SESSIONDETAILS_INFO_API_LATEST 2

EOS_STRUCT(EOS_SessionDetails_Info, (
	
	int32_t ApiVersion;
	
	const char* SessionId;
	
	const char* HostAddress;
	
	uint32_t NumOpenPublicConnections;
	
	const EOS_SessionDetails_Settings* Settings;
	
	EOS_ProductUserId OwnerUserId;
	
	const char* OwnerServerClientId;
));

EOS_DECLARE_FUNC(void) EOS_SessionDetails_Info_Release(EOS_SessionDetails_Info* SessionInfo);

#define EOS_SESSIONDETAILS_COPYINFO_API_LATEST 1

EOS_STRUCT(EOS_SessionDetails_CopyInfoOptions, (
	
	int32_t ApiVersion;
));

#define EOS_SESSIONDETAILS_GETSESSIONATTRIBUTECOUNT_API_LATEST 1

EOS_STRUCT(EOS_SessionDetails_GetSessionAttributeCountOptions, (
	
	int32_t ApiVersion;
));

#define EOS_SESSIONDETAILS_COPYSESSIONATTRIBUTEBYINDEX_API_LATEST 1

EOS_STRUCT(EOS_SessionDetails_CopySessionAttributeByIndexOptions, (
	
	int32_t ApiVersion;
	
	uint32_t AttrIndex;
));

#define EOS_SESSIONDETAILS_COPYSESSIONATTRIBUTEBYKEY_API_LATEST 1

EOS_STRUCT(EOS_SessionDetails_CopySessionAttributeByKeyOptions, (
	
	int32_t ApiVersion;
	
	const char* AttrKey;
));

#define EOS_ACTIVESESSION_INFO_API_LATEST 1

EOS_STRUCT(EOS_ActiveSession_Info, (
	
	int32_t ApiVersion;
	
	const char* SessionName;
	
	EOS_ProductUserId LocalUserId;
	
	EOS_EOnlineSessionState State;
	
	const EOS_SessionDetails_Info* SessionDetails;
));

EOS_DECLARE_FUNC(void) EOS_ActiveSession_Info_Release(EOS_ActiveSession_Info* ActiveSessionInfo);

#define EOS_SESSIONS_COPYACTIVESESSIONHANDLE_API_LATEST 1

EOS_STRUCT(EOS_Sessions_CopyActiveSessionHandleOptions, (
	
	int32_t ApiVersion;
	
	const char* SessionName;
));

#define EOS_SESSIONS_ADDNOTIFYSESSIONINVITERECEIVED_API_LATEST 1

EOS_STRUCT(EOS_Sessions_AddNotifySessionInviteReceivedOptions, (
	
	int32_t ApiVersion;
));

EOS_STRUCT(EOS_Sessions_SessionInviteReceivedCallbackInfo, (
	
	void* ClientData;
	
	EOS_ProductUserId LocalUserId;
	
	EOS_ProductUserId TargetUserId;
	
	const char* InviteId;
));

EOS_DECLARE_CALLBACK(EOS_Sessions_OnSessionInviteReceivedCallback, const EOS_Sessions_SessionInviteReceivedCallbackInfo* Data);

#define EOS_SESSIONS_ADDNOTIFYSESSIONINVITEACCEPTED_API_LATEST 1

EOS_STRUCT(EOS_Sessions_AddNotifySessionInviteAcceptedOptions, (
	
	int32_t ApiVersion;
));

EOS_STRUCT(EOS_Sessions_SessionInviteAcceptedCallbackInfo, (
	
	void* ClientData;
	
	const char* SessionId;
	
	EOS_ProductUserId LocalUserId;
	
	EOS_ProductUserId TargetUserId;
	
	const char* InviteId;
));

EOS_DECLARE_CALLBACK(EOS_Sessions_OnSessionInviteAcceptedCallback, const EOS_Sessions_SessionInviteAcceptedCallbackInfo* Data);

#define EOS_SESSIONS_ADDNOTIFYSESSIONINVITEREJECTED_API_LATEST 1

EOS_STRUCT(EOS_Sessions_AddNotifySessionInviteRejectedOptions, (
	
	int32_t ApiVersion;
));

EOS_STRUCT(EOS_Sessions_SessionInviteRejectedCallbackInfo, (
	
	void* ClientData;
	
	const char* InviteId;
	
	EOS_ProductUserId LocalUserId;
	
	EOS_ProductUserId TargetUserId;
	
	const char* SessionId;
));

EOS_DECLARE_CALLBACK(EOS_Sessions_OnSessionInviteRejectedCallback, const EOS_Sessions_SessionInviteRejectedCallbackInfo* Data);

#define EOS_SESSIONS_ADDNOTIFYJOINSESSIONACCEPTED_API_LATEST 1

EOS_STRUCT(EOS_Sessions_AddNotifyJoinSessionAcceptedOptions, (
	
	int32_t ApiVersion;
));

EOS_STRUCT(EOS_Sessions_JoinSessionAcceptedCallbackInfo, (
	
	void* ClientData;
	
	EOS_ProductUserId LocalUserId;
	
	EOS_UI_EventId UiEventId;
));

EOS_DECLARE_CALLBACK(EOS_Sessions_OnJoinSessionAcceptedCallback, const EOS_Sessions_JoinSessionAcceptedCallbackInfo* Data);

#define EOS_SESSIONS_COPYSESSIONHANDLEBYINVITEID_API_LATEST 1

EOS_STRUCT(EOS_Sessions_CopySessionHandleByInviteIdOptions, (
	
	int32_t ApiVersion;
	
	const char* InviteId;
));

#define EOS_SESSIONS_COPYSESSIONHANDLEBYUIEVENTID_API_LATEST 1

EOS_STRUCT(EOS_Sessions_CopySessionHandleByUiEventIdOptions, (
	
	int32_t ApiVersion;
	
	EOS_UI_EventId UiEventId;
));

#define EOS_SESSIONS_COPYSESSIONHANDLEFORPRESENCE_API_LATEST 1

EOS_STRUCT(EOS_Sessions_CopySessionHandleForPresenceOptions, (
	
	int32_t ApiVersion;
	
	EOS_ProductUserId LocalUserId;
));

#define EOS_SESSIONS_ISUSERINSESSION_API_LATEST 1

EOS_STRUCT(EOS_Sessions_IsUserInSessionOptions, (
	
	int32_t ApiVersion;
	
	const char* SessionName;
	
	EOS_ProductUserId TargetUserId;
));

#define EOS_SESSIONS_DUMPSESSIONSTATE_API_LATEST 1

EOS_STRUCT(EOS_Sessions_DumpSessionStateOptions, (
	
	int32_t ApiVersion;
	
	const char* SessionName;
));

#define EOS_SESSIONS_ADDNOTIFYLEAVESESSIONREQUESTED_API_LATEST 1

EOS_STRUCT(EOS_Sessions_AddNotifyLeaveSessionRequestedOptions, (
	
	int32_t ApiVersion;
));

EOS_STRUCT(EOS_Sessions_LeaveSessionRequestedCallbackInfo, (
	
	void* ClientData;
	
	EOS_ProductUserId LocalUserId;
	
	const char* SessionName;
));

EOS_DECLARE_CALLBACK(EOS_Sessions_OnLeaveSessionRequestedCallback, const EOS_Sessions_LeaveSessionRequestedCallbackInfo* Data);

#define EOS_SESSIONS_ADDNOTIFYSENDSESSIONNATIVEINVITEREQUESTED_API_LATEST 1

EOS_STRUCT(EOS_Sessions_AddNotifySendSessionNativeInviteRequestedOptions, (
	
	int32_t ApiVersion;
));

EOS_STRUCT(EOS_Sessions_SendSessionNativeInviteRequestedCallbackInfo, (
	
	void* ClientData;
	
	EOS_UI_EventId UiEventId;
	
	EOS_ProductUserId LocalUserId;
	
	EOS_IntegratedPlatformType TargetNativeAccountType;
	
	const char* TargetUserNativeAccountId;
	
	const char* SessionId;
));

EOS_DECLARE_CALLBACK(EOS_Sessions_OnSendSessionNativeInviteRequestedCallback, const EOS_Sessions_SendSessionNativeInviteRequestedCallbackInfo* Data);

#pragma pack(pop)
