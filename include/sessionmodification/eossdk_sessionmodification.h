#pragma once

#include "common_includes.h"
#include "callback_manager.h"

namespace sdk
{
    class EOSSDK_Sessionmodification : public IRunCallback
    {
    public:
        EOSSDK_Sessionmodification() {}
        ~EOSSDK_Sessionmodification() {}

        virtual bool CBRunFrame() { return false; }
        virtual bool RunCallbacks(pFrameResult_t res) { return false; }
        virtual void FreeCallback(pFrameResult_t res) {}
    };
}
