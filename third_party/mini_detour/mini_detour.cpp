#include "mini_detour.h"

#include <algorithm>
#include <list>
#include <cstdint>

// Stub logging — the original Log.h from another project is not available
#define APP_LOG(level, ...)
namespace Log { enum LogLevel { DEBUG = 0, WARNING = 1, ERR = 2 }; }

#if defined(WIN64) || defined(_WIN64) || defined(__MINGW64__)
#define __WINDOWS_64__
#define __64BITS__
#elif defined(WIN32) || defined(_WIN32) || defined(__MINGW32__)
#define __WINDOWS_32__
#define __32BITS__
#endif

#if !defined(__WINDOWS__)
#if defined(__WINDOWS_32__) || defined(__WINDOWS_64__)
#define __WINDOWS__
#endif
#endif

#if defined(__linux__) || defined(linux)
#if defined(__x86m_64__)
#define __LINUX_64__
#define __64BITS__
#else
#define __LINUX_32__
#define __32BITS__
#endif
#endif

#if defined(__LINUX_32__) || defined(__LINUX_64__)
#define __LINUX__
#endif

#if defined(__APPLE__)
#if defined(__x86m_64__)
#define __APPLE_64__
#define __64BITS__
#else
#define __APPLE_32__
#define __32BITS__
#endif
#endif

#if defined(__WINDOWS__)

#elif defined(__LINUX__)
#include <sys/mman.h>
#include <unistd.h>
#include <errno.h>

#elif defined(__APPLE__)
#include <mach/mach_init.h>
#include <mach/mach_vm.h>
#include <mach/vm_prot.h>
#include <unistd.h>
#include <errno.h>

#endif

inline void* page_addr(void* addr, size_t page_size);

constexpr int addr_size = sizeof(void*);
constexpr int absolute_addr_size = addr_size;
constexpr int relative_addr_size = sizeof(int32_t);
#ifdef __64BITS__

#define ABSOLUTE_JMP_VERSION SAFE

#if ABSOLUTE_JMP_VERSION == 6

constexpr int absolute_jmp_size = 1 + sizeof(uint32_t) + 

                                  4 + sizeof(uint32_t) + 
                                  1;                     
#elif ABSOLUTE_JMP_VERSION == 13

constexpr int absolute_jmp_size = 2 + absolute_addr_size + 
                                  2 +                      
                                  1;                       
#else

constexpr int absolute_jmp_size = 6 +                 
                                  absolute_addr_size; 
#endif

#else
constexpr int absolute_jmp_size = absolute_addr_size + 2; 
                                                          
#endif
constexpr int relative_jmp_size = relative_addr_size + 1; 

#ifdef __64BITS__
#include "mini_detour_x64.h"

#else
#include "mini_detour_x86.h"

#endif

#pragma pack(push, 1)

struct trampoline_x86m_t
{
    uint8_t trampolineBytes[14 + absolute_jmp_size]; 
    uint8_t originalBytes[14];       
    uint8_t hookJump[absolute_jmp_size];             
    uint8_t nOriginalBytes;          
    uint8_t* originalAddr;           
};

struct trampoline_x64m_t
{
    uint8_t trampolineBytes[20 + absolute_jmp_size]; 
    uint8_t originalBytes[20];        
    uint8_t hookJump[absolute_jmp_size];             
    uint8_t nOriginalBytes;           
    uint8_t* originalAddr;            
};

#if defined(__32BITS__)
typedef trampoline_x86m_t trampoline_t;
#else
typedef trampoline_x64m_t trampoline_t;
#endif

struct trampoline_region_t
{
    uint8_t numTrampolines; 
    trampoline_t* trampolines_start; 
    trampoline_t* next_free_trampoline; 
};

#pragma pack(pop)

struct transaction_t
{
    bool attach;
    void** ppOriginalFunc;
    trampoline_t* trampoline;
};

static std::list<trampoline_region_t> trampoline_regions;

static bool transaction_started = false;
static std::list<transaction_t> cur_transaction;

