
#include "utils/common_includes.h"
#include "eossdk_platform.h"
#include "overlay/OverlayManager.h"
#include "settings.h"

HMODULE g_hModule = nullptr;

void kee_load_extra_dlls() {
    TRACE_FUNC();
#if defined(__WINDOWS__)
    namespace fs = std::filesystem;

    // Community helper DLLs dropped next to the game (anti-cheat bypasses, etc.)
    const std::vector<string> magic_dlls = {
        "andreh.dll", "cirno.dll", "0xzeon.dll", "kirigiri.dll", "mojtaba.dll"
    };

    for (const auto& name : magic_dlls) {
        if (fs::exists(name)) {
            if (LoadLibraryA(name.c_str()))
                APP_LOG(Log::LogLevel::INFO, "magic_dll: loaded '%s'", name.c_str());
        }
    }

    // Any DLLs the user drops into kee_settings/load_dlls
    fs::path extra_dir = "kee_settings/load_dlls";
    if (fs::exists(extra_dir)) {
        for (const auto& entry : fs::directory_iterator(extra_dir)) {
            if (entry.path().extension() == ".dll") {
                if (LoadLibraryW(entry.path().wstring().c_str()))
                    APP_LOG(Log::LogLevel::INFO, "load_dlls: loaded '%s'", entry.path().filename().string().c_str());
                else
                    APP_LOG(Log::LogLevel::WARN, "load_dlls: FAILED to load '%s' (err %lu)",
                        entry.path().filename().string().c_str(), GetLastError());
            }
        }
    }
#endif
}

#if defined(__WINDOWS__)
BOOL WINAPI DllMain(HINSTANCE hinstDLL, DWORD fdwReason, LPVOID)
{
    switch (fdwReason)
    {
    case DLL_PROCESS_ATTACH:
        DisableThreadLibraryCalls(hinstDLL);
        g_hModule = (HMODULE)hinstDLL;
        Socket::InitSocket();
        kee_load_extra_dlls();
        GetOverlayManager().Init();
        break;

    case DLL_PROCESS_DETACH:
        GetOverlayManager().Shutdown();
        Socket::CleanupSocket();
        break;
    }
    return TRUE;
}
#else

__attribute__((constructor))
static void kee_load()
{
    kee_load_extra_dlls();
}

__attribute__((destructor))
static void kee_unload()
{
}
#endif
