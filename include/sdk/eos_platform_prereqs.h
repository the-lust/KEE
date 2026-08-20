

#pragma once

#if defined(EOS_BUILD_PLATFORM_NAME)

#if defined(EOS_USE_DLLEXPORT) || defined(USE_CALL) || defined(EOS_MEMORY_CALL)
#error \
The macros EOS_MEMORY_CALL, EOS_CALL, and EOS_USE_DLLEXPORT where unexpectedly partially defined. \
This can occur if `eos_platform_prereqs.h` is included after `eos_base.h` is included. \
Please refer to https://dev.epicgames.com/docs/services for more details.
#endif

#undef EOS_PREPROCESSOR_TO_STRING
#define EOS_PREPROCESSOR_TO_STRING(x) EOS_PREPROCESSOR_TO_STRING_INNER(x)
#undef EOS_PREPROCESSOR_TO_STRING_INNER
#define EOS_PREPROCESSOR_TO_STRING_INNER(x) #x
#undef EOS_PREPROCESSOR_JOIN
#define EOS_PREPROCESSOR_JOIN(x,y) EOS_PREPROCESSOR_JOIN_INNER(x,y)
#undef EOS_PREPROCESSOR_JOIN_INNER
#define EOS_PREPROCESSOR_JOIN_INNER(x,y) x##y
#define EOS_BUILD_PLATFORM_HEADER_BASE EOS_PREPROCESSOR_TO_STRING(EOS_PREPROCESSOR_JOIN(EOS_BUILD_PLATFORM_NAME/eos_,EOS_PREPROCESSOR_JOIN(EOS_BUILD_PLATFORM_NAME,m_base.h)))

#include EOS_BUILD_PLATFORM_HEADER_BASE

#endif