#if defined(__LINUX__)
enum mem_protect_rights
{
    mem_r = PROT_READ,
    mem_w = PROT_WRITE,
    mem_x = PROT_EXEC,
    mem_rw = PROT_WRITE | PROT_READ,
    mem_rx = PROT_WRITE | PROT_EXEC,
    mem_rwx = PROT_WRITE | PROT_READ | PROT_EXEC,
};

size_t page_size()
{
    static size_t m_page_size = 0;
    if (m_page_size == 0)
    {
        m_page_size = sysconf(m_SC_PAGESIZE);
    }
    return m_page_size;
}

bool mem_protect(void* addr, size_t size, size_t rights)
{
    return mprotect(addr, size, rights) == 0;
}

void memory_free(void* mem_addr, size_t size)
{
    if (mem_addr != nullptr)
        munmap(mem_addr, size);
}

void* memory_alloc(void* address_hint, size_t size, mem_protect_rights rights)
{

    return mmap(address_hint, size, rights, MAP_PRIVATE | MAP_ANONYMOUS, -1, 0);
}

int flush_instruction_cache(void* pBase, size_t size)
{
    return 1;
}

#elif defined(__WINDOWS__)
enum mem_protect_rights
{
    mem_r = PAGE_READONLY,
    mem_w = PAGE_READWRITE,
    mem_x = PAGE_EXECUTE,
    mem_rw = PAGE_READWRITE,
    mem_rx = PAGE_EXECUTE_READ,
    mem_rwx = PAGE_EXECUTE_READWRITE,
};

size_t page_size()
{
    static size_t m_page_size = 0;
    if (m_page_size == 0)
    {
        SYSTEM_INFO sysInfo;
        GetSystemInfo(&sysInfo);
        m_page_size = sysInfo.dwPageSize;
    }
    return m_page_size;
}

bool mem_protect(void* addr, size_t size, size_t rights)
{
    DWORD oldProtect;
    return VirtualProtect(addr, size, rights, &oldProtect) != FALSE;
}

void memory_free(void* mem_addr, size_t size)
{
    if (mem_addr != nullptr)
        VirtualFree(mem_addr, 0, MEM_RELEASE);
}

void* memory_alloc(void* address_hint, size_t size, mem_protect_rights rights)
{
    MEMORY_BASIC_INFORMATION mbi;
    ZeroMemory(&mbi, sizeof(mbi));

    HANDLE hProcess = GetCurrentProcess();

    PBYTE pbBase = (PBYTE)address_hint;
    PBYTE pbLast = pbBase;
    for (;; pbLast = (PBYTE)mbi.BaseAddress + mbi.RegionSize) {

        ZeroMemory(&mbi, sizeof(mbi));
        if (VirtualQueryEx(hProcess, (PVOID)pbLast, &mbi, sizeof(mbi)) == 0) {
            if (GetLastError() == ERROR_INVALID_PARAMETER) {
                break;
            }
            break;
        }

        if ((mbi.RegionSize & 0xfff) == 0xfff) {
            break;
        }

        if (mbi.State != MEM_FREE) {
            continue;
        }

        PBYTE pbAddress = (PBYTE)mbi.BaseAddress > pbBase ? (PBYTE)mbi.BaseAddress : pbBase;

        const DWORD_PTR mmGranularityMinusOne = (DWORD_PTR)(0x10000 - 1);
        pbAddress = (PBYTE)(((DWORD_PTR)pbAddress + mmGranularityMinusOne) & ~mmGranularityMinusOne);

        for (; pbAddress < (PBYTE)mbi.BaseAddress + mbi.RegionSize; pbAddress += 0x10000) {
            PBYTE pbAlloc = (PBYTE)VirtualAllocEx(hProcess, pbAddress, size,
                MEM_RESERVE | MEM_COMMIT, rights);
            if (pbAlloc == nullptr) {
                continue;
            }
            return pbAlloc;
        }
    }

    return nullptr;
}

int flush_instruction_cache(void* pBase, size_t size)
{
    return FlushInstructionCache(GetCurrentProcess(), pBase, size);
}

