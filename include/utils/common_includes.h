#pragma once

// Platform detection
#if defined(_WIN32) || defined(_WIN64)
    #ifndef __WINDOWS__
        #define __WINDOWS__
    #endif
#elif defined(__linux__)
    #ifndef __LINUX__
        #define __LINUX__
    #endif
#endif

#if defined(__WINDOWS__)
    #define WIN32_LEAN_AND_MEAN
    #define VC_EXTRALEAN
    #define NOMINMAX
    #include <Windows.h>
    #include <shlobj.h>
    #include <PathCch.h>
    #include <WinSock2.h>
    #include <ws2tcpip.h>
    #include <iphlpapi.h>
#endif

#ifdef min
#undef min
#endif
#ifdef max
#undef max
#endif

#if defined(__WINDOWS__)
    #define EOS_BUILD_DLL  1
    #define EXPORT_EPIC_API __declspec(dllexport)
    #define LOCAL_API
#else
    #define EOS_BUILD_DLL  1
    #define EXPORT_EPIC_API __attribute__((visibility("default")))
    #define LOCAL_API       __attribute__((visibility("hidden")))
#endif

#include <eos_sdk.h>
#include <eos_logging.h>
#include <eos_version.h>
#include <eos_rtc_audio.h>
#include <eos_rtc_data.h>
#include <eos_rtc_admin.h>

#include <nlohmann/json.hpp>
#include <fifo_map.hpp>

template<class K, class V, class dummy_compare, class A>
using my_workaround_fifo_map = nlohmann::fifo_map<K, V, nlohmann::fifo_map_compare<K>, A>;
using fifo_json = nlohmann::basic_json<my_workaround_fifo_map>;

#include "portable_api.h"

#include "sdk/sdk_compat.h"

#include <thread>
#include <mutex>
#include <atomic>
#include <limits>
#include <chrono>
#include <locale>
#include <codecvt>
#include <random>
#include <string>
#include <vector>
#include <list>
#include <set>
#include <queue>
#include <map>
#include <unordered_map>
#include <algorithm>
#include <functional>
#include <fstream>
#include <sstream>
#include <iomanip>
#include <utility>
#include <type_traits>
#include <cstdarg>
#include <cassert>
#include <memory>
#include <optional>
#include <filesystem>

#include "../userinfo/eos_epicaccountiddetails.h"

namespace sdk {
    using string  = std::string;
    using path    = std::filesystem::path;
    using json    = nlohmann::json;
    using lock    = std::lock_guard<std::recursive_mutex>;
    using clock   = std::chrono::steady_clock;
    using time_pt = clock::time_point;

    // Queue a deferred callback without repeating the boilerplate everywhere.
    template<typename T, typename F>
    inline void dispatch_cb(void* owner, F delegate, void* client_data, EOS_EResult result = EOS_EResult::EOS_Success) {
        auto res = std::make_shared<FrameResult>();
        auto& info = res->CreateCallback<T>((CallbackFunc)delegate);
        info.ClientData = client_data;
        info.ResultCode = result;
        res->done = true;
        GetCB_Manager().add_callback(reinterpret_cast<IRunCallback*>(owner), res);
    }

    // Macro pair used inside exported API functions to fire a callback.
    #define SE_CB_BEGIN(T, F) if (!F) return; auto res = std::make_shared<FrameResult>(); auto& info = res->CreateCallback<T>((CallbackFunc)F); info.ClientData = ClientData;
    #define SE_CB_END(owner) res->done = true; GetCB_Manager().add_callback(owner, res);
}

using namespace sdk;

#if defined(__WINDOWS__)
    #include <MinHook.h>
#endif

#include "md5.h"
#include "os_funcs.h"
#include "Log.h"
#include "file_manager.h"
#include "helper_funcs.h"

#include "../config/settings.h"

// Current emulator version, reported by EOS_GetVersion
constexpr char EMU_VERSION[] = "2.0.0-KEE";

