#pragma once

#include "common_includes.h"

class EOSSDK_Client;

namespace sdk
{
    static constexpr char NULL_USER_ID[EOS_EPICACCOUNTID_MAX_LENGTH + 1] = "00000000000000000000000000000000";
    static constexpr size_t max_accountid_length = EOS_EPICACCOUNTID_MAX_LENGTH + 1;

    static constexpr size_t max_productid_length = EOS_PRODUCTUSERID_MAX_LENGTH + 1;
}

struct EOS_EpicAccountIdDetails
{
    friend class EOSSDK_Client;

private:
    std::mutex m_local_mutex;
    std::string m_idstr;
    bool m_valid;

    EOS_EpicAccountIdDetails();
    EOS_EpicAccountIdDetails(EOS_EpicAccountIdDetails &&) noexcept;

    EOS_EpicAccountIdDetails& operator=(EOS_EpicAccountIdDetails&&) noexcept;

    void FromString(const char* accountIdStr);
    void from_string(std::string const& accountIdStr);
    void validate();

    ~EOS_EpicAccountIdDetails();
public:

    EOS_Bool IsValid();
    EOS_EResult ToString(char* outBuffer, int32_t* outBufferSize);

    std::string const& to_string();

    inline bool operator ==(EOS_EpicAccountIdDetails const& other) { return (m_idstr == other.m_idstr); }
    inline bool operator !=(EOS_EpicAccountIdDetails const& other) { return !(*this == other); }
};

struct EOS_ProductUserIdDetails
{
    friend class EOSSDK_Client;

private:
    std::mutex m_local_mutex;
    std::string m_idstr;
    bool m_valid;

    EOS_ProductUserIdDetails();
    EOS_ProductUserIdDetails(EOS_ProductUserIdDetails&&) noexcept;

    EOS_ProductUserIdDetails& operator=(EOS_ProductUserIdDetails&&) noexcept;

    void FromString(const char* accountIdStr);
    void from_string(std::string const& accountIdStr);
    void validate();

    ~EOS_ProductUserIdDetails();
public:
    
    EOS_Bool IsValid();
    EOS_EResult ToString(char* outBuffer, int32_t* outBufferSize);

    std::string const& to_string();

    inline bool operator ==(EOS_ProductUserIdDetails const& other) { return (m_idstr == other.m_idstr); }
    inline bool operator !=(EOS_ProductUserIdDetails const& other) { return !(*this == other); }
};