#pragma once

#include "common_includes.h"
#include "frame_result.h"

class IRunCallback
{
public:
    
    virtual bool CBRunFrame() = 0;

    virtual bool RunCallbacks(pFrameResult_t res) = 0;
    
    virtual void FreeCallback(pFrameResult_t res) = 0;
};

class Callback_Manager
{
    time_pt m_frame_start_time;
    std::chrono::milliseconds m_max_tick_budget;

    std::set<IRunCallback*> m_frames_to_run;
    std::map<IRunCallback*, std::list<pFrameResult_t>> m_callbacks_to_run;
    
    std::map<IRunCallback*, std::map<EOS_NotificationId, pFrameResult_t>> m_notifications;

    std::recursive_mutex local_mutex;

public:
    
    Callback_Manager();
    ~Callback_Manager();

    void register_frame  (IRunCallback* obj);
    void unregister_frame(IRunCallback* obj);
    
    void register_callbacks  (IRunCallback* obj);
    void unregister_callbacks(IRunCallback* obj);
    
    bool add_callback(IRunCallback* obj, pFrameResult_t res);

    EOS_NotificationId add_notification(IRunCallback* obj, pFrameResult_t res);
    bool remove_notification(IRunCallback* obj, EOS_NotificationId id);
    void remove_all_notifications(IRunCallback* obj);
    pFrameResult_t get_notification(IRunCallback* obj, EOS_NotificationId id);
    std::vector<pFrameResult_t> get_notifications(IRunCallback* obj, int callback_id);

    void run_frames();
    void run_callbacks();

    inline void set_max_tick_budget(uint32_t milliseconds)
    {
        m_max_tick_budget = std::chrono::milliseconds{ milliseconds };
    }

    inline void tick()
    {
        m_frame_start_time = clock::now();
        run_frames();
        run_callbacks();
    }
};