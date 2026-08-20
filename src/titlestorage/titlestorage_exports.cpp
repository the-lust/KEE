
#include "utils/common_includes.h"
#include "eossdk_platform.h"
#include "titlestorage/eossdk_titlestorage.h"

EOS_DECLARE_FUNC(void) EOS_TitleStorage_QueryFile(EOS_HTitleStorage Handle, const EOS_TitleStorage_QueryFileOptions* Options, void* ClientData, const EOS_TitleStorage_OnQueryFileCompleteCallback CompletionDelegate)
{
    auto* pInst = reinterpret_cast<sdk::EOSSDK_TitleStorage*>(Handle);
    pInst->QueryFile(Options, ClientData, CompletionDelegate);
}

EOS_DECLARE_FUNC(void) EOS_TitleStorage_QueryFileList(EOS_HTitleStorage Handle, const EOS_TitleStorage_QueryFileListOptions* Options, void* ClientData, const EOS_TitleStorage_OnQueryFileListCompleteCallback CompletionDelegate)
{
    auto* pInst = reinterpret_cast<sdk::EOSSDK_TitleStorage*>(Handle);
    pInst->QueryFileList(Options, ClientData, CompletionDelegate);
}

EOS_DECLARE_FUNC(EOS_EResult) EOS_TitleStorage_CopyFileMetadataByFilename(EOS_HTitleStorage Handle, const EOS_TitleStorage_CopyFileMetadataByFilenameOptions* Options, EOS_TitleStorage_FileMetadata** OutMetadata)
{
    auto* pInst = reinterpret_cast<sdk::EOSSDK_TitleStorage*>(Handle);
    return pInst->CopyFileMetadataByFilename(Options, OutMetadata);
}

EOS_DECLARE_FUNC(uint32_t) EOS_TitleStorage_GetFileMetadataCount(EOS_HTitleStorage Handle, const EOS_TitleStorage_GetFileMetadataCountOptions* Options)
{
    auto* pInst = reinterpret_cast<sdk::EOSSDK_TitleStorage*>(Handle);
    return pInst->GetFileMetadataCount(Options);
}

EOS_DECLARE_FUNC(EOS_EResult) EOS_TitleStorage_CopyFileMetadataAtIndex(EOS_HTitleStorage Handle, const EOS_TitleStorage_CopyFileMetadataAtIndexOptions* Options, EOS_TitleStorage_FileMetadata** OutMetadata)
{
    auto* pInst = reinterpret_cast<sdk::EOSSDK_TitleStorage*>(Handle);
    return pInst->CopyFileMetadataAtIndex(Options, OutMetadata);
}

EOS_DECLARE_FUNC(EOS_HTitleStorageFileTransferRequest) EOS_TitleStorage_ReadFile(EOS_HTitleStorage Handle, const EOS_TitleStorage_ReadFileOptions* Options, void* ClientData, const EOS_TitleStorage_OnReadFileCompleteCallback CompletionDelegate)
{
    auto* pInst = reinterpret_cast<sdk::EOSSDK_TitleStorage*>(Handle);
    return pInst->ReadFile(Options, ClientData, CompletionDelegate);
}

EOS_DECLARE_FUNC(EOS_EResult) EOS_TitleStorage_DeleteCache(EOS_HTitleStorage Handle, const EOS_TitleStorage_DeleteCacheOptions* Options, void* ClientData, const EOS_TitleStorage_OnDeleteCacheCompleteCallback CompletionDelegate)
{
    auto* pInst = reinterpret_cast<sdk::EOSSDK_TitleStorage*>(Handle);
    return pInst->DeleteCache(Options, ClientData, CompletionDelegate);
}

EOS_DECLARE_FUNC(EOS_EResult) EOS_TitleStorageFileTransferRequest_GetFileRequestState(EOS_HTitleStorageFileTransferRequest Handle)
{
    auto* pInst = reinterpret_cast<sdk::EOSSDK_TitleStorageFileTransferRequest*>(Handle);
    return pInst->GetFileRequestState();
}

EOS_DECLARE_FUNC(EOS_EResult) EOS_TitleStorageFileTransferRequest_GetFilename(EOS_HTitleStorageFileTransferRequest Handle, uint32_t FilenameStringBufferSizeBytes, char* OutStringBuffer, int32_t* OutStringLength)
{
    auto* pInst = reinterpret_cast<sdk::EOSSDK_TitleStorageFileTransferRequest*>(Handle);
    return pInst->GetFilename(FilenameStringBufferSizeBytes, OutStringBuffer, OutStringLength);
}

EOS_DECLARE_FUNC(EOS_EResult) EOS_TitleStorageFileTransferRequest_CancelRequest(EOS_HTitleStorageFileTransferRequest Handle)
{
    auto* pInst = reinterpret_cast<sdk::EOSSDK_TitleStorageFileTransferRequest*>(Handle);
    return pInst->CancelRequest();
}

EOS_DECLARE_FUNC(void) EOS_TitleStorageFileTransferRequest_Release(EOS_HTitleStorageFileTransferRequest Handle)
{
    delete reinterpret_cast<sdk::EOSSDK_TitleStorageFileTransferRequest*>(Handle);
}

EOS_DECLARE_FUNC(void) EOS_TitleStorage_FileMetadata_Release(EOS_TitleStorage_FileMetadata* FileMetadata)
{
    delete FileMetadata;
}
