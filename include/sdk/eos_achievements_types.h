

#pragma once

#include "eos_common.h"

#pragma pack(push, 8)

EOS_EXTERN_C typedef struct EOS_AchievementsHandle* EOS_HAchievements;

#define EOS_ACHIEVEMENTS_QUERYDEFINITIONS_API_LATEST 3

EOS_STRUCT(EOS_Achievements_QueryDefinitionsOptions, (
	
	int32_t ApiVersion;
	
	EOS_ProductUserId LocalUserId;
	
	EOS_EpicAccountId EpicUserId_DEPRECATED;
	
	const char** HiddenAchievementIds_DEPRECATED;
	
	uint32_t HiddenAchievementsCount_DEPRECATED;
));

#define EOS_ACHIEVEMENTS_STATTHRESHOLDS_API_LATEST 1

#define EOS_ACHIEVEMENTS_STATTHRESHOLD_API_LATEST EOS_ACHIEVEMENTS_STATTHRESHOLDS_API_LATEST

EOS_STRUCT(EOS_Achievements_StatThresholds, (
	
	int32_t ApiVersion;
	
	const char* Name;
	
	int32_t Threshold;
));

#define EOS_ACHIEVEMENTS_PLAYERSTATINFO_API_LATEST 1

EOS_STRUCT(EOS_Achievements_PlayerStatInfo, (
	
	int32_t ApiVersion;
	
	const char* Name;
	
	int32_t CurrentValue;
	
	int32_t ThresholdValue;
));

#define EOS_ACHIEVEMENTS_DEFINITIONV2m_API_LATEST 2

EOS_STRUCT(EOS_Achievements_DefinitionV2, (
	
	int32_t ApiVersion;
	
	const char* AchievementId;
	
	const char* UnlockedDisplayName;
	
	const char* UnlockedDescription;
	
	const char* LockedDisplayName;
	
	const char* LockedDescription;
	
	const char* FlavorText;
	
	const char* UnlockedIconURL;
	
	const char* LockedIconURL;
	
	EOS_Bool bIsHidden;
	
	uint32_t StatThresholdsCount;
	
	const EOS_Achievements_StatThresholds* StatThresholds;
));

EOS_DECLARE_FUNC(void) EOS_Achievements_DefinitionV2m_Release(EOS_Achievements_DefinitionV2* AchievementDefinition);

#define EOS_ACHIEVEMENTS_GETACHIEVEMENTDEFINITIONCOUNT_API_LATEST 1

EOS_STRUCT(EOS_Achievements_GetAchievementDefinitionCountOptions, (
	
	int32_t ApiVersion;
));

#define EOS_ACHIEVEMENTS_COPYACHIEVEMENTDEFINITIONV2BYINDEX_API_LATEST 2

#define EOS_ACHIEVEMENTS_COPYDEFINITIONV2BYINDEX_API_LATEST EOS_ACHIEVEMENTS_COPYACHIEVEMENTDEFINITIONV2BYINDEX_API_LATEST

EOS_STRUCT(EOS_Achievements_CopyAchievementDefinitionV2ByIndexOptions, (
	
	int32_t ApiVersion;
	
	uint32_t AchievementIndex;
));

#define EOS_ACHIEVEMENTS_COPYACHIEVEMENTDEFINITIONV2BYACHIEVEMENTID_API_LATEST 2

#define EOS_ACHIEVEMENTS_COPYDEFINITIONV2BYACHIEVEMENTID_API_LATEST EOS_ACHIEVEMENTS_COPYACHIEVEMENTDEFINITIONV2BYACHIEVEMENTID_API_LATEST

EOS_STRUCT(EOS_Achievements_CopyAchievementDefinitionV2ByAchievementIdOptions, (
	
	int32_t ApiVersion;
	
	const char* AchievementId;
));

EOS_STRUCT(EOS_Achievements_OnQueryDefinitionsCompleteCallbackInfo, (
	
	EOS_EResult ResultCode;
	
	void* ClientData;
));

EOS_DECLARE_CALLBACK(EOS_Achievements_OnQueryDefinitionsCompleteCallback, const EOS_Achievements_OnQueryDefinitionsCompleteCallbackInfo* Data);

#define EOS_ACHIEVEMENTS_QUERYPLAYERACHIEVEMENTS_API_LATEST 2

