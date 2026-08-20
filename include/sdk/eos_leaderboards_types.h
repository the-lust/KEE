

#pragma once

#include "eos_common.h"

#pragma pack(push, 8)

EOS_EXTERN_C typedef struct EOS_LeaderboardsHandle* EOS_HLeaderboards;

#define EOS_LEADERBOARDS_TIME_UNDEFINED -1

#define EOS_LEADERBOARDS_QUERYLEADERBOARDDEFINITIONS_API_LATEST 2

EOS_STRUCT(EOS_Leaderboards_QueryLeaderboardDefinitionsOptions, (
	
	int32_t ApiVersion;
	
	int64_t StartTime;
	
	int64_t EndTime;
	
	EOS_ProductUserId LocalUserId;
));

EOS_ENUM(EOS_ELeaderboardAggregation,
	
	EOS_LA_Min = 0,
	
	EOS_LA_Max = 1,
	
	EOS_LA_Sum = 2,
	
	EOS_LA_Latest = 3
);

#define EOS_LEADERBOARDS_DEFINITION_API_LATEST 1

EOS_STRUCT(EOS_Leaderboards_Definition, (
	
	int32_t ApiVersion;
	
	const char* LeaderboardId;
	
	const char* StatName;
	
	EOS_ELeaderboardAggregation Aggregation;
	
	int64_t StartTime;
	
	int64_t EndTime;
));

#define EOS_LEADERBOARDS_GETLEADERBOARDDEFINITIONCOUNT_API_LATEST 1

EOS_STRUCT(EOS_Leaderboards_GetLeaderboardDefinitionCountOptions, (
	
	int32_t ApiVersion;
));

#define EOS_LEADERBOARDS_COPYLEADERBOARDDEFINITIONBYINDEX_API_LATEST 1

EOS_STRUCT(EOS_Leaderboards_CopyLeaderboardDefinitionByIndexOptions, (
	
	int32_t ApiVersion;
	
	uint32_t LeaderboardIndex;
));

#define EOS_LEADERBOARDS_COPYLEADERBOARDDEFINITIONBYLEADERBOARDID_API_LATEST 1

EOS_STRUCT(EOS_Leaderboards_CopyLeaderboardDefinitionByLeaderboardIdOptions, (
	
	int32_t ApiVersion;
	
	const char* LeaderboardId;
));

EOS_DECLARE_FUNC(void) EOS_Leaderboards_Definition_Release(EOS_Leaderboards_Definition* LeaderboardDefinition);

EOS_STRUCT(EOS_Leaderboards_OnQueryLeaderboardDefinitionsCompleteCallbackInfo, (
	
	EOS_EResult ResultCode;
	
	void* ClientData;
));

EOS_DECLARE_CALLBACK(EOS_Leaderboards_OnQueryLeaderboardDefinitionsCompleteCallback, const EOS_Leaderboards_OnQueryLeaderboardDefinitionsCompleteCallbackInfo* Data);

#define EOS_LEADERBOARDS_USERSCORESQUERYSTATINFO_API_LATEST 1

EOS_STRUCT(EOS_Leaderboards_UserScoresQueryStatInfo, (
	
	int32_t ApiVersion;
	
	const char* StatName;
	
	EOS_ELeaderboardAggregation Aggregation;
));

#define EOS_LEADERBOARDS_QUERYLEADERBOARDUSERSCORES_API_LATEST 2

EOS_STRUCT(EOS_Leaderboards_QueryLeaderboardUserScoresOptions, (
	
	int32_t ApiVersion;
	
	const EOS_ProductUserId* UserIds;
	
	uint32_t UserIdsCount;
	
	const EOS_Leaderboards_UserScoresQueryStatInfo* StatInfo;
	
	uint32_t StatInfoCount;
	
	int64_t StartTime;
	
	int64_t EndTime;
	
	EOS_ProductUserId LocalUserId;
));

#define EOS_LEADERBOARDS_LEADERBOARDUSERSCORE_API_LATEST 1

EOS_STRUCT(EOS_Leaderboards_LeaderboardUserScore, (
	
	int32_t ApiVersion;
	
	EOS_ProductUserId UserId;
	
	int32_t Score;
));

