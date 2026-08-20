#include "overlay/OverlayManager.h"
#include <d3d11.h>
#include <dxgi.h>
#include <MinHook.h>
#include "utils/Log.h"

namespace sdk {

typedef HRESULT(WINAPI* DXGIPresent_t)(IDXGISwapChain* pSwapChain, UINT SyncInterval, UINT Flags);
typedef HRESULT(WINAPI* DXGIResizeBuffers_t)(IDXGISwapChain* pSwapChain, UINT BufferCount, UINT Width, UINT Height, DXGI_FORMAT NewFormat, UINT SwapChainFlags);

DXGIPresent_t OriginalPresent = nullptr;
DXGIResizeBuffers_t OriginalResizeBuffers = nullptr;

HRESULT WINAPI HookedPresent(IDXGISwapChain* pSwapChain, UINT SyncInterval, UINT Flags) {
    static bool first_time = true;
    if (first_time) {
        OverlayManager::Inst().Init();
        first_time = false;
    }

    OverlayManager::Inst().OnPresent();

    return OriginalPresent(pSwapChain, SyncInterval, Flags);
}

HRESULT WINAPI HookedResizeBuffers(IDXGISwapChain* pSwapChain, UINT BufferCount, UINT Width, UINT Height, DXGI_FORMAT NewFormat, UINT SwapChainFlags) {
    OverlayManager::Inst().OnResize(Width, Height);
    return OriginalResizeBuffers(pSwapChain, BufferCount, Width, Height, NewFormat, SwapChainFlags);
}

void InstallDXHooks() {
    APP_LOG(Log::LogLevel::INFO, "Installing DirectX Hooks...");

    WNDCLASSEXA wc = { sizeof(WNDCLASSEXA), CS_CLASSDC, DefWindowProc, 0L, 0L, GetModuleHandle(NULL), NULL, NULL, NULL, NULL, "DX_HOOK", NULL };
    RegisterClassExA(&wc);
    HWND hwnd = CreateWindowExA(0, wc.lpszClassName, "DX_HOOK_WINDOW", WS_OVERLAPPEDWINDOW, 100, 100, 100, 100, NULL, NULL, wc.hInstance, NULL);

    D3D_FEATURE_LEVEL featureLevel;
    ID3D11Device* pDevice = nullptr;
    ID3D11DeviceContext* pContext = nullptr;
    IDXGISwapChain* pSwapChain = nullptr;

    DXGI_SWAP_CHAIN_DESC sd;
    ZeroMemory(&sd, sizeof(sd));
    sd.BufferCount = 1;
    sd.BufferDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
    sd.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;
    sd.OutputWindow = hwnd;
    sd.SampleDesc.Count = 1;
    sd.Windowed = TRUE;
    sd.SwapEffect = DXGI_SWAP_EFFECT_DISCARD;

    if (FAILED(D3D11CreateDeviceAndSwapChain(NULL, D3D_DRIVER_TYPE_HARDWARE, NULL, 0, NULL, 0, D3D11_SDK_VERSION, &sd, &pSwapChain, &pDevice, &featureLevel, &pContext))) {
        APP_LOG(Log::LogLevel::ERR, "Failed to create dummy DX11 device");
        DestroyWindow(hwnd);
        UnregisterClass(wc.lpszClassName, wc.hInstance);
        return;
    }

    void** vtable = *(void***)pSwapChain;
    
    MH_Initialize();
    MH_CreateHook(vtable[8], (void*)&HookedPresent, (void**)&OriginalPresent);
    MH_CreateHook(vtable[13], (void*)&HookedResizeBuffers, (void**)&OriginalResizeBuffers);
    MH_EnableHook(MH_ALL_HOOKS);

    pSwapChain->Release();
    pDevice->Release();
    pContext->Release();
    DestroyWindow(hwnd);
    UnregisterClass(wc.lpszClassName, wc.hInstance);
    
    APP_LOG(Log::LogLevel::INFO, "DX11 Hooks installed successfully");
}

} 