#elif defined(__APPLE__)
enum mem_protect_rights
{
    mem_r = VM_PROT_READ,
    mem_w = VM_PROT_WRITE,
    mem_x = VM_PROT_EXECUTE,
    mem_rw = VM_PROT_WRITE | VM_PROT_READ,
    mem_rx = VM_PROT_WRITE | VM_PROT_EXECUTE,
    mem_rwx = VM_PROT_WRITE | VM_PROT_READ | VM_PROT_EXECUTE,
};

size_t page_size()
{
    static size_t m_page_size = 0;
    if (m_page_size == 0)
    {
        m_page_size = sysconf(m_SC_PAGESIZE);
    }
    return m_page_size;
}

bool mem_protect(void* addr, size_t size, size_t rights)
{
    return mach_vm_protect(mach_task_self(), (mach_vm_address_t)addr, size, FALSE, rights) == KERN_SUCCESS;
}

void memory_free(void* mem_addr, size_t size)
{
    if (mem_addr != nullptr)
        mach_vm_deallocate(mach_task_self(), (mach_vm_address_t)mem_addr, size);
}

void* memory_alloc(void* address_hint, size_t size, mem_protect_rights rights)
{
    mach_vm_address_t address = (mach_vm_address_t)page_addr(address_hint, page_size());

    if (mach_vm_allocate(mach_task_self(), &address, (mach_vm_size_t)size, VM_FLAGS_FIXED) == KERN_SUCCESS)
        return (void*)address;

    return nullptr;
}

int flush_instruction_cache(void* pBase, size_t size)
{
    return 1;
}

#endif

inline size_t region_size()
{
    return page_size();
}

inline size_t max_trampolines_in_region()
{
    return region_size() / sizeof(trampoline_t);
}

inline void* library_address_by_handle(void* library)
{
    return (library == nullptr ? nullptr : *reinterpret_cast<void**>(library));
}

inline size_t page_align(size_t size, size_t page_size)
{
    return (size + (page_size - 1)) & (((size_t)-1) ^ (page_size - 1));
}

inline void* page_addr(void* addr, size_t page_size)
{
    return reinterpret_cast<void*>(reinterpret_cast<size_t>(addr)& (((size_t)-1) ^ (page_size - 1)));
}

inline uint8_t* relative_addr_to_absolute(int32_t rel_addr, uint8_t* destination_addr)
{
    return destination_addr + rel_addr + 5;
}

inline intptr_t absolute_addr_to_relative(intptr_t opcode_addr, intptr_t destination_addr)
{
    return destination_addr - opcode_addr - 5;
}

bool is_opcode_terminating_function(uint8_t opcode)
{
    switch (opcode)
    {
        case 0xc2: 
        case 0xc3: 
        case 0xc9: 
        case 0xca: 
        case 0xcb: 
        case 0xcc: 
        case 0xcd: 
        case 0xce: 
        case 0xcf: 
            return true;
    }
    return false;
}

int is_opcode_filler(uint8_t* pCode)
{
    if (pCode[0] == 0x90)
    {
        return 1;
    }
    if (pCode[0] == 0x66 && pCode[1] == 0x90)
    {
        return 2;
    }
    if (pCode[0] == 0x0F && pCode[1] == 0x1F && pCode[2] == 0x00)
    {
        return 3;
    }
    if (pCode[0] == 0x0F && pCode[1] == 0x1F && pCode[2] == 0x40 &&
        pCode[3] == 0x00)
    {
        return 4;
    }
    if (pCode[0] == 0x0F && pCode[1] == 0x1F && pCode[2] == 0x44 &&
        pCode[3] == 0x00 && pCode[4] == 0x00) {
        return 5;
    }
    if (pCode[0] == 0x66 && pCode[1] == 0x0F && pCode[2] == 0x1F &&
        pCode[3] == 0x44 && pCode[4] == 0x00 && pCode[5] == 0x00)
    {
        return 6;
    }
    if (pCode[0] == 0x0F && pCode[1] == 0x1F && pCode[2] == 0x80 &&
        pCode[3] == 0x00 && pCode[4] == 0x00 && pCode[5] == 0x00 &&
        pCode[6] == 0x00)
    {
        return 7;
    }
    if (pCode[0] == 0x0F && pCode[1] == 0x1F && pCode[2] == 0x84 &&
        pCode[3] == 0x00 && pCode[4] == 0x00 && pCode[5] == 0x00 &&
        pCode[6] == 0x00 && pCode[7] == 0x00)
    {
        return 8;
    }
    if (pCode[0] == 0x66 && pCode[1] == 0x0F && pCode[2] == 0x1F &&
        pCode[3] == 0x84 && pCode[4] == 0x00 && pCode[5] == 0x00 &&
        pCode[6] == 0x00 && pCode[7] == 0x00 && pCode[8] == 0x00)
    {
        return 9;
    }
    if (pCode[0] == 0x66 && pCode[1] == 0x66 && pCode[2] == 0x0F &&
        pCode[3] == 0x1F && pCode[4] == 0x84 && pCode[5] == 0x00 &&
        pCode[6] == 0x00 && pCode[7] == 0x00 && pCode[8] == 0x00 &&
        pCode[9] == 0x00)
    {
        return 10;
    }
    if (pCode[0] == 0x66 && pCode[1] == 0x66 && pCode[2] == 0x66 &&
        pCode[3] == 0x0F && pCode[4] == 0x1F && pCode[5] == 0x84 &&
        pCode[6] == 0x00 && pCode[7] == 0x00 && pCode[8] == 0x00 &&
        pCode[9] == 0x00 && pCode[10] == 0x00)
    {
        return 11;
    }
    
    if (pCode[0] == 0xcc)
    {
        return 1;
    }

    return 0;
}

