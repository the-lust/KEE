#ifndef MINI_DETOUR_H
#define MINI_DETOUR_H

#if defined(WIN64) || defined(_WIN64) || defined(__MINGW64__) || \
    defined(WIN32) || defined(_WIN32) || defined(__MINGW32__)

#define NOMINMAX
#include <Windows.h>

typedef HANDLE threadid_t;
#elif defined(__linux__) || defined(linux)
#include <pthread.h>

typedef pthread_t threadid_t;
#elif defined(__APPLE__)
#include <pthread.h>

typedef pthread_t threadid_t;
#endif

namespace mini_detour
{
    int update_thread(threadid_t thread_id); 
    int transaction_begin();
    int transaction_abort();
    int transaction_commit();
    int replace_func(void* pOriginalFunc, void* m_hook); 
    int detour_func(void** ppOriginalFunc, void* m_hook); 
    int unhook_func(void** ppOriginalFunc, void* m_hook);
};

#endif 

