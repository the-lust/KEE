
#pragma once

#include "eos_common.h"

#pragma pack(push, 8)

EOS_EXTERN_C typedef struct EOS_KWSHandle* EOS_HKWS;

#define EOS_KWS_MAX_PERMISSIONS 16

#define EOS_KWS_MAX_PERMISSION_LENGTH 32

EOS_ENUM(EOS_EKWSPermissionStatus,
	
	EOS_KPS_GRANTED = 0,
	
	EOS_KPS_REJECTED = 1,
	
	EOS_KPS_PENDING = 2
);

#define EOS_KWS_PERMISSIONSTATUS_API_LATEST 1

EOS_STRUCT(EOS_KWS_PermissionStatus, (
	
	int32_t ApiVersion;
	
	const char* Name;
	
	EOS_EKWSPermissionStatus Status;
));

EOS_DECLARE_FUNC(void) EOS_KWS_PermissionStatus_Release(EOS_KWS_PermissionStatus* PermissionStatus);

#define EOS_KWS_QUERYAGEGATE_API_LATEST 1

EOS_STRUCT(EOS_KWS_QueryAgeGateOptions, (
	
	int32_t ApiVersion;
));

EOS_STRUCT(EOS_KWS_QueryAgeGateCallbackInfo, (
	
	EOS_EResult ResultCode;
	
	void* ClientData;
	
	const char* CountryCode;
	
	uint32_t AgeOfConsent;
));

EOS_DECLARE_CALLBACK(EOS_KWS_OnQueryAgeGateCallback, const EOS_KWS_QueryAgeGateCallbackInfo* Data);

#define EOS_KWS_CREATEUSER_API_LATEST 1

EOS_STRUCT(EOS_KWS_CreateUserOptions, (
	
	int32_t ApiVersion;
	
	EOS_ProductUserId LocalUserId;
	
	const char* DateOfBirth;
	
	const char* ParentEmail;
));

EOS_STRUCT(EOS_KWS_CreateUserCallbackInfo, (
	
	EOS_EResult ResultCode;
	
	void* ClientData;
	
	EOS_ProductUserId LocalUserId;
	
	const char* KWSUserId;
	
	EOS_Bool bIsMinor;
));

EOS_DECLARE_CALLBACK(EOS_KWS_OnCreateUserCallback, const EOS_KWS_CreateUserCallbackInfo* Data);

#define EOS_KWS_QUERYPERMISSIONS_API_LATEST 1

EOS_STRUCT(EOS_KWS_QueryPermissionsOptions, (
	
	int32_t ApiVersion;
	
	EOS_ProductUserId LocalUserId;
));

EOS_STRUCT(EOS_KWS_QueryPermissionsCallbackInfo, (
	
	EOS_EResult ResultCode;
	
	void* ClientData;
	
	EOS_ProductUserId LocalUserId;
	
	const char* KWSUserId;
	
	const char* DateOfBirth;
	
	EOS_Bool bIsMinor;
	
	const char* ParentEmail;
));

EOS_DECLARE_CALLBACK(EOS_KWS_OnQueryPermissionsCallback, const EOS_KWS_QueryPermissionsCallbackInfo* Data);

#define EOS_KWS_UPDATEPARENTEMAIL_API_LATEST 1

EOS_STRUCT(EOS_KWS_UpdateParentEmailOptions, (
	
	int32_t ApiVersion;
	
	EOS_ProductUserId LocalUserId;
	
	const char* ParentEmail;
));

EOS_STRUCT(EOS_KWS_UpdateParentEmailCallbackInfo, (
	
	EOS_EResult ResultCode;
	
	void* ClientData;
	
	EOS_ProductUserId LocalUserId;
));

EOS_DECLARE_CALLBACK(EOS_KWS_OnUpdateParentEmailCallback, const EOS_KWS_UpdateParentEmailCallbackInfo* Data);

#define EOS_KWS_REQUESTPERMISSIONS_API_LATEST 1

EOS_STRUCT(EOS_KWS_RequestPermissionsOptions, (
	
	int32_t ApiVersion;
	
	EOS_ProductUserId LocalUserId;
	
	uint32_t PermissionKeyCount;
	
	const char** PermissionKeys;
));

EOS_STRUCT(EOS_KWS_RequestPermissionsCallbackInfo, (
	
	EOS_EResult ResultCode;
	
	void* ClientData;
	
	EOS_ProductUserId LocalUserId;
));

EOS_DECLARE_CALLBACK(EOS_KWS_OnRequestPermissionsCallback, const EOS_KWS_RequestPermissionsCallbackInfo* Data);

#define EOS_KWS_GETPERMISSIONSCOUNT_API_LATEST 1

EOS_STRUCT(EOS_KWS_GetPermissionsCountOptions, (
	
	int32_t ApiVersion;
	
	EOS_ProductUserId LocalUserId;
));

#define EOS_KWS_COPYPERMISSIONBYINDEX_API_LATEST 1

EOS_STRUCT(EOS_KWS_CopyPermissionByIndexOptions, (
	
	int32_t ApiVersion;
	
	EOS_ProductUserId LocalUserId;
	
	uint32_t Index;
));

#define EOS_KWS_GETPERMISSIONBYKEY_API_LATEST 1

EOS_STRUCT(EOS_KWS_GetPermissionByKeyOptions, (
	
	int32_t ApiVersion;
	
	EOS_ProductUserId LocalUserId;
	
	const char* Key;
));

#define EOS_KWS_ADDNOTIFYPERMISSIONSUPDATERECEIVED_API_LATEST 1

EOS_STRUCT(EOS_KWS_AddNotifyPermissionsUpdateReceivedOptions, (
	
	int32_t ApiVersion;
));

EOS_STRUCT(EOS_KWS_PermissionsUpdateReceivedCallbackInfo, (
	
	void* ClientData;
	
	EOS_ProductUserId LocalUserId;
	
	const char* KWSUserId;
	
	const char* DateOfBirth;
	
	EOS_Bool bIsMinor;
	
	const char* ParentEmail;
));

EOS_DECLARE_CALLBACK(EOS_KWS_OnPermissionsUpdateReceivedCallback, const EOS_KWS_PermissionsUpdateReceivedCallbackInfo* Data);

#pragma pack(pop)