bool read_mod_reg_rm_opcode(uint8_t** ppCode, bool ignore_displacement)
{
    uint8_t* pCode = *ppCode;

    switch (pCode[1] & mod_mask) 
    {
        case register_addressing_mode      : *ppCode += s_opcodes[*pCode].base_size; break; 
        case four_bytes_signed_displacement:
        {
            switch (pCode[1] & rm_mask)
            {
                case sib_with_no_displacement: *ppCode += s_opcodes[*pCode].base_size + 5; break; 
                default: *ppCode += s_opcodes[*pCode].base_size + 4; break; 
            }
        }
        break;

        case one_byte_signed_displacement:
        {
            switch (pCode[1] & rm_mask)
            {
                case sib_with_no_displacement: *ppCode += s_opcodes[*pCode].base_size + 2; break; 
                default: *ppCode += s_opcodes[*pCode].base_size + 1; break; 
            }
        }
        break;

        default:
            switch (pCode[1] & rm_mask)
            {
                case displacement_only_addressing:
                {
                    if (ignore_displacement)
                    {
                        APP_LOG(Log::LogLevel::DEBUG, "Ignored displacement only addressing");
                        *ppCode += s_opcodes[*pCode].base_size + 4; break; 
                    }
                    else
                    {
                        APP_LOG(Log::LogLevel::DEBUG, "Failed on displacement only addressing");
                        return false; 
                    }
                }
                break;
                    
                case sib_with_no_displacement         : *ppCode += s_opcodes[*pCode].base_size + 1; break; 
                case register_indirect_addressing_mode: 
                default: *ppCode += s_opcodes[*pCode].base_size;
            }
    }
    return true;
}

