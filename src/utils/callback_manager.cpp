#include "callback_manager.h"
#include "eossdk_platform.h"

constexpr static std::chrono::seconds cleanup_timeout(60);

Callback_Manager::Callback_Manager()
{}

Callback_Manager::~Callback_Manager()
{
}

void Callback_Manager::register_frame(IRunCallback* obj)
{
    TRACE_FUNC();
    lock _(GLOBAL_MUTEX);

    m_frames_to_run.emplace(obj);
}

void Callback_Manager::unregister_frame(IRunCallback* obj)
{
    TRACE_FUNC();
    lock _(GLOBAL_MUTEX);

    m_frames_to_run.erase(obj);
}

void Callback_Manager::register_callbacks(IRunCallback* obj)
{
    TRACE_FUNC();
    lock _(GLOBAL_MUTEX);

    m_callbacks_to_run[obj];
}

void Callback_Manager::unregister_callbacks(IRunCallback* obj)
{
    TRACE_FUNC();
    lock _(GLOBAL_MUTEX);

    auto it = m_callbacks_to_run.find(obj);
    if (it != m_callbacks_to_run.end())
        m_callbacks_to_run.erase(it);
}

bool Callback_Manager::add_callback(IRunCallback* obj, pFrameResult_t res)
{
    
    lock _(GLOBAL_MUTEX);

    m_callbacks_to_run[obj].push_back(res);
    return true;
}

EOS_NotificationId Callback_Manager::add_notification(IRunCallback* obj, pFrameResult_t res)
{
    
    lock _(GLOBAL_MUTEX);

    static EOS_NotificationId notif_id = 1;

    m_notifications[obj][notif_id] = res;

    return notif_id++;
}

bool Callback_Manager::remove_notification(IRunCallback* obj, EOS_NotificationId id)
{
    
    lock _(GLOBAL_MUTEX);

    auto& notifs = m_notifications[obj];
    auto it = notifs.find(id);
    if (it == notifs.end())
        return false;

    obj->FreeCallback(it->second);
    notifs.erase(it);

    return true;
}

void Callback_Manager::remove_all_notifications(IRunCallback* obj)
{
    auto it = m_notifications.find(obj);
    if (it != m_notifications.end())
    {
        for (auto& res : it->second)
        {
            obj->FreeCallback(res.second);
        }

        m_notifications.erase(it);
    }
}

pFrameResult_t Callback_Manager::get_notification(IRunCallback* obj, EOS_NotificationId id)
{
    auto& notifs = m_notifications[obj];
    auto it = notifs.find(id);
    if (it != notifs.end())
        return it->second;

    return pFrameResult_t();
}

std::vector<pFrameResult_t> Callback_Manager::get_notifications(IRunCallback* obj, int callback_id)
{
    std::vector<pFrameResult_t> results;

    auto& notifs = m_notifications[obj];

    results.reserve(notifs.size()); 
    for (auto& notifs : notifs)
    {
        if (notifs.second->ICallback() == callback_id)
            results.push_back(notifs.second);
    }

    return results;
}

void Callback_Manager::run_frames()
{
    
    lock _(GLOBAL_MUTEX);

    for (auto& frame : m_frames_to_run)
    {
        frame->CBRunFrame();

    }

    //GetNetwork().CBRunFrame(0); -- needs protobuf
}

void Callback_Manager::run_callbacks()
{
    
    lock _(GLOBAL_MUTEX);

    for (auto& callback : m_callbacks_to_run)
    {
        IRunCallback* frame = callback.first;
        std::list<pFrameResult_t>& results = callback.second;
        for (auto result_it = results.begin(); result_it != results.end();)
        {
            pFrameResult_t& res = *result_it;
            if (res->CallbackOKTimeout())
            {
                if (res->done || frame->RunCallbacks(res))
                {
                    APP_LOG(Log::LogLevel::DEBUG, "Callback ready: %s", get_callback_name(res->ICallback()).c_str());
                    if (res->GetFunc() != nullptr)
                        res->GetFunc()(res->GetFuncParam());

                    frame->FreeCallback(res);
                    result_it = results.erase(result_it);
                }
                else
                    ++result_it;
            }
            else
                ++result_it;
        }

    }
}