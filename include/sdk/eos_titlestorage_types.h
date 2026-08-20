

#pragma once

#include "eos_common.h"

#pragma pack(push, 8)

#define EOS_TITLESTORAGE_FILENAME_MAX_LENGTH_BYTES 64

EOS_EXTERN_C typedef struct EOS_TitleStorageHandle* EOS_HTitleStorage;

#define EOS_TITLESTORAGE_FILEMETADATA_API_LATEST 2

EOS_STRUCT(EOS_TitleStorage_FileMetadata, (
	
	int32_t ApiVersion;
	
	uint32_t FileSizeBytes;
	
	const char* MD5Hash;
	
	const char* Filename;
	
	uint32_t UnencryptedDataSizeBytes;
));

EOS_DECLARE_FUNC(void) EOS_TitleStorage_FileMetadata_Release(EOS_TitleStorage_FileMetadata* FileMetadata);

#define EOS_TITLESTORAGE_QUERYFILE_API_LATEST 1

#define EOS_TITLESTORAGE_QUERYFILEOPTIONS_API_LATEST EOS_TITLESTORAGE_QUERYFILE_API_LATEST

EOS_STRUCT(EOS_TitleStorage_QueryFileOptions, (
	
	int32_t ApiVersion;
	
	EOS_ProductUserId LocalUserId;
	
	const char* Filename;
));

EOS_STRUCT(EOS_TitleStorage_QueryFileCallbackInfo, (
	
	EOS_EResult ResultCode;
	
	void* ClientData;
	
	EOS_ProductUserId LocalUserId;
));

EOS_DECLARE_CALLBACK(EOS_TitleStorage_OnQueryFileCompleteCallback, const EOS_TitleStorage_QueryFileCallbackInfo* Data);

#define EOS_TITLESTORAGE_QUERYFILELIST_API_LATEST 1

#define EOS_TITLESTORAGE_QUERYFILELISTOPTIONS_API_LATEST EOS_TITLESTORAGE_QUERYFILELIST_API_LATEST

EOS_STRUCT(EOS_TitleStorage_QueryFileListOptions, (
	
	int32_t ApiVersion;
	
	EOS_ProductUserId LocalUserId;
	
	const char* const* ListOfTags;
	
	uint32_t ListOfTagsCount;
));

EOS_STRUCT(EOS_TitleStorage_QueryFileListCallbackInfo, (
	
	EOS_EResult ResultCode;
	
	void* ClientData;
	
	EOS_ProductUserId LocalUserId;
	
	uint32_t FileCount;
));

EOS_DECLARE_CALLBACK(EOS_TitleStorage_OnQueryFileListCompleteCallback, const EOS_TitleStorage_QueryFileListCallbackInfo* Data);

#define EOS_TITLESTORAGE_GETFILEMETADATACOUNT_API_LATEST 1

#define EOS_TITLESTORAGE_GETFILEMETADATACOUNTOPTIONS_API_LATEST EOS_TITLESTORAGE_GETFILEMETADATACOUNT_API_LATEST

EOS_STRUCT(EOS_TitleStorage_GetFileMetadataCountOptions, (
	
	int32_t ApiVersion;
	
	EOS_ProductUserId LocalUserId;
));

#define EOS_TITLESTORAGE_COPYFILEMETADATAATINDEX_API_LATEST 1

#define EOS_TITLESTORAGE_COPYFILEMETADATAATINDEXOPTIONS_API_LATEST EOS_TITLESTORAGE_COPYFILEMETADATAATINDEX_API_LATEST

EOS_STRUCT(EOS_TitleStorage_CopyFileMetadataAtIndexOptions, (
	
	int32_t ApiVersion;
	
	EOS_ProductUserId LocalUserId;
	
	uint32_t Index;
));

#define EOS_TITLESTORAGE_COPYFILEMETADATABYFILENAME_API_LATEST 1

#define EOS_TITLESTORAGE_COPYFILEMETADATABYFILENAMEOPTIONS_API_LATEST EOS_TITLESTORAGE_COPYFILEMETADATABYFILENAME_API_LATEST

