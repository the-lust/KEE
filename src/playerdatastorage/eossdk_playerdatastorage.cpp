#include "eossdk_playerdatastorage.h"
#include "eossdk_platform.h"
#include "settings.h"

namespace sdk
{

EOSSDK_PlayerDataStorage::EOSSDK_PlayerDataStorage()
{
    remote_directory = FileManager::join(Settings::Inst().save_path, "remote");

    APP_LOG(Log::LogLevel::INFO, "PlayerDataStorage files will be search in %s", FileManager::canonical_path(remote_directory).c_str());

    GetCB_Manager().register_callbacks(this);
    GetCB_Manager().register_frame(this);
}

EOSSDK_PlayerDataStorage::~EOSSDK_PlayerDataStorage()
{
    GetCB_Manager().unregister_frame(this);
    GetCB_Manager().unregister_callbacks(this);
}

bool EOSSDK_PlayerDataStorage::get_metadata(string const& filename)
{
    string file_path(FileManager::join(remote_directory, FileManager::clean_path(filename)));
    std::ifstream in_file = FileManager::open_read(file_path);
    if (in_file)
    {
        auto& metadata = m_files_cache[filename];
        in_file.seekg(0, std::ios::end);
        metadata.file_size = in_file.tellg();
        in_file.seekg(0, std::ios::beg);

        string buffer(metadata.file_size, '\0');
        in_file.read(&buffer[0], metadata.file_size);

        metadata.md5sum = std::move(md5(buffer));

        metadata.file_path = std::move(file_path);

        return true;
    }
    
    auto it = m_files_cache.find(filename);
    if (it != m_files_cache.end())
    {
        m_files_cache.erase(it);
    }

    return false;
}

void EOSSDK_PlayerDataStorage::QueryFile(const EOS_PlayerDataStorage_QueryFileOptions* QueryFileOptions, void* ClientData, const EOS_PlayerDataStorage_OnQueryFileCompleteCallback CompletionCallback)
{
    TRACE_FUNC();
    lock _(GLOBAL_MUTEX);

    if (CompletionCallback == nullptr)
        return;

    pFrameResult_t res(new FrameResult);
    EOS_PlayerDataStorage_QueryFileCallbackInfo& qfci = res->CreateCallback<EOS_PlayerDataStorage_QueryFileCallbackInfo>((CallbackFunc)CompletionCallback);

    qfci.ClientData = ClientData;
    qfci.LocalUserId = GetEOS_Connect().get_myself()->first;

    if (QueryFileOptions == nullptr)
    {
        qfci.ResultCode = EOS_EResult::EOS_InvalidParameters;
    }
    else
    {
        std::vector<string> files(std::move(FileManager::list_files(remote_directory, true)));

        auto it = std::find(files.begin(), files.end(), string(QueryFileOptions->Filename));

        if (it == files.end())
        {
            qfci.ResultCode = EOS_EResult::EOS_NotFound;
        }
        else
        {
            std::replace(it->begin(), it->end(), '\\', '/');
            get_metadata(*it);
            qfci.ResultCode = EOS_EResult::EOS_Success;
        }
    }

    SE_CB_END(this);

}

void EOSSDK_PlayerDataStorage::QueryFileList(const EOS_PlayerDataStorage_QueryFileListOptions* QueryFileListOptions, void* ClientData, const EOS_PlayerDataStorage_OnQueryFileListCompleteCallback CompletionCallback)
{
    TRACE_FUNC();
    lock _(GLOBAL_MUTEX);

    if (CompletionCallback == nullptr)
        return;

    pFrameResult_t res(new FrameResult);
    EOS_PlayerDataStorage_QueryFileListCallbackInfo& qflci = res->CreateCallback<EOS_PlayerDataStorage_QueryFileListCallbackInfo>((CallbackFunc)CompletionCallback);

    qflci.ClientData = ClientData;
    qflci.LocalUserId = GetEOS_Connect().get_myself()->first;

    if (QueryFileListOptions == nullptr)
    {
        qflci.FileCount = 0;
        qflci.ResultCode = EOS_EResult::EOS_InvalidParameters;
    }
    else
    {
        std::vector<string> files(std::move(FileManager::list_files(remote_directory, true)));

        m_files_cache.clear();
        for (auto& file_name : files)
        {
            std::replace(file_name.begin(), file_name.end(), '\\', '/');
            get_metadata(file_name);
        }

        qflci.FileCount = m_files_cache.size();
        qflci.ResultCode = EOS_EResult::EOS_Success;
    }

    SE_CB_END(this);
}

EOS_EResult EOSSDK_PlayerDataStorage::CopyFileMetadataByFilename(const EOS_PlayerDataStorage_CopyFileMetadataByFilenameOptions* CopyFileMetadataOptions, EOS_PlayerDataStorage_FileMetadata** OutMetadata)
{
    TRACE_FUNC();
    lock _(GLOBAL_MUTEX);

    if (CopyFileMetadataOptions == nullptr || CopyFileMetadataOptions->Filename == nullptr || OutMetadata == nullptr)
    {
        set_nullptr(OutMetadata);
        return EOS_EResult::EOS_InvalidParameters;
    }

    auto it = m_files_cache.find(CopyFileMetadataOptions->Filename);
    if (it == m_files_cache.end())
    {
        set_nullptr(OutMetadata);
        return EOS_EResult::EOS_NotFound;
    }
    else
    {
        EOS_PlayerDataStorage_FileMetadata* metadata = new EOS_PlayerDataStorage_FileMetadata;

        metadata->ApiVersion = EOS_PLAYERDATASTORAGE_FILEMETADATA_API_LATEST;
        {
            size_t len = it->first.length() + 1;
            char* str = new char[len];
            strncpy(str, it->first.c_str(), len);
            metadata->Filename = str;
        }
        {
            size_t len = it->second.md5sum.length() + 1;
            char* str = new char[len];
            strncpy(str, it->second.md5sum.c_str(), len);
            metadata->MD5Hash = str;
        }
        metadata->FileSizeBytes = it->second.file_size;

        *OutMetadata = metadata;
    }

    return EOS_EResult::EOS_Success;
}

EOS_EResult EOSSDK_PlayerDataStorage::GetFileMetadataCount(const EOS_PlayerDataStorage_GetFileMetadataCountOptions* GetFileMetadataCountOptions, int32_t* OutFileMetadataCount)
{
    TRACE_FUNC();
    lock _(GLOBAL_MUTEX);

    if (GetFileMetadataCountOptions == nullptr || OutFileMetadataCount == nullptr)
    {
        if (OutFileMetadataCount != nullptr)
            *OutFileMetadataCount = 0;
        
        return EOS_EResult::EOS_InvalidParameters;
    }

    *OutFileMetadataCount = m_files_cache.size();
    return EOS_EResult::EOS_Success;
}

EOS_EResult EOSSDK_PlayerDataStorage::CopyFileMetadataAtIndex(const EOS_PlayerDataStorage_CopyFileMetadataAtIndexOptions* CopyFileMetadataOptions, EOS_PlayerDataStorage_FileMetadata** OutMetadata)
{
    TRACE_FUNC();
    lock _(GLOBAL_MUTEX);

    if (CopyFileMetadataOptions == nullptr || CopyFileMetadataOptions->Index >= m_files_cache.size() || OutMetadata == nullptr)
    {
        set_nullptr(OutMetadata);
        return EOS_EResult::EOS_InvalidParameters;
    }

    auto it = m_files_cache.begin();
    std::advance(it, CopyFileMetadataOptions->Index);

    EOS_PlayerDataStorage_FileMetadata* metadata = new EOS_PlayerDataStorage_FileMetadata;

    metadata->ApiVersion = EOS_PLAYERDATASTORAGE_FILEMETADATA_API_LATEST;
    {
        size_t len = it->first.length() + 1;
        char* str = new char[len];
        strncpy(str, it->first.c_str(), len);
        metadata->Filename = str;
    }
    {
        size_t len = it->second.md5sum.length() + 1;
        char* str = new char[len];
        strncpy(str, it->second.md5sum.c_str(), len);
        metadata->MD5Hash = str;
    }
    metadata->FileSizeBytes = it->second.file_size;

    *OutMetadata = metadata;

    return EOS_EResult::EOS_Success;
}

void EOSSDK_PlayerDataStorage::DuplicateFile(const EOS_PlayerDataStorage_DuplicateFileOptions* DuplicateOptions, void* ClientData, const EOS_PlayerDataStorage_OnDuplicateFileCompleteCallback CompletionCallback)
{
    TRACE_FUNC();
    lock _(GLOBAL_MUTEX);

    if (CompletionCallback == nullptr)
        return;

    pFrameResult_t res(new FrameResult);
    EOS_PlayerDataStorage_DuplicateFileCallbackInfo& dfci = res->CreateCallback<EOS_PlayerDataStorage_DuplicateFileCallbackInfo>((CallbackFunc)CompletionCallback);

    dfci.ClientData = ClientData;
    dfci.LocalUserId = GetEOS_Connect().get_myself()->first;
    
    if (DuplicateOptions == nullptr || DuplicateOptions->SourceFilename == nullptr || DuplicateOptions->DestinationFilename == nullptr)
    {
        dfci.ResultCode = EOS_EResult::EOS_InvalidParameters;
    }
    else
    {
        string src_file(FileManager::join(remote_directory, FileManager::clean_path(DuplicateOptions->SourceFilename)));
        std::ifstream in_file = FileManager::open_read(src_file);

        if (in_file)
        {
            string dst_file(FileManager::join(remote_directory, FileManager::clean_path(DuplicateOptions->DestinationFilename)));
            std::ofstream out_file = FileManager::open_write(dst_file);
            if (out_file)
            {
                char* buff = new char[1024 * 1024];

                while(in_file.read(buff, 1024 * 1024).good())
                {
                    out_file.write(buff, in_file.gcount());
                }

                delete[]buff;

                auto& src_cache = m_files_cache[DuplicateOptions->SourceFilename];
                auto& dst_cache = m_files_cache[DuplicateOptions->DestinationFilename];
                dst_cache.file_path = dst_file;
                dst_cache.file_size = src_cache.file_size;
                dst_cache.md5sum = src_cache.md5sum;
            }
            else
            {
                dfci.ResultCode = EOS_EResult::EOS_UnexpectedError;
            }
        }
        else
        {
            dfci.ResultCode = EOS_EResult::EOS_NotFound;
        }
    }

    SE_CB_END(this);
}

void EOSSDK_PlayerDataStorage::DeleteFile(const EOS_PlayerDataStorage_DeleteFileOptions* DeleteOptions, void* ClientData, const EOS_PlayerDataStorage_OnDeleteFileCompleteCallback CompletionCallback)
{
    TRACE_FUNC();
    lock _(GLOBAL_MUTEX);

    if (CompletionCallback == nullptr)
        return;

    pFrameResult_t res(new FrameResult);
    EOS_PlayerDataStorage_DeleteFileCallbackInfo& dfci = res->CreateCallback<EOS_PlayerDataStorage_DeleteFileCallbackInfo>((CallbackFunc)CompletionCallback);

    dfci.ClientData = ClientData;
    dfci.LocalUserId = GetEOS_Connect().get_myself()->first;
    
    if (DeleteOptions == nullptr || DeleteOptions->Filename == nullptr)
    {
        dfci.ResultCode = EOS_EResult::EOS_InvalidParameters;
    }
    else
    {
        auto it = m_files_cache.find(DeleteOptions->Filename);
        if (it != m_files_cache.end())
        {
            m_files_cache.erase(it);
        }

        if (FileManager::delete_file(FileManager::join(remote_directory, DeleteOptions->Filename)))
        {
            dfci.ResultCode = EOS_EResult::EOS_Success;
        }
        else
        {
            dfci.ResultCode = EOS_EResult::EOS_NotFound;
        }
    }

    SE_CB_END(this);
}

EOS_HPlayerDataStorageFileTransferRequest EOSSDK_PlayerDataStorage::ReadFile(const EOS_PlayerDataStorage_ReadFileOptions* ReadOptions, void* ClientData, const EOS_PlayerDataStorage_OnReadFileCompleteCallback CompletionCallback)
{
    TRACE_FUNC();
    lock _(GLOBAL_MUTEX);

    if (CompletionCallback == nullptr)
        return nullptr;

    EOS_HPlayerDataStorageFileTransferRequest func_result = nullptr;
    pFrameResult_t res(new FrameResult);

    EOS_PlayerDataStorage_ReadFileCallbackInfo& rfci = res->CreateCallback<EOS_PlayerDataStorage_ReadFileCallbackInfo>((CallbackFunc)CompletionCallback);
    rfci.ClientData = ClientData;
    rfci.LocalUserId = Settings::Inst().productuserid;

    if (ReadOptions == nullptr || ReadOptions->Filename == nullptr)
    {
        char* str = new char[1];
        *str = '\0';
        rfci.Filename = str;
    }
    else
    {
        size_t len = strlen(ReadOptions->Filename) + 1;
        char* str = new char[len];
        strncpy(str, ReadOptions->Filename, len);
        rfci.Filename = str;
    }

    if (ReadOptions == nullptr || ReadOptions->Filename == nullptr || ReadOptions->ReadFileDataCallback == nullptr)
    {
        rfci.ResultCode = EOS_EResult::EOS_InvalidParameters;
        res->done = true;
    }
    else
    {
        size_t len = strlen(ReadOptions->Filename) + 1;
        char* str = new char[len];
        strncpy(str, ReadOptions->Filename, len);
        rfci.Filename = str;

        string file_path(FileManager::join(remote_directory, FileManager::clean_path(rfci.Filename)));
        if (FileManager::is_file(file_path))
        {
            APP_LOG(Log::LogLevel::INFO, "Start Reading file: %s", file_path.c_str());

            EOSSDK_PlayerDataStorageFileTransferRequest*& res_obj = m_transferts[res];
            res_obj = new EOSSDK_PlayerDataStorageFileTransferRequest;
            res_obj->set_read_transfert(remote_directory, ReadOptions);

            func_result = reinterpret_cast<EOS_HPlayerDataStorageFileTransferRequest>(res_obj);
        }
        else
        {
            APP_LOG(Log::LogLevel::INFO, "File not found: %s", file_path.c_str());
            rfci.ResultCode = EOS_EResult::EOS_NotFound;

            res->done = true;
        }
    }

    GetCB_Manager().add_callback(this, res);
    return func_result;
}

EOS_HPlayerDataStorageFileTransferRequest EOSSDK_PlayerDataStorage::WriteFile(const EOS_PlayerDataStorage_WriteFileOptions* WriteOptions, void* ClientData, const EOS_PlayerDataStorage_OnWriteFileCompleteCallback CompletionCallback)
{
    TRACE_FUNC();
    lock _(GLOBAL_MUTEX);

    if (CompletionCallback == nullptr)
    {
        return nullptr;
    }

    EOS_HPlayerDataStorageFileTransferRequest func_result = nullptr;
    pFrameResult_t res(new FrameResult);
    EOS_PlayerDataStorage_WriteFileCallbackInfo& wfci = res->CreateCallback<EOS_PlayerDataStorage_WriteFileCallbackInfo>((CallbackFunc)CompletionCallback);

    wfci.ClientData = ClientData;
    wfci.LocalUserId = GetEOS_Connect().get_myself()->first;

    if (WriteOptions == nullptr || WriteOptions->Filename == nullptr)
    {
        char* str = new char[1];
        *str = '\0';
        wfci.Filename = str;
    }
    else
    {
        size_t len = strlen(WriteOptions->Filename) + 1;
        char* str = new char[len];
        strncpy(str, WriteOptions->Filename, len);
        wfci.Filename = str;
    }

    if (WriteOptions == nullptr || WriteOptions->Filename == nullptr || WriteOptions->WriteFileDataCallback == nullptr)
    {
        wfci.ResultCode = EOS_EResult::EOS_InvalidParameters;
        res->done = true;
    }
    else
    {
        EOSSDK_PlayerDataStorageFileTransferRequest*& res_obj = m_transferts[res];
        res_obj = new EOSSDK_PlayerDataStorageFileTransferRequest;
        res_obj->set_write_transfert(remote_directory, WriteOptions);

        APP_LOG(Log::LogLevel::INFO, "Start Writing file: %s", res_obj->m_file_name.c_str());

        func_result = reinterpret_cast<EOS_HPlayerDataStorageFileTransferRequest>(res_obj);
    }

    GetCB_Manager().add_callback(this, res);
    return func_result;
}

bool EOSSDK_PlayerDataStorage::CBRunFrame()
{
    lock _(GLOBAL_MUTEX);

    for (auto it = m_transferts.begin(); it != m_transferts.end();)
    {
        if (it->second->released())
        {
            delete it->second;
            it = m_transferts.erase(it);
        }
        else
        {
            ++it;
        }
    }

    return true;
}

bool EOSSDK_PlayerDataStorage::RunCallbacks(pFrameResult_t res)
{
    lock _(GLOBAL_MUTEX);

    switch (res->ICallback())
    {
        case EOS_PlayerDataStorage_ReadFileCallbackInfo::k_iCallback:
        {
            EOS_PlayerDataStorage_ReadFileCallbackInfo& callback = res->GetCallback<EOS_PlayerDataStorage_ReadFileCallbackInfo>();
            EOSSDK_PlayerDataStorageFileTransferRequest& transfert = *m_transferts[res];

            if (transfert.canceled())
            {
                callback.ResultCode = EOS_EResult::EOS_Canceled;
                transfert.m_done = true;
                res->done = true;
            }
            else
            {
                EOS_PlayerDataStorage_ReadFileDataCallbackInfo rfdci;
                rfdci.ClientData = callback.ClientData;
                rfdci.Filename = callback.Filename;
                rfdci.LocalUserId = callback.LocalUserId;
                rfdci.TotalFileSizeBytes = transfert.m_file_size;

                transfert.m_input_file.read((char*)&transfert.m_file_buffer[0], transfert.m_chunk_size);
                size_t read_len = transfert.m_input_file.gcount();

                rfdci.bIsLastChunk = (read_len != transfert.m_chunk_size ? EOS_TRUE : EOS_FALSE);
                if (rfdci.bIsLastChunk == EOS_TRUE)
                {
                    transfert.m_done = true;
                    res->done = true;
                    callback.ResultCode = EOS_EResult::EOS_Success;
                }

                rfdci.DataChunk = &transfert.m_file_buffer[0];
                rfdci.DataChunkLengthBytes = read_len;
                switch (transfert.m_read_callback(&rfdci))
                {
                    case EOS_PlayerDataStorage_EReadResult::EOS_RR_FailRequest:
                    {
                        callback.ResultCode = EOS_EResult::EOS_PlayerDataStorage_UserErrorFromDataCallback;
                        transfert.m_done = true;
                        res->done = true;
                    }
                    break;

                    case EOS_PlayerDataStorage_EReadResult::EOS_RR_CancelRequest:
                    {
                        callback.ResultCode = EOS_EResult::EOS_Canceled;
                        transfert.m_canceled = true;
                        transfert.m_done = true;
                        res->done = true;
                    }
                    break;

                    case EOS_PlayerDataStorage_EReadResult::EOS_RR_ContinueReading:
                    {
                    }
                    break;
                }
            }
        }
        break;

        case EOS_PlayerDataStorage_WriteFileCallbackInfo::k_iCallback:
        {
            EOS_PlayerDataStorage_WriteFileCallbackInfo& callback = res->GetCallback<EOS_PlayerDataStorage_WriteFileCallbackInfo>();
            EOSSDK_PlayerDataStorageFileTransferRequest& transfert = *m_transferts[res];

            if (transfert.m_canceled)
            {
                callback.ResultCode = EOS_EResult::EOS_Canceled;
                transfert.m_done = true;
                res->done = true;
            }
            else
            {
                size_t offset = transfert.m_file_buffer.size();
                transfert.m_file_buffer.resize(offset + transfert.m_chunk_size * 2);

                EOS_PlayerDataStorage_WriteFileDataCallbackInfo wfdci;
                wfdci.ClientData = callback.ClientData;
                wfdci.Filename = callback.Filename;
                wfdci.LocalUserId = callback.LocalUserId;

                uint32_t buff_len = transfert.m_chunk_size * 2;
                wfdci.DataBufferLengthBytes = buff_len;

                switch (transfert.m_write_callback(&wfdci, &transfert.m_file_buffer[offset], &buff_len))
                {
                    case EOS_PlayerDataStorage_EWriteResult::EOS_WR_FailRequest:
                    {
                        callback.ResultCode = EOS_EResult::EOS_PlayerDataStorage_UserErrorFromDataCallback;
                        transfert.m_done = true;
                        res->done = true;
                    }
                    break;

                    case EOS_PlayerDataStorage_EWriteResult::EOS_WR_CancelRequest:
                    {
                        transfert.m_canceled = true;
                        transfert.m_done = true;
                        callback.ResultCode = EOS_EResult::EOS_Canceled;
                        res->done = true;
                    }
                    break;

                    case EOS_PlayerDataStorage_EWriteResult::EOS_WR_ContinueWriting:
                    {
                        transfert.m_file_buffer.resize(offset + buff_len);
                    }
                    break;

                    case EOS_PlayerDataStorage_EWriteResult::EOS_WR_CompleteRequest:
                    {
                        transfert.m_file_buffer.resize(offset + buff_len);
                        transfert.m_done = true;
                        std::ofstream out_file = FileManager::open_write(FileManager::join(remote_directory, FileManager::clean_path(transfert.m_file_name)));
                        if (out_file)
                        {
                            out_file.write((const char*)transfert.m_file_buffer.data(), transfert.m_file_buffer.size());
                            out_file.close();
                            if(get_metadata(transfert.m_file_name))
                            {
                                callback.ResultCode = EOS_EResult::EOS_Success;
                            }
                            else
                            {
                                FileManager::delete_file(FileManager::join(remote_directory, transfert.m_file_name));
                                callback.ResultCode = EOS_EResult::EOS_UnexpectedError;
                            }
                        }
                        else
                        {
                            callback.ResultCode = EOS_EResult::EOS_UnexpectedError;
                        }
                        res->done = true;
                    }
                }
            }
        }
        break;
    }

    return res->done;
}

void EOSSDK_PlayerDataStorage::FreeCallback(pFrameResult_t res)
{
    lock _(GLOBAL_MUTEX);

    switch (res->ICallback())
    {

        case EOS_PlayerDataStorage_ReadFileCallbackInfo::k_iCallback:
        {
            EOS_PlayerDataStorage_ReadFileCallbackInfo& callback = res->GetCallback<EOS_PlayerDataStorage_ReadFileCallbackInfo>();
            
            delete[] callback.Filename;
        }
        break;

        case EOS_PlayerDataStorage_WriteFileCallbackInfo::k_iCallback:
        {
            EOS_PlayerDataStorage_WriteFileCallbackInfo& callback = res->GetCallback<EOS_PlayerDataStorage_WriteFileCallbackInfo>();
            
            delete[] callback.Filename;
        }
        break;

    }
}

}