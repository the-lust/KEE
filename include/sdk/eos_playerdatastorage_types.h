

#pragma once

#include "eos_common.h"

#pragma pack(push, 8)

#define EOS_PLAYERDATASTORAGE_FILENAME_MAX_LENGTH_BYTES 64

EOS_EXTERN_C typedef struct EOS_PlayerDataStorageHandle* EOS_HPlayerDataStorage;

#define EOS_PLAYERDATASTORAGE_TIME_UNDEFINED -1

#define EOS_PLAYERDATASTORAGE_FILEMETADATA_API_LATEST 3

EOS_STRUCT(EOS_PlayerDataStorage_FileMetadata, (
	
	int32_t ApiVersion;
	
	uint32_t FileSizeBytes;
	
	const char* MD5Hash;
	
	const char* Filename;
	
	int64_t LastModifiedTime;
	
	uint32_t UnencryptedDataSizeBytes;
));

EOS_DECLARE_FUNC(void) EOS_PlayerDataStorage_FileMetadata_Release(EOS_PlayerDataStorage_FileMetadata* FileMetadata);

#define EOS_PLAYERDATASTORAGE_QUERYFILE_API_LATEST 1

#define EOS_PLAYERDATASTORAGE_QUERYFILEOPTIONS_API_LATEST EOS_PLAYERDATASTORAGE_QUERYFILE_API_LATEST

EOS_STRUCT(EOS_PlayerDataStorage_QueryFileOptions, (
	
	int32_t ApiVersion;
	
	EOS_ProductUserId LocalUserId;
	
	const char* Filename;
));

EOS_STRUCT(EOS_PlayerDataStorage_QueryFileCallbackInfo, (
	
	EOS_EResult ResultCode;
	
	void* ClientData;
	
	EOS_ProductUserId LocalUserId;
));

EOS_DECLARE_CALLBACK(EOS_PlayerDataStorage_OnQueryFileCompleteCallback, const EOS_PlayerDataStorage_QueryFileCallbackInfo* Data);

#define EOS_PLAYERDATASTORAGE_QUERYFILELIST_API_LATEST 2

#define EOS_PLAYERDATASTORAGE_QUERYFILELISTOPTIONS_API_LATEST EOS_PLAYERDATASTORAGE_QUERYFILELIST_API_LATEST

EOS_STRUCT(EOS_PlayerDataStorage_QueryFileListOptions, (
	
	int32_t ApiVersion;
	
	EOS_ProductUserId LocalUserId;
));

EOS_STRUCT(EOS_PlayerDataStorage_QueryFileListCallbackInfo, (
	
	EOS_EResult ResultCode;
	
	void* ClientData;
	
	EOS_ProductUserId LocalUserId;
	
	uint32_t FileCount;
));

EOS_DECLARE_CALLBACK(EOS_PlayerDataStorage_OnQueryFileListCompleteCallback, const EOS_PlayerDataStorage_QueryFileListCallbackInfo* Data);

#define EOS_PLAYERDATASTORAGE_GETFILEMETADATACOUNT_API_LATEST 1

#define EOS_PLAYERDATASTORAGE_GETFILEMETADATACOUNTOPTIONS_API_LATEST EOS_PLAYERDATASTORAGE_GETFILEMETADATACOUNT_API_LATEST

EOS_STRUCT(EOS_PlayerDataStorage_GetFileMetadataCountOptions, (
	
	int32_t ApiVersion;
	
	EOS_ProductUserId LocalUserId;
));

#define EOS_PLAYERDATASTORAGE_COPYFILEMETADATAATINDEX_API_LATEST 1

#define EOS_PLAYERDATASTORAGE_COPYFILEMETADATAATINDEXOPTIONS_API_LATEST EOS_PLAYERDATASTORAGE_COPYFILEMETADATAATINDEX_API_LATEST

EOS_STRUCT(EOS_PlayerDataStorage_CopyFileMetadataAtIndexOptions, (
	
	int32_t ApiVersion;
	
	EOS_ProductUserId LocalUserId;
	
	uint32_t Index;
));

#define EOS_PLAYERDATASTORAGE_COPYFILEMETADATABYFILENAME_API_LATEST 1

