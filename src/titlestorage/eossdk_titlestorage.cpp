#include "eossdk_titlestorage.h"
#include "eossdk_platform.h"
#include "eos_client_api.h"
#include "settings.h"

namespace sdk
{

EOSSDK_TitleStorage::EOSSDK_TitleStorage()
{
    title_directory = FileManager::join(Settings::Inst().save_path, "titlestorage");
    
    APP_LOG(Log::LogLevel::INFO, "TitleStorage files will be search in %s", FileManager::clean_path(title_directory).c_str());

    GetCB_Manager().register_callbacks(this);
    GetCB_Manager().register_frame(this);
}

EOSSDK_TitleStorage::~EOSSDK_TitleStorage()
{
    GetCB_Manager().unregister_frame(this);
    GetCB_Manager().unregister_callbacks(this);
}

bool EOSSDK_TitleStorage::get_metadata(string const& filename)
{
    string file_path(FileManager::join(title_directory, FileManager::clean_path(filename)));
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

void EOSSDK_TitleStorage::QueryFile(const EOS_TitleStorage_QueryFileOptions* Options, void* ClientData, const EOS_TitleStorage_OnQueryFileCompleteCallback CompletionCallback)
{
    TRACE_FUNC();
    lock _(GLOBAL_MUTEX);

    if (CompletionCallback == nullptr)
        return;

    pFrameResult_t res(new FrameResult);
    EOS_TitleStorage_QueryFileCallbackInfo& qfci = res->CreateCallback<EOS_TitleStorage_QueryFileCallbackInfo>((CallbackFunc)CompletionCallback);

    qfci.ClientData = ClientData;
    qfci.LocalUserId = GetEOS_Connect().get_myself()->first;

    if (Options == nullptr || Options->Filename == nullptr)
    {
        qfci.ResultCode = EOS_EResult::EOS_InvalidParameters;
    }
    else
    {
        std::vector<string> files(std::move(FileManager::list_files(title_directory, true)));

        auto it = std::find(files.begin(), files.end(), string(Options->Filename));

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

void EOSSDK_TitleStorage::QueryFileList(const EOS_TitleStorage_QueryFileListOptions* Options, void* ClientData, const EOS_TitleStorage_OnQueryFileListCompleteCallback CompletionCallback)
{
    TRACE_FUNC();
    lock _(GLOBAL_MUTEX);

    if (CompletionCallback == nullptr)
        return;

    pFrameResult_t res(new FrameResult);
    EOS_TitleStorage_QueryFileListCallbackInfo& qflci = res->CreateCallback<EOS_TitleStorage_QueryFileListCallbackInfo>((CallbackFunc)CompletionCallback);

    qflci.ClientData = ClientData;
    qflci.LocalUserId = GetEOS_Connect().get_myself()->first;

    if (Options == nullptr)
    {
        qflci.FileCount = 0;
        qflci.ResultCode = EOS_EResult::EOS_InvalidParameters;
    }
    else
    {
        std::vector<string> files(std::move(FileManager::list_files(title_directory, true)));

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

EOS_EResult EOSSDK_TitleStorage::CopyFileMetadataByFilename(const EOS_TitleStorage_CopyFileMetadataByFilenameOptions* Options, EOS_TitleStorage_FileMetadata** OutMetadata)
{
    TRACE_FUNC();
    lock _(GLOBAL_MUTEX);

    if (Options == nullptr || Options->Filename == nullptr || OutMetadata == nullptr)
    {
        set_nullptr(OutMetadata);
        return EOS_EResult::EOS_InvalidParameters;
    }

    auto it = m_files_cache.find(Options->Filename);
    if (it == m_files_cache.end())
    {
        set_nullptr(OutMetadata);
        return EOS_EResult::EOS_NotFound;
    }
    else
    {
        EOS_TitleStorage_FileMetadata* metadata = new EOS_TitleStorage_FileMetadata;
        
        metadata->ApiVersion = EOS_TITLESTORAGE_FILEMETADATA_API_LATEST;
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

uint32_t EOSSDK_TitleStorage::GetFileMetadataCount(const EOS_TitleStorage_GetFileMetadataCountOptions* Options)
{
    TRACE_FUNC();
    lock _(GLOBAL_MUTEX);

    if (Options == nullptr)
    {
        return 0;
    }

    return m_files_cache.size();
}

EOS_EResult EOSSDK_TitleStorage::CopyFileMetadataAtIndex(const EOS_TitleStorage_CopyFileMetadataAtIndexOptions* Options, EOS_TitleStorage_FileMetadata** OutMetadata)
{
    TRACE_FUNC();
    lock _(GLOBAL_MUTEX);

    if (Options == nullptr || Options->Index >= m_files_cache.size() || OutMetadata == nullptr)
    {
        set_nullptr(OutMetadata);
        return EOS_EResult::EOS_InvalidParameters;
    }

    auto it = m_files_cache.begin();
    std::advance(it, Options->Index);

    EOS_TitleStorage_FileMetadata* metadata = new EOS_TitleStorage_FileMetadata;

    metadata->ApiVersion = EOS_TITLESTORAGE_FILEMETADATA_API_LATEST;
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

EOS_HTitleStorageFileTransferRequest EOSSDK_TitleStorage::ReadFile(const EOS_TitleStorage_ReadFileOptions* Options, void* ClientData, const EOS_TitleStorage_OnReadFileCompleteCallback CompletionCallback)
{
    TRACE_FUNC();
    lock _(GLOBAL_MUTEX);

    if (CompletionCallback == nullptr)
        return nullptr;

    EOS_HTitleStorageFileTransferRequest func_result = nullptr;
    pFrameResult_t res(new FrameResult);

    EOS_TitleStorage_ReadFileCallbackInfo& rfci = res->CreateCallback<EOS_TitleStorage_ReadFileCallbackInfo>((CallbackFunc)CompletionCallback);
    rfci.ClientData = ClientData;
    rfci.LocalUserId = Settings::Inst().productuserid;

    if (Options == nullptr || Options->Filename == nullptr)
    {
        char* str = new char[1];
        *str = '\0';
        rfci.Filename = str;
    }
    else
    {
        size_t len = strlen(Options->Filename) + 1;
        char* str = new char[len];
        strncpy(str, Options->Filename, len);
        rfci.Filename = str;
    }

    if (Options == nullptr || Options->Filename == nullptr || Options->ReadFileDataCallback == nullptr)
    {
        rfci.ResultCode = EOS_EResult::EOS_InvalidParameters;
        res->done = true;
    }
    else
    {
        size_t len = strlen(Options->Filename) + 1;
        char* str = new char[len];
        strncpy(str, Options->Filename, len);
        rfci.Filename = str;

        string file_path(FileManager::join(title_directory, FileManager::clean_path(rfci.Filename)));
        if (FileManager::exists(file_path))
        {
            APP_LOG(Log::LogLevel::INFO, "Start Reading file: %s", file_path.c_str());

            EOSSDK_TitleStorageFileTransferRequest*& res_obj = m_transferts[res];
            res_obj = new EOSSDK_TitleStorageFileTransferRequest;
            res_obj->set_read_transfert(title_directory, Options);

            func_result = reinterpret_cast<EOS_HTitleStorageFileTransferRequest>(res_obj);
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

EOS_EResult EOSSDK_TitleStorage::DeleteCache(const EOS_TitleStorage_DeleteCacheOptions* Options, void* ClientData, const EOS_TitleStorage_OnDeleteCacheCompleteCallback CompletionCallback)
{
    TRACE_FUNC();
    lock _(GLOBAL_MUTEX);

    if (CompletionCallback != nullptr)
    {
        pFrameResult_t res(new FrameResult);
        EOS_TitleStorage_DeleteCacheCallbackInfo& dcci = res->CreateCallback<EOS_TitleStorage_DeleteCacheCallbackInfo>((CallbackFunc)CompletionCallback);

        dcci.ClientData = ClientData;
        dcci.LocalUserId = Settings::Inst().productuserid;
        dcci.ResultCode = EOS_EResult::EOS_Success;

        SE_CB_END(this);
    }

    return EOS_EResult::EOS_Success;
}

bool EOSSDK_TitleStorage::CBRunFrame()
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

bool EOSSDK_TitleStorage::RunCallbacks(pFrameResult_t res)
{
    lock _(GLOBAL_MUTEX);

    switch (res->ICallback())
    {
        case EOS_TitleStorage_ReadFileCallbackInfo::k_iCallback:
        {
            EOS_TitleStorage_ReadFileCallbackInfo& callback = res->GetCallback<EOS_TitleStorage_ReadFileCallbackInfo>();
            EOSSDK_TitleStorageFileTransferRequest& transfert = *m_transferts[res];

            if (transfert.canceled())
            {
                callback.ResultCode = EOS_EResult::EOS_Canceled;
                transfert.m_done = true;
                res->done = true;
            }
            else
            {
                EOS_TitleStorage_ReadFileDataCallbackInfo rfdci;
                rfdci.ClientData = callback.ClientData;
                rfdci.Filename = callback.Filename;
                rfdci.LocalUserId = callback.LocalUserId;
                rfdci.TotalFileSizeBytes = transfert.m_file_size;

                transfert.m_input_file.read((char*)&transfert.m_file_buffer[0], transfert.m_chunk_size);
                size_t read_len = transfert.m_input_file.gcount();
                transfert.m_bytes_transferred += static_cast<uint32_t>(read_len);

                if (transfert.m_progress_callback)
                {
                    EOS_TitleStorage_FileTransferProgressCallbackInfo prog{};
                    prog.ClientData         = callback.ClientData;
                    prog.LocalUserId        = callback.LocalUserId;
                    prog.Filename           = callback.Filename;
                    prog.BytesTransferred   = transfert.m_bytes_transferred;
                    prog.TotalFileSizeBytes = transfert.m_file_size;
                    transfert.m_progress_callback(&prog);
                }

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
                    case EOS_TitleStorage_EReadResult::EOS_TS_RR_FailRequest:
                    {
                        callback.ResultCode = EOS_EResult::EOS_PlayerDataStorage_UserErrorFromDataCallback;
                        transfert.m_done = true;
                        res->done = true;
                    }
                    break;

                    case EOS_TitleStorage_EReadResult::EOS_TS_RR_CancelRequest:
                    {
                        callback.ResultCode = EOS_EResult::EOS_Canceled;
                        transfert.m_canceled = true;
                        transfert.m_done = true;
                        res->done = true;
                    }
                    break;

                    case EOS_TitleStorage_EReadResult::EOS_TS_RR_ContinueReading:
                    {
                    }
                    break;
                }
            }
        }
        break;
    }

    return res->done;
}

void EOSSDK_TitleStorage::FreeCallback(pFrameResult_t res)
{
    lock _(GLOBAL_MUTEX);

    switch (res->ICallback())
    {

        case EOS_TitleStorage_ReadFileCallbackInfo::k_iCallback:
        {
            EOS_TitleStorage_ReadFileCallbackInfo& callback = res->GetCallback<EOS_TitleStorage_ReadFileCallbackInfo>();
            
            delete[] callback.Filename;
        }
        break;

    }
}

}