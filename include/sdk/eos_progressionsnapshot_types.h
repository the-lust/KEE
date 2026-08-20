

#pragma once

#include "eos_common.h"

#pragma pack(push, 8)

EOS_EXTERN_C typedef struct EOS_ProgressionSnapshotHandle* EOS_HProgressionSnapshot;

#define EOS_INVALID_PROGRESSIONSNAPSHOTID  0

#define EOS_PROGRESSIONSNAPSHOT_BEGINSNAPSHOT_API_LATEST 1

EOS_STRUCT(EOS_ProgressionSnapshot_BeginSnapshotOptions, (
	
	int32_t ApiVersion;
	
	EOS_ProductUserId LocalUserId;
));

#define EOS_PROGRESSIONSNAPSHOT_ADDPROGRESSION_API_LATEST 1

EOS_STRUCT(EOS_ProgressionSnapshot_AddProgressionOptions, (
	
	int32_t ApiVersion;
	
	uint32_t SnapshotId;
	
	const char* Key;
	
	const char* Value;
));

#define EOS_PROGRESSIONSNAPSHOT_SUBMITSNAPSHOT_API_LATEST 1

EOS_STRUCT(EOS_ProgressionSnapshot_SubmitSnapshotOptions, (
	
	int32_t ApiVersion;
	
	uint32_t SnapshotId;
));

#define EOS_PROGRESSIONSNAPSHOT_ENDSNAPSHOT_API_LATEST 1

EOS_STRUCT(EOS_ProgressionSnapshot_EndSnapshotOptions, (
	
	int32_t ApiVersion;
	
	uint32_t SnapshotId;
));

EOS_STRUCT(EOS_ProgressionSnapshot_SubmitSnapshotCallbackInfo, (
	
	EOS_EResult ResultCode;
	
	uint32_t SnapshotId;
	
	void* ClientData;
));

EOS_DECLARE_CALLBACK(EOS_ProgressionSnapshot_OnSubmitSnapshotCallback, const EOS_ProgressionSnapshot_SubmitSnapshotCallbackInfo* Data);

#define EOS_PROGRESSIONSNAPSHOT_DELETESNAPSHOT_API_LATEST 1

EOS_STRUCT(EOS_ProgressionSnapshot_DeleteSnapshotOptions, (
	
	int32_t ApiVersion;
	
	EOS_ProductUserId LocalUserId;
));

EOS_STRUCT(EOS_ProgressionSnapshot_DeleteSnapshotCallbackInfo, (
	
	EOS_EResult ResultCode;
	
	EOS_ProductUserId LocalUserId;
	
	void* ClientData;
));

EOS_DECLARE_CALLBACK(EOS_ProgressionSnapshot_OnDeleteSnapshotCallback, const EOS_ProgressionSnapshot_DeleteSnapshotCallbackInfo* Data);

#pragma pack(pop)
