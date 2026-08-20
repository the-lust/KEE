#include "eos_epicaccountiddetails.h"

EOS_EpicAccountIdDetails::EOS_EpicAccountIdDetails():
    m_idstr(sdk::NULL_USER_ID),
    m_valid(false)
{}

EOS_EpicAccountIdDetails::EOS_EpicAccountIdDetails(EOS_EpicAccountIdDetails && other) noexcept :
    m_idstr(std::move(other.m_idstr)),
    m_valid(std::move(other.m_valid))
{}

EOS_EpicAccountIdDetails::~EOS_EpicAccountIdDetails()
{}

EOS_EpicAccountIdDetails& EOS_EpicAccountIdDetails::operator=(EOS_EpicAccountIdDetails && other) noexcept
{
    std::lock_guard<std::mutex> lk(m_local_mutex);
    m_idstr = std::move(other.m_idstr);
    m_valid = other.m_valid;
    return *this;
}

EOS_Bool EOS_EpicAccountIdDetails::IsValid()
{
    std::lock_guard<std::mutex> lk(m_local_mutex);
    return m_valid;
}

EOS_EResult EOS_EpicAccountIdDetails::ToString(char* outBuffer, int32_t* outBufferSize)
{
    std::lock_guard<std::mutex> lk(m_local_mutex);

    if (outBuffer == nullptr || outBufferSize == nullptr)
        return EOS_EResult::EOS_InvalidParameters;

    size_t len = m_idstr.length() + 1;
    if (*outBufferSize < len)
    {
        *outBufferSize = static_cast<int32_t>(len);
        return EOS_EResult::EOS_LimitExceeded;
    }

    strncpy(outBuffer, m_idstr.c_str(), len);
    *outBufferSize = static_cast<int32_t>(len);
    return EOS_EResult::EOS_Success;
}

void EOS_EpicAccountIdDetails::FromString(const char* accountIdStr)
{
    if (accountIdStr != nullptr)
    {
        from_string(accountIdStr);
    }
    else
    {
        std::lock_guard<std::mutex> lk(m_local_mutex);
        m_valid = false;
        m_idstr = sdk::NULL_USER_ID;
    }
}

void EOS_EpicAccountIdDetails::from_string(string const& accountIdStr)
{
    std::lock_guard<std::mutex> lk(m_local_mutex);
    m_idstr = accountIdStr;
    validate();
}

string const& EOS_EpicAccountIdDetails::to_string()
{
    std::lock_guard<std::mutex> lk(m_local_mutex);
    return m_idstr;
}

void EOS_EpicAccountIdDetails::validate()
{
    auto it = m_idstr.begin();
    m_valid = false;

    if (m_idstr.length() > 2 &&
        m_idstr[0] == '0' &&
        m_idstr[1] == 'x')
    {
        it = m_idstr.erase(m_idstr.begin(), m_idstr.begin() + 2);
    }

    if (it != m_idstr.end())
    {
        if (string(it, m_idstr.end()) == sdk::NULL_USER_ID)
            return;

        m_valid = true;
        for (; it != m_idstr.end(); ++it)
        {
            char c = *it;
            if ((c < '0' || c > '9') &&
                (c < 'A' || c > 'F') &&
                (c < 'a' || c > 'f')
                )
            {
                m_valid = false;
                return;
            }
        }
    }
}

EOS_ProductUserIdDetails::EOS_ProductUserIdDetails() :
    m_idstr(),
    m_valid(false)
{}

EOS_ProductUserIdDetails::EOS_ProductUserIdDetails(EOS_ProductUserIdDetails&& other) noexcept :
    m_idstr(std::move(other.m_idstr)),
    m_valid(std::move(other.m_valid))
{}

EOS_ProductUserIdDetails::~EOS_ProductUserIdDetails()
{}

EOS_ProductUserIdDetails& EOS_ProductUserIdDetails::operator=(EOS_ProductUserIdDetails&& other) noexcept
{
    std::lock_guard<std::mutex> lk(m_local_mutex);
    m_idstr = std::move(other.m_idstr);
    m_valid = other.m_valid;
    return *this;
}

EOS_Bool EOS_ProductUserIdDetails::IsValid()
{
    std::lock_guard<std::mutex> lk(m_local_mutex);
    return m_valid;
}

EOS_EResult EOS_ProductUserIdDetails::ToString(char* outBuffer, int32_t* outBufferSize)
{
    std::lock_guard<std::mutex> lk(m_local_mutex);

    if (outBuffer == nullptr || outBufferSize == nullptr)
        return EOS_EResult::EOS_InvalidParameters;

    size_t len = m_idstr.length() + 1;
    if (*outBufferSize < len)
    {
        *outBufferSize = static_cast<int32_t>(len);
        return EOS_EResult::EOS_LimitExceeded;
    }

    strncpy(outBuffer, m_idstr.c_str(), len);
    *outBufferSize = static_cast<int32_t>(len);
    return EOS_EResult::EOS_Success;
}

void EOS_ProductUserIdDetails::FromString(const char* accountIdStr)
{
    if (accountIdStr != nullptr)
    {
        from_string(accountIdStr);
    }
    else
    {
        std::lock_guard<std::mutex> lk(m_local_mutex);
        m_idstr = sdk::NULL_USER_ID;
        m_valid = false;
    }
}

string const& EOS_ProductUserIdDetails::to_string()
{
    std::lock_guard<std::mutex> lk(m_local_mutex);
    return m_idstr;
}

void EOS_ProductUserIdDetails::from_string(string const& accountIdStr)
{
    std::lock_guard<std::mutex> lk(m_local_mutex);
    m_idstr = accountIdStr;
    validate();
}

void EOS_ProductUserIdDetails::validate()
{
    auto it = m_idstr.begin();
    m_valid = false;

    if (m_idstr.length() > 2 &&
        m_idstr[0] == '0' &&
        m_idstr[1] == 'x')
    {
        it = m_idstr.erase(m_idstr.begin(), m_idstr.begin() + 2);
    }

    if (it != m_idstr.end())
    {
        if (string(it, m_idstr.end()) == sdk::NULL_USER_ID)
            return;

        m_valid = true;
        for (; it != m_idstr.end(); ++it)
        {
            char c = *it;
            if ((c < '0' || c > '9') &&
                (c < 'A' || c > 'F') &&
                (c < 'a' || c > 'f')
                )
            {
                m_valid = false;
                return;
            }
        }
    }
}