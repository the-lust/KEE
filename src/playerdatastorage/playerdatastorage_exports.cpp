#include "playerdatastorage/eossdk_playerdatastorage.h"

using namespace sdk;

extern "C"
{
    EOS_DECLARE_FUNC(void) EOS_PlayerDataStorage_QueryFile(EOS_HPlayerDataStorage Handle, const EOS_PlayerDataStorage_QueryFileOptions* Options, void* ClientData, const EOS_PlayerDataStorage_OnQueryFileCompleteCallback CompletionDelegate)
    {
        if (Handle == nullptr)
            return;

        auto pInst = reinterpret_cast<EOSSDK_PlayerDataStorage*>(Handle);
        pInst->QueryFile(Options, ClientData, CompletionDelegate);
    }

    EOS_DECLARE_FUNC(void) EOS_PlayerDataStorage_QueryFileList(EOS_HPlayerDataStorage Handle, const EOS_PlayerDataStorage_QueryFileListOptions* Options, void* ClientData, const EOS_PlayerDataStorage_OnQueryFileListCompleteCallback CompletionDelegate)
    {
        if (Handle == nullptr)
            return;

        auto pInst = reinterpret_cast<EOSSDK_PlayerDataStorage*>(Handle);
        pInst->QueryFileList(Options, ClientData, CompletionDelegate);
    }

    EOS_DECLARE_FUNC(EOS_EResult) EOS_PlayerDataStorage_CopyFileMetadataAtIndex(EOS_HPlayerDataStorage Handle, const EOS_PlayerDataStorage_CopyFileMetadataAtIndexOptions* Options, EOS_PlayerDataStorage_FileMetadata** OutMetadata)
    {
        if (Handle == nullptr)
            return EOS_EResult::EOS_InvalidParameters;

        auto pInst = reinterpret_cast<EOSSDK_PlayerDataStorage*>(Handle);
        return pInst->CopyFileMetadataAtIndex(Options, OutMetadata);
    }

    EOS_DECLARE_FUNC(EOS_EResult) EOS_PlayerDataStorage_CopyFileMetadataByFilename(EOS_HPlayerDataStorage Handle, const EOS_PlayerDataStorage_CopyFileMetadataByFilenameOptions* Options, EOS_PlayerDataStorage_FileMetadata** OutMetadata)
    {
        if (Handle == nullptr)
            return EOS_EResult::EOS_InvalidParameters;

        auto pInst = reinterpret_cast<EOSSDK_PlayerDataStorage*>(Handle);
        return pInst->CopyFileMetadataByFilename(Options, OutMetadata);
    }

    EOS_DECLARE_FUNC(EOS_EResult) EOS_PlayerDataStorage_GetFileMetadataCount(EOS_HPlayerDataStorage Handle, const EOS_PlayerDataStorage_GetFileMetadataCountOptions* Options, int32_t* OutFileMetadataCount)
    {
        if (Handle == nullptr)
            return EOS_EResult::EOS_InvalidParameters;

        auto pInst = reinterpret_cast<EOSSDK_PlayerDataStorage*>(Handle);
        return pInst->GetFileMetadataCount(Options, OutFileMetadataCount);
    }

    EOS_DECLARE_FUNC(EOS_HPlayerDataStorageFileTransferRequest) EOS_PlayerDataStorage_ReadFile(EOS_HPlayerDataStorage Handle, const EOS_PlayerDataStorage_ReadFileOptions* Options, void* ClientData, const EOS_PlayerDataStorage_OnReadFileCompleteCallback CompletionDelegate)
    {
        if (Handle == nullptr)
            return nullptr;

        auto pInst = reinterpret_cast<EOSSDK_PlayerDataStorage*>(Handle);
        return pInst->ReadFile(Options, ClientData, CompletionDelegate);
    }

    EOS_DECLARE_FUNC(EOS_HPlayerDataStorageFileTransferRequest) EOS_PlayerDataStorage_WriteFile(EOS_HPlayerDataStorage Handle, const EOS_PlayerDataStorage_WriteFileOptions* Options, void* ClientData, const EOS_PlayerDataStorage_OnWriteFileCompleteCallback CompletionDelegate)
    {
        if (Handle == nullptr)
            return nullptr;

        auto pInst = reinterpret_cast<EOSSDK_PlayerDataStorage*>(Handle);
        return pInst->WriteFile(Options, ClientData, CompletionDelegate);
    }

    EOS_DECLARE_FUNC(void) EOS_PlayerDataStorage_DeleteFile(EOS_HPlayerDataStorage Handle, const EOS_PlayerDataStorage_DeleteFileOptions* Options, void* ClientData, const EOS_PlayerDataStorage_OnDeleteFileCompleteCallback CompletionDelegate)
    {
        if (Handle == nullptr)
            return;

        auto pInst = reinterpret_cast<EOSSDK_PlayerDataStorage*>(Handle);
        pInst->DeleteFile(Options, ClientData, CompletionDelegate);
    }

    EOS_DECLARE_FUNC(EOS_EResult) EOS_PlayerDataStorageFileTransferRequest_GetFileRequestState(EOS_HPlayerDataStorageFileTransferRequest Handle)
    {
        if (Handle == nullptr)
            return EOS_EResult::EOS_InvalidParameters;

        auto pInst = reinterpret_cast<EOSSDK_PlayerDataStorageFileTransferRequest*>(Handle);
        return pInst->GetFileRequestState();
    }

    EOS_DECLARE_FUNC(EOS_EResult) EOS_PlayerDataStorageFileTransferRequest_GetFilename(EOS_HPlayerDataStorageFileTransferRequest Handle, uint32_t FilenameSizeBytes, char* OutBuffer, int32_t* OutBytesWritten)
    {
        if (Handle == nullptr)
            return EOS_EResult::EOS_InvalidParameters;

        auto pInst = reinterpret_cast<EOSSDK_PlayerDataStorageFileTransferRequest*>(Handle);
        return pInst->GetFilename(FilenameSizeBytes, OutBuffer, OutBytesWritten);
    }

    EOS_DECLARE_FUNC(EOS_EResult) EOS_PlayerDataStorageFileTransferRequest_CancelRequest(EOS_HPlayerDataStorageFileTransferRequest Handle)
    {
        if (Handle == nullptr)
            return EOS_EResult::EOS_InvalidParameters;

        auto pInst = reinterpret_cast<EOSSDK_PlayerDataStorageFileTransferRequest*>(Handle);
        return pInst->CancelRequest();
    }

    EOS_DECLARE_FUNC(void) EOS_PlayerDataStorageFileTransferRequest_Release(EOS_HPlayerDataStorageFileTransferRequest Handle)
    {
        if (Handle == nullptr)
            return;

        auto pInst = reinterpret_cast<EOSSDK_PlayerDataStorageFileTransferRequest*>(Handle);
        pInst->Release();
    }

    EOS_DECLARE_FUNC(void) EOS_PlayerDataStorage_FileMetadata_Release(EOS_PlayerDataStorage_FileMetadata* FileMetadata)
    {
        if (FileMetadata == nullptr)
            return;

        delete[] FileMetadata->Filename;
        delete[] FileMetadata->MD5Hash;
        delete FileMetadata;
    }

    EOS_DECLARE_FUNC(void) EOS_PlayerDataStorage_DeleteCache(EOS_HPlayerDataStorage Handle, const EOS_PlayerDataStorage_DeleteCacheOptions* Options, void* ClientData, const EOS_PlayerDataStorage_OnDeleteCacheCompleteCallback CompletionDelegate) { if(CompletionDelegate) { EOS_PlayerDataStorage_DeleteCacheCallbackInfo info; memset(&info, 0, sizeof(info)); info.ResultCode = EOS_EResult::EOS_Success; info.ClientData = ClientData; info.LocalUserId = Options->LocalUserId; CompletionDelegate(&info); } }
    EOS_DECLARE_FUNC(void) EOS_PlayerDataStorage_DuplicateFile(EOS_HPlayerDataStorage Handle, const EOS_PlayerDataStorage_DuplicateFileOptions* Options, void* ClientData, const EOS_PlayerDataStorage_OnDuplicateFileCompleteCallback CompletionDelegate) { if(CompletionDelegate) { EOS_PlayerDataStorage_DuplicateFileCallbackInfo info; memset(&info, 0, sizeof(info)); info.ResultCode = EOS_EResult::EOS_Success; info.ClientData = ClientData; info.LocalUserId = Options->LocalUserId; CompletionDelegate(&info); } }

}