#define EOS_PLAYERDATASTORAGE_COPYFILEMETADATABYFILENAMEOPTIONS_API_LATEST EOS_PLAYERDATASTORAGE_COPYFILEMETADATABYFILENAME_API_LATEST

EOS_STRUCT(EOS_PlayerDataStorage_CopyFileMetadataByFilenameOptions, (
	
	int32_t ApiVersion;
	
	EOS_ProductUserId LocalUserId;
	
	const char* Filename;
));

#define EOS_PLAYERDATASTORAGE_DUPLICATEFILE_API_LATEST 1

#define EOS_PLAYERDATASTORAGE_DUPLICATEFILEOPTIONS_API_LATEST EOS_PLAYERDATASTORAGE_DUPLICATEFILE_API_LATEST

EOS_STRUCT(EOS_PlayerDataStorage_DuplicateFileOptions, (
	
	int32_t ApiVersion;
	
	EOS_ProductUserId LocalUserId;
	
	const char* SourceFilename;
	
	const char* DestinationFilename;
));

EOS_STRUCT(EOS_PlayerDataStorage_DuplicateFileCallbackInfo, (
	
	EOS_EResult ResultCode;
	
	void* ClientData;
	
	EOS_ProductUserId LocalUserId;
));

EOS_DECLARE_CALLBACK(EOS_PlayerDataStorage_OnDuplicateFileCompleteCallback, const EOS_PlayerDataStorage_DuplicateFileCallbackInfo* Data);

#define EOS_PLAYERDATASTORAGE_DELETEFILE_API_LATEST 1

#define EOS_PLAYERDATASTORAGE_DELETEFILEOPTIONS_API_LATEST EOS_PLAYERDATASTORAGE_DELETEFILE_API_LATEST

EOS_STRUCT(EOS_PlayerDataStorage_DeleteFileOptions, (
	
	int32_t ApiVersion;
	
	EOS_ProductUserId LocalUserId;
	
	const char* Filename;
));

EOS_STRUCT(EOS_PlayerDataStorage_DeleteFileCallbackInfo, (
	
	EOS_EResult ResultCode;
	
	void* ClientData;
	
	EOS_ProductUserId LocalUserId;
));

EOS_DECLARE_CALLBACK(EOS_PlayerDataStorage_OnDeleteFileCompleteCallback, const EOS_PlayerDataStorage_DeleteFileCallbackInfo* Data);

EOS_EXTERN_C typedef struct EOS_PlayerDataStorageFileTransferRequestHandle* EOS_HPlayerDataStorageFileTransferRequest;

EOS_DECLARE_FUNC(void) EOS_PlayerDataStorageFileTransferRequest_Release(EOS_HPlayerDataStorageFileTransferRequest PlayerDataStorageFileTransferHandle);

EOS_STRUCT(EOS_PlayerDataStorage_FileTransferProgressCallbackInfo, (
	
	void* ClientData;
	
	EOS_ProductUserId LocalUserId;
	
	const char* Filename;
	
	uint32_t BytesTransferred;
	
	uint32_t TotalFileSizeBytes;
));

EOS_DECLARE_CALLBACK(EOS_PlayerDataStorage_OnFileTransferProgressCallback, const EOS_PlayerDataStorage_FileTransferProgressCallbackInfo* Data);

EOS_ENUM(EOS_PlayerDataStorage_EReadResult,
	
	EOS_RR_ContinueReading = 1,
	
	EOS_RR_FailRequest = 2,
	
	EOS_RR_CancelRequest = 3
);

EOS_STRUCT(EOS_PlayerDataStorage_ReadFileDataCallbackInfo, (
	
	void* ClientData;
	
	EOS_ProductUserId LocalUserId;
	
	const char* Filename;
	
	uint32_t TotalFileSizeBytes;
	
	EOS_Bool bIsLastChunk;
	
	uint32_t DataChunkLengthBytes;
	
	const void* DataChunk;
));

EOS_DECLARE_CALLBACK_RETVALUE(EOS_PlayerDataStorage_EReadResult, EOS_PlayerDataStorage_OnReadFileDataCallback, const EOS_PlayerDataStorage_ReadFileDataCallbackInfo* Data);

