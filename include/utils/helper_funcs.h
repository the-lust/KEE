#pragma once

#include "common_includes.h"

#if defined(__clang__) || defined(__GNUC__)
    #define CLANG_GCC_DONT_OPTIMIZE __attribute__((optnone))
#else
    #define CLANG_GCC_DONT_OPTIMIZE
#endif

using global_lock_t = std::lock_guard<std::recursive_mutex>;
LOCAL_API std::recursive_mutex& global_mutex();
#define GLOBAL_LOCK() global_lock_t __global_lock(global_mutex())
#define GLOBAL_MUTEX global_mutex()

LOCAL_API std::random_device& get_rd();
LOCAL_API std::mt19937_64& get_gen();

LOCAL_API void random_string(string const& charset, char* buff, size_t length);
LOCAL_API string generate_account_id();
LOCAL_API string generate_account_id_from_name(string const& username);
LOCAL_API string generate_epic_id_user();
LOCAL_API string generate_epic_id_user_from_name(string const& username);

LOCAL_API string generate_product_user_id();
LOCAL_API string generate_product_user_id_from_name(string const& app_id, string const& epic_id);

LOCAL_API void fatal_throw(const char* msg);

LOCAL_API string get_callback_name(int iCallback);
LOCAL_API const char* search_attr_to_string(EOS_EOnlineComparisonOp comp);

template<typename T>
constexpr inline void set_nullptr(T& v) { if (v != nullptr) *v = nullptr; }

template<typename T_Full, typename T_Legacy>
inline const T_Full* read_versioned_struct(const void* Options, int32_t ApiVersion, int32_t MinApiVersion)
{
    if (ApiVersion >= MinApiVersion)
        return reinterpret_cast<const T_Full*>(Options);
    return reinterpret_cast<const T_Full*>(reinterpret_cast<const T_Legacy*>(Options));
}