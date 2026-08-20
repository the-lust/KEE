#define WIN32_LEAN_AND_MEAN
#include <windows.h>
#include <tlhelp32.h>
#include <string>
#include <vector>
#include <sstream>
#include <fstream>
#include <filesystem>

static std::string g_target_exe;
static std::string g_work_dir;
static std::string g_args;
static std::string g_eos_dll;
static std::string g_egclient_dll;
static bool g_inject_eos = true;
static bool g_inject_egclient = true;
static bool g_use_dll_proxy = false;

static std::string trim(const std::string& s) {
    size_t start = s.find_first_not_of(" \t\r\n");
    size_t end = s.find_last_not_of(" \t\r\n");
    return (start == std::string::npos) ? "" : s.substr(start, end - start + 1);
}

static bool read_config(const char* path) {
    std::ifstream f(path);
    if (!f.is_open()) return false;

    std::string section, line;
    while (std::getline(f, line)) {
        line = trim(line);
        if (line.empty() || line[0] == ';' || line[0] == '#') continue;
        if (line[0] == '[') {
            size_t end = line.find(']');
            if (end != std::string::npos) section = line.substr(1, end - 1);
            continue;
        }
        size_t eq = line.find('=');
        if (eq == std::string::npos) continue;
        std::string key = trim(line.substr(0, eq));
        std::string val = trim(line.substr(eq + 1));

        if (section == "Loader") {
            if (key == "Target") g_target_exe = val;
            else if (key == "WorkingDirectory") g_work_dir = val;
            else if (key == "Arguments") g_args = val;
            else if (key == "EOSSDKDLL") g_eos_dll = val;
            else if (key == "EGClientDLL") g_egclient_dll = val;
            else if (key == "InjectEOS") g_inject_eos = (val != "0" && val != "false");
            else if (key == "InjectEGClient") g_inject_egclient = (val != "0" && val != "false");
            else if (key == "UseDLLProxy") g_use_dll_proxy = (val == "1" || val == "true");
        }
    }
    return !g_target_exe.empty();
}

static bool inject_dll(HANDLE hProcess, const std::string& dll_path) {
    if (!std::filesystem::exists(dll_path)) return false;

    size_t path_size = dll_path.size() + 1;
    void* remote_mem = VirtualAllocEx(hProcess, NULL, path_size,
                                       MEM_COMMIT | MEM_RESERVE, PAGE_READWRITE);
    if (!remote_mem) return false;

    if (!WriteProcessMemory(hProcess, remote_mem, dll_path.c_str(), path_size, NULL)) {
        VirtualFreeEx(hProcess, remote_mem, 0, MEM_RELEASE);
        return false;
    }

    HMODULE kernel32 = GetModuleHandleW(L"kernel32.dll");
    LPTHREAD_START_ROUTINE load_lib = (LPTHREAD_START_ROUTINE)
        GetProcAddress(kernel32, "LoadLibraryA");

    HANDLE hThread = CreateRemoteThread(hProcess, NULL, 0, load_lib,
                                         remote_mem, 0, NULL);
    if (!hThread) {
        VirtualFreeEx(hProcess, remote_mem, 0, MEM_RELEASE);
        return false;
    }

    WaitForSingleObject(hThread, INFINITE);
    CloseHandle(hThread);
    VirtualFreeEx(hProcess, remote_mem, 0, MEM_RELEASE);
    return true;
}

static bool setup_proxy_dlls(const std::string& target_dir) {
    std::error_code ec;
    std::string emu_dir = std::filesystem::current_path().string();

    auto copy_if_needed = [&](const std::string& name) {
        std::string src = emu_dir + "\\" + name;
        std::string dst = target_dir + "\\" + name;
        if (std::filesystem::exists(src) && !std::filesystem::exists(dst)) {
            std::filesystem::copy_file(src, dst, ec);
        }
    };

#if defined(_WIN64)
    copy_if_needed("EOSSDK-Win64-Shipping.dll");
    copy_if_needed("EOSSDK-Win64-Shipping.lib");
    copy_if_needed("egclient64.dll");
    copy_if_needed("egclient64.lib");
#else
    copy_if_needed("EOSSDK-Win32-Shipping.dll");
    copy_if_needed("EOSSDK-Win32-Shipping.lib");
    copy_if_needed("egclient.dll");
    copy_if_needed("egclient.lib");
#endif
    copy_if_needed("version.dll");
    copy_if_needed("winmm.dll");
    return true;
}

int CALLBACK WinMain(HINSTANCE hInst, HINSTANCE hPrev, LPSTR lpCmdLine, int nCmdShow) {
    std::string config_path = "cold_loader.ini";
    if (__argc > 1) config_path = __argv[1];

    if (!read_config(config_path.c_str())) {
        std::string msg = "Failed to read config: " + config_path +
            "\n\nCreate cold_loader.ini:\n"
            "[Loader]\nTarget = game.exe\nArguments = \nWorkingDirectory = \n"
            "InjectEOS = 1\nInjectEGClient = 0\n"
            "EOSSDKDLL = auto\nEGClientDLL = auto";
        MessageBoxA(NULL, msg.c_str(), "KEE Cold Loader",
                    MB_OK | MB_ICONERROR);
        return 1;
    }

    std::string target = g_target_exe;
    std::string work_dir = g_work_dir.empty() ?
        std::filesystem::path(target).parent_path().string() : g_work_dir;

    if (!std::filesystem::exists(target)) {
        std::string msg = "Target not found:\n" + target;
        MessageBoxA(NULL, msg.c_str(), "KEE Cold Loader",
                    MB_OK | MB_ICONERROR);
        return 1;
    }

    if (g_use_dll_proxy) {
        setup_proxy_dlls(work_dir);
    }

#if defined(_WIN64)
    if (g_eos_dll.empty() || g_eos_dll == "auto")
        g_eos_dll = "EOSSDK-Win64-Shipping.dll";
    if (g_egclient_dll.empty() || g_egclient_dll == "auto")
        g_egclient_dll = "egclient64.dll";
#else
    if (g_eos_dll.empty() || g_eos_dll == "auto")
        g_eos_dll = "EOSSDK-Win32-Shipping.dll";
    if (g_egclient_dll.empty() || g_egclient_dll == "auto")
        g_egclient_dll = "egclient.dll";
#endif

    std::string cmd_line = "\"" + target + "\"";
    if (!g_args.empty()) cmd_line += " " + g_args;

    STARTUPINFOA si = { sizeof(si) };
    PROCESS_INFORMATION pi;
    if (!CreateProcessA(target.c_str(), &cmd_line[0], NULL, NULL, FALSE,
                        CREATE_SUSPENDED, NULL,
                        work_dir.empty() ? NULL : work_dir.c_str(),
                        &si, &pi)) {
        std::string msg = "Failed to launch:\n" + target;
        MessageBoxA(NULL, msg.c_str(), "KEE Cold Loader",
                    MB_OK | MB_ICONERROR);
        return 1;
    }

    if (g_inject_egclient && std::filesystem::exists(g_egclient_dll)) {
        inject_dll(pi.hProcess, g_egclient_dll);
    }

    if (g_inject_eos && std::filesystem::exists(g_eos_dll)) {
        inject_dll(pi.hProcess, g_eos_dll);
    }

    ResumeThread(pi.hThread);
    CloseHandle(pi.hThread);
    CloseHandle(pi.hProcess);

    return 0;
}
