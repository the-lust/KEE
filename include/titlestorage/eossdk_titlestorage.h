#pragma once

#include "common_includes.h"
#include "callback_manager.h"

namespace sdk
{
    class EOSSDK_TitleStorageFileTransferRequest
    {
        friend class EOSSDK_TitleStorage;

        std::mutex m_local_mutex;

        string m_file_name;

        bool m_done;
        bool m_canceled;
        bool m_released;

        EOS_TitleStorage_OnReadFileDataCallback  m_read_callback;
        EOS_TitleStorage_OnFileTransferProgressCallback m_progress_callback;
        uint32_t m_chunk_size;
        uint32_t m_file_size;
        uint32_t m_bytes_transferred;  

        std::vector<uint8_t> m_file_buffer;
        std::ifstream m_input_file;

        void set_read_transfert(const string& base_path, const EOS_TitleStorage_ReadFileOptions* ReadOptions);

    public:
        EOSSDK_TitleStorageFileTransferRequest();
        ~EOSSDK_TitleStorageFileTransferRequest();

        bool canceled();
        bool released();

        EOS_EResult GetFileRequestState();
        EOS_EResult GetFilename(uint32_t FilenameStringBufferSizeBytes, char* OutStringBuffer, int32_t* OutStringLength);
        EOS_EResult CancelRequest();
        void Release();
    };

    class EOSSDK_TitleStorage :
        public IRunCallback
    {
        struct file_metadata_t
        {
            string file_path;
            size_t file_size;
            string md5sum;
        };

        std::unordered_map<pFrameResult_t, EOSSDK_TitleStorageFileTransferRequest*> m_transferts;
        nlohmann::fifo_map<string, file_metadata_t> m_files_cache;

        bool get_metadata(string const& filename);

    public:
        EOSSDK_TitleStorage();
        ~EOSSDK_TitleStorage();

        string title_directory;

        virtual bool CBRunFrame();
        virtual bool RunCallbacks(pFrameResult_t res);
        virtual void FreeCallback(pFrameResult_t res);

        void                                 QueryFile(const EOS_TitleStorage_QueryFileOptions* Options, void* ClientData, const EOS_TitleStorage_OnQueryFileCompleteCallback CompletionCallback);
        void                                 QueryFileList(const EOS_TitleStorage_QueryFileListOptions* Options, void* ClientData, const EOS_TitleStorage_OnQueryFileListCompleteCallback CompletionCallback);
        EOS_EResult                          CopyFileMetadataByFilename(const EOS_TitleStorage_CopyFileMetadataByFilenameOptions* Options, EOS_TitleStorage_FileMetadata** OutMetadata);
        uint32_t                             GetFileMetadataCount(const EOS_TitleStorage_GetFileMetadataCountOptions* Options);
        EOS_EResult                          CopyFileMetadataAtIndex(const EOS_TitleStorage_CopyFileMetadataAtIndexOptions* Options, EOS_TitleStorage_FileMetadata** OutMetadata);
        EOS_HTitleStorageFileTransferRequest ReadFile(const EOS_TitleStorage_ReadFileOptions* Options, void* ClientData, const EOS_TitleStorage_OnReadFileCompleteCallback CompletionCallback);
        EOS_EResult                          DeleteCache(const EOS_TitleStorage_DeleteCacheOptions* Options, void* ClientData, const EOS_TitleStorage_OnDeleteCacheCompleteCallback CompletionCallback);
    };
}
