#pragma once

#include "common_includes.h"
#include "callback_manager.h"

namespace sdk
{
    class EOSSDK_Gameclips : public IRunCallback
    {
    public:
        EOSSDK_Gameclips() {}
        ~EOSSDK_Gameclips() {}

        virtual bool CBRunFrame() { return false; }
        virtual bool RunCallbacks(pFrameResult_t res) { return false; }
        virtual void FreeCallback(pFrameResult_t res) {}
    };
}