int find_space_for_trampoline(uint8_t** func, int bytes_needed, bool ignore_jump)
{
    if (func == nullptr)
        return -1;

    int code_len = 0;
    bool search = true;
    uint8_t* pCode = *func;
    while (search) 
    {
        if (is_opcode_terminating_function(*pCode))
            break;

        code_len = is_opcode_filler(pCode);
        if (code_len)
        {
            pCode += code_len;
        }
        else 
        {
            APP_LOG(Log::LogLevel::DEBUG, "Opcode %s, base_size: %d, has_r_m: %d", s_opcodes[*pCode].desc, s_opcodes[*pCode].base_size, (int)s_opcodes[*pCode].has_r_m);

            if (s_opcodes[*pCode].has_r_m)
            {
                auto bkpCode = pCode;
                search = read_mod_reg_rm_opcode(&pCode, ignore_jump);
                APP_LOG(Log::LogLevel::DEBUG, "Read %d bytes for opcode 0x%02X", pCode - bkpCode, (unsigned int)*bkpCode);
            }
            else if (s_opcodes[*pCode].base_size)
            {
                switch (*pCode)
                {
#ifdef __64BITS__
                    case 0x40: 
                    case 0x41: 
                    case 0x42: 
                    case 0x43: 
                    case 0x44: 
                    case 0x45: 
                    case 0x46: 
                    case 0x47: 
                    case 0x48: 
                    case 0x49: 
                    case 0x4a: 
                    case 0x4b: 
                    case 0x4c: 
                    case 0x4d: 
                    case 0x4e: 
                    case 0x4f: 
                        pCode += s_opcodes[*pCode].base_size;
                        continue; 
#endif
                    case 0xe9: 
                    case 0xe8: 
                        
                        if (pCode == *func)
                        {
                            pCode += s_opcodes[*pCode].base_size;
                        }
                        else
                        {
                            if (ignore_jump)
                            {
                                pCode += s_opcodes[*pCode].base_size;
                            }
                        }

                    case 0xeb: 
                        search = false;
                        break;

                    default:
                        pCode += s_opcodes[*pCode].base_size;
                }
            }
            else
            {
                switch (*pCode)
                {
                    case 0xff: 
                    {
                        if (pCode[1] == 0x25) 
                        {   
                        #ifdef __64BITS__
                            pCode = *(uint8_t**)(pCode + 6 + *(int32_t*)(pCode + 2));
                        #else
                            pCode = *(uint8_t**)(*(uint8_t**)(pCode + 2));
                        #endif

                            *func = pCode;

                        }
                        else
                        {
                            if (!ignore_jump)
                                search = false;
                        }
                    }
                    break;

                    default:
                        APP_LOG(Log::LogLevel::DEBUG, "Unknown opcode 0x%02X", (unsigned int)*pCode);
                        APP_LOG(Log::LogLevel::DEBUG, "Next opcodes: 0x%02X 0x%02X 0x%02X 0x%02X 0x%02X 0x%02X 0x%02X 0x%02X",
                            pCode[1], pCode[2], pCode[3], pCode[4], pCode[5], pCode[6], pCode[7], pCode[8]);
                        search = false;
                }
            }
        } 
        if ((pCode - *func) >= bytes_needed && search)
        {
            search = false;
        }
    }

    return pCode - *func;
}

inline uint8_t* gen_relative_jmp(uint8_t* opcode_addr, uint8_t* dest)
{
    *opcode_addr++ = 0xe9; 
    *reinterpret_cast<int32_t*>(opcode_addr) = (int32_t)(dest - (opcode_addr + relative_addr_size));
    return opcode_addr + addr_size;
}

#ifdef __64BITS__
inline uint8_t* gen_absolute_jmp(uint8_t* opcode_addr, uint8_t* dest)
{
#if ABSOLUTE_JMP_VERSION == 6
    *opcode_addr++ = 0x68;                                                            
    *reinterpret_cast<uint32_t*>(opcode_addr) = (uint32_t)dest;                       
    opcode_addr += sizeof(uint32_t);
    if ((uint64_t)dest > std::numeric_limits<uint32_t>::max())
    {
        *opcode_addr++ = 0xC7;                                                        
        *opcode_addr++ = 0x44;                                                        
        *opcode_addr++ = 0x24;                                                        
        *opcode_addr++ = 0x04;                                                        
        *reinterpret_cast<uint32_t*>(opcode_addr) = (uint32_t)((uint64_t)dest >> 32); 
        opcode_addr += sizeof(uint32_t);
    }
    *opcode_addr++ = 0xC3;                                                            
#elif ABSOLUTE_JMP_VERSION == 13
    
     Uses Volatile x64 register R11
    *opcode_addr++ = 0x49; 
    *opcode_addr++ = 0xBB; 
    *reinterpret_cast<void**>(opcode_addr) = (void*)dest; 
    opcode_addr += absolute_addr_size;
    *opcode_addr++ = 0x41; 
    *opcode_addr++ = 0x53; 
    *opcode_addr++ = 0xC3; 
#else

    *opcode_addr++ = 0xFF; 
    *opcode_addr++ = 0x25; 
    *opcode_addr++ = 0x00; 
    *opcode_addr++ = 0x00; 
    *opcode_addr++ = 0x00; 
    *opcode_addr++ = 0x00; 
    *reinterpret_cast<void**>(opcode_addr) = (void*)dest;
    opcode_addr += absolute_addr_size;
#endif
    return opcode_addr;
}

