
#pragma once

#include "eos_types.h"

#pragma pack(push, 8)

EOS_EXTERN_C typedef void* (EOS_MEMORY_CALL * EOS_AllocateMemoryFunc)(size_t SizeInBytes, size_t Alignment);

EOS_EXTERN_C typedef void* (EOS_MEMORY_CALL * EOS_ReallocateMemoryFunc)(void* Pointer, size_t SizeInBytes, size_t Alignment);

EOS_EXTERN_C typedef void (EOS_MEMORY_CALL * EOS_ReleaseMemoryFunc)(void* Pointer);

#define EOS_INITIALIZE_THREADAFFINITY_API_LATEST 4

EOS_STRUCT(EOS_Initialize_ThreadAffinity, (
	
	int32_t ApiVersion;
	
	uint64_t NetworkWork;
	
	uint64_t StorageIo;
	
	uint64_t WebSocketIo;
	
	uint64_t P2PIo;
	
	uint64_t HttpRequestIo;
	
	uint64_t RTCIo;
	
	uint64_t EmbeddedOverlayMainThread;
	
	uint64_t EmbeddedOverlayWorkerThreads;
	
	uint64_t CryptographyWork;
));

#define EOS_INITIALIZE_API_LATEST 5

#define EOS_INITIALIZEOPTIONS_PRODUCTNAME_MAX_LENGTH 64

#define EOS_INITIALIZEOPTIONS_PRODUCTVERSION_MAX_LENGTH 64

EOS_STRUCT(EOS_InitializeOptions, (
	
	int32_t ApiVersion;
	
	EOS_AllocateMemoryFunc AllocateMemoryFunction;
	
	EOS_ReallocateMemoryFunc ReallocateMemoryFunction;
	
	EOS_ReleaseMemoryFunc ReleaseMemoryFunction;
	
	const char* ProductName;
	
	const char* ProductVersion;
	
	void* Reserved;
	
	void* SystemInitializeOptions;
	
	EOS_Initialize_ThreadAffinity* OverrideThreadAffinity;
));

EOS_DECLARE_FUNC(EOS_EResult) EOS_Initialize(const EOS_InitializeOptions* Options);

EOS_DECLARE_FUNC(EOS_EResult) EOS_Shutdown();

EOS_DECLARE_FUNC(EOS_HPlatform) EOS_Platform_Create(const EOS_Platform_Options* Options);

EOS_DECLARE_FUNC(void) EOS_Platform_Release(EOS_HPlatform Handle);

#pragma pack(pop)
