#include "eossdk_titlestorage.h"
#include "eossdk_platform.h"
#include "eos_client_api.h"
#include "settings.h"

namespace sdk
{

EOSSDK_TitleStorageFileTransferRequest::EOSSDK_TitleStorageFileTransferRequest():
    m_done(false),
    m_canceled(false),
    m_released(false)
{

}

EOSSDK_TitleStorageFileTransferRequest::~EOSSDK_TitleStorageFileTransferRequest()
{

}

void EOSSDK_TitleStorageFileTransferRequest::set_read_transfert(const string& base_path, const EOS_TitleStorage_ReadFileOptions* ReadOptions)
{
    std::lock_guard<std::mutex> m_lk(m_local_mutex);
    string file_path = FileManager::join(base_path, FileManager::clean_path(ReadOptions->Filename));

    m_read_callback = ReadOptions->ReadFileDataCallback;
    m_progress_callback = ReadOptions->FileTransferProgressCallback;
    m_chunk_size = ReadOptions->ReadChunkLengthBytes;
    m_file_name = ReadOptions->Filename;
    m_file_size = FileManager::file_size(file_path);
    m_bytes_transferred = 0;

    m_file_buffer.resize(m_chunk_size);
    m_input_file = FileManager::open_read(file_path);
}

bool EOSSDK_TitleStorageFileTransferRequest::canceled()
{
    std::lock_guard<std::mutex> m_lk(m_local_mutex);
    return m_canceled;
}

bool EOSSDK_TitleStorageFileTransferRequest::released()
{
    std::lock_guard<std::mutex> m_lk(m_local_mutex);
    return m_released;
}

EOS_EResult EOSSDK_TitleStorageFileTransferRequest::GetFileRequestState()
{
    TRACE_FUNC();
    std::lock_guard<std::mutex> m_lk(m_local_mutex);

    return (m_done ? EOS_EResult::EOS_Success : EOS_EResult::EOS_RequestInProgress);
}

EOS_EResult EOSSDK_TitleStorageFileTransferRequest::GetFilename(uint32_t FilenameStringBufferSizeBytes, char* OutStringBuffer, int32_t* OutStringLength)
{
    TRACE_FUNC();
    std::lock_guard<std::mutex> m_lk(m_local_mutex);

    if (OutStringLength == nullptr || OutStringBuffer == nullptr)
        return EOS_EResult::EOS_InvalidParameters;

    *OutStringLength = m_file_name.length();
    size_t copy_len = std::min<size_t>(FilenameStringBufferSizeBytes, m_file_name.length());
    m_file_name.copy(OutStringBuffer, copy_len);
    OutStringBuffer[copy_len - 1] = '\0';

    return EOS_EResult::EOS_Success;
}

EOS_EResult EOSSDK_TitleStorageFileTransferRequest::CancelRequest()
{
    TRACE_FUNC();
    std::lock_guard<std::mutex> m_lk(m_local_mutex);

    if (m_done)
        return EOS_EResult::EOS_NoChange;

    m_canceled = true;
    return EOS_EResult::EOS_Success;
}

void EOSSDK_TitleStorageFileTransferRequest::Release()
{
    TRACE_FUNC();
    std::lock_guard<std::mutex> m_lk(m_local_mutex);
    m_released = true;
}

}