
#pragma once

#include "eos_common.h"

#pragma pack(push, 8)

EOS_EXTERN_C typedef struct EOS_SanctionsHandle* EOS_HSanctions;

#define EOS_SANCTIONS_PLAYERSANCTION_API_LATEST 2

EOS_STRUCT(EOS_Sanctions_PlayerSanction, (
	
	int32_t ApiVersion;
	
	int64_t TimePlaced;
	
	const char* Action;
	
	int64_t TimeExpires;
	
	const char* ReferenceId;
));

#define EOS_SANCTIONS_QUERYACTIVEPLAYERSANCTIONS_API_LATEST 2

EOS_STRUCT(EOS_Sanctions_QueryActivePlayerSanctionsOptions, (
	
	int32_t ApiVersion;
	
	EOS_ProductUserId TargetUserId;
	
	EOS_ProductUserId LocalUserId;
));

EOS_STRUCT(EOS_Sanctions_QueryActivePlayerSanctionsCallbackInfo, (
	
	EOS_EResult ResultCode;
	
	void* ClientData;
	
	EOS_ProductUserId TargetUserId;
	
	EOS_ProductUserId LocalUserId;
));

EOS_DECLARE_CALLBACK(EOS_Sanctions_OnQueryActivePlayerSanctionsCallback, const EOS_Sanctions_QueryActivePlayerSanctionsCallbackInfo* Data);

#define EOS_SANCTIONS_GETPLAYERSANCTIONCOUNT_API_LATEST 1

EOS_STRUCT(EOS_Sanctions_GetPlayerSanctionCountOptions, (
	
	int32_t ApiVersion;
	
	EOS_ProductUserId TargetUserId;
));

#define EOS_SANCTIONS_COPYPLAYERSANCTIONBYINDEX_API_LATEST 1

EOS_STRUCT(EOS_Sanctions_CopyPlayerSanctionByIndexOptions, (
	
	int32_t ApiVersion;
	
	EOS_ProductUserId TargetUserId;
	
	uint32_t SanctionIndex;
));

EOS_DECLARE_FUNC(void) EOS_Sanctions_PlayerSanction_Release(EOS_Sanctions_PlayerSanction* Sanction);

EOS_ENUM(EOS_ESanctionAppealReason,
	
	EOS_SAR_Invalid = 0,
	
	EOS_SAR_IncorrectSanction = 1,
	
	EOS_SAR_CompromisedAccount = 2,
	
	EOS_SAR_UnfairPunishment = 3,
	
	EOS_SAR_AppealForForgiveness = 4
);

#define EOS_SANCTIONS_CREATEPLAYERSANCTIONAPPEAL_API_LATEST 1

EOS_STRUCT(EOS_Sanctions_CreatePlayerSanctionAppealOptions, (
	
	int32_t ApiVersion;
	
	EOS_ProductUserId LocalUserId;
	
	EOS_ESanctionAppealReason Reason;
	
	const char* ReferenceId;
));

EOS_STRUCT(EOS_Sanctions_CreatePlayerSanctionAppealCallbackInfo, (
	
	EOS_EResult ResultCode;
	
	void* ClientData;
	
	const char* ReferenceId;
));

EOS_DECLARE_CALLBACK(EOS_Sanctions_CreatePlayerSanctionAppealCallback, const EOS_Sanctions_CreatePlayerSanctionAppealCallbackInfo* Data);

#pragma pack(pop)
