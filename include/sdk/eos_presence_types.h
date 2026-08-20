

#pragma once

#include "eos_common.h"
#include "eos_ui_types.h"

#pragma pack(push, 8)

EOS_EXTERN_C typedef struct EOS_PresenceHandle* EOS_HPresence;
EOS_EXTERN_C typedef struct EOS_PresenceModificationHandle* EOS_HPresenceModification;

EOS_ENUM(EOS_Presence_EStatus,
	
	EOS_PS_Offline = 0,
	
	EOS_PS_Online = 1,
	
	EOS_PS_Away = 2,
	
	EOS_PS_ExtendedAway = 3,
	
	EOS_PS_DoNotDisturb = 4
);

#define EOS_PRESENCE_DATARECORD_API_LATEST 1

EOS_STRUCT(EOS_Presence_DataRecord, (
	
	int32_t ApiVersion;
	
	const char* Key;
	
	const char* Value;
));

#define EOS_PRESENCE_INFO_API_LATEST 3

EOS_STRUCT(EOS_Presence_Info, (
	
	int32_t ApiVersion;
	
	EOS_Presence_EStatus Status;
	
	EOS_EpicAccountId UserId;
	
	const char* ProductId;
	
	const char* ProductVersion;
	
	const char* Platform;
	
	const char* RichText;
	
	int32_t RecordsCount;
	
	const EOS_Presence_DataRecord* Records;
	
	const char* ProductName;
	
	const char* IntegratedPlatform;
));

#define EOS_PRESENCE_QUERYPRESENCE_API_LATEST 1

EOS_STRUCT(EOS_Presence_QueryPresenceOptions, (
	
	int32_t ApiVersion;
	
	EOS_EpicAccountId LocalUserId;
	
	EOS_EpicAccountId TargetUserId;
));

EOS_STRUCT(EOS_Presence_QueryPresenceCallbackInfo, (
	
	EOS_EResult ResultCode;
	
	void* ClientData;
	
	EOS_EpicAccountId LocalUserId;
	
	EOS_EpicAccountId TargetUserId;
));

EOS_DECLARE_CALLBACK(EOS_Presence_OnQueryPresenceCompleteCallback, const EOS_Presence_QueryPresenceCallbackInfo* Data);

#define EOS_PRESENCE_HASPRESENCE_API_LATEST 1

EOS_STRUCT(EOS_Presence_HasPresenceOptions, (
	
	int32_t ApiVersion;
	
	EOS_EpicAccountId LocalUserId;
	
	EOS_EpicAccountId TargetUserId;
));

#define EOS_PRESENCE_COPYPRESENCE_API_LATEST 3

EOS_STRUCT(EOS_Presence_CopyPresenceOptions, (
	
	int32_t ApiVersion;
	
	EOS_EpicAccountId LocalUserId;
	
	EOS_EpicAccountId TargetUserId;
));

EOS_DECLARE_FUNC(void) EOS_Presence_Info_Release(EOS_Presence_Info* PresenceInfo);

#define EOS_PRESENCE_CREATEPRESENCEMODIFICATION_API_LATEST 1

EOS_STRUCT(EOS_Presence_CreatePresenceModificationOptions, (
	
	int32_t ApiVersion;
	
	EOS_EpicAccountId LocalUserId;
));

EOS_DECLARE_FUNC(void) EOS_PresenceModification_Release(EOS_HPresenceModification PresenceModificationHandle);

#define EOS_PRESENCE_SETPRESENCE_API_LATEST 1

EOS_STRUCT(EOS_Presence_SetPresenceOptions, (
	
	int32_t ApiVersion;
	
	EOS_EpicAccountId LocalUserId;
	
	EOS_HPresenceModification PresenceModificationHandle;
));

EOS_STRUCT(EOS_Presence_SetPresenceCallbackInfo, (
	
	EOS_EResult ResultCode;
	
	void* ClientData;
	
	EOS_EpicAccountId LocalUserId;
	
	EOS_EResult RichPresenceResultCode;
));

EOS_DECLARE_CALLBACK(EOS_Presence_SetPresenceCompleteCallback, const EOS_Presence_SetPresenceCallbackInfo* Data);

#define EOS_PRESENCE_ADDNOTIFYONPRESENCECHANGED_API_LATEST 1

EOS_STRUCT(EOS_Presence_AddNotifyOnPresenceChangedOptions, (
	
	int32_t ApiVersion;
));