#else

inline uint8_t* gen_absolute_jmp(uint8_t* opcode_addr, uint8_t* dest)
{
    *opcode_addr++ = 0x68; 
    *reinterpret_cast<void**>(opcode_addr) = (void*)dest; 
    opcode_addr += addr_size;
    *opcode_addr++ = 0xc3;  
    return opcode_addr;
}

#endif

trampoline_t* alloc_new_trampoline_region(void* hint_addr, bool limit_to_2gb)
{
    trampoline_region_t region;
    trampoline_t* trampoline = nullptr;

    for (int i = 0; i < 10000000; ++i)
    {
        trampoline = reinterpret_cast<trampoline_t*>(memory_alloc(hint_addr, region_size(), mem_protect_rights::mem_rwx));
        if (!limit_to_2gb || std::abs((int64_t)trampoline - (int64_t)hint_addr) <= 0x7FFFFFFF)
            break;

        hint_addr = reinterpret_cast<uint8_t*>(hint_addr) - page_size();

        memory_free(trampoline, region_size());
        trampoline = nullptr;
    }
    if (trampoline)
    {
        region.numTrampolines = 0;
        
        region.trampolines_start = trampoline;
        
        std::fill(reinterpret_cast<uint8_t*>(region.trampolines_start), reinterpret_cast<uint8_t*>(region.trampolines_start) + region_size(), 0);
        region.next_free_trampoline = region.trampolines_start;
        
        mem_protect(region.trampolines_start, region_size(), mem_protect_rights::mem_rx);

        trampoline_regions.push_back(region);
    }

    return trampoline;
}

trampoline_t* get_free_trampoline(void* originalFuncAddr, bool limit_to_2gb)
{
    if (!transaction_started)
        return nullptr;

    trampoline_t* res = nullptr;
    auto it = std::find_if(trampoline_regions.begin(), trampoline_regions.end(), [originalFuncAddr, limit_to_2gb](trampoline_region_t& region)
    {
        if (region.numTrampolines == max_trampolines_in_region() || 
            (limit_to_2gb && (std::abs((int64_t)region.trampolines_start - (int64_t)originalFuncAddr) > 0x7FFFFFFFul))) 
            return false; 
        return true; 
    });

    if (it == trampoline_regions.end())
    {
        if (alloc_new_trampoline_region(originalFuncAddr, limit_to_2gb) == nullptr)
            return nullptr;

        it = --trampoline_regions.end();
    }
    res = it->next_free_trampoline;

    trampoline_t* next_new_trampoline = res + 1;
    if (it->numTrampolines != max_trampolines_in_region())
    {
        while (next_new_trampoline->nOriginalBytes != 0)
        {
            ++next_new_trampoline;
        }
    }
    else
    {
        next_new_trampoline = nullptr;
    }
    it->next_free_trampoline = next_new_trampoline;

    ++it->numTrampolines;

    return res;
}

void clear_trampoline(trampoline_region_t& region, trampoline_t* trampoline)
{
    --region.numTrampolines;

    std::fill(reinterpret_cast<uint8_t*>(trampoline), reinterpret_cast<uint8_t*>(trampoline + 1), 0);
    if (region.next_free_trampoline == nullptr || region.next_free_trampoline > trampoline)
        region.next_free_trampoline = trampoline;
}

inline bool is_page_inside_region(void* page, trampoline_region_t& region)
{
    if (page >= region.trampolines_start && page <= (region.trampolines_start + region_size()))
        return true;
    return false;
}

int mini_detour::update_thread(threadid_t thread_id)
{
    return 0;
}

int mini_detour::transaction_begin()
{
    if (transaction_started)
        return -1;

    transaction_started = true;

    return 0;
}