#define EOS_PLAYERDATASTORAGE_READFILE_API_LATEST 2

#define EOS_PLAYERDATASTORAGE_READFILEOPTIONS_API_LATEST EOS_PLAYERDATASTORAGE_READFILE_API_LATEST

EOS_STRUCT(EOS_PlayerDataStorage_ReadFileOptions, (
	
	int32_t ApiVersion;
	
	EOS_ProductUserId LocalUserId;
	
	const char* Filename;
	
	uint32_t ReadChunkLengthBytes;
	
	EOS_PlayerDataStorage_OnReadFileDataCallback ReadFileDataCallback;
	
	EOS_PlayerDataStorage_OnFileTransferProgressCallback FileTransferProgressCallback;
));

EOS_STRUCT(EOS_PlayerDataStorage_ReadFileCallbackInfo, (
	
	EOS_EResult ResultCode;
	
	void* ClientData;
	
	EOS_ProductUserId LocalUserId;
	
	const char* Filename;
));

EOS_DECLARE_CALLBACK(EOS_PlayerDataStorage_OnReadFileCompleteCallback, const EOS_PlayerDataStorage_ReadFileCallbackInfo* Data);

EOS_ENUM(EOS_PlayerDataStorage_EWriteResult,
	
	EOS_WR_ContinueWriting = 1,
	
	EOS_WR_CompleteRequest = 2,
	
	EOS_WR_FailRequest = 3,
	
	EOS_WR_CancelRequest = 4
);

EOS_STRUCT(EOS_PlayerDataStorage_WriteFileDataCallbackInfo, (
	
	void* ClientData;
	
	EOS_ProductUserId LocalUserId;
	
	const char* Filename;
	
	uint32_t DataBufferLengthBytes;
));

EOS_DECLARE_CALLBACK_RETVALUE(EOS_PlayerDataStorage_EWriteResult, EOS_PlayerDataStorage_OnWriteFileDataCallback, const EOS_PlayerDataStorage_WriteFileDataCallbackInfo* Data, void* OutDataBuffer, uint32_t* OutDataWritten);

#define EOS_PLAYERDATASTORAGE_WRITEFILE_API_LATEST 2

#define EOS_PLAYERDATASTORAGE_WRITEFILEOPTIONS_API_LATEST EOS_PLAYERDATASTORAGE_WRITEFILE_API_LATEST

EOS_STRUCT(EOS_PlayerDataStorage_WriteFileOptions, (
	
	int32_t ApiVersion;
	
	EOS_ProductUserId LocalUserId;
	
	const char* Filename;
	
	uint32_t ChunkLengthBytes;
	
	EOS_PlayerDataStorage_OnWriteFileDataCallback WriteFileDataCallback;
	
	EOS_PlayerDataStorage_OnFileTransferProgressCallback FileTransferProgressCallback;
));

EOS_STRUCT(EOS_PlayerDataStorage_WriteFileCallbackInfo, (
	
	EOS_EResult ResultCode;
	
	void* ClientData;
	
	EOS_ProductUserId LocalUserId;
	
	const char* Filename;
));

EOS_DECLARE_CALLBACK(EOS_PlayerDataStorage_OnWriteFileCompleteCallback, const EOS_PlayerDataStorage_WriteFileCallbackInfo* Data);

#define EOS_PLAYERDATASTORAGE_DELETECACHE_API_LATEST 1

#define EOS_PLAYERDATASTORAGE_DELETECACHEOPTIONS_API_LATEST EOS_PLAYERDATASTORAGE_DELETECACHE_API_LATEST

EOS_STRUCT(EOS_PlayerDataStorage_DeleteCacheOptions, (
	
	int32_t ApiVersion;
	
	EOS_ProductUserId LocalUserId;
));

EOS_STRUCT(EOS_PlayerDataStorage_DeleteCacheCallbackInfo, (
	
	EOS_EResult ResultCode;
	
	void* ClientData;
	
	EOS_ProductUserId LocalUserId;
));

EOS_DECLARE_CALLBACK(EOS_PlayerDataStorage_OnDeleteCacheCompleteCallback, const EOS_PlayerDataStorage_DeleteCacheCallbackInfo* Data);

#pragma pack(pop)