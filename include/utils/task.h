

#pragma once

#include <thread>
#include <future>
#include <chrono>

class task
{
    std::promise<void> m_exit_token;
    std::future<void> m_future;
    std::thread m_thread;
    bool m_used;

public:
    task():
        m_future(m_exit_token.get_future()),
        m_used(false)
    {}

    task(task &&other):
        m_exit_token(std::move(other.m_exit_token)),
        m_future(std::move(other.m_future)),
        m_thread(std::move(other.m_thread)),
        m_used(std::move(other.m_used))
    {}

    task& operator=(task&& other)
    {
        m_exit_token = std::move(other.m_exit_token);
        m_future = std::move(other.m_future);
        m_thread = std::move(other.m_thread);
        m_used = std::move(other.m_used);
        return *this;
    }

    ~task()
    {
    }

    template<typename Function, typename... Args>
    bool run(Function&& f, Args&&... args)
    {
        if (!m_used)
        {
            m_used = true;
            m_thread = std::thread(f, args...);
            return true;
        }
        return false;
    }

    bool want_stop()
    {
        if (m_future.wait_for(std::chrono::milliseconds(0)) == std::future_status::timeout)
            return false;

        return true;
    }

    void stop()
    {
        m_exit_token.set_value();
    }

    void join()
    {
        if(m_thread.joinable())
            m_thread.join();
    }
};