#define EOS_LEADERBOARDS_GETLEADERBOARDUSERSCORECOUNT_API_LATEST 1

EOS_STRUCT(EOS_Leaderboards_GetLeaderboardUserScoreCountOptions, (
	
	int32_t ApiVersion;
	
	const char* StatName;
));

#define EOS_LEADERBOARDS_COPYLEADERBOARDUSERSCOREBYINDEX_API_LATEST 1

EOS_STRUCT(EOS_Leaderboards_CopyLeaderboardUserScoreByIndexOptions, (
	
	int32_t ApiVersion;
	
	uint32_t LeaderboardUserScoreIndex;
	
	const char* StatName;
));

#define EOS_LEADERBOARDS_COPYLEADERBOARDUSERSCOREBYUSERID_API_LATEST 1

EOS_STRUCT(EOS_Leaderboards_CopyLeaderboardUserScoreByUserIdOptions, (
	
	int32_t ApiVersion;
	
	EOS_ProductUserId UserId;
	
	const char* StatName;
));

EOS_DECLARE_FUNC(void) EOS_Leaderboards_LeaderboardUserScore_Release(EOS_Leaderboards_LeaderboardUserScore* LeaderboardUserScore);

EOS_STRUCT(EOS_Leaderboards_OnQueryLeaderboardUserScoresCompleteCallbackInfo, (
	
	EOS_EResult ResultCode;
	
	void* ClientData;
));

EOS_DECLARE_CALLBACK(EOS_Leaderboards_OnQueryLeaderboardUserScoresCompleteCallback, const EOS_Leaderboards_OnQueryLeaderboardUserScoresCompleteCallbackInfo* Data);

#define EOS_LEADERBOARDS_QUERYLEADERBOARDRANKS_API_LATEST 2

EOS_STRUCT(EOS_Leaderboards_QueryLeaderboardRanksOptions, (
	
	int32_t ApiVersion;
	
	const char* LeaderboardId;
	
	EOS_ProductUserId LocalUserId;
));

#define EOS_LEADERBOARDS_LEADERBOARDRECORD_API_LATEST 2

EOS_STRUCT(EOS_Leaderboards_LeaderboardRecord, (
	
	int32_t ApiVersion;
	
	EOS_ProductUserId UserId;
	
	uint32_t Rank;
	
	int32_t Score;
	
	const char* UserDisplayName;
));

#define EOS_LEADERBOARDS_GETLEADERBOARDRECORDCOUNT_API_LATEST 1

EOS_STRUCT(EOS_Leaderboards_GetLeaderboardRecordCountOptions, (
	
	int32_t ApiVersion;
));

#define EOS_LEADERBOARDS_COPYLEADERBOARDRECORDBYINDEX_API_LATEST 2

EOS_STRUCT(EOS_Leaderboards_CopyLeaderboardRecordByIndexOptions, (
	
	int32_t ApiVersion;
	
	uint32_t LeaderboardRecordIndex;
));

#define EOS_LEADERBOARDS_COPYLEADERBOARDRECORDBYUSERID_API_LATEST 2

EOS_STRUCT(EOS_Leaderboards_CopyLeaderboardRecordByUserIdOptions, (
	
	int32_t ApiVersion;
	
	EOS_ProductUserId UserId;
));

EOS_DECLARE_FUNC(void) EOS_Leaderboards_LeaderboardRecord_Release(EOS_Leaderboards_LeaderboardRecord* LeaderboardRecord);

EOS_STRUCT(EOS_Leaderboards_OnQueryLeaderboardRanksCompleteCallbackInfo, (
	
	EOS_EResult ResultCode;
	
	void* ClientData;
	
	const char* LeaderboardId;
));

EOS_DECLARE_CALLBACK(EOS_Leaderboards_OnQueryLeaderboardRanksCompleteCallback, const EOS_Leaderboards_OnQueryLeaderboardRanksCompleteCallbackInfo* Data);

#pragma pack(pop)

#include "eos_leaderboards_types_deprecated.inl"