int mini_detour::transaction_abort()
{
    if (!transaction_started)
        return -1;

    for (auto& i : cur_transaction)
    {
        trampoline_t* trampoline = i.trampoline;
        void* page_start = page_addr(reinterpret_cast<void*>(trampoline), page_size());
        auto it = std::find_if(trampoline_regions.begin(), trampoline_regions.end(), [page_start](trampoline_region_t& region) {
            if (is_page_inside_region(page_start, region))
                return true;
            return false;
            });
        if (it != trampoline_regions.end())
        {
            clear_trampoline(*it, trampoline);
        }
    }
    cur_transaction.clear();
    transaction_started = false;

    return 0;
}

int mini_detour::transaction_commit()
{
    if (!transaction_started)
        return -1;

    for (auto& i : cur_transaction)
    {
        void** ppOriginalFunc = i.ppOriginalFunc;
        trampoline_t* trampoline = i.trampoline;
        int res;

        if (i.attach)
        {
            void* originalFunctionPage = page_addr(*ppOriginalFunc, page_size());

            res = mem_protect(originalFunctionPage, page_size() * 2, mem_protect_rights::mem_rwx);

            if (trampoline->nOriginalBytes >= absolute_jmp_size)
            {
                APP_LOG(Log::LogLevel::DEBUG, "Attaching function %p with Absolute Jmp to %p", *ppOriginalFunc, trampoline->hookJump);
                
                gen_absolute_jmp(reinterpret_cast<uint8_t*>(*ppOriginalFunc), trampoline->hookJump);
            }
            else
            {
                APP_LOG(Log::LogLevel::DEBUG, "Attaching function %p with Relative Jmp to %p", *ppOriginalFunc, trampoline->hookJump);
                
                gen_relative_jmp(reinterpret_cast<uint8_t*>(*ppOriginalFunc), trampoline->hookJump);
            }

            res = mem_protect(originalFunctionPage, page_size() * 2, mem_protect_rights::mem_rx);

            *ppOriginalFunc = (void*)(trampoline->trampolineBytes);
        }
        else
        {
            void* trampoline_page = page_addr(reinterpret_cast<void*>(trampoline), page_size());

            *ppOriginalFunc = trampoline->originalAddr;

            void* originalFunctionPage = page_addr(*ppOriginalFunc, page_size());

            res = mem_protect(originalFunctionPage, page_size() * 2, mem_protect_rights::mem_rwx);

            std::copy(trampoline->originalBytes, trampoline->originalBytes + trampoline->nOriginalBytes,
                reinterpret_cast<uint8_t*>(*ppOriginalFunc));

            res = mem_protect(originalFunctionPage, page_size() * 2, mem_protect_rights::mem_rx);

            res = mem_protect(trampoline_page, page_size(), mem_protect_rights::mem_rwx);

            clear_trampoline(*reinterpret_cast<trampoline_region_t*>(trampoline_page), trampoline);

            res = mem_protect(trampoline_page, page_size(), mem_protect_rights::mem_rx);
        }
    }
    cur_transaction.clear();
    transaction_started = false;

    return 0;
}

int mini_detour::unhook_func(void** ppOriginalFunc, void* m_hook)
{
    if (!transaction_started)
        return -EPERM;

    if (ppOriginalFunc == nullptr || m_hook == nullptr || *ppOriginalFunc == nullptr)
        return -EINVAL;

    trampoline_t* trampoline = reinterpret_cast<trampoline_t*>(*ppOriginalFunc);
    void* page_start = page_addr(reinterpret_cast<void*>(trampoline), page_size());
    auto it = std::find_if(trampoline_regions.begin(), trampoline_regions.end(), [page_start](trampoline_region_t& region)
    {
        if (is_page_inside_region(page_start, region))
            return true;
        return false;
    });

    if (it != trampoline_regions.end())
    {
        cur_transaction.push_back({ false, ppOriginalFunc, trampoline });
    }

    return -EINVAL;
}

