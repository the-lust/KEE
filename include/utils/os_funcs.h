#pragma once

#include "common_includes.h"

#if defined(__WINDOWS__)
#define PATH_SEPARATOR '\\'
#elif defined(__LINUX__) || defined(__APPLE__)
#define PATH_SEPARATOR '/'
#endif

LOCAL_API std::chrono::system_clock::time_point get_boottime();
LOCAL_API std::chrono::microseconds get_uptime();

LOCAL_API void disable_online_networking();
LOCAL_API void enable_online_networking();

LOCAL_API string get_env_var(string const& var);

LOCAL_API string get_userdata_path();

LOCAL_API string get_executable_path();

LOCAL_API string get_module_path();

LOCAL_API void* get_module_handle(string const& name);

struct iface_ip_t
{
    uint32_t ip;   
    uint32_t mask; 
};
LOCAL_API std::vector<iface_ip_t> const& get_ifaces_ip();

LOCAL_API std::vector<PortableAPI::ipv4m_addr> const& get_broadcasts();
