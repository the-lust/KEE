
#pragma once

#include "eos_common.h"

#pragma pack(push, 8)

EOS_EXTERN_C typedef struct EOS_ReportsHandle* EOS_HReports;

EOS_ENUM(EOS_EPlayerReportsCategory,
	
	EOS_PRC_Invalid = 0,
	
	EOS_PRC_Cheating = 1,
	
	EOS_PRC_Exploiting = 2,
	
	EOS_PRC_OffensiveProfile = 3,
	
	EOS_PRC_VerbalAbuse = 4,
	
	EOS_PRC_Scamming = 5,
	
	EOS_PRC_Spamming = 6,
	
	EOS_PRC_Other = 7
);

#define EOS_REPORTS_REPORTMESSAGE_MAX_LENGTH 512

#define EOS_REPORTS_REPORTCONTEXT_MAX_LENGTH 4096

#define EOS_REPORTS_SENDPLAYERBEHAVIORREPORT_API_LATEST 2

EOS_STRUCT(EOS_Reports_SendPlayerBehaviorReportOptions, (
	
	int32_t ApiVersion;
	
	EOS_ProductUserId ReporterUserId;
	
	EOS_ProductUserId ReportedUserId;
	
	EOS_EPlayerReportsCategory Category;
	
	const char* Message;
	
	const char* Context;
));

EOS_STRUCT(EOS_Reports_SendPlayerBehaviorReportCompleteCallbackInfo, (
	
	EOS_EResult ResultCode;
	
	void* ClientData;
));

EOS_DECLARE_CALLBACK(EOS_Reports_OnSendPlayerBehaviorReportCompleteCallback, const EOS_Reports_SendPlayerBehaviorReportCompleteCallbackInfo* Data);

#pragma pack(pop)