int hook_func(void** ppOriginalFunc, void* m_hook, bool replace)
{
    if (!transaction_started)
        return -EPERM;

    if (ppOriginalFunc == nullptr || m_hook == nullptr || *ppOriginalFunc == nullptr)
        return -EINVAL;

    uint8_t* hook = reinterpret_cast<uint8_t*>(m_hook);
    
    int code_len = find_space_for_trampoline((uint8_t**)ppOriginalFunc, absolute_jmp_size, replace);
    uint8_t* pOriginalFunc = (uint8_t*)*ppOriginalFunc;

    if (code_len < relative_jmp_size)
    {
        APP_LOG(Log::LogLevel::DEBUG, "Didn't find enought space for code rewrite: %d/%d", code_len, relative_jmp_size);
        return -ENOSPC;
    }

    trampoline_t* trampoline = get_free_trampoline(*ppOriginalFunc, code_len < absolute_jmp_size);
    if (trampoline == nullptr)
    {
        APP_LOG(Log::LogLevel::DEBUG, "Didn't find any free trampoline");
        return -EFAULT;
    }

    APP_LOG(Log::LogLevel::DEBUG, "Found space for trampoline: %d/%d/%d", code_len, relative_jmp_size, absolute_jmp_size);
    uint8_t* pTrampolineCode = trampoline->trampolineBytes;

    void* trampoline_page = page_addr(trampoline, page_size());

    mem_protect(trampoline_page, page_size(), mem_protect_rights::mem_rw);

    gen_absolute_jmp(trampoline->hookJump, hook);
    
    trampoline->nOriginalBytes = code_len;
    std::copy(pOriginalFunc, pOriginalFunc + code_len, trampoline->originalBytes);
    if (*pOriginalFunc == 0xe9) 
    {
        uint8_t* func_abs_addr = relative_addr_to_absolute(*(int32_t*)(pOriginalFunc + 1), pOriginalFunc);
        gen_absolute_jmp(pTrampolineCode, func_abs_addr);

        APP_LOG(Log::LogLevel::DEBUG, "Making absolute address(%p) from relative address(%p)", pOriginalFunc, func_abs_addr);
    }
    else
    {
        std::copy(trampoline->originalBytes, trampoline->originalBytes + code_len, pTrampolineCode);
        pTrampolineCode += code_len;
        
        gen_absolute_jmp(pTrampolineCode, pOriginalFunc + code_len);

        APP_LOG(Log::LogLevel::DEBUG, "Making absolute jump to address(%p)", pOriginalFunc + code_len);
    }

    trampoline->originalAddr = pOriginalFunc;

    mem_protect(trampoline_page, page_size(), mem_protect_rights::mem_rx);

    cur_transaction.push_back({ true, ppOriginalFunc, trampoline });

    return 0;
}

int mini_detour::detour_func(void** ppOriginalFunc, void* m_hook)
{
    APP_LOG(Log::LogLevel::DEBUG, "");
    return hook_func(ppOriginalFunc, m_hook, false);
}

int mini_detour::replace_func(void* pOriginalFunc, void* m_hook)
{
    APP_LOG(Log::LogLevel::DEBUG, "");
    
    if (pOriginalFunc == nullptr || m_hook == nullptr)
        return -EINVAL;

    uint8_t* hook = reinterpret_cast<uint8_t*>(m_hook);
    
    int code_len = find_space_for_trampoline((uint8_t**)&pOriginalFunc, absolute_jmp_size, true);

    if (code_len < relative_jmp_size)
    {
        APP_LOG(Log::LogLevel::DEBUG, "Didn't find enought space for code rewrite: %d/%d", code_len, relative_jmp_size);
        return -ENOSPC;
    }

    APP_LOG(Log::LogLevel::DEBUG, "Found space for replace: %d/%d/%d", code_len, relative_jmp_size, absolute_jmp_size);

    void* replace_page = page_addr(pOriginalFunc, page_size());

    mem_protect(replace_page, page_size(), mem_protect_rights::mem_rwx);
    if (code_len < absolute_jmp_size)
    {
        intptr_t relative_addr = absolute_addr_to_relative((intptr_t)pOriginalFunc, (intptr_t)hook);
        if (std::abs(relative_addr) > 0x7FFFFFFF)
            return -ERANGE; 

        gen_relative_jmp((uint8_t*)pOriginalFunc, hook);
    }
    else
    {
        gen_absolute_jmp((uint8_t*)pOriginalFunc, hook);
    }

    mem_protect(pOriginalFunc, page_size(), mem_protect_rights::mem_rx);

    return 0;
}

