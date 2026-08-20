
#pragma once

#include "eos_types.h"

#pragma pack(push, 8)

#define EOS_WINDOWS_RTCOPTIONS_API_LATEST 1

EOS_STRUCT(EOS_Windows_RTCOptions, (
	
	int32_t ApiVersion;

	const char* XAudio29DllPath;
));

#pragma pack(pop)
