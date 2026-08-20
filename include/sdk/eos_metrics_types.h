

#pragma once

#include "eos_common.h"

#pragma pack(push, 8)

EOS_EXTERN_C typedef struct EOS_MetricsHandle* EOS_HMetrics;

EOS_ENUM(EOS_EUserControllerType,
	
	EOS_UCT_Unknown = 0,
	
	EOS_UCT_MouseKeyboard = 1,
	
	EOS_UCT_GamepadControl = 2,
	
	EOS_UCT_TouchControl = 3
);

EOS_ENUM(EOS_EMetricsAccountIdType,
	
	EOS_MAIT_Epic = 0,
	
	EOS_MAIT_External = 1
);

#define EOS_METRICS_BEGINPLAYERSESSION_API_LATEST 1

EOS_STRUCT(EOS_Metrics_BeginPlayerSessionOptions, (
	
	int32_t ApiVersion;
	
	EOS_EMetricsAccountIdType AccountIdType;
	
	union
	{
		
		EOS_EpicAccountId Epic;
		
		const char* External;
	} AccountId;
	
	const char* DisplayName;
	
	EOS_EUserControllerType ControllerType;
	
	const char* ServerIp;
	
	const char* GameSessionId;
));

#define EOS_METRICS_ENDPLAYERSESSION_API_LATEST 1

EOS_STRUCT(EOS_Metrics_EndPlayerSessionOptions, (
	
	int32_t ApiVersion;
	
	EOS_EMetricsAccountIdType AccountIdType;
	
	union
	{
		
		EOS_EpicAccountId Epic;
		
		const char* External;
	} AccountId;
));

#pragma pack(pop)