EOS_STRUCT(EOS_TitleStorage_CopyFileMetadataByFilenameOptions, (
	
	int32_t ApiVersion;
	
	EOS_ProductUserId LocalUserId;
	
	const char* Filename;
));

EOS_EXTERN_C typedef struct EOS_TitleStorageFileTransferRequestHandle* EOS_HTitleStorageFileTransferRequest;

EOS_DECLARE_FUNC(void) EOS_TitleStorageFileTransferRequest_Release(EOS_HTitleStorageFileTransferRequest TitleStorageFileTransferHandle);

EOS_STRUCT(EOS_TitleStorage_FileTransferProgressCallbackInfo, (
	
	void* ClientData;
	
	EOS_ProductUserId LocalUserId;
	
	const char* Filename;
	
	uint32_t BytesTransferred;
	
	uint32_t TotalFileSizeBytes;
));

EOS_DECLARE_CALLBACK(EOS_TitleStorage_OnFileTransferProgressCallback, const EOS_TitleStorage_FileTransferProgressCallbackInfo* Data);

EOS_ENUM(EOS_TitleStorage_EReadResult,
	
	EOS_TS_RR_ContinueReading = 1,
	
	EOS_TS_RR_FailRequest = 2,
	
	EOS_TS_RR_CancelRequest = 3
);

EOS_STRUCT(EOS_TitleStorage_ReadFileDataCallbackInfo, (
	
	void* ClientData;
	
	EOS_ProductUserId LocalUserId;
	
	const char* Filename;
	
	uint32_t TotalFileSizeBytes;
	
	EOS_Bool bIsLastChunk;
	
	uint32_t DataChunkLengthBytes;
	
	const void* DataChunk;
));

EOS_DECLARE_CALLBACK_RETVALUE(EOS_TitleStorage_EReadResult, EOS_TitleStorage_OnReadFileDataCallback, const EOS_TitleStorage_ReadFileDataCallbackInfo* Data);

#define EOS_TITLESTORAGE_READFILE_API_LATEST 2

#define EOS_TITLESTORAGE_READFILEOPTIONS_API_LATEST EOS_TITLESTORAGE_READFILE_API_LATEST

EOS_STRUCT(EOS_TitleStorage_ReadFileOptions, (
	
	int32_t ApiVersion;
	
	EOS_ProductUserId LocalUserId;
	
	const char* Filename;
	
	uint32_t ReadChunkLengthBytes;
	
	EOS_TitleStorage_OnReadFileDataCallback ReadFileDataCallback;
	
	EOS_TitleStorage_OnFileTransferProgressCallback FileTransferProgressCallback;
));

EOS_STRUCT(EOS_TitleStorage_ReadFileCallbackInfo, (
	
	EOS_EResult ResultCode;
	
	void* ClientData;
	
	EOS_ProductUserId LocalUserId;
	
	const char* Filename;
));

EOS_DECLARE_CALLBACK(EOS_TitleStorage_OnReadFileCompleteCallback, const EOS_TitleStorage_ReadFileCallbackInfo* Data);

#define EOS_TITLESTORAGE_DELETECACHE_API_LATEST 1

#define EOS_TITLESTORAGE_DELETECACHEOPTIONS_API_LATEST EOS_TITLESTORAGE_DELETECACHE_API_LATEST

EOS_STRUCT(EOS_TitleStorage_DeleteCacheOptions, (
	
	int32_t ApiVersion;
	
	EOS_ProductUserId LocalUserId;
));

EOS_STRUCT(EOS_TitleStorage_DeleteCacheCallbackInfo, (
	
	EOS_EResult ResultCode;
	
	void* ClientData;
	
	EOS_ProductUserId LocalUserId;
));

EOS_DECLARE_CALLBACK(EOS_TitleStorage_OnDeleteCacheCompleteCallback, const EOS_TitleStorage_DeleteCacheCallbackInfo* Data);

#pragma pack(pop)