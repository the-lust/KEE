#pragma once

#include "common_includes.h"

using CallbackObj = std::function<void(void*)>;
using CallbackFunc = void(EOS_CALL*)(void*);

struct CallbackMessage_t
{
    int callback_type_id;
    uint8_t* func_param;
    size_t func_param_size;
    CallbackObj cb_func;
};

class FrameResult
{
    std::chrono::milliseconds ok_timeout;
    bool remove_on_timeout; 
    CallbackMessage_t res;
public:
    const std::chrono::time_point<std::chrono::steady_clock> created_time;
    bool done;    

    FrameResult();
    FrameResult(FrameResult const& other);
    ~FrameResult();

    void* AllocCallback(CallbackObj func, size_t func_param_size, int i_callback, std::chrono::milliseconds ok_timeout = std::chrono::milliseconds(100));
    void SetCallback(CallbackObj func, uint8_t* func_param, size_t func_param_size, int i_callback, std::chrono::milliseconds ok_timeout = std::chrono::milliseconds(100));

    inline CallbackMessage_t const& GetCallbackMsg() const { return res; }
    inline int ICallback() const { return res.callback_type_id; }
    inline void* GetFuncParam() const { return res.func_param; }
    inline size_t CallbackSize() const { return res.func_param_size; }
    inline CallbackObj GetFunc() const { return res.cb_func; }

    inline bool CallbackOKTimeout() { return ((std::chrono::steady_clock::now() - created_time) >= ok_timeout); }

    template<typename T>
    inline void SetCallback(CallbackObj func, T* func_param, std::chrono::milliseconds ok_timeout = std::chrono::milliseconds(100))
    {
        SetCallback(func, reinterpret_cast<uint8_t*>(func_param), sizeof(T), T::k_iCallback, ok_timeout);
    }

    template<typename T>
    inline T& GetCallback()
    {
        assert((res.callback_type_id == T::k_iCallback));
        return *reinterpret_cast<T*>(res.func_param);
    }

    template<typename T>
    inline T& CreateCallback(CallbackObj func, std::chrono::milliseconds ok_timeout = std::chrono::milliseconds(100))
    {
        void* buff = AllocCallback(func, sizeof(T), T::k_iCallback, ok_timeout);
        T* cb = new (buff) T;
        return *cb;
    }
};

using pFrameResult_t = std::shared_ptr<FrameResult>;