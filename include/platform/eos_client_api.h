#pragma once

#include "common_includes.h"

class EOSSDK_Client
{
public:
    
    EOS_AllocateMemoryFunc m_allocate_memory_func;

    EOS_ReallocateMemoryFunc m_reallocate_memory_func;

    EOS_ReleaseMemoryFunc m_release_memory_func;

    bool m_sdk_initialized;

    std::map<string, EOS_EpicAccountId> m_epicuserids;
    std::map<string, EOS_ProductUserId> m_productuserids;

    int32_t api_version;
    string m_product_name;
    string m_product_version;

    EOSSDK_Client();
    ~EOSSDK_Client();

    static EOSSDK_Client& Inst();

    EOS_EpicAccountId get_epicuserid(string const& userid);
    EOS_ProductUserId get_productuserid(string const& userid);
};

inline EOS_EpicAccountId GetInvalidEpicUserId()
{
    return EOSSDK_Client::Inst().get_epicuserid(sdk::NULL_USER_ID);
}

inline EOS_EpicAccountId GetEpicUserId(string const& userid)
{
    return EOSSDK_Client::Inst().get_epicuserid(userid);
}

inline EOS_ProductUserId GetInvalidProductUserId()
{
    return EOSSDK_Client::Inst().get_productuserid(sdk::NULL_USER_ID);
}

inline EOS_ProductUserId GetProductUserId(string const& userid)
{
    return EOSSDK_Client::Inst().get_productuserid(userid);
}