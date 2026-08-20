#pragma once

#include "common_includes.h"
#include "callback_manager.h"

namespace sdk
{
    class EOSSDK_EApplicationStatus : public IRunCallback
    {
    public:
        EOSSDK_EApplicationStatus() {}
        ~EOSSDK_EApplicationStatus() {}

        virtual bool CBRunFrame() { return false; }
        virtual bool RunCallbacks(pFrameResult_t res) { return false; }
        virtual void FreeCallback(pFrameResult_t res) {}
    };
}
