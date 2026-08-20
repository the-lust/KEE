#define WIN32_LEAN_AND_MEAN
#include <windows.h>
#include <cstdio>
#include <string>
#include <vector>
#include <filesystem>

struct ExportEntry {
    std::string name;
    uint16_t ordinal;
};

static std::vector<ExportEntry> g_exports;

static void scan_exports(HMODULE hMod) {
    IMAGE_DOS_HEADER* dos = (IMAGE_DOS_HEADER*)hMod;
    IMAGE_NT_HEADERS* nt = (IMAGE_NT_HEADERS*)((BYTE*)hMod + dos->e_lfanew);
    IMAGE_DATA_DIRECTORY* dir = &nt->OptionalHeader.DataDirectory[IMAGE_DIRECTORY_ENTRY_EXPORT];

    if (!dir->Size) return;

    IMAGE_EXPORT_DIRECTORY* exp = (IMAGE_EXPORT_DIRECTORY*)((BYTE*)hMod + dir->VirtualAddress);
    DWORD* names = (DWORD*)((BYTE*)hMod + exp->AddressOfNames);
    WORD* ordinals = (WORD*)((BYTE*)hMod + exp->AddressOfNameOrdinals);
    DWORD* functions = (DWORD*)((BYTE*)hMod + exp->AddressOfFunctions);

    for (DWORD i = 0; i < exp->NumberOfNames; i++) {
        char* name = (char*)((BYTE*)hMod + names[i]);
        uint16_t ord = (uint16_t)(exp->Base + ordinals[i]);
        g_exports.push_back({ name, ord });
    }
}

int main(int argc, char* argv[]) {
    if (argc < 2) {
        printf("Usage: %s <path_to_eossdk.dll>\n", argv[0]);
        printf("Scans an EOS SDK DLL for all exported functions.\n");
        return 1;
    }

    std::string dll_path = argv[1];
    if (!std::filesystem::exists(dll_path)) {
        printf("File not found: %s\n", dll_path.c_str());
        return 1;
    }

    HMODULE hMod = LoadLibraryExA(dll_path.c_str(), NULL,
                                   LOAD_LIBRARY_AS_DATAFILE);
    if (!hMod) {
        printf("Failed to load DLL (error %lu)\n", GetLastError());
        return 1;
    }

    scan_exports(hMod);

    printf("\n=== Found %zu total exports ===\n\n", g_exports.size());
    for (const auto& e : g_exports) {
        if (e.name.find("EOS_") == 0) {
            printf("  %d %s\n", e.ordinal, e.name.c_str());
        }
    }

    FreeLibrary(hMod);
    return 0;
}
