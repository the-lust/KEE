#pragma once

#include "eos_platform_prereqs.h"
#include "eos_common.h"
#include "eos_presence_types.h"

#pragma pack(push, 8)

#define EOS_PRESENCEMODIFICATION_SETTEMPLATEID_API_LATEST 1

EOS_STRUCT(EOS_PresenceModification_SetTemplateIdOptions, (
	
	int32_t ApiVersion;
	
	const char* TemplateId;
));

#define EOS_PRESENCEMODIFICATION_SETTEMPLATEDATA_API_LATEST 1

EOS_ENUM(EOS_EPresenceModificationTemplateType,
	
	EOS_PMT_INT = 1,
	
	EOS_PMT_STRING = 2
);

EOS_STRUCT(EOS_PresenceModification_SetTemplateDataOptions, (
	
	int32_t ApiVersion;
	
	const char* Key;
	
	union
	{
		
		int32_t AsInt32;
		
		const char* AsStringId;
	} Value;
	
	EOS_EPresenceModificationTemplateType ValueType;
));

#pragma pack(pop)
