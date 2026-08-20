
#pragma once

#include "eos_common.h"

#pragma pack(push, 8)

EOS_ENUM(EOS_ELogLevel,
	
	EOS_LOG_Off = 0,
	
	EOS_LOG_Fatal = 100,
	
	EOS_LOG_Error = 200,
	
	EOS_LOG_Warning = 300,
	
	EOS_LOG_Info = 400,
	
	EOS_LOG_Verbose = 500,
	
	EOS_LOG_VeryVerbose = 600
);

#define PROCESS_CATEGORY(CategoryName, EOSCategoryLabel, EOSCategoryValue) EOSCategoryLabel = EOSCategoryValue,
#define PROCESS_CATEGORY_LAST(CategoryName, EOSCategoryLabel, EOSCategoryValue) EOSCategoryLabel = EOSCategoryValue

EOS_ENUM_START(EOS_ELogCategory)
#include "eos_logging_categories.h"
EOS_ENUM_END(EOS_ELogCategory);

#undef PROCESS_CATEGORY
#undef PROCESS_CATEGORY_LAST

EOS_STRUCT(EOS_LogMessage, (
	
	const char* Category;
	
	const char* Message;
	
	EOS_ELogLevel Level;
));

EOS_EXTERN_C typedef void (EOS_CALL * EOS_LogMessageFunc)(const EOS_LogMessage* Message);

EOS_DECLARE_FUNC(EOS_EResult) EOS_Logging_SetCallback(EOS_LogMessageFunc Callback);

EOS_DECLARE_FUNC(EOS_EResult) EOS_Logging_SetLogLevel(EOS_ELogCategory LogCategory, EOS_ELogLevel LogLevel);

#pragma pack(pop)