EOS_STRUCT(EOS_Presence_PresenceChangedCallbackInfo, (
	
	void* ClientData;
	
	EOS_EpicAccountId LocalUserId;
	
	EOS_EpicAccountId PresenceUserId;
));

#define EOS_PRESENCE_ADDNOTIFYJOINGAMEACCEPTED_API_LATEST 2

EOS_STRUCT(EOS_Presence_AddNotifyJoinGameAcceptedOptions, (
	
	int32_t ApiVersion;
));

EOS_STRUCT(EOS_Presence_JoinGameAcceptedCallbackInfo, (
	
	void* ClientData;
	
	const char* JoinInfo;
	
	EOS_EpicAccountId LocalUserId;
	
	EOS_EpicAccountId TargetUserId;
	
	EOS_UI_EventId UiEventId;
));

EOS_DECLARE_CALLBACK(EOS_Presence_OnJoinGameAcceptedCallback, const EOS_Presence_JoinGameAcceptedCallbackInfo* Data);

EOS_DECLARE_CALLBACK(EOS_Presence_OnPresenceChangedCallback, const EOS_Presence_PresenceChangedCallbackInfo* Data);

#define EOS_PRESENCE_GETJOININFO_API_LATEST 1

EOS_STRUCT(EOS_Presence_GetJoinInfoOptions, (
	
	int32_t ApiVersion;
	
	EOS_EpicAccountId LocalUserId;
	
	EOS_EpicAccountId TargetUserId;
));

#define EOS_PRESENCEMODIFICATION_JOININFO_MAX_LENGTH EOS_PRESENCE_DATA_MAX_VALUE_LENGTH

#define EOS_PRESENCEMODIFICATION_SETJOININFO_API_LATEST 1

EOS_STRUCT(EOS_PresenceModification_SetJoinInfoOptions, (
	
	int32_t ApiVersion;
	
	const char* JoinInfo;
));

#define EOS_PRESENCE_DATA_MAX_KEYS 32

#define EOS_PRESENCE_DATA_MAX_KEY_LENGTH 64

#define EOS_PRESENCE_DATA_MAX_VALUE_LENGTH 255

#define EOS_PRESENCE_RICH_TEXT_MAX_VALUE_LENGTH 255

#define EOS_PRESENCE_KEY_PLATFORM_PRESENCE "EOS_PlatformPresence"

#define EOS_PRESENCEMODIFICATION_SETSTATUS_API_LATEST 1

#define EOS_PRESENCE_SETSTATUS_API_LATEST EOS_PRESENCEMODIFICATION_SETSTATUS_API_LATEST

EOS_STRUCT(EOS_PresenceModification_SetStatusOptions, (
	
	int32_t ApiVersion;
	
	EOS_Presence_EStatus Status;
));

#define EOS_PRESENCEMODIFICATION_SETRAWRICHTEXT_API_LATEST 1

#define EOS_PRESENCE_SETRAWRICHTEXT_API_LATEST EOS_PRESENCEMODIFICATION_SETRAWRICHTEXT_API_LATEST

EOS_STRUCT(EOS_PresenceModification_SetRawRichTextOptions, (
	
	int32_t ApiVersion;
	
	const char* RichText;
));

#define EOS_PRESENCEMODIFICATION_SETDATA_API_LATEST 1

#define EOS_PRESENCE_SETDATA_API_LATEST EOS_PRESENCEMODIFICATION_SETDATA_API_LATEST

EOS_STRUCT(EOS_PresenceModification_SetDataOptions, (
	
	int32_t ApiVersion;
	
	int32_t RecordsCount;
	
	const EOS_Presence_DataRecord* Records;
));

#define EOS_PRESENCEMODIFICATION_DATARECORDID_API_LATEST 1

EOS_STRUCT(EOS_PresenceModification_DataRecordId, (
	
	int32_t ApiVersion;
	
	const char* Key;
));

#define EOS_PRESENCEMODIFICATION_DELETEDATA_API_LATEST 1

#define EOS_PRESENCE_DELETEDATA_API_LATEST EOS_PRESENCEMODIFICATION_DELETEDATA_API_LATEST

EOS_STRUCT(EOS_PresenceModification_DeleteDataOptions, (
	
	int32_t ApiVersion;
	
	int32_t RecordsCount;
	
	const EOS_PresenceModification_DataRecordId* Records;
));

#pragma pack(pop)
