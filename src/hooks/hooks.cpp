#include "hooks/hooks.h"
#include <MinHook.h>
#include <iostream>
#include "utils/Log.h"

extern HMODULE g_hModule;

namespace Hooks {

    typedef HMODULE(WINAPI* LoadLibraryW_t)(LPCWSTR lpLibFileName);
    LoadLibraryW_t Original_LoadLibraryW = nullptr;

    typedef HMODULE(WINAPI* LoadLibraryA_t)(LPCSTR lpLibFileName);
    LoadLibraryA_t Original_LoadLibraryA = nullptr;

    typedef HMODULE(WINAPI* LoadLibraryExA_t)(LPCSTR lpLibFileName, HANDLE hFile, DWORD dwFlags);
    LoadLibraryExA_t Original_LoadLibraryExA = nullptr;

    typedef HMODULE(WINAPI* LoadLibraryExW_t)(LPCWSTR lpLibFileName, HANDLE hFile, DWORD dwFlags);
    LoadLibraryExW_t Original_LoadLibraryExW = nullptr;

    typedef FARPROC(WINAPI* GetProcAddress_t)(HMODULE hModule, LPCSTR lpProcName);
    GetProcAddress_t Original_GetProcAddress = nullptr;

    bool IsEOSSDK(const std::wstring& name)
    {
        return name.find(L"EOSSDK-Win64-Shipping.dll") != std::wstring::npos ||
               name.find(L"EOSSDK-Win32-Shipping.dll") != std::wstring::npos;
    }

    bool IsEOSSDK(const std::string& name)
    {
        return name.find("EOSSDK-Win64-Shipping.dll") != std::string::npos ||
               name.find("EOSSDK-Win32-Shipping.dll") != std::string::npos;
    }

    HMODULE WINAPI Hooked_LoadLibraryW(LPCWSTR lpLibFileName)
    {
        if (lpLibFileName && IsEOSSDK(lpLibFileName)) {
            APP_LOG(Log::LogLevel::INFO, "Intercepted LoadLibraryW for EOS SDK.");
            return g_hModule;
        }
        return Original_LoadLibraryW(lpLibFileName);
    }

    HMODULE WINAPI Hooked_LoadLibraryA(LPCSTR lpLibFileName)
    {
        if (lpLibFileName && IsEOSSDK(lpLibFileName)) {
            APP_LOG(Log::LogLevel::INFO, "Intercepted LoadLibraryA for EOS SDK.");
            return g_hModule;
        }
        return Original_LoadLibraryA(lpLibFileName);
    }

    HMODULE WINAPI Hooked_LoadLibraryExA(LPCSTR lpLibFileName, HANDLE hFile, DWORD dwFlags)
    {
        if (lpLibFileName && IsEOSSDK(lpLibFileName)) {
            APP_LOG(Log::LogLevel::INFO, "Intercepted LoadLibraryExA for EOS SDK.");
            return g_hModule;
        }
        return Original_LoadLibraryExA(lpLibFileName, hFile, dwFlags);
    }

    HMODULE WINAPI Hooked_LoadLibraryExW(LPCWSTR lpLibFileName, HANDLE hFile, DWORD dwFlags)
    {
        if (lpLibFileName && IsEOSSDK(lpLibFileName)) {
            APP_LOG(Log::LogLevel::INFO, "Intercepted LoadLibraryExW for EOS SDK.");
            return g_hModule;
        }
        return Original_LoadLibraryExW(lpLibFileName, hFile, dwFlags);
    }

    FARPROC WINAPI Hooked_GetProcAddress(HMODULE hModule, LPCSTR lpProcName)
    {
        if (hModule == g_hModule && lpProcName) {

        }
        return Original_GetProcAddress(hModule, lpProcName);
    }

    bool Initialize()
    {
        if (MH_Initialize() != MH_OK) {
            APP_LOG(Log::LogLevel::ERR, "Failed to initialize MinHook.");
            return false;
        }

        MH_CreateHook(&LoadLibraryW, &Hooked_LoadLibraryW, reinterpret_cast<LPVOID*>(&Original_LoadLibraryW));
        MH_CreateHook(&LoadLibraryA, &Hooked_LoadLibraryA, reinterpret_cast<LPVOID*>(&Original_LoadLibraryA));
        MH_CreateHook(&LoadLibraryExA, &Hooked_LoadLibraryExA, reinterpret_cast<LPVOID*>(&Original_LoadLibraryExA));
        MH_CreateHook(&LoadLibraryExW, &Hooked_LoadLibraryExW, reinterpret_cast<LPVOID*>(&Original_LoadLibraryExW));
        MH_CreateHook(&GetProcAddress, &Hooked_GetProcAddress, reinterpret_cast<LPVOID*>(&Original_GetProcAddress));

        if (MH_EnableHook(MH_ALL_HOOKS) != MH_OK) {
            APP_LOG(Log::LogLevel::ERR, "Failed to enable hooks.");
            return false;
        }

        APP_LOG(Log::LogLevel::INFO, "MinHook initialized and library hooks enabled.");
        return true;
    }

    void Shutdown()
    {
        MH_DisableHook(MH_ALL_HOOKS);
        MH_Uninitialize();
    }
}