EOS_STRUCT(EOS_Achievements_QueryPlayerAchievementsOptions, (
	
	int32_t ApiVersion;
	
	EOS_ProductUserId TargetUserId;
	
	EOS_ProductUserId LocalUserId;
));

#define EOS_ACHIEVEMENTS_ACHIEVEMENT_UNLOCKTIME_UNDEFINED -1

#define EOS_ACHIEVEMENTS_PLAYERACHIEVEMENT_API_LATEST 2

EOS_STRUCT(EOS_Achievements_PlayerAchievement, (
	
	int32_t ApiVersion;
	
	const char* AchievementId;
	
	double Progress;
	
	int64_t UnlockTime;
	
	int32_t StatInfoCount;
	
	const EOS_Achievements_PlayerStatInfo* StatInfo;
	
	const char* DisplayName;
	
	const char* Description;
	
	const char* IconURL;
	
	const char* FlavorText;
));

#define EOS_ACHIEVEMENTS_GETPLAYERACHIEVEMENTCOUNT_API_LATEST 1

EOS_STRUCT(EOS_Achievements_GetPlayerAchievementCountOptions, (
	
	int32_t ApiVersion;
	
	EOS_ProductUserId UserId;
));

#define EOS_ACHIEVEMENTS_COPYPLAYERACHIEVEMENTBYINDEX_API_LATEST 2

EOS_STRUCT(EOS_Achievements_CopyPlayerAchievementByIndexOptions, (
	
	int32_t ApiVersion;
	
	EOS_ProductUserId TargetUserId;
	
	uint32_t AchievementIndex;
	
	EOS_ProductUserId LocalUserId;
));

#define EOS_ACHIEVEMENTS_COPYPLAYERACHIEVEMENTBYACHIEVEMENTID_API_LATEST 2

EOS_STRUCT(EOS_Achievements_CopyPlayerAchievementByAchievementIdOptions, (
	
	int32_t ApiVersion;
	
	EOS_ProductUserId TargetUserId;
	
	const char* AchievementId;
	
	EOS_ProductUserId LocalUserId;
));

EOS_DECLARE_FUNC(void) EOS_Achievements_PlayerAchievement_Release(EOS_Achievements_PlayerAchievement* Achievement);

EOS_STRUCT(EOS_Achievements_OnQueryPlayerAchievementsCompleteCallbackInfo, (
	
	EOS_EResult ResultCode;
	
	void* ClientData;
	
	EOS_ProductUserId TargetUserId;
	
	EOS_ProductUserId LocalUserId;
));

EOS_DECLARE_CALLBACK(EOS_Achievements_OnQueryPlayerAchievementsCompleteCallback, const EOS_Achievements_OnQueryPlayerAchievementsCompleteCallbackInfo* Data);

#define EOS_ACHIEVEMENTS_UNLOCKACHIEVEMENTS_API_LATEST 1

EOS_STRUCT(EOS_Achievements_UnlockAchievementsOptions, (
	
	int32_t ApiVersion;
	
	EOS_ProductUserId UserId;
	
	const char** AchievementIds;
	
	uint32_t AchievementsCount;
));

EOS_STRUCT(EOS_Achievements_OnUnlockAchievementsCompleteCallbackInfo, (
	
	EOS_EResult ResultCode;
	
	void* ClientData;
	
	EOS_ProductUserId UserId;
	
	uint32_t AchievementsCount;
));

EOS_DECLARE_CALLBACK(EOS_Achievements_OnUnlockAchievementsCompleteCallback, const EOS_Achievements_OnUnlockAchievementsCompleteCallbackInfo* Data);

#define EOS_ACHIEVEMENTS_ADDNOTIFYACHIEVEMENTSUNLOCKEDV2m_API_LATEST 2

EOS_STRUCT(EOS_Achievements_AddNotifyAchievementsUnlockedV2Options, (
	
	int32_t ApiVersion;
));

EOS_STRUCT(EOS_Achievements_OnAchievementsUnlockedCallbackV2Info, (
	
	void* ClientData;
	
	EOS_ProductUserId UserId;
	
	const char* AchievementId;
	
	int64_t UnlockTime;
));

EOS_DECLARE_CALLBACK(EOS_Achievements_OnAchievementsUnlockedCallbackV2, const EOS_Achievements_OnAchievementsUnlockedCallbackV2Info* Data);

#pragma pack(pop)

#include "eos_achievements_types_deprecated.inl"
