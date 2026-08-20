#pragma once

#include "frame_result.h"

FrameResult::FrameResult():
    created_time(clock::now()),
    ok_timeout(std::chrono::milliseconds(0)),
    done(false),
    remove_on_timeout(true),
    res({})
{
}

FrameResult::FrameResult(FrameResult const& other) :
    created_time(other.created_time),
    ok_timeout(other.ok_timeout),
    done(other.done),
    remove_on_timeout(other.remove_on_timeout),
    res({})
{
    res.cb_func = other.res.cb_func;
    res.callback_type_id = other.res.callback_type_id;
    res.func_param_size = other.res.func_param_size;
    res.func_param = new uint8_t[other.res.func_param_size];
    memcpy(res.func_param, other.res.func_param, other.res.func_param_size);
}

FrameResult::~FrameResult()
{
    delete[] res.func_param;
}

void* FrameResult::AllocCallback(CallbackObj func, size_t func_param_size, int i_callback, std::chrono::milliseconds ok_timeout)
{
    uint8_t* buff = new uint8_t[func_param_size];
    SetCallback(func, buff, func_param_size, i_callback, ok_timeout);
    return buff;
}

void FrameResult::SetCallback(CallbackObj func, uint8_t* func_param, size_t func_param_size, int i_callback, std::chrono::milliseconds ok_timeout)
{
    this->ok_timeout = ok_timeout;
    delete[]res.func_param;
    res.cb_func = func;
    res.callback_type_id = i_callback;
    res.func_param_size = func_param_size;
    res.func_param = func_param;
}