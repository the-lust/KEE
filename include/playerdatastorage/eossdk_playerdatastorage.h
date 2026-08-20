#pragma once

#include "common_includes.h"
#include "callback_manager.h"

#ifdef DeleteFile
#undef DeleteFile
#endif

namespace sdk
{
    class EOSSDK_PlayerDataStorageFileTransferRequest
    {
        friend class EOSSDK_PlayerDataStorage;

        std::mutex m_local_mutex;

        string m_file_name;

        bool m_done;
        bool m_canceled;
        bool m_released;

        union
        {
            EOS_PlayerDataStorage_OnReadFileDataCallback  m_read_callback;
            EOS_PlayerDataStorage_OnWriteFileDataCallback m_write_callback;
        };
        EOS_PlayerDataStorage_OnFileTransferProgressCallback m_progress_callback;
        uint32_t m_chunk_size;
        uint32_t m_file_size;

        std::vector<uint8_t> m_file_buffer;
        std::ifstream m_input_file;

        void set_read_transfert(const string& base_path, const EOS_PlayerDataStorage_ReadFileOptions* ReadOptions);
        void set_write_transfert(const string& base_path, const EOS_PlayerDataStorage_WriteFileOptions* WriteOptions);

    public:
        EOSSDK_PlayerDataStorageFileTransferRequest();
        ~EOSSDK_PlayerDataStorageFileTransferRequest();

        bool canceled();
        bool released();

        EOS_EResult GetFileRequestState();
        EOS_EResult GetFilename(uint32_t FilenameStringBufferSizeBytes, char* OutStringBuffer, int32_t* OutStringLength);
        EOS_EResult CancelRequest();
        void Release();
    };

    class EOSSDK_PlayerDataStorage :
        public IRunCallback
    {
        struct file_metadata_t
        {
            string file_path;
            size_t file_size;
            string md5sum;
        };

        std::unordered_map<pFrameResult_t, EOSSDK_PlayerDataStorageFileTransferRequest*> m_transferts;
        nlohmann::fifo_map<string, file_metadata_t> m_files_cache;

        bool get_metadata(string const& filename);

    public:
        string remote_directory;

        EOSSDK_PlayerDataStorage();
        ~EOSSDK_PlayerDataStorage();

        virtual bool CBRunFrame();
        virtual bool RunCallbacks(pFrameResult_t res);
        virtual void FreeCallback(pFrameResult_t res);

        void                                      QueryFile(const EOS_PlayerDataStorage_QueryFileOptions* QueryFileOptions, void* ClientData, const EOS_PlayerDataStorage_OnQueryFileCompleteCallback CompletionCallback);
        void                                      QueryFileList(const EOS_PlayerDataStorage_QueryFileListOptions* QueryFileListOptions, void* ClientData, const EOS_PlayerDataStorage_OnQueryFileListCompleteCallback CompletionCallback);
        EOS_EResult                               CopyFileMetadataByFilename(const EOS_PlayerDataStorage_CopyFileMetadataByFilenameOptions* CopyFileMetadataOptions, EOS_PlayerDataStorage_FileMetadata** OutMetadata);
        EOS_EResult                               GetFileMetadataCount(const EOS_PlayerDataStorage_GetFileMetadataCountOptions* GetFileMetadataCountOptions, int32_t* OutFileMetadataCount);
        EOS_EResult                               CopyFileMetadataAtIndex(const EOS_PlayerDataStorage_CopyFileMetadataAtIndexOptions* CopyFileMetadataOptions, EOS_PlayerDataStorage_FileMetadata** OutMetadata);
        void                                      DuplicateFile(const EOS_PlayerDataStorage_DuplicateFileOptions* DuplicateOptions, void* ClientData, const EOS_PlayerDataStorage_OnDuplicateFileCompleteCallback CompletionCallback);
        void                                      DeleteFile(const EOS_PlayerDataStorage_DeleteFileOptions* DeleteOptions, void* ClientData, const EOS_PlayerDataStorage_OnDeleteFileCompleteCallback CompletionCallback);
        EOS_HPlayerDataStorageFileTransferRequest ReadFile(const EOS_PlayerDataStorage_ReadFileOptions* ReadOptions, void* ClientData, const EOS_PlayerDataStorage_OnReadFileCompleteCallback CompletionCallback);
        EOS_HPlayerDataStorageFileTransferRequest WriteFile(const EOS_PlayerDataStorage_WriteFileOptions* WriteOptions, void* ClientData, const EOS_PlayerDataStorage_OnWriteFileCompleteCallback CompletionCallback);
    };
}