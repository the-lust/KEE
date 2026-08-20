
// KEE proxy loader (winmm.dll / version.dll)
//
// Sits next to a game executable and loads the emulator DLL before the game
// starts. It also hooks GetCommandLineW to inject the same Epic launcher
// arguments the real EGS would pass, so games that read the command line
// for auth/account data work offline.

#include <windows.h>
#include <string>
#include <vector>
#include <filesystem>
#include <MinHook.h>

typedef LPWSTR (WINAPI *GetCommandLineW_t)();
static GetCommandLineW_t Original_GetCommandLineW = nullptr;

static std::wstring g_injected_cmd;

std::wstring get_injected_cmd() {
    std::wstring cmd = GetCommandLineW();
    if (cmd.find(L"-AUTH_TYPE") == std::wstring::npos) {
        cmd += L" -AUTH_LOGIN=unused -AUTH_PASSWORD=kee-token -AUTH_TYPE=exchangecode -epicapp=kee -epicenv=Prod -EpicPortal";
    }
    return cmd;
}

LPWSTR WINAPI Hooked_GetCommandLineW() {
    return (LPWSTR)g_injected_cmd.c_str();
}

void install_hooks() {
    g_injected_cmd = get_injected_cmd();
    if (MH_Initialize() == MH_OK) {
        if (MH_CreateHookApi(L"kernel32.dll", "GetCommandLineW",
            &Hooked_GetCommandLineW, (LPVOID*)&Original_GetCommandLineW) == MH_OK) {
            MH_EnableHook(MH_ALL_HOOKS);
        }
    }
}

void load_emu() {
    static bool loaded = false;
    if (loaded) return;
    loaded = true;

#if defined(_WIN64)
    const wchar_t* emu_name = L"EOSSDK-Win64-Shipping.dll";
#else
    const wchar_t* emu_name = L"EOSSDK-Win32-Shipping.dll";
#endif

    HMODULE h = GetModuleHandleW(emu_name);
    if (!h) h = LoadLibraryW(emu_name);
}

// Forward declared to avoid conflict with windows.h declarations
extern "C" void Proxy_PlaySoundA() { load_emu(); }
extern "C" void Proxy_PlaySoundW() { load_emu(); }
extern "C" void Proxy_waveOutOpen() { load_emu(); }
extern "C" void Proxy_GetFileVersionInfoA() { load_emu(); }
extern "C" void Proxy_GetFileVersionInfoW() { load_emu(); }
extern "C" void Proxy_GetFileVersionInfoSizeA() { load_emu(); }
extern "C" void Proxy_GetFileVersionInfoSizeW() { load_emu(); }
extern "C" void Proxy_VerQueryValueA() { load_emu(); }
extern "C" void Proxy_VerQueryValueW() { load_emu(); }

BOOL APIENTRY DllMain(HMODULE hModule, DWORD ul_reason_for_call, LPVOID lpReserved) {
    if (ul_reason_for_call == DLL_PROCESS_ATTACH) {
        DisableThreadLibraryCalls(hModule);
        install_hooks();
        load_emu();
    }
    return TRUE;
}
