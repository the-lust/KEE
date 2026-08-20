

#pragma once

#include "eos_common.h"

#pragma pack(push, 8)

EOS_EXTERN_C typedef struct EOS_StatsHandle* EOS_HStats;

#define EOS_STATS_INGESTDATA_API_LATEST 1

EOS_STRUCT(EOS_Stats_IngestData, (
	
	int32_t ApiVersion;
	
	const char* StatName;
	
	int32_t IngestAmount;
));

#define EOS_STATS_MAX_INGEST_STATS 3000

#define EOS_STATS_INGESTSTAT_API_LATEST 3

EOS_STRUCT(EOS_Stats_IngestStatOptions, (
	
	int32_t ApiVersion;
	
	EOS_ProductUserId LocalUserId;
	
	const EOS_Stats_IngestData* Stats;
	
	uint32_t StatsCount;
	
	EOS_ProductUserId TargetUserId;
));

EOS_STRUCT(EOS_Stats_IngestStatCompleteCallbackInfo, (
	
	EOS_EResult ResultCode;
	
	void* ClientData;
	
	EOS_ProductUserId LocalUserId;
	
	EOS_ProductUserId TargetUserId;
));

EOS_DECLARE_CALLBACK(EOS_Stats_OnIngestStatCompleteCallback, const EOS_Stats_IngestStatCompleteCallbackInfo* Data);

#define EOS_STATS_MAX_QUERY_STATS 1000

#define EOS_STATS_QUERYSTATS_API_LATEST 3

EOS_STRUCT(EOS_Stats_QueryStatsOptions, (
	
	int32_t ApiVersion;
	
	EOS_ProductUserId LocalUserId;
	
	int64_t StartTime;
	
	int64_t EndTime;
	
	const char** StatNames;
	
	uint32_t StatNamesCount;
	
	EOS_ProductUserId TargetUserId;
));

#define EOS_STATS_TIME_UNDEFINED -1

#define EOS_STATS_STAT_API_LATEST 1

EOS_STRUCT(EOS_Stats_Stat, (
	
	int32_t ApiVersion;
	
	const char* Name;
	
	int64_t StartTime;
	
	int64_t EndTime;
	
	int32_t Value;
));

#define EOS_STATS_GETSTATSCOUNT_API_LATEST 1

#define EOS_STATS_GETSTATCOUNT_API_LATEST EOS_STATS_GETSTATSCOUNT_API_LATEST

EOS_STRUCT(EOS_Stats_GetStatCountOptions, (
	
	int32_t ApiVersion;
	
	EOS_ProductUserId TargetUserId;
));

#define EOS_STATS_COPYSTATBYINDEX_API_LATEST 1

EOS_STRUCT(EOS_Stats_CopyStatByIndexOptions, (
	
	int32_t ApiVersion;
	
	EOS_ProductUserId TargetUserId;
	
	uint32_t StatIndex;
));

#define EOS_STATS_COPYSTATBYNAME_API_LATEST 1

EOS_STRUCT(EOS_Stats_CopyStatByNameOptions, (
	
	int32_t ApiVersion;
	
	EOS_ProductUserId TargetUserId;
	
	const char* Name;
));

EOS_DECLARE_FUNC(void) EOS_Stats_Stat_Release(EOS_Stats_Stat* Stat);

EOS_STRUCT(EOS_Stats_OnQueryStatsCompleteCallbackInfo, (
	
	EOS_EResult ResultCode;
	
	void* ClientData;
	
	EOS_ProductUserId LocalUserId;
	
	EOS_ProductUserId TargetUserId;
));

EOS_DECLARE_CALLBACK(EOS_Stats_OnQueryStatsCompleteCallback, const EOS_Stats_OnQueryStatsCompleteCallbackInfo* Data);

#pragma pack(pop)
