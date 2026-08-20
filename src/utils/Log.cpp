#ifndef DISABLE_LOG

#include "Log.h"
#include "common_includes.h"

decltype(Log::m_log_user_param) Log::m_log_user_param;
decltype(Log::m_log_level)      Log::m_log_level = Log::LogLevel::OFF;
decltype(Log::m_log_func)       Log::m_log_func  = default_log_func;

void Log::default_log_func(void* user_param, Log::LogLevel lv, const char* log_message)
{
    static std::ofstream log_file("nemirtingassteamemu.log", std::ios::trunc | std::ios::out);

#if defined(__WINDOWS__)
    if (IsDebuggerPresent())
    {
        OutputDebugString(log_message);
    }
    else
    {
        static bool console = false;
        if (!console)
        {
            AllocConsole();
            freopen("CONOUT$", "w", stdout);
        }

        fprintf(stdout, "%s", log_message);
    }
#endif

    log_file << log_message;
    log_file.flush();
    fprintf(stderr, "%s", log_message);
}

bool Log::m_trace(const char* format, va_list argptr)
{
    string fmt = format;
    if (*fmt.rbegin() != '\n')
        fmt += '\n';

    va_list argptr2;
    va_copy(argptr2, argptr);

    int len = vsnprintf(nullptr, 0, fmt.c_str(), argptr);
    char* buffer = new char[++len];

    vsnprintf(buffer, len, fmt.c_str(), argptr2);
    va_end(argptr);
    va_end(argptr2);

    m_log_func(m_log_user_param, m_log_level, buffer);

    delete[]buffer;
    return true;
}

void Log::Format(LogLevel lv, const char* format, ...)
{
    if (lv >= m_log_level && m_log_level < LogLevel::MAX)
    {
        va_list argptr;
        va_start(argptr, format);
        m_trace(format, argptr);
    }
}

#endif
